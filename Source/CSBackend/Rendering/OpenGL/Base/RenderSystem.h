//
//  RenderSystem.h
//  Chilli Source
//  Created by Scott Downie on 27/09/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_OPENGL_ES2_RENDER_SYSTEM_H_
#define _CHILLISOURCE_OPENGL_ES2_RENDER_SYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>
#include <CSBackend/Rendering/OpenGL/Base/ContextRestorer.h>

#include <unordered_map>

namespace CSBackend
{
	namespace OpenGL
	{
		typedef u32 GLClientStateFlag;
		
		//=======================================================
		/// Description:
		///
		/// OpenGL ES 2.0 Renderer - Base
		///
		/// Creates and manages the OpenGL ES context and
		/// handles the rendering of scene objects
		//=======================================================
		class RenderSystem final : public CSRendering::RenderSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(RenderSystem);
            
            //----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType inInterfaceID) const override;
            //----------------------------------------------------------
            /// Called when the system is created. Sets up the
            /// render target, viewport, etc.
            ///
            /// @author S Downie
			//----------------------------------------------------------
			void Init() override;
            //----------------------------------------------------------
            /// Called when the app resumes from an inactive state.
            /// Something may have stole our context so we need to
            /// check and bind it. On Android our context needs to be
            /// recreated
            ///
            /// @author S Downie
            //----------------------------------------------------------
            void Resume() override;
            //----------------------------------------------------------
			/// Called when the app suspends. Android stores the assets that
            /// require recreation when the context is reacreate on resume.
            ///
            /// @author Ian Copland
			//----------------------------------------------------------
			void Suspend() override;
            //----------------------------------------------------------
			/// Release the OpenGL ES context and any assets
            ///
            /// @author S Downie
			//----------------------------------------------------------
            void Destroy() override;
            //----------------------------------------------------------
			/// Create Buffer
			///
			/// Create a hardware accelerated VBO
			/// @return An instantiated buffer
			//----------------------------------------------------------
			CSRendering::MeshBuffer* CreateBuffer(CSRendering::BufferDescription &inDesc) override;
			//----------------------------------------------------------
			/// Render Vertex Buffer
			///
			/// Render the contents of the given buffer with no indices
			///
			/// @param The buffer to be rendered
            /// @param Offset into vertex buffer
            /// @param Number of vertices to render
			/// @param The world matrix to apply transformations
			//----------------------------------------------------------
			void RenderVertexBuffer(CSRendering::MeshBuffer* inpBuffer, u32 inudwOffset, u32 inudwNumVerts, const CSCore::Matrix4& inmatWorld) override;
			//----------------------------------------------------------
			/// Render Buffer
			///
			/// Render the contents of the given mesh buffer and apply
			/// the world space transformation to the geometry
			///
			/// @param The buffer to be rendered
            /// @param Offset into index buffer
            /// @param Number of indices to render
			/// @param The world matrix to apply transformations
			//----------------------------------------------------------
			void RenderBuffer(CSRendering::MeshBuffer* inpBuffer, u32 inudwOffset, u32 inudwNumIndices, const CSCore::Matrix4& inmatWorld) override;
            //----------------------------------------------------------
			/// Apply Camera
			///
			/// Apply the camera transform to the projection matrix
            ///
            /// @param Camera position
			/// @param Camera projection matrix
			/// @param Camera view matrix
			/// @param Frame buffer clear colour
			//----------------------------------------------------------
			void ApplyCamera(const CSCore::Vector3& invPos, const CSCore::Matrix4& inmatView, const CSCore::Matrix4& inmatProj, const CSCore::Colour& inClearCol) override;
            //----------------------------------------------------------
            /// Set Light
            ///
            /// Pass the lighting to the render system for the active scene
            ///
            /// @param lighting components
            //----------------------------------------------------------
            void SetLight(CSRendering::LightComponent* inpLightComponent) override;
			//----------------------------------------------------------
			/// Apply Material
			///
			/// Apply the material to the next render batch
			///
			/// @param Material
            /// @param Shader pass
			//----------------------------------------------------------
			void ApplyMaterial(const CSRendering::MaterialCSPtr& inMaterial, CSRendering::ShaderPass in_shaderPass) override;
            //----------------------------------------------------------
			/// Apply Joints
			///
			/// Applies the joint matrices to the next render batch.
			///
			/// @param Dynamic array of joint matrices.
			//----------------------------------------------------------
            void ApplyJoints(const std::vector<CSCore::Matrix4>& inaJoints) override;
			//----------------------------------------------------------
			/// Create Render Target
			///
			/// Create a GL render target
            ///
            /// @param Width
            /// @param Height
			/// @return An instantiated target
			//----------------------------------------------------------
			CSRendering::RenderTarget* CreateRenderTarget(u32 inudwWidth, u32 inudwHeight) override;
            
            //---Render states
            //----------------------------------------------------------
            /// Lock Alpha Blending
            ///
            /// Prevent alpha blending being altered
            //----------------------------------------------------------
            void LockAlphaBlending() override;
            //----------------------------------------------------------
            /// Unlock Alpha Blending
            ///
            /// Allow alpha blending to be altered
            //----------------------------------------------------------
            void UnlockAlphaBlending() override;
            //----------------------------------------------------------
			/// Enable Alpha Blending
			///
			/// Turns alpha blending on or off
			/// @param Boolean to toggle enable/disable
			//----------------------------------------------------------
			void EnableAlphaBlending(bool inbIsEnabled) override;
			//----------------------------------------------------------
			/// Enable Depth Testing
			///
			/// Turns depth testing on or off
			/// @param Boolean to toggle enable/disable
			//----------------------------------------------------------
			void EnableDepthTesting(bool inbIsEnabled) override;
			//----------------------------------------------------------
			/// Enable Face Culling
			///
			/// Turns culling on or off
			/// @param Boolean to toggle enable/disable
			//----------------------------------------------------------
			void EnableFaceCulling(bool inbIsEnabled) override;
			//----------------------------------------------------------
			/// Enable Colour Writing
			///
			/// Turns colour writing on or off.
			/// @param Boolean to toggle enable/disable
			//----------------------------------------------------------
			void EnableColourWriting(bool inbIsEnabled) override;
            //----------------------------------------------------------
            /// Lock Depth Writing
            ///
            /// Prevent depth writing from being altered
            //----------------------------------------------------------
            void LockDepthWriting() override;
            //----------------------------------------------------------
            /// Unlock Depth Writing
            ///
            /// Allow depth write to be altered
            //----------------------------------------------------------
            void UnlockDepthWriting() override;
            //----------------------------------------------------------
			/// Enable Depth Writing
			///
			/// Turns depth writing on or off. Used for alpha blending
			/// @param Boolean to toggle enable/disable
            /// @param Whether to lock to prevent this being changed
			//----------------------------------------------------------
			void EnableDepthWriting(bool inbIsEnabled) override;
            //----------------------------------------------------------
            /// Enable Scissor Testing
            ///
            /// Turns on scissoring. Used to render to only a portion
            /// of the screen
            /// @param Enable/disable
            //----------------------------------------------------------
            void EnableScissorTesting(bool inbIsEnabled) override;
            //----------------------------------------------------------
            /// Set Scissor Region
            ///
            /// Set the rectangle region of the screen that is active
            /// when scissoring is enabled
            ///
            /// @param Position of the bottom left corner of the rect
            /// @param Size of the scissor region
            //---------------------------------------------------------
            void SetScissorRegion(const CSCore::Vector2& invPosition, const CSCore::Vector2& invSize) override;
            //----------------------------------------------------------
			/// Set Cull Face
			///
			/// Sets the cull face
			/// @param Cull face
			//----------------------------------------------------------
			void SetCullFace(CSRendering::CullFace ineCullface) override;
            //----------------------------------------------------------
            /// Lock Blend Function
            ///
            /// Prevent the blend function being altered
            //----------------------------------------------------------
            void LockBlendFunction() override;
            //----------------------------------------------------------
            /// Unlock Blend Function
            ///
            /// Allow the blend function to be altered
            //----------------------------------------------------------
            void UnlockBlendFunction() override;
			//----------------------------------------------------------
			/// Set Blend Function
			///
			/// @param Source function
			/// @param Destination function
			//----------------------------------------------------------
			void SetBlendFunction(CSRendering::BlendMode ineSrcFunc, CSRendering::BlendMode ineDstFunc) override;
            //----------------------------------------------------------
			/// Set Depth Function
			///
            /// @param Function to be used for depth testing
			//----------------------------------------------------------
            void SetDepthFunction(CSRendering::DepthTestComparison ineFunc) override;
            //----------------------------------------------------------
            /// Remove Buffer
            ///
            /// Removes a mesh buffer from the tracked list. Does
            /// not destroy it!
            ///
            /// @param Mesh buffer pointer.
            //----------------------------------------------------------
            void RemoveBuffer(CSRendering::MeshBuffer* inpBuffer);
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @return If the GL context still exists
            //----------------------------------------------------------
            bool HasContext() const;
			
		private:
            
            friend CSRendering::RenderSystemUPtr CSRendering::RenderSystem::Create(CSRendering::RenderCapabilities*);
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            ///
            /// @param Render capabilities
            //-------------------------------------------------------
            RenderSystem(CSRendering::RenderCapabilities* in_renderCapabilities);
            //----------------------------------------------------------
            /// Backup Context
            ///
            /// Stores all non re-createable information about the current
			/// context in memory.
            //----------------------------------------------------------
            void BackupContext();
            //----------------------------------------------------------
			/// Restore Context
			///
			/// Restores the new opengl context to the state of the
            /// previous one.
			//----------------------------------------------------------
			void RestoreContext();
            //----------------------------------------------------------
            /// Apply Render States
            ///
            /// @param Material
            //----------------------------------------------------------
            void ApplyRenderStates(const CSRendering::Material* inMaterial);
            //----------------------------------------------------------
            /// Apply Textures
            ///
            /// Pass the texture variables to the shader
            ///
            /// @param Material
            /// @param Shader
            //----------------------------------------------------------
            void ApplyTextures(const CSRendering::Material* inMaterial, Shader* out_shader);
            //----------------------------------------------------------
            /// Apply Lighting Values
            ///
            /// Pass the lighting variables to the shader
            ///
            /// @param Material
            /// @param [Out] Shader
            //----------------------------------------------------------
            void ApplyLightingValues(const CSRendering::Material* inMaterial, Shader* out_shader);
			//------------------------------------------------------------
			/// Enable Vertex Attribute For Semantic (Programmable pipeline)
			///
			/// Set the pointers and enable the state based on the vertex
			/// layout of the buffer.
			///
			/// @param Element
			//------------------------------------------------------------
			void EnableVertexAttributeForSemantic(CSRendering::MeshBuffer* inpBuffer);
			//------------------------------------------------------------
			/// Get Primitive Type
			///
			/// Convert from CS primitive type to GL primitive type
			///
			/// @param CS primitive type
			//------------------------------------------------------------
			s32 GetPrimitiveType(CSRendering::PrimitiveType inType);
            //----------------------------------------------------------
			/// Begin Frame
			///
			/// Clear the back buffer to the clear colour
			//----------------------------------------------------------
			void BeginFrame(CSRendering::RenderTarget* inpActiveRenderTarget) override;
			//----------------------------------------------------------
			/// End Frame
			///
			/// Present the buffer to the screen
			//----------------------------------------------------------
			void EndFrame(CSRendering::RenderTarget* inpActiveRenderTarget) override;
			//----------------------------------------------------------
			/// Apply Shader Variables
			///
			/// Send the custom variables to the given shader
			///
			/// @param Material
			/// @param Shader
			//----------------------------------------------------------
			void ApplyShaderVariables(const CSRendering::Material* inMaterial, Shader* out_shader);
            //----------------------------------------------------------
            /// Apply Lighting
            ///
            /// Pass the lighting variables to the shader
            ///
            /// @param Lighting component
            /// @param [Out] Shader
            //----------------------------------------------------------
            void ApplyLighting(CSRendering::LightComponent* inpLightComponent, Shader* out_shader);
			//----------------------------------------------------------
			/// Force Refresh Render States
			///
			/// Ignores all caching and force changes render states
			//----------------------------------------------------------
			void ForceRefreshRenderStates();
            //----------------------------------------------------------
            /// Is Attrib Pointer Set
            ///
            /// @param Attribute location
            /// @return Whether it has been set
            //----------------------------------------------------------
            bool IsAttribPointerSet(GLint indwAttribLocation) const;
            //----------------------------------------------------------
            /// Create Attrib State Cache
            //----------------------------------------------------------
            void CreateAttribStateCache();
            //----------------------------------------------------------
            /// Called when the resolution of the screen changes. This
            /// then updates the size of the frame buffer.
            ///
            /// @author Ian Copland
            ///
            /// @param The new screen resolution.
            //----------------------------------------------------------
            void OnScreenResolutionChanged(const CSCore::Vector2& in_resolution);
            //----------------------------------------------------------
            /// Triggered when window is set to Fullscreen mode
            /// or away from Fullscreen mode; which results in loss of state
            ///
            /// @author S Downie
			///
			/// @param Display mode
            //----------------------------------------------------------
			void OnDisplayModeChanged(CSCore::Screen::DisplayMode in_mode);
            
		private:
			
            void ApplyVertexAttributePointer(CSRendering::MeshBuffer* inpBuffer,
                                            const char* in_attribName, GLint indwSize, GLenum ineType, GLboolean inbNormalized, GLsizei indwStride, const GLvoid* inpOffset);
            
            CSRendering::LightComponent* mpLightComponent;
            
            CSCore::Colour mNewClearColour;
			CSCore::Colour mClearColour;
            CSCore::Colour mCurrentEmissive;
            CSCore::Colour mCurrentAmbient;
            CSCore::Colour mCurrentDiffuse;
            CSCore::Colour mCurrentSpecular;
            bool mbEmissiveSet;
            bool mbAmbientSet;
            bool mbDiffuseSet;
            bool mbSpecularSet;
            
			CSCore::Matrix4 mmatProj;
			CSCore::Matrix4 mmatViewProj;
            CSCore::Matrix4 mmatView;
            CSCore::Vector3 mvCameraPos;
			
            RenderCapabilities* mpRenderCapabilities;
            TextureUnitSystem* m_textureUnitSystem;
			
            GLint mdwMaxVertAttribs;
            
            bool* mpbLastVertexAttribState;
            bool* mpbCurrentVertexAttribState;
            
            std::vector<std::string> m_textureUniformNames;
            
            CSCore::Vector2 mvCachedScissorPos;
            CSCore::Vector2 mvCachedScissorSize;
            
            const CSRendering::Material* mpCurrentMaterial;
            Shader* m_currentShader;
            
            u32 mudwViewWidth;
            u32 mudwViewHeight;
            
            u32 mudwNumBoundTextures;
            
            bool mbInvalidateAllCaches;
            
            struct VertexAttribSet
            {
                CSRendering::MeshBuffer* pBuffer;
                GLint size;
                GLenum type;
                GLboolean normalised;
                GLsizei stride;
                const GLvoid* offset;
            };
            
            std::unordered_map<std::string, VertexAttribSet> m_attributeCache;
            
            struct RenderStates
			{
                RenderStates()
                :
                mbIsAlphaBlendingEnabled(false), mbIsDepthTestEnabled(false), mbIsCullingEnabled(false),
                mbIsColourWriteEnabled(false), mbIsDepthWriteEnabled(false), mbIsScissorTestingEnabled(false)
                {
                    
                }
				bool mbIsAlphaBlendingEnabled : 1;
				bool mbIsDepthTestEnabled : 1;
				bool mbIsCullingEnabled : 1;
				bool mbIsColourWriteEnabled : 1;
				bool mbIsDepthWriteEnabled : 1;
                bool mbIsScissorTestingEnabled : 1;
			};
			
			RenderStates msCurrentRenderFlags;
            RenderStates msCurrentRenderLocks;
            
            bool mbBlendFunctionLocked;
            bool mbInvalidateLightingCache;
            
            bool m_hasContextBeenBackedUp;
            
            CSRendering::BlendMode mSrcBlendFunc;
            CSRendering::BlendMode mDstBlendFunc;
            CSRendering::CullFace meCurrentCullFace;
            CSRendering::DepthTestComparison meDepthFunc;
            
#ifdef CS_TARGETPLATFORM_ANDROID
            ContextRestorer m_contextRestorer;
#endif
   
            bool m_hasContext = false;
            
            CSCore::Screen* m_screen;
            CSCore::EventConnectionUPtr m_resolutionChangeConnection;
            CSCore::EventConnectionUPtr m_displayModeChangeConnection;
		};
	}
}
#endif
