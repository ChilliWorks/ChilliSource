//
//  HttpRequest.h
//  ChilliSource
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
#include <ChilliSource/Networking/Http/HttpResponse.h>

#import <Foundation/Foundation.h>

@class HttpDelegate;

namespace CSBackend
{
	namespace iOS
	{
        //-------------------------------------------------------------------------
        /// Concrete implementation of an Http request using iOS NSURLConnection
        ///
        /// @author S Downie
        //-------------------------------------------------------------------------
        class HttpRequest final : public ChilliSource::HttpRequest
        {
        public:
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The type of the request (POST or GET)
            //----------------------------------------------------------------------------------------
            Type GetType() const override;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The original url to which the request was sent
            //----------------------------------------------------------------------------------------
            const std::string& GetUrl() const override;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The body of the POST request (GET request will return empty)
            //----------------------------------------------------------------------------------------
            const std::string& GetBody() const override;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The original headers of the request as keys/values
            //----------------------------------------------------------------------------------------
            const ChilliSource::ParamDictionary& GetHeaders() const override;
            //----------------------------------------------------------------------------------------
            /// @author N Tanda
            ///
            /// @return The expected total size of the request
            //----------------------------------------------------------------------------------------
            u64 GetExpectedSize() const override;
            //----------------------------------------------------------------------------------------
            /// @author N Tanda
            ///
            /// @return The current transferred size of the request
            //----------------------------------------------------------------------------------------
            u64 GetDownloadedBytes() const override;
            //------------------------------------------------------------------
            /// Close the request. Note: The completion delegate is not invoked
            ///
            /// @author S Downie
            //------------------------------------------------------------------
            void Cancel() override;
            //------------------------------------------------------------------
            /// Destructor.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------
            ~HttpRequest();
            
        private:
            
            friend class HttpRequestSystem;
            //------------------------------------------------------------------
            /// Constructor. Can only be created via HttpRequestSystem
            ///
            /// @author S Downie
            ///
            /// @param Type (POST or GET)
            /// @param Url to send request to
            /// @param POST body
            /// @param Headers
            /// @param Timeout in seconds
            /// @param Max buffer size in bytes
            /// @param Completion delegate
            //------------------------------------------------------------------
            HttpRequest(Type in_type, const std::string& in_url, const std::string& in_body, const ChilliSource::ParamDictionary& in_headers, u32 in_timeoutSecs, u32 in_maxBufferSize, const Delegate& in_delegate);
            
        private:
            
            const Type m_type;
            const std::string m_url;
            const std::string m_body;
            const ChilliSource::ParamDictionary m_headers;
            const Delegate m_completionDelegate;
            
            //These need to be shared_ptr so we can access them in result lambdas if
            //(*this) is destroyed, which may occur if a callback occurs while waiting to cancel a request
            //on the system thread.
            std::shared_ptr<bool> m_complete;
            std::shared_ptr<bool> m_isCancelled;
            
            NSURLConnection* m_connection = nil;
            HttpDelegate* m_httpDelegate = nil;
            
            u64 m_downloadedBytes = 0;
            u64 m_expectedSize = 0;
        };
	}
}

#endif
