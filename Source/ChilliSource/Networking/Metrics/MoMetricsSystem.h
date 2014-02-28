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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Networking/Metrics/MoMetricsSession.h>

namespace ChilliSource 
{
	namespace Networking 
    {
		class MoMetricsSystem : public Core::System 
        {
		public:
			CS_DECLARE_NAMEDTYPE(MoMetricsSystem);
			
			MoMetricsSystem(HttpConnectionSystem * inpHttpSystem, const std::string& instrMoMetricsServerURL, const std::string& instrAppID, IExternalMetrics* inpExternalMetrics = nullptr);
            ~MoMetricsSystem();
			
            bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			
			static MoMetricsSystem & GetSingleton();
			static MoMetricsSystem * GetSingletonPtr();
            
            void StartSessions();
            void StopSessions();
            
            void SetConstant(const std::string& instrKey, const std::string& instrValue);
            
            void SetLocation(const MetricsLocation& insLocation);
            
            void SetServerTimeDelta(s32 indwServerDelta);
            
			void RecordEvent(const std::string & instrType, const std::vector<std::string>& inastrParams, bool inbSummarise);
            
            void ForceFlushCache();
			
		private:

            void LoadPendingSessions();
            void FlushPendingClosedSessions();
            
            void Destroy();
			
        private:
			
			std::string mstrAppID;
            std::string mstrMoMetricsURL;
			
            std::vector<MoMetricsSession*> maPendingClosedSessions;
            MoMetricsSession* mpCurrentSession;
            
            HttpConnectionSystem * mpHttpSystem;
            
            bool mbSessionsStarted;
            
            IExternalMetrics* mpExternalMetrics;
            
            static MoMetricsSystem * mpSingletonInstance;
		};
		
	}
}

#endif
