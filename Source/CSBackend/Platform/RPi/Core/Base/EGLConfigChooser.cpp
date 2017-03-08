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

#include <CSBackend/Platform/RPi/Core/Base/EGLConfigChooser.h>

namespace ChilliSource
{
    namespace RPi
    {
        //---------------------------------------------------------------------
        EGLConfigChooser::EGLConfigChooser(u8 redSize, u8 greenSize, u8 blueSize, u8 alphaSize, u8 minDepthSize, u8 preferredDepthSize, u8 minStencilSize)
        {
            m_redSize = redSize;
            m_greenSize = greenSize;
            m_blueSize = blueSize;
            m_alphaSize = alphaSize;
            m_minDepthSize = minDepthSize;
            m_preferredDepthSize = preferredDepthSize;
            m_minStencilSize = m_minStencilSize;
        }

        //---------------------------------------------------------------------
        EGLConfig EGLConfigChooser::ChooseBestConfig(EGLDisplay eglDisplay)
        {
            // Define the minimum-possible spec.
            static const EGLint attributeList[] =
            {
                EGL_RED_SIZE, 4,
                EGL_GREEN_SIZE, 4,
                EGL_BLUE_SIZE, 4,
                EGL_ALPHA_SIZE, 0,
                EGL_DEPTH_SIZE, 0,
                EGL_STENCIL_SIZE, 0,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL_NONE
            };

            // Find out how many configs satisfy this requirement.
            EGLint numAvailableConfigs = 0;
            eglChooseConfig(eglDisplay, attributeList, nullptr, 0, &numAvailableConfigs);

            if(numAvailableConfigs <= 0)
            {
                CS_LOG_FATAL("Couldn't find any EGL Configurations that fit the minimum criteria; EGL possibly unsupported on device!");
            }

            // Create a list with that size, and fill it.
            EGLConfig* availableConfigs = new EGLConfig[numAvailableConfigs];
            eglChooseConfig(eglDisplay, attributeList, availableConfigs, numAvailableConfigs, &numAvailableConfigs);

            // Use this list with the Choose-From-List function.

            return ChooseBestConfigFromList(eglDisplay, availableConfigs, numAvailableConfigs);
        }

        //---------------------------------------------------------------------
        EGLConfig EGLConfigChooser::ChooseBestConfigFromList(EGLDisplay eglDisplay, EGLConfig eglConfigs[], EGLint numConfigs)
        {
            u8 depthSize = m_preferredDepthSize;

            // Loop through configs until we find one that matches or the depth
            // size gets too small.
            while(depthSize >= m_minDepthSize)
            {
                for(EGLint i = 0; i < numConfigs; i++)
                {
                    u8 currentDepthSize = FindConfigAttribute(eglDisplay, eglConfigs[i], EGL_DEPTH_SIZE, 0);
                    u8 currentStencilSize = FindConfigAttribute(eglDisplay, eglConfigs[i], EGL_STENCIL_SIZE, 0);

                    // We need >= minimum stencil/depth size.
                    if(currentDepthSize >= depthSize && currentStencilSize >= m_minStencilSize)
                    {
                        // Need exact matches for colour depth.
                        u8 redSize = FindConfigAttribute(eglDisplay, eglConfigs[i], EGL_RED_SIZE, 0);
                        u8 greenSize = FindConfigAttribute(eglDisplay, eglConfigs[i], EGL_GREEN_SIZE, 0);
                        u8 blueSize = FindConfigAttribute(eglDisplay, eglConfigs[i], EGL_BLUE_SIZE, 0);
                        u8 alphaSize = FindConfigAttribute(eglDisplay, eglConfigs[i], EGL_ALPHA_SIZE, 0);

                        if(redSize == m_redSize && greenSize == m_greenSize && blueSize == m_blueSize && alphaSize == m_alphaSize)
                        {
                            return eglConfigs[i];
                        }
                    }
                }

                depthSize -= 8;
            }

            return nullptr;
        }

        //---------------------------------------------------------------------
        u8 EGLConfigChooser::FindConfigAttribute(EGLDisplay eglDisplay, EGLConfig eglConfig, EGLint attribute, u8 defaultValue)
        {
            EGLint* value = new EGLint[1];
            if(eglGetConfigAttrib(eglDisplay, eglConfig, attribute, value) == true)
            {
                return (u8)value[0];
            }

            return defaultValue;
        }


    }
}


#endif
