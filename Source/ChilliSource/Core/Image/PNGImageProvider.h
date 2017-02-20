//
//  PNGImageProvider.h
//  ChilliSource
//  Created by Scott Downie on 01/08/2011.
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

#ifndef _CHILLISOURCE_CORE_IMAGE_PNGIMAGEPROVIDER_H_
#define _CHILLISOURCE_CORE_IMAGE_PNGIMAGEPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource
{
    //---------------------------------------------------------------
    /// An image loader interface that providers PNG resources
    ///
    /// @author S Downie
    //---------------------------------------------------------------
    class PNGImageProvider : public ResourceProvider
    {
    protected:
        friend class Application;
        //-------------------------------------------------------
        /// Create the platform dependent backend
        ///
        /// @author S Downie
        ///
        /// @return New backend instance
        //-------------------------------------------------------
        static PNGImageProviderUPtr Create();
        //-------------------------------------------------------
        /// Private constructor to force use of factory method
        ///
        /// @author S Downie
        //-------------------------------------------------------
        PNGImageProvider() = default;
    };
}

#endif
