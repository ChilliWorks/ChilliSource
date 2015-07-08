//
//  JavaStaticClassDef.h
//  ChilliSource
//  Created by Ian Copland on 08/06/2015.
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

#ifndef _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_JAVASTATICCLASSDEF_H_
#define _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_JAVASTATICCLASSDEF_H_

#include <ChilliSource/ChilliSource.h>

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>

#include <unordered_map>

namespace CSBackend
{
	namespace Android
	{
		//------------------------------------------------------------------------------
		/// Defines the static class and the methods which will be accessible when
		/// using JavaStaticClass.
		///
		/// Although JavaStaticClass uses this for setup, it isn't retained, so it can
		/// be further modified and passed to additional JavaStaticClass instances.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		class JavaStaticClassDef final
		{
		public:
		    using MethodMap = std::unordered_map<std::string, std::string>;
            //------------------------------------------------------------------------------
            /// Constructor.
            ///
            /// @author Ian Copland
            ///
            /// @param in_className - The fully qualified name of the static class this
            /// refers to.
            //------------------------------------------------------------------------------
            JavaStaticClassDef(const std::string& in_className);
            //------------------------------------------------------------------------------
            /// Adds a new method definition. Note that multiple overloaded methods are
            /// not supported.
            ///
            /// @author Ian Copland
            ///
            /// @param in_methodName - The name of the method.
            /// @param in_methodSignature - The signature of the method. This is in the
            /// form described in the JNI documentation.
            //------------------------------------------------------------------------------
            void AddMethod(const std::string& in_methodName, const std::string& in_methodSignature);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The fully qualified class name.
            //------------------------------------------------------------------------------
            const std::string& GetClassName() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The map of methods names and signatures.
            //------------------------------------------------------------------------------
            const MethodMap& GetMethods() const;

        private:
            const std::string m_className;
            MethodMap m_methods;
		};
	}
}

#endif

#endif
