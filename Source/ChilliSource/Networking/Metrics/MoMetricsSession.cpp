//
//  MoMetricsSession.cpp
//  MoFlow
//
//  Created by Scott Downie on 04/03/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#include <ChilliSource/Networking/Metrics/MoMetricsSession.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Device.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Base/Utils.h>

namespace ChilliSource
{
    namespace Networking
    {
        const u32 kudwMaxEventsPerBatch = 20;
        const TimeIntervalSecs kSessionGraceTimeSecs = 300;
        
        const u32 kudwNumRetrys = 5;
        const u32 kaRetryDelaysSecs[kudwNumRetrys] = {0, 5, 15, 20, 45};
        
        u32 udwNonceCounter = 0;
        
        void GenerateRequestID(std::string& outstrID)
        {
            std::string strTime = Core::ToString(Core::Application::GetSystemTime());
            std::string strCounter = Core::ToString(udwNonceCounter);
            
            udwNonceCounter++;
            outstrID = strTime + strCounter;
        }
        
        void MetricsEvent::ToJsonForServer(Json::Value & incValue) const
        {
			incValue["Type"] = strType;
			incValue["Timestamp"] = udwTimeStamp;
            incValue["Count"] = udwCount;
            
            Json::Value jParamArray(Json::arrayValue);
            for (u32 i = 0; i < astrParams.size(); ++i)
            {
                incValue["Param" + Core::ToString(i+1)] = astrParams[i];
            }
		}
        
        void MetricsEvent::ToJsonForCache(Json::Value & incValue) const
        {
            incValue["Type"] = strType;
			incValue["Timestamp"] = udwTimeStamp;
            incValue["Count"] = udwCount;
            incValue["Summarise"] = bSummarisable;
            incValue["NumParams"] = (u32)astrParams.size();
            
            Json::Value jParamArray(Json::arrayValue);
            for (u32 i = 0; i < astrParams.size(); ++i)
            {
                incValue["Param" + Core::ToString(i+1)] = astrParams[i];
            }
        }
        
		void MetricsEvent::FromCachedJson(const Json::Value & incValue)
        {
			strType = incValue["Type"].asString();
            udwCount = incValue["Count"].asUInt();
            bSummarisable = incValue["Summarise"].asBool();
            bIgnore = false;
			udwTimeStamp = incValue["Timestamp"].asUInt();
            
            u32 udwNumParams = incValue["NumParams"].asUInt();
            astrParams.reserve(udwNumParams);
            for (u32 i = 0; i < udwNumParams; ++i)
            {
                astrParams.push_back(incValue["Param" + Core::ToString(i+1)].asString());
            }
		}
        
        MoMetricsSession::MoMetricsSession(HttpConnectionSystem * inpHttpSystem, const std::string& instrMetricsUrl, const std::string& instrAppID, IExternalMetrics* inpExternalMetrics)
        : mpHttpSystem(inpHttpSystem), mstrRealmUrl(instrMetricsUrl), mstrAppID(instrAppID), mpExternalMetrics(inpExternalMetrics), mbIsClosed(false), mdwServerTimeDelta(0), mbActionInProgress(false), mbRequestInProgress(false),
        mudwCurrentDelayIndex(0), mLastActionTime(0)
        {
            mstrID = Core::ToString(Core::Application::GetSystemTime());
        }
        
        void MoMetricsSession::SetConstant(const std::string& instrKey, const std::string& instrValue)
        {
            mmapConstants[instrKey] = instrValue;
        }

        bool MoMetricsSession::ContainsConstant(const std::string& instrKey) const
        {
            return mmapConstants.find(instrKey) != mmapConstants.end();
        }
        
        void MoMetricsSession::SetServerTimeDelta(s32 indwServerDelta)
		{
            mdwServerTimeDelta = indwServerDelta;
		}
        
        const std::string& MoMetricsSession::GetID() const
        {
            return mstrID;
        }
        
        void MoMetricsSession::LoadFromCache(const std::string& instrID)
        {
            mQueuedEvents.clear();
            
            Json::Value jSession;
            Core::CUtils::ReadJson(Core::StorageLocation::k_cache, instrID + ".mosession", &jSession);
            
            mstrID = jSession["ID"].asString();
            mstrToken = jSession["Token"].asString();
            mLastActivityTime = jSession["Timestamp"].asUInt();
            mbIsClosed = jSession["Closed"].asBool();
            
            // load previously set constants
            if(jSession.isMember("Constants"))
            {
                // Read object
                for(Json::ValueIterator it = jSession["Constants"].begin() ; it != jSession["Constants"].end() ; ++it)
                {
                    if((*it).isString())
                    {
                        std::string strKey(it.key().asString());
                        mmapConstants[strKey] = (*it).asString();
                    }
                }
            }
            
            if(jSession.isMember("Country"))
            {
                msLocation.strCountry = jSession["Country"].asString();
                RequestLocationUpdate(msLocation);
            }

            Json::Value jEvents = jSession["Events"];
            for (u32 nEvent = 0; nEvent < jEvents.size(); nEvent++)
            {
                MetricsEvent sEvent;
                sEvent.FromCachedJson(jEvents[nEvent]);
                QueueEvent(sEvent);
            }
            
            Core::Application::GetFileSystemPtr()->DeleteFile(Core::StorageLocation::k_cache,  instrID + ".mosession");
        }
        
        void MoMetricsSession::SaveToCache()
        {
            Json::Value jSession;
            
            jSession["ID"] = mstrID;
            jSession["Token"] = mstrToken;
            jSession["Timestamp"] = (u32)GetLastActivityTime();
            jSession["Closed"] = mbIsClosed;
            
			Json::Value jEvents(Json::arrayValue);
			for (std::deque<MetricsEvent>::const_iterator it = mQueuedEvents.begin(); it != mQueuedEvents.end(); ++it)
            {
                if(!it->bIgnore)
                {
                    Json::Value jEvent;
                    it->ToJsonForCache(jEvent);
                    jEvents.append(jEvent);
                }
			}
            
            jSession["Events"] = jEvents;
            
            // save set constants
            if(mmapConstants.size() > 0)
            {
                Json::Value jConstants(Json::objectValue);
                
                for(std::unordered_map<std::string, std::string>::const_iterator it = mmapConstants.begin(); it != mmapConstants.end(); ++it)
                {
                    jConstants[it->first] = it->second;
                }
                
                jSession["Constants"] = jConstants;
			}
            
            if(msLocation.strCountry.empty() == false)
            {
                jSession["Country"] = msLocation.strCountry;
            }
            
            Core::CUtils::StringToFile(Core::StorageLocation::k_cache, mstrID + ".mosession", jSession.toUnformattedString());
            
			mQueuedEvents.clear();
        }
        
        bool MoMetricsSession::IsExpired() const
        {
            s32 dwTimeSinceLastActivity = (s32)(Core::Application::GetSystemTime() - GetLastActivityTime());
            return (dwTimeSinceLastActivity > kSessionGraceTimeSecs);
        }
        
        bool MoMetricsSession::IsOpen() const
        {
            return !mstrToken.empty();
        }
        
        bool MoMetricsSession::IsClosed() const
        {
            return mbIsClosed;
        }
        
        void MoMetricsSession::QueueAction(Action ineAction)
        {
            mQueuedActions.push_back(ineAction);
            PerformNextAction();
        }
        
        bool MoMetricsSession::CanPerformNextAction() const
        {
            return !mQueuedActions.empty() && !mbActionInProgress && ((Core::Application::GetSystemTime() - mLastActionTime) >= kaRetryDelaysSecs[mudwCurrentDelayIndex]);
        }
        
        void MoMetricsSession::PerformNextAction()
        {
            if(!CanPerformNextAction())
                return;
            
            mLastActionTime = Core::Application::GetSystemTime();
            mbActionInProgress = true;
            
            switch(mQueuedActions.front())
            {
                case Action::k_createSession:
                    MakeSessionRequest();
                    break;
                case Action::k_updateLocation:
                    MakeLocationUpdateRequest(msLocation);
                    break;
                case Action::k_flushBatch:
                    MakeFlushRequest(kudwMaxEventsPerBatch);
                    break;
                case Action::k_closeSession:
                    MakeCloseRequest();
                    break;
            }
        }
        
        void MoMetricsSession::OnActionComplete()
        {
            mbActionInProgress = false;
            if(!mQueuedActions.empty())
                mQueuedActions.pop_front();
            PerformNextAction();
        }
        
        void MoMetricsSession::OnActionRetry()
        {
            if(mudwCurrentDelayIndex < (kudwNumRetrys-1))
            {
                mudwCurrentDelayIndex++;
            }
            
            mbActionInProgress = false;
            PerformNextAction();
        }
        
        void MoMetricsSession::RequestAuthTokens()
        {
            QueueAction(Action::k_createSession);
        }
        
        void MoMetricsSession::MakeSessionRequest()
        {
            UpdateLastActivityTime();
			
			Json::Value cJData(Json::objectValue);
			
			Json::Value cJApp(Json::objectValue);
            cJApp["ApplicationID"] = mstrAppID;
			cJApp["Version"] = Core::Application::GetAppVersion();
			cJData["Application"] = cJApp;
			
			Json::Value cJDevice(Json::objectValue);
			cJDevice["Type"] = Core::CDevice::GetManufacturerName() + Core::CDevice::GetModelName() + Core::CDevice::GetModelTypeName();
			cJDevice["OS"] = Core::CDevice::GetOSVersion();
			cJDevice["Locale"] = Core::CDevice::GetLocale().GetCountryCode();
			cJDevice["Language"] = Core::CDevice::GetLanguage().GetLocaleCode();
			cJDevice["DeviceID"] =  Core::CDevice::GetUDID();
			cJData["Device"] = cJDevice;
			
            Json::Value cJSession(Json::objectValue);
			cJSession["Data"] = cJData;
            cJSession["Timestamp"] = (u32)Core::Application::GetSystemTime();
			
            MetricsRequest sRequest = {cJSession, "/session/create", Core::MakeDelegate(this, &MoMetricsSession::OnAuthTokensRequestComplete), false};
            QueueRequest(sRequest);
            MakeNextRequest();
        }
        
        void MoMetricsSession::OnAuthTokensRequestComplete(HttpRequest* inpRequest, HttpRequest::CompletionResult ineResult)
		{
			if (ineResult == HttpRequest::CompletionResult::k_timeout || inpRequest->GetResponseCode() == kHTTPBusy)
            {
                OnActionRetry();
                return;
            }
            
            //We should never get an error as we have no way of handling it
            //except to push on without any new tokens
            if(inpRequest->GetResponseCode() == kHTTPResponseOK)
            {
                Json::Reader cJReader;
                Json::Value cJResponse;
                cJReader.parse(inpRequest->GetResponseString(), cJResponse);
                mstrID = cJResponse["SessionID"].asString();
                mstrToken = cJResponse["Password"].asString();
                if(cJResponse.isMember("Realm"))
                {
                    mstrRealmUrl = cJResponse["Realm"].asString();
                }
            }
            
            OnActionComplete();
		}
        
        void MoMetricsSession::RequestLocationUpdate(const MetricsLocation& insLocation)
        {
            msLocation = insLocation;
            
            QueueAction(Action::k_updateLocation);
        }
        
        void MoMetricsSession::MakeLocationUpdateRequest(const MetricsLocation& insLocation)
        {
            UpdateLastActivityTime();
			
            Json::Value cJLocation(Json::objectValue);
			cJLocation["Country"] = insLocation.strCountry;
            
            Json::Value cJData(Json::objectValue);
            cJData["Location"] = cJLocation;
			
            Json::Value cJSession(Json::objectValue);
			cJSession["Data"] = cJData;
            cJSession["Timestamp"] = (u32)Core::Application::GetSystemTime();
			
            MetricsRequest sRequest = {cJSession, "/session/update", Core::MakeDelegate(this, &MoMetricsSession::OnLocationUpdateRequestComplete), true};
            QueueRequest(sRequest);
            MakeNextRequest();
        }
        
        void MoMetricsSession::OnLocationUpdateRequestComplete(HttpRequest* inpRequest, HttpRequest::CompletionResult ineResult)
        {
            if (ineResult == HttpRequest::CompletionResult::k_completed)
            {
                if(inpRequest->GetResponseCode() == kHTTPBusy)
                {
                    OnActionRetry();
                    return;
                }
			}
            
            msLocation = MetricsLocation();
            
            OnActionComplete();
        }
        
        void MoMetricsSession::UpdateLastActivityTime()
        {
            mLastActivityTime = Core::Application::GetSystemTime();
        }
        
        TimeIntervalSecs MoMetricsSession::GetLastActivityTime() const
        {
            return mLastActivityTime;
        }
        
        void MoMetricsSession::RecordEvent(const std::string & instrType, const std::vector<std::string>& inastrParams, bool inbSummarise)
        {
            UpdateLastActivityTime();
            
            MetricsEvent sEvent;
            
            sEvent.strType = instrType;
			sEvent.astrParams = inastrParams;
			sEvent.udwTimeStamp = Core::Application::GetSystemTime() + mdwServerTimeDelta;
			sEvent.udwCount = 1;
            sEvent.bSummarisable = inbSummarise;
            sEvent.bIgnore = false;
            
			QueueEvent(sEvent);
        }
        
        void MoMetricsSession::QueueRequest(const MetricsRequest& insRequest)
        {
            mQueuedRequests.push_back(insRequest);
        }
        
        void MoMetricsSession::MakeNextRequest()
        {
            if(mQueuedRequests.empty() || mbRequestInProgress)
                return;
            
            mbRequestInProgress = true;
            
            MetricsRequest& sRequest = mQueuedRequests.front();
            
            if(mpExternalMetrics)
            {
                mpExternalMetrics->MakeRequest(sRequest);
            }
            
            if(sRequest.bRequiresAuth)
            {
                AppendAuthDetails(sRequest);
            }
            
            HttpRequestDetails sRequestDetails;
			sRequestDetails.eType = HttpRequestDetails::Type::k_post;
            sRequestDetails.strURL = mstrRealmUrl + sRequest.strEndpoint;
            std::string strRequestID;
            GenerateRequestID(strRequestID);
            sRequestDetails.sHeaders.SetValueForKey("X-RequestID", strRequestID);
            
            Json::FastWriter jWriter;
			sRequestDetails.strBody = jWriter.write(sRequest.jBody);
            
            HttpRequest* pRequest = mpHttpSystem->MakeRequest(sRequestDetails, Core::MakeDelegate(this, &MoMetricsSession::OnQueuedRequestComplete));
            
            mmapRequestToDelegate.insert(std::make_pair(pRequest, sRequest.Delegate));
        }
        
        void MoMetricsSession::OnQueuedRequestComplete(HttpRequest* inpRequest, HttpRequest::CompletionResult ineResult)
        {
            mQueuedRequests.pop_front();
            
            mbRequestInProgress = false;
            
            std::unordered_map<HttpRequest*, HttpRequest::CompletionDelegate>::iterator it = mmapRequestToDelegate.find(inpRequest);
            
            if(it != mmapRequestToDelegate.end())
            {
                HttpRequest::CompletionDelegate Delegate = it->second;
                mmapRequestToDelegate.erase(it);
                Delegate(inpRequest, ineResult);
            }
            
            MakeNextRequest();
        }
        
		void MoMetricsSession::QueueEvent(const MetricsEvent & insEvent)
        {
			mQueuedEvents.push_back(insEvent);
            
			RequestFlushBatchEvents();
		}
        
        void MoMetricsSession::AppendAuthDetails(MetricsRequest& insRequest) const
        {
			Json::Value jAuth;
			
			jAuth["SessionID"] = mstrID;
			jAuth["Password"] = mstrToken;
			
			insRequest.jBody["Auth"] = jAuth;
		}
        
        void MoMetricsSession::RequestFlushBatchEvents()
        {
			bool bMeetsBatchRequirement = (mQueuedEvents.size() >= kudwMaxEventsPerBatch);
            
			if (bMeetsBatchRequirement && !mbRequestInProgress && IsOpen())
            {
                QueueAction(Action::k_flushBatch);
			}
		}
        
        void MoMetricsSession::RequestFlushEvents()
        {
            u32 udwNumFullBatched = mQueuedEvents.size()/kudwMaxEventsPerBatch;
            u32 udwNumRemaining = mQueuedEvents.size()%kudwMaxEventsPerBatch;
            for(u32 i=0; i<udwNumFullBatched; ++i)
            {
                QueueAction(Action::k_flushBatch);
            }
            
            if(udwNumRemaining > 0)
            {
                QueueAction(Action::k_flushBatch);
            }
        }
        
        void MoMetricsSession::CompressEventBatch()
        {
            for(std::deque<MetricsEvent>::iterator iterOuter = mQueuedEvents.begin(); iterOuter < mQueuedEvents.end(); ++iterOuter)
            {
                if(iterOuter->bSummarisable)
                {
                    for(std::deque<MetricsEvent>::iterator iterInner = iterOuter+1; iterInner < mQueuedEvents.end(); ++iterInner)
                    {
                        if((iterOuter->bIgnore) || (iterOuter->astrParams.size() != iterInner->astrParams.size()) || (iterOuter->strType != iterInner->strType))
                        {
                            continue;
                        }
                        
                        bool bDoParamsMatch = true;
                        
                        for(u32 i=0; i<iterInner->astrParams.size(); ++i)
                        {
                            if(iterOuter->astrParams[0] != iterInner->astrParams[0])
                            {
                                bDoParamsMatch = false;
                                break;
                            }
                        }
                        
                        if(bDoParamsMatch)
                        {
                            iterOuter->udwCount++;
                            
                            iterInner->bIgnore = true;
                        }
                    }
                }
            }
        }
        
		void MoMetricsSession::MakeFlushRequest(u32 inudwNumEventsToFlush)
        {
            CompressEventBatch();
            
			Json::Value jMessage;
			Json::Value jEvents(Json::arrayValue);
            
			mudwCurrentBatchSize = inudwNumEventsToFlush;
			
			for (u32 i=0; i<inudwNumEventsToFlush; ++i)
            {
                std::deque<MetricsEvent>::const_iterator it = mQueuedEvents.begin() + i;
                
                if(it == mQueuedEvents.end())
                    break;
                
                if(!it->bIgnore)
                {
                    Json::Value jEvent;
                    it->ToJsonForServer(jEvent);
                    
                    //Add the constants
                    for(std::unordered_map<std::string, std::string>::const_iterator it = mmapConstants.begin(); it != mmapConstants.end(); ++it)
                    {
                        jEvent[it->first] = it->second;
                    }
                    
                    jEvents.append(jEvent);
				}
			}
			
			jMessage["Data"] = jEvents;
            
            MetricsRequest sRequest = {jMessage, "/events/add", Core::MakeDelegate(this, &MoMetricsSession::FlushEventsRequestCompletes), true};
            QueueRequest(sRequest);
            MakeNextRequest();
		}
		
		void MoMetricsSession::FlushEventsRequestCompletes(HttpRequest* inpRequest, HttpRequest::CompletionResult ineResult)
        {
			if (ineResult == HttpRequest::CompletionResult::k_completed)
            {
                if(inpRequest->GetResponseCode() == kHTTPBusy)
                {
                    OnActionRetry();
                    return;
                }
			}
            
            //We have no way of handling an error except to push on as if nothing has happened
            u32 udwNumPops = 0;
            while ((udwNumPops < mudwCurrentBatchSize) && !mQueuedEvents.empty())
            {
                mQueuedEvents.pop_front();
                udwNumPops++;
            }
            
            RequestFlushBatchEvents();
            OnActionComplete();
		}
        
        void MoMetricsSession::RequestClose()
        {
            QueueAction(Action::k_closeSession);
        }
        
        void MoMetricsSession::MakeCloseRequest()
        {
            Json::Value jPostBody(Json::objectValue);
			jPostBody["Timestamp"] = (u32)GetLastActivityTime();
            
            MetricsRequest sRequest = {jPostBody, "/session/close", Core::MakeDelegate(this, &MoMetricsSession::OnCloseRequestComplete), true};
            QueueRequest(sRequest);
            MakeNextRequest();
			
			mstrID.clear();
			mstrToken.clear();
        }
        
        void MoMetricsSession::OnCloseRequestComplete(HttpRequest* inpRequest, HttpRequest::CompletionResult ineResult)
        {
            if (ineResult == HttpRequest::CompletionResult::k_completed)
            {
                if(inpRequest->GetResponseCode() == kHTTPBusy)
                {
                    OnActionRetry();
                }
            }
            
            mbIsClosed = true;
            OnActionComplete();
        }
        
        void MoMetricsSession::Destroy()
        {
            for(std::unordered_map<HttpRequest*, HttpRequest::CompletionDelegate>::iterator it = mmapRequestToDelegate.begin(); it != mmapRequestToDelegate.end(); ++it)
            {
                HttpRequest* pRequest = it->first;
                if(pRequest)
                {
                    pRequest->Cancel();
                }
            }
            
            mmapRequestToDelegate.clear();
        }
        
        MoMetricsSession::~MoMetricsSession()
        {
            Destroy();
        }
    }
}