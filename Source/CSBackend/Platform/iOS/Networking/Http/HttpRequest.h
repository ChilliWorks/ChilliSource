//
//  HttpRequest.h
//  Chilli Source
//  Created by Scott Downie on 23/05/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifdef CS_TARGETPLATFORM_IOS

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Networking/Http/HttpRequest.h>

#import <Foundation/Foundation.h>

#include <vector>

@class HttpDelegate;

namespace CSBackend
{
	namespace iOS
	{
        //-------------------------------------------------------------------------
        /// Concrete implementation of iOS http request using the CFNetworking
        /// library. Requests are performed on a background thread.
        ///
        /// @author S Downie
        //-------------------------------------------------------------------------
        class HttpRequest final : public CSNetworking::HttpRequest
        {
        public:
            //------------------------------------------------------------------
            /// Constructor. The request is started as a background task.
            ///
            /// @author S Downie
            ///
            /// @param Request description
            /// @param Timeout in seconds
            /// @param Completion delegate
            //------------------------------------------------------------------
            HttpRequest(const Desc& in_requestDesc, u32 in_timeoutSecs, const Delegate& in_delegate);
            //------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The original request details, including whether it is a
            /// post of get request and the body and header.
            //------------------------------------------------------------------
            const Desc& GetDescription() const override;
            //------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The contents of the response as a string. This could be
            /// binary data
            //------------------------------------------------------------------
            const std::string& GetResponse() const override;
            //------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return HTTP response code (i.e. 200 = OK, 400 = Error)
            //------------------------------------------------------------------
            u32 GetResponseCode() const override;
            //------------------------------------------------------------------
            /// Close the request. Note: The completion delegate is not invoked
            ///
            /// @author S Downie
            //------------------------------------------------------------------
            void Cancel() override;
            //------------------------------------------------------------------
            /// Called by the Http Delegate once the request has complete. This
            /// is for internal use only and should not be called by the user.
            ///
            /// @author Ian Copland
            ///
            /// @param The result.
            /// @param The response code.
            /// @param The data in string form.
            //------------------------------------------------------------------
            void OnComplete(CSNetworking::HttpRequest::Result in_result, u32 in_responseCode, const std::string& in_data);
            //------------------------------------------------------------------
            /// Destructor.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------
            ~HttpRequest();
        private:
            const Desc m_desc;
            Delegate m_completionDelegate;
            
            bool m_complete = false;
            u32 m_responseCode;
            std::string m_responseData;
            
            NSURLConnection* m_connection = nil;
            HttpDelegate* m_httpDelegate = nil;
        };
	}
}

#endif