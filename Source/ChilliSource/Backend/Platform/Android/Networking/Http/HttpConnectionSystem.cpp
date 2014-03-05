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
		bool HttpConnectionSystem::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Networking::HttpConnectionSystem::InterfaceID || inInterfaceID == IUpdateable::InterfaceID;
		}
		//--------------------------------------------------------------------------------------------------
		/// Make Request 
		//--------------------------------------------------------------------------------------------------
		Networking::HttpRequest* HttpConnectionSystem::MakeRequest(const Networking::HttpRequestDetails & insRequestDetails, Networking::HttpRequest::CompletionDelegate inOnComplete)
        {
            //The ownership of the request is with the request itself
			HttpRequest* pRequest = new HttpRequest(insRequestDetails, inOnComplete);
            
			mapRequests.push_back(pRequest);
			return pRequest;
		}
		//--------------------------------------------------------------------------------------------------
		/// Cancel All Requests
		//--------------------------------------------------------------------------------------------------
		void HttpConnectionSystem::CancelAllRequests()
        {
			for(u32 nRequest = 0; nRequest < mapRequests.size(); nRequest++)
            {
				mapRequests[nRequest]->Cancel();
			}
		}
        //--------------------------------------------------------------------------------------------------
        /// Check Reachability
        //--------------------------------------------------------------------------------------------------
        bool HttpConnectionSystem::CheckReachability() const
        {
            return HttpConnectionJavaInterface::IsConnected();
        }
		//--------------------------------------------------------------------------------------------------
		/// Update
		//--------------------------------------------------------------------------------------------------
		void HttpConnectionSystem::Update(f32 infDT)
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
		HttpConnectionSystem::HttpRequest::HttpRequest(const Networking::HttpRequestDetails& insDetails,
														const Networking::HttpRequest::CompletionDelegate& inCompletionDelegate) : msDetails(insDetails),
														  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  mbCompleted(false),
														  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  mCompletionDelegate(inCompletionDelegate),
														  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  mbThreadCompleted(false),
														  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  mudwResponseCode(0),
														  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  mbRequestCompleted(false)
		{
			//Begin the read loop
			//Run this as a threaded task
			Core::TaskScheduler::ScheduleTask(Core::Task<>(this, &HttpConnectionSystem::HttpRequest::PerformRequest));
		}
		//------------------------------------------------------------------
		/// Update
		//------------------------------------------------------------------
		void HttpConnectionSystem::HttpRequest::Update(f32 infDT)
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
		void HttpConnectionSystem::HttpRequest::PerformRequest()
		{
			HttpRequestType eType = HttpRequestType::k_get;
			if (msDetails.eType == Networking::HttpRequestDetails::Type::k_post)
				eType = HttpRequestType::k_post;

			s32 dwResponseCode;
			HttpRequestResultCode eRequestResult;
			if(msDetails.sHeaders.empty())
			{
				eRequestResult = HttpConnectionJavaInterface::HttpRequest(msDetails.strURL, eType, msDetails.strBody, mResponseData, msDetails.strRedirectionURL, dwResponseCode);
			}
			else
			{
				eRequestResult = HttpConnectionJavaInterface::HttpRequest(msDetails.strURL, eType, msDetails.sHeaders, msDetails.strBody, mResponseData, msDetails.strRedirectionURL, dwResponseCode, true /* TODO: Add bKeepAlive boolean to msDetails*/);
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
		void HttpConnectionSystem::HttpRequest::Cancel()
		{
            mbCompleted = true;
            meRequestResult = HttpRequest::CompletionResult::k_cancelled;
		}
		//----------------------------------------------------------------------------------------
		/// Has Completed
		//----------------------------------------------------------------------------------------
		bool HttpConnectionSystem::HttpRequest::HasCompleted() const
		{
			return mbRequestCompleted && mbThreadCompleted;
		}
		//----------------------------------------------------------------------------------------
		/// Get Details
		//----------------------------------------------------------------------------------------
		const Networking::HttpRequestDetails & HttpConnectionSystem::HttpRequest::GetDetails() const
		{
			return msDetails;
		}
		//----------------------------------------------------------------------------------------
		/// Get Completion Delegate
		//----------------------------------------------------------------------------------------
		const Networking::HttpRequest::CompletionDelegate & HttpConnectionSystem::HttpRequest::GetCompletionDelegate() const
		{
			return mCompletionDelegate;
		}
		//----------------------------------------------------------------------------------------
		/// Get Response String
		//----------------------------------------------------------------------------------------
		const std::string & HttpConnectionSystem::HttpRequest::GetResponseString() const
		{
			return mResponseData;
		}
		//----------------------------------------------------------------------------------------
		/// Get Response Code
		///
		/// @return HTTP response code (i.e. 200 = OK, 400 = Error)
		//----------------------------------------------------------------------------------------
		u32 HttpConnectionSystem::HttpRequest::GetResponseCode() const
		{
			return mudwResponseCode;
		}
        //----------------------------------------------------------------------------------------
		/// Get Bytes Read
		///
		/// @return Number of bytes read til now
		//----------------------------------------------------------------------------------------
		u32 HttpConnectionSystem::HttpRequest::GetBytesRead() const
		{
			//TODO: Implement this.
			return 0;
		}
	}
}
