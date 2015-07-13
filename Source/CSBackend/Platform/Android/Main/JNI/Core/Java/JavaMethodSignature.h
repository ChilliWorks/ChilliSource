//
//  JavaMethodSignature.h
//  ChilliSource
//  Created by Ian Copland on 09/06/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_JAVAMETHODSIGNATURE_H_
#define _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_JAVAMETHODSIGNATURE_H_

#include <ChilliSource/ChilliSource.h>

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>

namespace CSBackend
{
	namespace Android
	{
	    //------------------------------------------------------------------------------
	    /// A collection of methods for working with JNI method signatures.  This
	    /// includes methods for calculating the number of parameters a signature has
	    /// and a means to calculate the return type.
	    ///
	    /// @author Ian Copland
	    //------------------------------------------------------------------------------
		namespace JavaMethodSignature
		{
            //------------------------------------------------------------------------------
            /// An enum describing the various return types for methods which can be called
            /// through JavaClass.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            enum class ReturnType
            {
                k_void,
                k_bool,
                k_byte,
                k_char,
                k_short,
                k_int,
                k_long,
                k_float,
                k_double,
                k_object,
                k_string
            };
			//------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param in_methodSignature - A JNI method signature.
			///
			/// @return The return type of the given method signature.
			//------------------------------------------------------------------------------
			ReturnType CalcReturnType(const std::string& in_methodSignature);
			//------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param in_methodSignature - A JNI method signature.
			///
			/// @return The number of parameters the method described by the given signature
			/// takes.
			//------------------------------------------------------------------------------
			u32 CalcNumArguments(const std::string& in_methodSignature);
		}
	}
}

#endif

#endif
