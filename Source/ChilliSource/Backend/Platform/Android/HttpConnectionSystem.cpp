/*
 *  AndroidHttpConnectionSystem.cpp
 *  moFlow
 *
 *  Created by Ian Copland on 08/11/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Platform/Android/HttpConnectionSystem.h>

#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/Threading/Thread.h>
#include <ChilliSource/Core/Math/MathUtils.h>

#include <sstream>
#include <ChilliSource/Platform/Android/JavaInterface/HttpConnectionJavaInterface.h>

namespace ChilliSource
{
	using namespace Networking;
	
	namespace Android
	{
		const u32 kudwBufferSize = 1024 * 50;
		//--------------------------------------------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------------------------------------------
		bool CHttpConnectionSystem::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IHttpConnectionSystem::InterfaceID || inInterfaceID == IUpdateable::InterfaceID;
		}
		//--------------------------------------------------------------------------------------------------
		/// Make Request 
		//--------------------------------------------------------------------------------------------------
		HttpRequestPtr CHttpConnectionSystem::MakeRequest(const HttpRequestDetails & insRequestDetails, IHttpRequest::CompletionDelegate inOnComplete)
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
                    SAFE_DELETE(*it);
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
														  const Networking::IHttpRequest::CompletionDelegate& inCompletionDelegate) : msDetails(insDetails),
														  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  mbCompleted(false),
														  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  mCompletionDelegate(inCompletionDelegate),
														  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  mbThreadCompleted(false),
														  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  mudwResponseCode(0),
														  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  mbRequestCompleted(false)
		{
			//Begin the read loop
			//Run this as a threaded task
			ChilliSource::CTaskScheduler::ScheduleTask(Task0(this, &CHttpConnectionSystem::CHttpRequest::PerformRequest));
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
                if(mCompletionDelegate != NULL)
                {
                    if(meRequestResult != CANCELLED)
                    {
                    	mCompletionDelegate(this, meRequestResult);
                    }

                    mCompletionDelegate = NULL;
                }
			}
		}
		//------------------------------------------------------------------
		/// Poll Read Stream
		//------------------------------------------------------------------
		void CHttpConnectionSystem::CHttpRequest::PerformRequest()
		{
			HTTP_REQUEST_TYPE eType = HRT_GET;
			if (msDetails.eType == Networking::HttpRequestDetails::POST)
				eType = HRT_POST;

			DEBUG_LOG("CHttpConnectionSystem::CHttpRequest::PerformRequest() - Check for request type...");

			s32 dwResponseCode;
			HTTP_REQUEST_RESULT_CODE eRequestResult;
			if(msDetails.sHeaders.empty())
			{
				DEBUG_LOG("CHttpConnectionSystem::CHttpRequest::PerformRequest() - Performing standard request...");
				eRequestResult = SCHttpConnectionJavaInterface::HttpRequest(msDetails.strURL, eType, msDetails.strBody, mResponseData, msDetails.strRedirectionURL, dwResponseCode);
			}
			else
			{
				DEBUG_LOG("CHttpConnectionSystem::CHttpRequest::PerformRequest() - Performing request with headers...");
				eRequestResult = SCHttpConnectionJavaInterface::HttpRequest(msDetails.strURL, eType, msDetails.sHeaders, msDetails.strBody, mResponseData, msDetails.strRedirectionURL, dwResponseCode, true /* TODO: Add bKeepAlive boolean to msDetails*/);
			}
			mudwResponseCode = dwResponseCode;

			if (mbCompleted == false)
			{
				switch (eRequestResult)
				{
				case HRRC_SUCCESS:
					meRequestResult = IHttpRequest::COMPLETED;
					break;
				case HRRC_COULDNOTCONNECT:
				case HRRC_COULDNOTMAKEREQUEST:
					meRequestResult = IHttpRequest::FAILED;
					break;
				case HRRC_TIMEOUT:
					meRequestResult = IHttpRequest::TIMEOUT;
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
            meRequestResult = IHttpRequest::CANCELLED;
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
		const Networking::IHttpRequest::CompletionDelegate & CHttpConnectionSystem::CHttpRequest::GetCompletionDelegate() const
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
			return 0;
		}
	}
}
