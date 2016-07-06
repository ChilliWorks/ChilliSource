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

#ifndef _CSBACKEND_RENDERING_OPENGL_BASE_GLCONTEXTRESTORER_H_
#define _CSBACKEND_RENDERING_OPENGL_BASE_GLCONTEXTRESTORER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>

#include <vector>

namespace CSBackend
{
    namespace OpenGL
    {
        /// OpenGL on Android has its context killed whenever the
        /// app is suspended. This means all GL resources such
        /// as textures, shaders and buffers are destroyed and
        /// must be recreated on resume.
        ///
        class GLContextRestorer final : public ChilliSource::AppSystem
        {
        public:
            
            CS_DECLARE_NAMEDTYPE(GLContextRestorer);
           
            /// Allows querying of whether or not this system implements the interface described by the
            /// given interface Id. Typically this is not called directly as the templated equivalent
            /// IsA<Interface>() is preferred.
            ///
            /// @param interfaceId
            ///     The Id of the interface.
            ///
            /// @return Whether or not the interface is implemented.
            ///
            bool IsA(ChilliSource::InterfaceIDType interfaceId) const noexcept override;
            
        private:
            friend class ChilliSource::Application;
            friend class ChilliSource::LifecycleManager;
            
            /// A factory method for creating new instances of the system. This must be called by
            /// Application.
            ///
            /// @return The new instance of the system.
            ///
            static GLContextRestorerUPtr Create() noexcept;
            
            /// Constructor
            ///
            GLContextRestorer() = default;
            
            /// Iterate any GL resources and place them in an invalid state
            ///
            void OnContextLost() noexcept;
            
            /// Iterate all resources and either re-load them from file or
            /// restore them from cached memory
            ///
            void OnContextRestored() noexcept;
            
            /// Called when the app system is resumed
            ///
            void OnResume() noexcept override;
            
            /// Called when the application delegate is suspended. This is called directly
            /// from lifecycle manager and will be called before the OnSuspend.
            ///
            void OnSystemSuspend() noexcept;
            
        private:
            
            bool m_initialised = false;
            bool m_hasContextBeenBackedUp = false;
        };
    }
}

#endif
