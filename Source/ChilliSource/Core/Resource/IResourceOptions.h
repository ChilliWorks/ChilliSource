//
//  IResourceOptions.h
//  Chilli Source
//  Created by Scott Downie on 16/05/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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


#ifndef _CHILLISOURCE_CORE_RESOURCE_IRESOURCEOPTIONS_H_
#define _CHILLISOURCE_CORE_RESOURCE_IRESOURCEOPTIONS_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Core
	{
        //-------------------------------------------------------
        /// Interface class for custom loading options for
        /// resources. This allows custom loading to take place
        /// via the generic resource pool loaders.
        ///
        /// @author S Downie
        //-------------------------------------------------------
        class IResourceOptionsBase
        {
        public:
            //-------------------------------------------------------
            /// Overriden to generate a unique hash based on the
            /// currently set options
            ///
            /// @author S Downie
            ///
            /// @return Hash of the options contents
            //-------------------------------------------------------
            virtual u32 GenerateHash() const = 0;
            //-------------------------------------------------------
            /// Virual destructor
            ///
            /// @author S Downie
            //-------------------------------------------------------
            virtual ~IResourceOptionsBase(){}
        };
        //-------------------------------------------------------
        /// Interface class that provides type protection when
        /// passing into resource pool.
        ///
        /// @author S Downie
        //-------------------------------------------------------
		template <typename TResourceType> class IResourceOptions : public IResourceOptionsBase
		{
		public:
            
            //-------------------------------------------------------
            /// Virual destructor
            ///
            /// @author S Downie
            //-------------------------------------------------------
            virtual ~IResourceOptions(){}
		};
	}
}

#endif
