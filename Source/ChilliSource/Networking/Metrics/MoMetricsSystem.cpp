/*
 *  MoMetricsSystem.cpp
 *  moFlow
 *
 *  Created by Stuart McGaw on 25/05/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Networking/Metrics/MoMetricsSystem.h>

#include <ChilliSource/Core/Base/ApplicationEvents.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Device.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>

namespace ChilliSource
{
	using namespace Core;
	
	namespace Networking
    {
		DEFINE_NAMED_INTERFACE(CMoMetricsSystem);
		
		CMoMetricsSystem * CMoMetricsSystem::mpSingletonInstance = nullptr;
		
		CMoMetricsSystem::CMoMetricsSystem(IHttpConnectionSystem * inpHttpSystem, const std::string& instrMoMetricsServerURL, const std::string& instrAppID, IExternalMetrics* inpExternalMetrics)
		: mpHttpSystem(inpHttpSystem), mstrMoMetricsURL(instrMoMetricsServerURL), mstrAppID(instrAppID), mpCurrentSession(nullptr), mbSessionsStarted(false), mpExternalMetrics(inpExternalMetrics)
		{		
			mpSingletonInstance = this;
		}
		
		bool CMoMetricsSystem::IsA(Core::InterfaceIDType inInterfaceID) const
        {
			return inInterfaceID == CMoMetricsSystem::InterfaceID;
		}
        
		CMoMetricsSystem & CMoMetricsSystem::GetSingleton()
        {
			return *mpSingletonInstance;
		}
        
		CMoMetricsSystem * CMoMetricsSystem::GetSingletonPtr()
        {
			return mpSingletonInstance;
		}
        
        void CMoMetricsSystem::SetConstant(const std::string& instrKey, const std::string& instrValue)
        {
            MOFLOW_ASSERT(mpCurrentSession, "Cannot set constant without first starting a session");
            if(mpCurrentSession)
            {
                mpCurrentSession->SetConstant(instrKey, instrValue);
            }
        }
        
        void CMoMetricsSystem::SetLocation(const MetricsLocation& insLocation)
        {
            MOFLOW_ASSERT(mpCurrentSession, "Cannot set location without first starting a session");
            if(mpCurrentSession)
            {
                mpCurrentSession->RequestLocationUpdate(insLocation);
            }
        }
        
        void CMoMetricsSystem::SetServerTimeDelta(s32 indwServerDelta)
		{
            MOFLOW_ASSERT(mpCurrentSession, "Cannot set server time delta without first starting a session");
            if(mpCurrentSession)
            {
                mpCurrentSession->SetServerTimeDelta(indwServerDelta);
            }
		}
		
		void CMoMetricsSystem::StartSessions()
        {
            if(mbSessionsStarted)
                return;
            
            mbSessionsStarted = true;
            
            LoadPendingSessions();
            FlushPendingClosedSessions();
            
            if(mpCurrentSession)
            {
                if(!mpCurrentSession->IsOpen())
                {
                    mpCurrentSession->RequestAuthTokens();
                }

                if(mpCurrentSession->IsExpired())
                {
                    mpCurrentSession->RequestFlushEvents();
                    mpCurrentSession->RequestClose();
                    
                    maPendingClosedSessions.push_back(mpCurrentSession);
                    mpCurrentSession = nullptr;
                }
            }
            
            if(!mpCurrentSession)
            {
                mpCurrentSession = new CMoMetricsSession(mpHttpSystem, mstrMoMetricsURL, mstrAppID, mpExternalMetrics);
                mpCurrentSession->RequestAuthTokens();
            }
		}
        
        void CMoMetricsSystem::LoadPendingSessions()
        {
            Json::Value jSessions;
            if(Core::CUtils::ReadJson(Core::StorageLocation::k_cache, "PendingSessions.mometrics", &jSessions))
            {
                if(jSessions.isMember("Closed"))
                {
                    Json::Value jClosedSessions = jSessions["Closed"];
                    maPendingClosedSessions.reserve(jClosedSessions.size());
                    
                    for(u32 i=0; i<jClosedSessions.size(); ++i)
                    {
                        CMoMetricsSession* pSession = new CMoMetricsSession(mpHttpSystem, mstrMoMetricsURL, mstrAppID);
                        pSession->LoadFromCache(jClosedSessions[i].asString());
                        maPendingClosedSessions.push_back(pSession);
                    }
                }
                
                if(jSessions.isMember("Current"))
                {
                    if(!jSessions["Current"].isNull())
                    {
                        mpCurrentSession = new CMoMetricsSession(mpHttpSystem, mstrMoMetricsURL, mstrAppID);
                        mpCurrentSession->LoadFromCache(jSessions["Current"].asString());
                    }
                }
                
                Core::CApplication::GetFileSystemPtr()->DeleteFile(Core::StorageLocation::k_cache, "PendingSessions.mometrics");
            }
        }
        
        void CMoMetricsSystem::FlushPendingClosedSessions()
        {
            for(std::vector<CMoMetricsSession*>::iterator it = maPendingClosedSessions.begin(); it != maPendingClosedSessions.end(); ++it)
            {
                if(!(*it)->IsOpen())
                {
                    (*it)->RequestAuthTokens();
                }
                
                (*it)->RequestFlushEvents();
                (*it)->RequestClose();
            }
        }
        
        void CMoMetricsSystem::RecordEvent(const std::string & instrType, const std::vector<std::string>& inastrParams, bool inbSummarise)
        {
            MOFLOW_ASSERT(mpCurrentSession, "Cannot record event without first starting a session");
            if(mpCurrentSession)
            {
                mpCurrentSession->RecordEvent(instrType, inastrParams, inbSummarise);
            }
		}
    
        void CMoMetricsSystem::ForceFlushCache()
        {
            if(mpCurrentSession && mpCurrentSession->IsOpen())
            {
                mpCurrentSession->RequestFlushEvents();
            }
        }
        
        void CMoMetricsSystem::StopSessions()
        {
            //Save the cached metrics
            Json::Value jClosed;
            for(std::vector<CMoMetricsSession*>::iterator it = maPendingClosedSessions.begin(); it != maPendingClosedSessions.end(); ++it)
            {
                if(!(*it)->IsClosed())
                {
                    jClosed.append((*it)->GetID());
                    (*it)->SaveToCache();
                }
            }
            
            Json::Value jSessions;
            jSessions["Closed"] = jClosed;
            
            if(mpCurrentSession)
            {
            	jSessions["Current"] = mpCurrentSession->GetID();
            	mpCurrentSession->SaveToCache();
            }
            
            Destroy();
            
            Core::CUtils::StringToFile(Core::StorageLocation::k_cache, "PendingSessions.mometrics", jSessions.toUnformattedString());
            
            mbSessionsStarted = false;
		}
        
        void CMoMetricsSystem::Destroy()
        {
            SAFE_DELETE(mpCurrentSession);
            
            for(std::vector<CMoMetricsSession*>::iterator it = maPendingClosedSessions.begin(); it != maPendingClosedSessions.end(); ++it)
            {
                CMoMetricsSession* pSession = (*it);
                delete pSession;
            }
            maPendingClosedSessions.clear();
        }
        
        CMoMetricsSystem::~CMoMetricsSystem()
        {
            Destroy();
        }
	}
}
