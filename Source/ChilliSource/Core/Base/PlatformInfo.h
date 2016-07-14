//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_BASE_PLATFORMINFO_H_
#define _CHILLISOURCE_CORE_BASE_PLATFORMINFO_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    /// This class contains information related to any platform
    /// specific info.
    ///
    /// This class is immutable, and therefore thread-safe.
    ///
    class PlatformInfo final
    {
    public:
        /// @param appVersion
        ///     The application version for the current platform.
        ///
        PlatformInfo(const std::string& appVersion) noexcept;
        
        /// @return The application version
        ///
        const std::string& GetApplicationVersion() const noexcept { return m_appVersion; }
    
    private:
        
        std::string m_appVersion;
    };
}

#endif
