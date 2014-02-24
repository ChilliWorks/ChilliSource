/*
 *  MoMetricsSystem.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 25/05/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_NETWORKING_MOMETRICSSYSTEM_H_
#define _MOFLO_NETWORKING_MOMETRICSSYSTEM_H_

#include <ChilliSource/Networking/ForwardDeclarations.h>
#include <ChilliSource/Core/System/System.h>

#include <ChilliSource/Networking/Metrics/MoMetricsSession.h>

#define RECORD_METRICS_EVENT(type, params, summarise)\
moFlo::Networking::CMoMetricsSystem::GetSingleton().RecordEvent(type, params, summarise)

#define RECORD_METRICS_EVENT0(type, summarise)\
moFlo::Networking::CMoMetricsSystem::GetSingleton().RecordEvent(type, DYNAMIC_ARRAY<std::string>(), summarise)

#define RECORD_METRICS_EVENT1(type, p1, summarise)\
{DYNAMIC_ARRAY<std::string> aParams; aParams.reserve(1);\
aParams.push_back(p1);\
moFlo::Networking::CMoMetricsSystem::GetSingleton().RecordEvent(type, aParams, summarise);}

#define RECORD_METRICS_EVENT2(type, p1, p2, summarise)\
{DYNAMIC_ARRAY<std::string> aParams; aParams.reserve(2);\
aParams.push_back(p1);\
aParams.push_back(p2);\
moFlo::Networking::CMoMetricsSystem::GetSingleton().RecordEvent(type, aParams, summarise);}

#define RECORD_METRICS_EVENT3(type, p1, p2, p3, summarise)\
{DYNAMIC_ARRAY<std::string> aParams; aParams.reserve(3);\
aParams.push_back(p1);\
aParams.push_back(p2);\
aParams.push_back(p3);\
moFlo::Networking::CMoMetricsSystem::GetSingleton().RecordEvent(type, aParams, summarise);}

#define RECORD_METRICS_EVENT4(type, p1, p2, p3, p4, summarise)\
{DYNAMIC_ARRAY<std::string> aParams; aParams.reserve(4);\
aParams.push_back(p1);\
aParams.push_back(p2);\
aParams.push_back(p3);\
aParams.push_back(p4);\
moFlo::Networking::CMoMetricsSystem::GetSingleton().RecordEvent(type, aParams, summarise);}

#define RECORD_METRICS_EVENT5(type, p1, p2, p3, p4, p5, summarise) {DYNAMIC_ARRAY<std::string> aParams; aParams.reserve(5);\
aParams.push_back(p1);\
aParams.push_back(p2);\
aParams.push_back(p3);\
aParams.push_back(p4);\
aParams.push_back(p5);\
moFlo::Networking::CMoMetricsSystem::GetSingleton().RecordEvent(type, aParams, summarise);}

#define RECORD_METRICS_EVENT6(type, p1, p2, p3, p4, p5, p6, summarise) {DYNAMIC_ARRAY<std::string> aParams; aParams.reserve(6);\
aParams.push_back(p1);\
aParams.push_back(p2);\
aParams.push_back(p3);\
aParams.push_back(p4);\
aParams.push_back(p5);\
aParams.push_back(p6);\
moFlo::Networking::CMoMetricsSystem::GetSingleton().RecordEvent(type, aParams, summarise);}


namespace ChilliSource 
{
	namespace Networking 
    {
		class CMoMetricsSystem : public Core::ISystem 
        {
		public:
			DECLARE_NAMED_INTERFACE(CMoMetricsSystem);
			
			CMoMetricsSystem(IHttpConnectionSystem * inpHttpSystem, const std::string& instrMoMetricsServerURL, const std::string& instrAppID, IExternalMetrics* inpExternalMetrics = NULL);
            ~CMoMetricsSystem();
			
            bool IsA(Core::InterfaceIDType inInterfaceID) const;
			
			static CMoMetricsSystem & GetSingleton();
			static CMoMetricsSystem * GetSingletonPtr();
            
            void StartSessions();
            void StopSessions();
            
            void SetConstant(const std::string& instrKey, const std::string& instrValue);
            
            void SetLocation(const MetricsLocation& insLocation);
            
            void SetServerTimeDelta(s32 indwServerDelta);
            
			void RecordEvent(const std::string & instrType, const DYNAMIC_ARRAY<std::string>& inastrParams, bool inbSummarise);
            
            void ForceFlushCache();
			
		private:

            void LoadPendingSessions();
            void FlushPendingClosedSessions();
            
            void Destroy();
			
        private:
			
			std::string mstrAppID;
            std::string mstrMoMetricsURL;
			
            DYNAMIC_ARRAY<CMoMetricsSession*> maPendingClosedSessions;
            CMoMetricsSession* mpCurrentSession;
            
            IHttpConnectionSystem * mpHttpSystem;
            
            bool mbSessionsStarted;
            
            IExternalMetrics* mpExternalMetrics;
            
            static CMoMetricsSystem * mpSingletonInstance;
		};
		
	}
}

#endif
