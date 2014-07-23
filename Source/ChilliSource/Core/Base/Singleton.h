//
//  Singleton.h
//  Chilli Source
//  Created by Scott Downie on 11/03/2014.
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

#ifndef _CHILLISOURCE_CORE_BASE_SINGLETON_H_
#define _CHILLISOURCE_CORE_BASE_SINGLETON_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Core
	{
        //---------------------------------------------------------------
        /// Inherit from this class when you wish to implement
        /// a singleton interface. The singleton is not lazily created
        /// but instead must be explicitly created (and destroyed)
        ///
        /// @author S Downie
        //---------------------------------------------------------------
        template <typename TType> class Singleton
        {
        public:
            //---------------------------------------------------------------
            /// Destructor
            ///
            /// @author S Downie
            //---------------------------------------------------------------
            virtual ~Singleton(){}
            //---------------------------------------------------------------
            /// Create the singleton instance. Once this has been created
            /// it is accessible through Get() and creating again (w/o destroying)
            /// will cause an assertion.
            ///
            /// @author S Downie
            ///
            /// @param Variable number of argument types required to construct
            /// the singleton instance
            //---------------------------------------------------------------
            template <typename... TArgTypes> static void Create(TArgTypes... in_args)
            {
                CS_ASSERT(m_singleton == nullptr, "Singleton can only be created once");
                m_singleton = new TType(in_args...);
            }
            //---------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Pre-created singleton instance or null if not yet
            /// created
            //---------------------------------------------------------------
            inline static TType* Get()
            {
                return m_singleton;
            }
            //---------------------------------------------------------------
            /// Destruct the singleton instance. It can then be created again
            ///
            /// @author S Downie
            //---------------------------------------------------------------
            inline static void Destroy()
            {
                CS_SAFEDELETE(m_singleton);
            }
            
        protected:
            
            //---------------------------------------------------------------
            /// Private constructor to enforce use of creation method
            ///
            /// @author S Downie
            //---------------------------------------------------------------
            Singleton(){}
            
        private:
            
            static TType* m_singleton;
        };

		template <typename TType> TType* Singleton<TType>::m_singleton = nullptr;
    }
}

#endif
