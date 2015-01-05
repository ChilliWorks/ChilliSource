//
//  HttpResponse.h
//  Chilli Source
//  Created by Scott Downie on 17/12/2014.
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

#ifndef _CHILLISOURCE_NETWORKING_HTTP_HTTPRESPONSE_H_
#define _CHILLISOURCE_NETWORKING_HTTP_HTTPRESPONSE_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Networking
    {
        //----------------------------------------------------------------------------------------
        /// The response result of an http request. This contains the http response code and
        /// the response data. The response also contains the result which is whether the request
        /// succeeded or failed. If the request fails then the response data will be empty
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------------
        class HttpResponse final
        {
        public:
            //----------------------------------------------------------------------------------------
            /// Result of the http request
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            enum class Result
            {
                k_completed,    //The request completed (response data is available)
                k_failed,       //The request failed (no response data available)
                k_timeout,      //The request timed out (no response data available)
                k_flushed       //The request buffer is full and has been flushed. (response data is partial and more will follow)
            };
            //----------------------------------------------------------------------------------------
            /// Constructor
            ///
            /// @param Result
            /// @param Response code
            /// @param Response data
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            HttpResponse(Result in_result, u32 in_responseCode, const std::string& in_data);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The result of the request (determines whether data is available)
            //----------------------------------------------------------------------------------------
            Result GetResult() const;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The contents of the response. (This could be null if a failure occured)
            //----------------------------------------------------------------------------------------
            const u8* GetData() const;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Size of data in bytes (This could be 0 if a failure occured)
            //----------------------------------------------------------------------------------------
            u32 GetDataSize() const;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The contents of the response as a string. (This could be binary data and could
            /// also be empty if the result is failure. Some string operations may fail on binary data).
            //----------------------------------------------------------------------------------------
            const std::string& GetDataAsString() const;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return HTTP response code (i.e. 200 = OK).
            //----------------------------------------------------------------------------------------
            u32 GetCode() const;
            
        private:
            
            const std::string m_data;
            const Result m_result;
            const u32 m_code;
        };
    }
}

#endif
