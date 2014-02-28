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
		CS_DEFINE_NAMEDTYPE(MoMetricsSystem);
		
		MoMetricsSystem * MoMetricsSystem::mpSingletonInstance = nullptr;
		
		MoMetricsSystem::MoMetricsSystem(HttpConnectionSystem * inpHttpSystem, const std::string& instrMoMetricsServerURL, const std::string& instrAppID, IExternalMetrics* inpExternalMetrics)
		: mpHttpSystem(inpHttpSystem), mstrMoMetricsURL(instrMoMetricsServerURL), mstrAppID(instrAppID), mpCurrentSession(nullptr), mbSessionsStarted(false), mpExternalMetrics(inpExternalMetrics)
		{		
			mpSingletonInstance = this;
		}
		
		bool MoMetricsSystem::IsA(Core::InterfaceIDType inInterfaceID) const
        {
			return inInterfaceID == MoMetricsSystem::InterfaceID;
		}
        
		MoMetricsSystem & MoMetricsSystem::GetSingleton()
        {
			return *mpSingletonInstance;
		}
        
		MoMetricsSystem * MoMetricsSystem::GetSingletonPtr()
        {
			return mpSingletonInstance;
		}
        
        void MoMetricsSystem::SetConstant(const std::string& instrKey, const std::string& instrValue)
        {
            CS_ASSERT(mpCurrentSession, "Cannot set constant without first starting a session");
            if(mpCurrentSession)
            {
                mpCurrentSession->SetConstant(instrKey, instrValue);
            }
        }
        
        void MoMetricsSystem::SetLocation(const MetricsLocation& insLocation)
        {
            CS_ASSERT(mpCurrentSession, "Cannot set location without first starting a session");
            if(mpCurrentSession)
            {
                mpCurrentSession->RequestLocationUpdate(insLocation);
            }
        }
        
        void MoMetricsSystem::SetServerTimeDelta(s32 indwServerDelta)
		{
            CS_ASSERT(mpCurrentSession, "Cannot set server time delta without first starting a session");
            if(mpCurrentSession)
            {
                mpCurrentSession->SetServerTimeDelta(indwServerDelta);
            }
		}
		
		void MoMetricsSystem::StartSessions()
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
                mpCurrentSession = new MoMetricsSession(mpHttpSystem, mstrMoMetricsURL, mstrAppID, mpExternalMetrics);
                mpCurrentSession->RequestAuthTokens();
            }
		}
        
        void MoMetricsSystem::LoadPendingSessions()
        {
            Json::Value jSessions;
            if(Core::Utils::ReadJson(Core::StorageLocation::k_cache, "PendingSessions.mometrics", &jSessions))
            {
                if(jSessions.isMember("Closed"))
                {
                    Json::Value jClosedSessions = jSessions["Closed"];
                    maPendingClosedSessions.reserve(jClosedSessions.size());
                    
                    for(u32 i=0; i<jClosedSessions.size(); ++i)
                    {
                        MoMetricsSession* pSession = new MoMetricsSession(mpHttpSystem, mstrMoMetricsURL, mstrAppID);
                        pSession->LoadFromCache(jClosedSessions[i].asString());
                        maPendingClosedSessions.push_back(pSession);
                    }
                }
                
                if(jSessions.isMember("Current"))
                {
                    if(!jSessions["Current"].isNull())
                    {
                        mpCurrentSession = new MoMetricsSession(mpHttpSystem, mstrMoMetricsURL, mstrAppID);
                        mpCurrentSession->LoadFromCache(jSessions["Current"].asString());
                    }
                }
                
                Core::Application::GetFileSystemPtr()->DeleteFile(Core::StorageLocation::k_cache, "PendingSessions.mometrics");
            }
        }
        
        void MoMetricsSystem::FlushPendingClosedSessions()
        {
            for(std::vector<MoMetricsSession*>::iterator it = maPendingClosedSessions.begin(); it != maPendingClosedSessions.end(); ++it)
            {
                if(!(*it)->IsOpen())
                {
                    (*it)->RequestAuthTokens();
                }
                
                (*it)->RequestFlushEvents();
                (*it)->RequestClose();
            }
        }
        
        void MoMetricsSystem::RecordEvent(const std::string & instrType, const std::vector<std::string>& inastrParams, bool inbSummarise)
        {
            CS_ASSERT(mpCurrentSession, "Cannot record event without first starting a session");
            if(mpCurrentSession)
            {
                mpCurrentSession->RecordEvent(instrType, inastrParams, inbSummarise);
            }
		}
    
        void MoMetricsSystem::ForceFlushCache()
        {
            if(mpCurrentSession && mpCurrentSession->IsOpen())
            {
                mpCurrentSession->RequestFlushEvents();
            }
        }
        
        void MoMetricsSystem::StopSessions()
        {
            //Save the cached metrics
            Json::Value jClosed;
            for(std::vector<MoMetricsSession*>::iterator it = maPendingClosedSessions.begin(); it != maPendingClosedSessions.end(); ++it)
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
            
            Core::Utils::StringToFile(Core::StorageLocation::k_cache, "PendingSessions.mometrics", jSessions.toUnformattedString());
            
            mbSessionsStarted = false;
		}
        
        void MoMetricsSystem::Destroy()
        {
            CS_SAFEDELETE(mpCurrentSession);
            
            for(std::vector<MoMetricsSession*>::iterator it = maPendingClosedSessions.begin(); it != maPendingClosedSessions.end(); ++it)
            {
                MoMetricsSession* pSession = (*it);
                delete pSession;
            }
            maPendingClosedSessions.clear();
        }
        
        MoMetricsSystem::~MoMetricsSystem()
        {
            Destroy();
        }
	}
}
