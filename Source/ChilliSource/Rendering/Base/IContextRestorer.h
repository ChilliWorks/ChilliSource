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

#ifndef _CHILLISOURCE_RENDERING_BASE_ICONTEXTRESTORER_H_
#define _CHILLISOURCE_RENDERING_BASE_ICONTEXTRESTORER_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    class IContextRestorer
    {
    public:
        /// Creates a new instance of the context restorer depending on the current
        /// platform. If the current platform does not need to have its context restored
        /// then nullptr will be returned.
        ///
        /// @return The newly created instance.
        ///
        static IContextRestorerUPtr Create() noexcept;
        
        /// Take a snapshot of the GL context for resources
        /// that cannot be recreated from file and store them
        /// so they can be recreated
        ///
        virtual void Backup() = 0;
        
        /// Recreate the GL context with the backed up resources
        ///
        virtual void Restore() = 0;
    };
}

#endif
