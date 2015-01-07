//
//  HttpResponse.cpp
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

#include <ChilliSource/Networking/Http/HttpResponse.h>

namespace ChilliSource
{
    namespace Networking
    {
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        HttpResponse::HttpResponse(Result in_result, u32 in_code, const std::string& in_data)
        : m_data(in_data), m_result(in_result), m_code(in_code)
        {
            
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        HttpResponse::Result HttpResponse::GetResult() const
        {
            return m_result;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        const u8* HttpResponse::GetData() const
        {
            return (const u8*)m_data.data();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        u32 HttpResponse::GetDataSize() const
        {
            return m_data.size();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        const std::string& HttpResponse::GetDataAsString() const
        {
            return m_data;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        u32 HttpResponse::GetCode() const
        {
            return m_code;
        }
    }
}