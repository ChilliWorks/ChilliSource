/*
 *  OpenGLES_2_0.h
 *  moFlo
 *
 *  Created by Scott Downie on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_OPENGL_ES2_RENDER_SYSTEM_H_
#define _MOFLOW_OPENGL_ES2_RENDER_SYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Rendering/OpenGL/ForwardDeclarations.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Backend/Rendering/OpenGL/ForwardDeclarations.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/GLIncludes.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/ContextRestorer.h>

#include <unordered_map>

namespace ChilliSource
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
		class RenderSystem final : public Rendering::RenderSystem
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
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
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
            /// @author I Copland
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
			Rendering::MeshBuffer* CreateBuffer(Rendering::BufferDescription &inDesc) override;
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
			void RenderVertexBuffer(Rendering::MeshBuffer* inpBuffer, u32 inudwOffset, u32 inudwNumVerts, const Core::Matrix4& inmatWorld) override;
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
			void RenderBuffer(Rendering::MeshBuffer* inpBuffer, u32 inudwOffset, u32 inudwNumIndices, const Core::Matrix4& inmatWorld) override;
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
			void ApplyCamera(const Core::Vector3& invPos, const Core::Matrix4& inmatView, const Core::Matrix4& inmatProj, const Core::Colour& inClearCol) override;
            //----------------------------------------------------------
            /// Set Light
            ///
            /// Pass the lighting to the render system for the active scene
            ///
            /// @param lighting components
            //----------------------------------------------------------
            void SetLight(Rendering::LightComponent* inpLightComponent) override;
			//----------------------------------------------------------
			/// Apply Material
			///
			/// Apply the material to the next render batch
			///
			/// @param Material
            /// @param Shader pass
			//----------------------------------------------------------
			void ApplyMaterial(const Rendering::MaterialCSPtr& inMaterial, Rendering::ShaderPass in_shaderPass) override;
            //----------------------------------------------------------
			/// Apply Joints
			///
			/// Applies the joint matrices to the next render batch.
			///
			/// @param Dynamic array of joint matrices.
			//----------------------------------------------------------
            void ApplyJoints(const std::vector<Core::Matrix4>& inaJoints) override;
			//----------------------------------------------------------
			/// Create Render Target
			///
			/// Create a GL render target
            ///
            /// @param Width
            /// @param Height
			/// @return An instantiated target
			//----------------------------------------------------------
			Rendering::RenderTarget* CreateRenderTarget(u32 inudwWidth, u32 inudwHeight) override;
            
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
            void SetScissorRegion(const Core::Vector2& invPosition, const Core::Vector2& invSize) override;
            //----------------------------------------------------------
			/// Set Cull Face
			///
			/// Sets the cull face
			/// @param Cull face
			//----------------------------------------------------------
			void SetCullFace(Rendering::CullFace ineCullface) override;
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
			void SetBlendFunction(Rendering::BlendMode ineSrcFunc, Rendering::BlendMode ineDstFunc) override;
            //----------------------------------------------------------
			/// Set Depth Function
			///
            /// @param Function to be used for depth testing
			//----------------------------------------------------------
            void SetDepthFunction(Rendering::DepthTestComparison ineFunc) override;
            //----------------------------------------------------------
            /// Remove Buffer
            ///
            /// Removes a mesh buffer from the tracked list. Does
            /// not destroy it!
            ///
            /// @param Mesh buffer pointer.
            //----------------------------------------------------------
            void RemoveBuffer(Rendering::MeshBuffer* inpBuffer);
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @return If the GL context still exists
            //----------------------------------------------------------
            bool HasContext() const;
			
		private:
            
            friend Rendering::RenderSystemUPtr Rendering::RenderSystem::Create(Rendering::RenderCapabilities*);
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            ///
            /// @param Render capabilities
            //-------------------------------------------------------
            RenderSystem(Rendering::RenderCapabilities* in_renderCapabilities);
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
            void ApplyRenderStates(const Rendering::Material* inMaterial);
            //----------------------------------------------------------
            /// Apply Textures
            ///
            /// Pass the texture variables to the shader
            ///
            /// @param Material
            /// @param Shader
            //----------------------------------------------------------
            void ApplyTextures(const Rendering::Material* inMaterial, Shader* out_shader);
            //----------------------------------------------------------
            /// Apply Lighting Values
            ///
            /// Pass the lighting variables to the shader
            ///
            /// @param Material
            /// @param [Out] Shader
            //----------------------------------------------------------
            void ApplyLightingValues(const Rendering::Material* inMaterial, Shader* out_shader);
			//------------------------------------------------------------
			/// Enable Vertex Attribute For Semantic (Programmable pipeline)
			///
			/// Set the pointers and enable the state based on the vertex
			/// layout of the buffer.
			///
			/// @param Element
			//------------------------------------------------------------
			void EnableVertexAttributeForSemantic(Rendering::MeshBuffer* inpBuffer);
			//------------------------------------------------------------
			/// Get Primitive Type
			///
			/// Convert fro moFlo primitive type to GL primitive type
			///
			/// @param moFlo primitive type
			//------------------------------------------------------------
			s32 GetPrimitiveType(Rendering::PrimitiveType inType);
            //----------------------------------------------------------
			/// Begin Frame
			///
			/// Clear the back buffer to the clear colour
			//----------------------------------------------------------
			void BeginFrame(Rendering::RenderTarget* inpActiveRenderTarget) override;
			//----------------------------------------------------------
			/// End Frame
			///
			/// Present the buffer to the screen
			//----------------------------------------------------------
			void EndFrame(Rendering::RenderTarget* inpActiveRenderTarget) override;
			//----------------------------------------------------------
			/// Apply Shader Variables
			///
			/// Send the custom variables to the given shader
			///
			/// @param Material
			/// @param Shader
			//----------------------------------------------------------
			void ApplyShaderVariables(const Rendering::Material* inMaterial, Shader* out_shader);
            //----------------------------------------------------------
            /// Apply Lighting
            ///
            /// Pass the lighting variables to the shader
            ///
            /// @param Lighting component
            /// @param [Out] Shader
            //----------------------------------------------------------
            void ApplyLighting(Rendering::LightComponent* inpLightComponent, Shader* out_shader);
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
			/// CheckForGLErrors
			///
			/// Checks for any opengl errors and logs any errors found.
			//----------------------------------------------------------
			static void CheckForGLErrors();
            //----------------------------------------------------------
            /// Create Attrib State Cache
            //----------------------------------------------------------
            void CreateAttribStateCache();
            //----------------------------------------------------------
            /// Called when the resolution of the screen changes. This
            /// then updates the size of the frame buffer.
            ///
            /// @author I Copland
            ///
            /// @param The new screen resolution.
            //----------------------------------------------------------
            void OnScreenResolutionChanged(const Core::Vector2& in_resolution);
            
		private:
			
            void ApplyVertexAttributePointr(Rendering::MeshBuffer* inpBuffer,
                                            const char* in_attribName, GLint indwSize, GLenum ineType, GLboolean inbNormalized, GLsizei indwStride, const GLvoid* inpOffset);
            
            Rendering::LightComponent* mpLightComponent;
            
            Core::Colour mNewClearColour;
			Core::Colour mClearColour;
            Core::Colour mCurrentEmissive;
            Core::Colour mCurrentAmbient;
            Core::Colour mCurrentDiffuse;
            Core::Colour mCurrentSpecular;
            bool mbEmissiveSet;
            bool mbAmbientSet;
            bool mbDiffuseSet;
            bool mbSpecularSet;
            
			Core::Matrix4 mmatProj;
			Core::Matrix4 mmatViewProj;
            Core::Matrix4 mmatView;
            Core::Vector3 mvCameraPos;
			
            RenderCapabilities* mpRenderCapabilities;
            TextureUnitSystem* m_textureUnitSystem;
			
            GLint mdwMaxVertAttribs;
            
            bool* mpbLastVertexAttribState;
            bool* mpbCurrentVertexAttribState;
            
            std::vector<std::string> m_textureUniformNames;
            
            Core::Vector2 mvCachedScissorPos;
            Core::Vector2 mvCachedScissorSize;
            
            const Rendering::Material* mpCurrentMaterial;
            Shader* m_currentShader;
            
            u32 mudwViewWidth;
            u32 mudwViewHeight;
            
            u32 mudwNumBoundTextures;
            
            bool mbInvalidateAllCaches;
            
            struct VertexAttribSet
            {
                Rendering::MeshBuffer* pBuffer;
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
            bool mbInvalidateLigthingCache;
            
            bool m_hasContextBeenBackedUp;
            
            Rendering::BlendMode mSrcBlendFunc;
            Rendering::BlendMode mDstBlendFunc;
            Rendering::CullFace meCurrentCullFace;
            Rendering::DepthTestComparison meDepthFunc;
            
#ifdef CS_TARGETPLATFORM_ANDROID
            ContextRestorer m_contextRestorer;
#endif
            
            bool m_hasContext = false;
            
            Core::Screen* m_screen;
            Core::EventConnectionUPtr m_resolutionChangeConnection;
		};
	}
}
#endif
