//
//  TextureUnitSystem.h
//  Chilli Source
//  Created by Scott Downie on 15/04/2014.
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

#ifndef _CSBACKEND_RENDERING_OPENGL_TEXTURE_TEXTUREUNITSYSTEM_H_
#define _CSBACKEND_RENDERING_OPENGL_TEXTURE_TEXTUREUNITSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <vector>

namespace CSBackend
{
	namespace OpenGL
	{
		//-----------------------------------------------------------
		/// A system used to track which textures are currently
        /// bound to which texture unit so that binds do not
        /// occur unnecessarily
        ///
        /// @author S Downie
		//-----------------------------------------------------------
		class TextureUnitSystem final : public CSCore::AppSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(TextureUnitSystem);
            
            //--------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface ID type
            ///
			/// @return Whether the object is of this type
			//--------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //-------------------------------------------------------
            /// Bind the object to the given texture unit only if
            /// required (i.e. not already bound). GL hates
            /// redundant operations.
            ///
            /// @author S Downie
            ///
            /// @param Texture
            /// @param Texture unit
            //-------------------------------------------------------
            void Bind(const Texture* in_texture, u32 in_unit);
            //-------------------------------------------------------
            /// Bind the object to the given texture unit only if
            /// required (i.e. not already bound). GL hates
            /// redundant operations.
            ///
            /// @author S Downie
            ///
            /// @param Cubemap
            /// @param GL texture handle
            /// @param Texture unit
            //-------------------------------------------------------
            void Bind(const Cubemap* in_cubemap, u32 in_unit);
            //-------------------------------------------------------
            /// Unbind the object from its current texture unit
            ///
            /// @author S Downie
            ///
            /// @param Texture
            //-------------------------------------------------------
            void Unbind(const Texture* in_texture);
            //-------------------------------------------------------
            /// Unbind the object from its current texture unit
            ///
            /// @author S Downie
            ///
            /// @param Cubemap
            //-------------------------------------------------------
            void Unbind(const Cubemap* in_cubemap);
            //-------------------------------------------------------
            /// Clear the cache so that future binds will be forced
            ///
            /// @author S Downie
            //-------------------------------------------------------
            void Clear();

        private:
            friend class CSCore::Application;
            //----------------------------------------------------
			/// Creates a new instance of this system.
            ///
            /// @author S Downie
            ///
            /// @return The new instance.
			//----------------------------------------------------
            static TextureUnitSystemUPtr Create();
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            TextureUnitSystem() = default;
            //-------------------------------------------------------
            /// Bind the object to the given texture unit only if
            /// required (i.e. not already bound). GL hates
            /// redundant operations.
            ///
            /// @author S Downie
            ///
            /// @param Type - Tex2D or Cubemap
            /// @param Pointer to resource object
            /// @param GL texture handle
            /// @param Texture unit
            //-------------------------------------------------------
            void Bind(GLenum in_type, const void* in_object, GLint in_handle, u32 in_unit);
            //-------------------------------------------------------
            /// Unbind the object from its current texture unit
            ///
            /// @author S Downie
            ///
            /// @param Object
            //-------------------------------------------------------
            void Unbind(const void* in_object);
            //-------------------------------------------------------
            /// Called when the system is destroyed
            ///
            /// @author S Downie
            //-------------------------------------------------------
            void OnDestroy() override;
            
        private:
            
            s32 m_currentTextureUnit = -1;
            std::vector<const void*> m_texUnits;
		};
	}
}

#endif
