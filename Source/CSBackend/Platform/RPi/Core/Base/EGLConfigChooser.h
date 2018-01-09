//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_RPI

#ifndef _CSBACKEND_PLATFORM_RPI_CORE_BASE_EGLCONFIGCHOOSER_H_
#define _CSBACKEND_PLATFORM_RPI_CORE_BASE_EGLCONFIGCHOOSER_H_

#include <CSBackend/Platform/RPi/ForwardDeclarations.h>
#include <ChilliSource/ChilliSource.h>

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

namespace CSBackend
{
    namespace RPi
    {
        /// This class is used to pick an EGLConfig that matches as close as
        /// possible with the specified attributes.
        ///
        class EGLConfigChooser final
        {
        public:

            /// Constructs the ConfigChooser with the specified desired attributes.
            ///
            ///
            EGLConfigChooser(EGLint redSize, EGLint greenSize, EGLint blueSize, EGLint alphaSize, EGLint minDepthSize, EGLint preferredDepthSize, EGLint minStencilSize) noexcept;

            /// Picks the best EGLConfig using the specified desired attributes.
            ///
            /// @param The EGLDisplay to choose for.
            EGLConfig ChooseBestConfig(EGLDisplay eglDisplay) noexcept;

        private:
            /// Picks the best EGLConfig out of the supplied config list.
            ///
            /// @param The EGLDisplay to choose for.
            /// @param The array of EGLConfig objects to sort through.
            /// @param The number of EGLConfig objects in that list.
            EGLConfig ChooseBestConfigFromList(EGLDisplay eglDisplay, EGLConfig* eglConfigs, EGLint numConfigs) noexcept;

            /// Finds a config attribute within a config, and returns its value.
            /// If the value isn't found, returns the defaultValue parameter.
            ///
            /// @param The EGLDisplay to test against.
            /// @param The EGLConfig to test against.
            /// @param The EGLint representing the attribute to check for.
            /// @param The default value of the attribute - returned if attribute isn't found in the config.
            ///
            EGLint FindConfigAttribute(EGLDisplay eglDisplay, EGLConfig eglConfig, EGLint attribute, EGLint defaultValue) noexcept;

            EGLint m_redSize;
            EGLint m_greenSize;
            EGLint m_blueSize;
            EGLint m_alphaSize;
            EGLint m_minDepthSize;
            EGLint m_preferredDepthSize;
            EGLint m_minStencilSize;
        };
    }
}

#endif

#endif
