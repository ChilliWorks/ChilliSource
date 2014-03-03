/*
 *  AndroidHttpConnectionSystem.cpp
 *  moFlow
 *
 *  Created by Ian Copland on 08/11/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Networking/Http/HttpConnectionSystem.h>

#include <ChilliSource/Backend/Platform/Android/Networking/Http/HttpConnectionJavaInterface.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/Math/MathUtils.h>

#include <sstream>

namespace ChilliSource
{
	namespace Android
	{
		const u32 kudwBufferSize = 1024 * 50;
		//--------------------------------------------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------------------------------------------
		bool CHttpConnectionSystem::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Networking::HttpConnectionSystem::InterfaceID || inInterfaceID == IUpdateable::InterfaceID;
		}
		//--------------------------------------------------------------------------------------------------
		/// Make Request 
		//--------------------------------------------------------------------------------------------------
		Networking::HttpRequest* CHttpConnectionSystem::MakeRequest(const Networking::HttpRequestDetails & insRequestDetails, Networking::HttpRequest::CompletionDelegate inOnComplete)
        {
            //The ownership of the request is with the request itself
			CHttpRequest* pRequest = new CHttpRequest(insRequestDetails, inOnComplete);
            
			mapRequests.push_back(pRequest);
			return pRequest;
		}
		//--------------------------------------------------------------------------------------------------
		/// Cancel All Requests
		//--------------------------------------------------------------------------------------------------
		void CHttpConnectionSystem::CancelAllRequests()
        {
			for(u32 nRequest = 0; nRequest < mapRequests.size(); nRequest++)
            {
				mapRequests[nRequest]->Cancel();
			}
		}
        //--------------------------------------------------------------------------------------------------
        /// Check Reachability
        //--------------------------------------------------------------------------------------------------
        bool CHttpConnectionSystem::CheckReachability() const
        {
            return SCHttpConnectionJavaInterface::IsConnected();
        }
		//--------------------------------------------------------------------------------------------------
		/// Update
		//--------------------------------------------------------------------------------------------------
		void CHttpConnectionSystem::Update(f32 infDT)
		{
            RequestVector RequestCopy = mapRequests;
            
            //We should do this in two loops incase anyone tries to insert from the callback
			for(RequestVector::iterator it = RequestCopy.begin(); it != RequestCopy.end(); ++it) 
            {
                (*it)->Update(infDT);
            }

            for(RequestVector::iterator it = mapRequests.begin(); it != mapRequests.end(); )
            {
                if((*it)->HasCompleted())
                {
                    CS_SAFEDELETE(*it);
                    mapRequests.erase(it);
                }
                else
                {
                    ++it;
                }
			}
		}		
		//=====================================================================================================
		/// Http Request
		//=====================================================================================================
		CHttpConnectionSystem::CHttpRequest::CHttpRequest(const Networking::HttpRequestDetails& insDetails,
														  const Networking::HttpRequest::CompletionDelegate& inCompletionDelegate) : msDetails(insDetails),
														  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  mbCompleted(false),
														  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  mCompletionDelegate(inCompletionDelegate),
														  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  mbThreadCompleted(false),
														  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  mudwResponseCode(0),
														  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  mbRequestCompleted(false)
		{
			//Begin the read loop
			//Run this as a threaded task
			Core::TaskScheduler::ScheduleTask(Core::Task<>(this, &CHttpConnectionSystem::CHttpRequest::PerformRequest));
		}
		//------------------------------------------------------------------
		/// Update
		//------------------------------------------------------------------
		void CHttpConnectionSystem::CHttpRequest::Update(f32 infDT)
		{
			//Check if the data has finished streaming and invoke the completion delegate on the main thread
			if(mbCompleted == true)
			{
                mbRequestCompleted = true;
                if(mCompletionDelegate != nullptr)
                {
                    if(meRequestResult != HttpRequest::CompletionResult::k_cancelled)
                    {
                    	mCompletionDelegate(this, meRequestResult);
                    }

                    mCompletionDelegate = nullptr;
                }
			}
		}
		//------------------------------------------------------------------
		/// Poll Read Stream
		//------------------------------------------------------------------
		void CHttpConnectionSystem::CHttpRequest::PerformRequest()
		{
			HttpRequestType eType = HttpRequestType::k_get;
			if (msDetails.eType == Networking::HttpRequestDetails::Type::k_post)
				eType = HttpRequestType::k_post;

			CS_LOG_DEBUG("CHttpConnectionSystem::CHttpRequest::PerformRequest() - Check for request type...");

			s32 dwResponseCode;
			HttpRequestResultCode eRequestResult;
			if(msDetails.sHeaders.empty())
			{
				CS_LOG_DEBUG("CHttpConnectionSystem::CHttpRequest::PerformRequest() - Performing standard request...");
				eRequestResult = SCHttpConnectionJavaInterface::HttpRequest(msDetails.strURL, eType, msDetails.strBody, mResponseData, msDetails.strRedirectionURL, dwResponseCode);
			}
			else
			{
				CS_LOG_DEBUG("CHttpConnectionSystem::CHttpRequest::PerformRequest() - Performing request with headers...");
				eRequestResult = SCHttpConnectionJavaInterface::HttpRequest(msDetails.strURL, eType, msDetails.sHeaders, msDetails.strBody, mResponseData, msDetails.strRedirectionURL, dwResponseCode, true /* TODO: Add bKeepAlive boolean to msDetails*/);
			}
			mudwResponseCode = dwResponseCode;

			if (mbCompleted == false)
			{
				switch (eRequestResult)
				{
				case HttpRequestResultCode::k_success:
					meRequestResult = HttpRequest::CompletionResult::k_completed;
					break;
				case HttpRequestResultCode::k_couldNotConnect:
				case HttpRequestResultCode::k_couldNotMakeRequest:
					meRequestResult = HttpRequest::CompletionResult::k_failed;
					break;
				case HttpRequestResultCode::k_timeout:
					meRequestResult = HttpRequest::CompletionResult::k_timeout;
					break;
				default:
					break;
				}
			}

			mbCompleted = true;
            mbThreadCompleted = true;
		}
		//----------------------------------------------------------------------------------------
		/// Cancel
		//----------------------------------------------------------------------------------------
		void CHttpConnectionSystem::CHttpRequest::Cancel()
		{
            mbCompleted = true;
            meRequestResult = HttpRequest::CompletionResult::k_cancelled;
		}
		//----------------------------------------------------------------------------------------
		/// Has Completed
		//----------------------------------------------------------------------------------------
		bool CHttpConnectionSystem::CHttpRequest::HasCompleted() const
		{
			return mbRequestCompleted && mbThreadCompleted;
		}
		//----------------------------------------------------------------------------------------
		/// Get Details
		//----------------------------------------------------------------------------------------
		const Networking::HttpRequestDetails & CHttpConnectionSystem::CHttpRequest::GetDetails() const
		{
			return msDetails;
		}
		//----------------------------------------------------------------------------------------
		/// Get Completion Delegate
		//----------------------------------------------------------------------------------------
		const Networking::HttpRequest::CompletionDelegate & CHttpConnectionSystem::CHttpRequest::GetCompletionDelegate() const
		{
			return mCompletionDelegate;
		}
		//----------------------------------------------------------------------------------------
		/// Get Response String
		//----------------------------------------------------------------------------------------
		const std::string & CHttpConnectionSystem::CHttpRequest::GetResponseString() const
		{
			return mResponseData;
		}
		//----------------------------------------------------------------------------------------
		/// Get Response Code
		///
		/// @return HTTP response code (i.e. 200 = OK, 400 = Error)
		//----------------------------------------------------------------------------------------
		u32 CHttpConnectionSystem::CHttpRequest::GetResponseCode() const 
		{
			return mudwResponseCode;
		}
        //----------------------------------------------------------------------------------------
		/// Get Bytes Read
		///
		/// @return Number of bytes read til now
		//----------------------------------------------------------------------------------------
		u32 CHttpConnectionSystem::CHttpRequest::GetBytesRead() const
		{
			//TODO: Implement this.
			return 0;
		}
	}
}
