//
//  MoMetricsSession.h
//  MoFlow
//
//  Created by Scott Downie on 04/03/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#ifndef _MOFLO_NETWORKING_MOMETRICS_SESSION_H_
#define _MOFLO_NETWORKING_MOMETRICS_SESSION_H_

#include <ChilliSource/Networking/ForwardDeclarations.h>
#include <ChilliSource/Networking/Http/HttpConnectionSystem.h>

#include <ChilliSource/Core/JSON/json.h>

#include <queue>

namespace moFlo
{
    namespace Networking
    {
        struct MetricsEvent
        {
			std::string strType;
            DYNAMIC_ARRAY<std::string> astrParams;
            u32 udwCount;
			u32 udwTimeStamp;
            bool bSummarisable;
            bool bIgnore;
			
			void ToJsonForServer(Json::Value & incValue) const;
            void ToJsonForCache(Json::Value & incValue) const;
            
			void FromCachedJson(const Json::Value & incValue);
		};
        
        struct MetricsRequest
        {
            Json::Value jBody;
            std::string strEndpoint;
            IHttpRequest::CompletionDelegate Delegate;
            bool bRequiresAuth;
        };
        
        struct MetricsLocation
        {
            std::string strCountry;
        };
        
        class IExternalMetrics
        {
        public:
            virtual void MakeRequest(MetricsRequest& insRequest) = 0;

        };
        
        typedef SHARED_PTR<IExternalMetrics> ExternalMetricsPtr;
        
        
        class CMoMetricsSession
        {
        public:
            
            CMoMetricsSession(IHttpConnectionSystem * inpHttpSystem, const std::string& instrMetricsUrl, const std::string& instrAppID, IExternalMetrics* inpExternalMetrics = NULL);
            ~CMoMetricsSession();
            
            void SetConstant(const std::string& instrKey, const std::string& instrValue);
            bool ContainsConstant(const std::string& instrKey) const;
            
            void SetServerTimeDelta(s32 indwServerDelta);
            
            void RequestAuthTokens();
            void RequestLocationUpdate(const MetricsLocation& insLocation);
            void RecordEvent(const std::string & instrType, const DYNAMIC_ARRAY<std::string>& inastrParams, bool inbSummarise);
            void RequestFlushEvents();
            void RequestClose();
            
            bool IsExpired() const;
            bool IsOpen() const;
            bool IsClosed() const;
            
            void LoadFromCache(const std::string& instrID);
            void SaveToCache();
            
            const std::string& GetID() const;
            
        private:
            
            enum Action
            {
                CREATE_SESSION,
                UPDATE_LOCATION,
                FLUSH_BATCH,
                CLOSE_SESSION
            };
            
            bool CanPerformNextAction() const;
            void PerformNextAction();
            void OnActionComplete();
            void OnActionRetry();
            
            void MakeSessionRequest();
            void OnAuthTokensRequestComplete(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult);
            
            void MakeLocationUpdateRequest(const MetricsLocation& insLocation);
            void OnLocationUpdateRequestComplete(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult);
            
            void MakeCloseRequest();
            void OnCloseRequestComplete(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult);
            
            void UpdateLastActivityTime();
            TimeIntervalSecs GetLastActivityTime() const;
            
            void QueueAction(Action ineAction);
            
            void QueueEvent(const MetricsEvent & insEvent);
            
            void QueueRequest(const MetricsRequest& insRequest);
            void MakeNextRequest();
            void OnQueuedRequestComplete(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult);
            
            void CompressEventBatch();
            void RequestFlushBatchEvents();
            
            void AppendAuthDetails(MetricsRequest& insRequest) const;
			void MakeFlushRequest(u32 inudwNumEventsToFlush);
			void FlushEventsRequestCompletes(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult);
            
            void Destroy();

        private:
            
            std::deque<MetricsEvent> mQueuedEvents;
            std::deque<MetricsRequest> mQueuedRequests;
            std::deque<Action> mQueuedActions;
            
            HASH_MAP<HttpRequestPtr, IHttpRequest::CompletionDelegate> mmapRequestToDelegate;
            
            HASH_MAP<std::string, std::string> mmapConstants;
            
            MetricsLocation msLocation;
            
            std::string mstrID;
            std::string mstrToken;
            
            std::string mstrAppID;
			
            std::string mstrRealmUrl;
			
			s32 mdwServerTimeDelta;
            
            TimeIntervalSecs mLastActivityTime;
            TimeIntervalSecs mLastActionTime;
            
            u32 mudwCurrentBatchSize;
            u32 mudwCurrentDelayIndex;
            
            bool mbIsClosed;
            
            bool mbRequestInProgress;
            bool mbActionInProgress;
            IHttpConnectionSystem * mpHttpSystem;
            
            IExternalMetrics* mpExternalMetrics;
        };
    }
}

#endif

