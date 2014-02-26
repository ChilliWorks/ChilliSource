/*
 *  HttpConnectionSystem.cpp
 *  moFlow
 *
 *  Created by Stuart McGaw on 23/05/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Networking/Http/HttpConnectionSystem.h>

namespace ChilliSource
{
	namespace Networking
    {
		DEFINE_NAMED_INTERFACE(HttpConnectionSystem);
        
        u32 HttpConnectionSystem::mudwMaxBufferSize = 0;
        
        //--------------------------------------------------------------------------------------------------
        /// Set Max Buffer Size
        ///
        /// @param The number of bytes read before the buffer is flushed
        //--------------------------------------------------------------------------------------------------
        void HttpConnectionSystem::SetMaxBufferSize(u32 inudwSize)
        {
            mudwMaxBufferSize = inudwSize;
        }
        
        HttpRequest* HttpConnectionSystem::HandleRedirection(const HttpRequest* inpRequest)
        {
            CS_ASSERT(inpRequest, "inpRequest cannot be null!");
            if(inpRequest->GetResponseCode() != kHTTPMovedTemporarily)
            {
                CS_ERROR_LOG("Trying to redirect on a non-redirected request");
            }
            else
            {
                ChilliSource::Networking::HttpRequestDetails sDetails;
                
                sDetails = inpRequest->GetDetails();
                sDetails.strURL = sDetails.strRedirectionURL;
                sDetails.strRedirectionURL = "";
                
                return MakeRequest(sDetails, inpRequest->GetCompletionDelegate());
            }
            
            return nullptr;
        }
	}
}
