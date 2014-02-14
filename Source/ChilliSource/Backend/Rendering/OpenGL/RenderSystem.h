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

#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Main/Colour.h>

#include <ChilliSource/Rendering/ForwardDeclarations.h>
#include <ChilliSource/Rendering/Main/RenderSystem.h>

#include <ChilliSource/Backend/Rendering/OpenGL/ForwardDeclarations.h>
#include <ChilliSource/Backend/Rendering/OpenGL/TextureManager.h>
#include <ChilliSource/Backend/Rendering/OpenGL/CubemapManager.h>
#include <ChilliSource/Backend/Rendering/OpenGL/ShaderManager.h>

#include <ChilliSource/Backend/Rendering/OpenGL/GLIncludes.h>

namespace moFlo
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
		class CRenderSystem : public moFlo::Rendering::IRenderSystem
		{
		public:
			DECLARE_NAMED_INTERFACE(CRenderSystem);
			CRenderSystem();
            ~CRenderSystem();
            //----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(moFlo::Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------
			/// Init
			///
			/// Set up default client states etc
            ///
            /// @param Width
            /// @param Height
            /// @return Success
			//----------------------------------------------------------
			bool Init(u32 inudwWidth, u32 inudwHeight);
            //----------------------------------------------------------
            /// Resume
            ///
            /// Called when the app resumes from an inactive state.
            /// Something may have stole our context so we need to
            /// check and bind it
            //----------------------------------------------------------
            void Resume();
            //----------------------------------------------------------
			/// Suspend
			///
			/// Called when the app suspends. This can be used to prepare
			/// For re-setting up the context in resume.
			//----------------------------------------------------------
			void Suspend();
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
			/// Create Buffer
			///
			/// Create a hardware accelerated VBO
			/// @return An instantiated buffer
			//----------------------------------------------------------
			Rendering::IMeshBuffer* CreateBuffer(Rendering::BufferDescription &inDesc);
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
			void RenderVertexBuffer(Rendering::IMeshBuffer* inpBuffer, u32 inudwOffset, u32 inudwNumVerts, const Core::CMatrix4x4& inmatWorld);
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
			void RenderBuffer(Rendering::IMeshBuffer* inpBuffer, u32 inudwOffset, u32 inudwNumIndices, const Core::CMatrix4x4& inmatWorld);
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
			void ApplyCamera(const Core::CVector3& invPos, const Core::CMatrix4x4& inmatView, const Core::CMatrix4x4& inmatProj, const Core::CColour& inClearCol);
            //----------------------------------------------------------
            /// Set Light
            ///
            /// Pass the lighting to the render system for the active scene
            ///
            /// @param lighting components
            //----------------------------------------------------------
            void SetLight(Rendering::ILightComponent* inpLightComponent);
			//----------------------------------------------------------
			/// Apply Material
			///
			/// Apply the material to the next render batch
			///
			/// @param Material
			//----------------------------------------------------------
			void ApplyMaterial(const Rendering::CMaterial& inMaterial);
            //----------------------------------------------------------
			/// Apply Joints
			///
			/// Applies the joint matrices to the next render batch.
			///
			/// @param Dynamic array of joint matrices.
			//----------------------------------------------------------
            void ApplyJoints(const DYNAMIC_ARRAY<Core::CMatrix4x4>& inaJoints);
			//----------------------------------------------------------
			/// Create Render Target
			///
			/// Create a GL render target
            ///
            /// @param Width
            /// @param Height
			/// @return An instantiated target
			//----------------------------------------------------------
			Rendering::IRenderTarget* CreateRenderTarget(u32 inudwWidth, u32 inudwHeight);
			//----------------------------------------------------------
			/// Get Default Render Target
			///
			/// @return Onscreen render target
			//----------------------------------------------------------
			Rendering::IRenderTarget* GetDefaultRenderTarget();
			//----------------------------------------------------------
			/// Resize Frame Buffer
			///
			/// Re-build the frame buffer if the screen dimensions
			/// change
			///
			/// @param Width
			/// @param Height
			//----------------------------------------------------------
			void ResizeFrameBuffer(u32 inudwWidth, u32 inudwHeight);
            //----------------------------------------------------------
            /// Get Path To Shaders
            ///
            /// @return Where the shaders live in the bundle
            //----------------------------------------------------------
            std::string GetPathToShaders() const;
			//----------------------------------------------------------
			/// On Screen Orientation Changed
			///
			/// Resize the frame buffer
			/// @param Width
			/// @param Height
			//----------------------------------------------------------
			void OnScreenOrientationChanged(u32 inudwWidth, u32 inudwHeight);
            //----------------------------------------------------------
			/// Destroy
			///
			/// Release the OpenGL ES context and any assets
			//----------------------------------------------------------
            void Destroy();
            
            //---Render states
            //----------------------------------------------------------
            /// Lock Alpha Blending
            ///
            /// Prevent alpha blending being altered
            //----------------------------------------------------------
            void LockAlphaBlending();
            //----------------------------------------------------------
            /// Unlock Alpha Blending
            ///
            /// Allow alpha blending to be altered
            //----------------------------------------------------------
            void UnlockAlphaBlending();
            //----------------------------------------------------------
			/// Enable Alpha Blending
			///
			/// Turns alpha blending on or off
			/// @param Boolean to toggle enable/disable
			//----------------------------------------------------------
			void EnableAlphaBlending(bool inbIsEnabled);
			//----------------------------------------------------------
			/// Enable Depth Testing
			///
			/// Turns depth testing on or off
			/// @param Boolean to toggle enable/disable
			//----------------------------------------------------------
			void EnableDepthTesting(bool inbIsEnabled);
			//----------------------------------------------------------
			/// Enable Face Culling
			///
			/// Turns culling on or off
			/// @param Boolean to toggle enable/disable
			//----------------------------------------------------------
			void EnableFaceCulling(bool inbIsEnabled);
			//----------------------------------------------------------
			/// Enable Colour Writing
			///
			/// Turns colour writing on or off.
			/// @param Boolean to toggle enable/disable
			//----------------------------------------------------------
			void EnableColourWriting(bool inbIsEnabled);
            //----------------------------------------------------------
            /// Lock Depth Writing
            ///
            /// Prevent depth writing from being altered
            //----------------------------------------------------------
            void LockDepthWriting();
            //----------------------------------------------------------
            /// Unlock Depth Writing
            ///
            /// Allow depth write to be altered
            //----------------------------------------------------------
            void UnlockDepthWriting();
            //----------------------------------------------------------
			/// Enable Depth Writing
			///
			/// Turns depth writing on or off. Used for alpha blending
			/// @param Boolean to toggle enable/disable
            /// @param Whether to lock to prevent this being changed
			//----------------------------------------------------------
			void EnableDepthWriting(bool inbIsEnabled);
            //----------------------------------------------------------
            /// Enable Scissor Testing
            ///
            /// Turns on scissoring. Used to render to only a portion
            /// of the screen
            /// @param Enable/disable
            //----------------------------------------------------------
            void EnableScissorTesting(bool inbIsEnabled);
            //----------------------------------------------------------
            /// Set Scissor Region
            ///
            /// Set the rectangle region of the screen that is active
            /// when scissoring is enabled
            ///
            /// @param Position of the bottom left corner of the rect
            /// @param Size of the scissor region
            //---------------------------------------------------------
            void SetScissorRegion(const Core::CVector2& invPosition, const Core::CVector2& invSize);
            //----------------------------------------------------------
			/// Set Cull Face
			///
			/// Sets the cull face
			/// @param Cull face
			//----------------------------------------------------------
			void SetCullFace(Rendering::CullFace ineCullface);
            //----------------------------------------------------------
            /// Lock Blend Function
            ///
            /// Prevent the blend function being altered
            //----------------------------------------------------------
            void LockBlendFunction();
            //----------------------------------------------------------
            /// Unlock Blend Function
            ///
            /// Allow the blend function to be altered
            //----------------------------------------------------------
            void UnlockBlendFunction();
			//----------------------------------------------------------
			/// Set Blend Function
			///
			/// @param Source function
			/// @param Destination function
			//----------------------------------------------------------
			void SetBlendFunction(Rendering::AlphaBlend ineSrcFunc, Rendering::AlphaBlend ineDstFunc);
            //----------------------------------------------------------
			/// Set Depth Function
			///
            /// @param Function to be used for depth testing
			//----------------------------------------------------------
            void SetDepthFunction(Rendering::DepthFunction ineFunc);
            //----------------------------------------------------------
            /// Remove Buffer
            ///
            /// Removes a mesh buffer from the tracked list. Does
            /// not destroy it!
            ///
            /// @param Mesh buffer pointer.
            //----------------------------------------------------------
            void RemoveBuffer(Rendering::IMeshBuffer* inpBuffer);
            //----------------------------------------------------------
			/// Check for OpenGL Extension
			///
			/// @return Whether the given extension is supported
			//----------------------------------------------------------
			static bool CheckForOpenGLExtension(const std::string& instrExtension);
			
		private:
            
            //----------------------------------------------------------
            /// Apply Render States
            ///
            /// @param Material
            //----------------------------------------------------------
            void ApplyRenderStates(const Rendering::CMaterial& inMaterial);
            //----------------------------------------------------------
            /// Get Attribute Locations
            ///
            /// @param Shader
            //----------------------------------------------------------
            void GetAttributeLocations(const ShaderPtr& inpShader);
            //----------------------------------------------------------
            /// Get Uniform Locations
            ///
            /// @param Material
            //----------------------------------------------------------
            void GetUniformLocations(const Rendering::CMaterial &inMaterial);
            //----------------------------------------------------------
            /// Apply Textures
            ///
            /// Pass the texture variables to the shader
            ///
            /// @param Material
            //----------------------------------------------------------
            void ApplyTextures(const Rendering::CMaterial &inMaterial);
            //----------------------------------------------------------
            /// Apply Lighting Values
            ///
            /// Pass the lighting variables to the shader
            ///
            /// @param Material
            //----------------------------------------------------------
            void ApplyLightingValues(const Rendering::CMaterial &inMaterial);
			//------------------------------------------------------------
			/// Enable Vertex Attribute For Semantic (Programmable pipeline)
			///
			/// Set the pointers and enable the state based on the vertex
			/// layout of the buffer.
			///
			/// @param Element
			//------------------------------------------------------------
			void EnableVertexAttributeForSemantic(Rendering::IMeshBuffer* inpBuffer);
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
			void BeginFrame(Rendering::IRenderTarget* inpActiveRenderTarget);
			//----------------------------------------------------------
			/// End Frame
			///
			/// Present the buffer to the screen
			//----------------------------------------------------------
			void EndFrame(Rendering::IRenderTarget* inpActiveRenderTarget);
			//----------------------------------------------------------
			/// Apply Shader Variables
			///
			/// Send the custom variables to the given shader
			///
			/// @param Material
			/// @param Shader program
			//----------------------------------------------------------
			void ApplyShaderVariables(const Rendering::CMaterial &inMaterial, GLuint inShaderProg);
            //----------------------------------------------------------
            /// Apply Lighting
            ///
            /// Pass the lighting variables to the shader
            ///
            /// @param Material
            /// @param Lighting component
            //----------------------------------------------------------
            void ApplyLighting(const Rendering::CMaterial &inMaterial, Rendering::ILightComponent* inpLightComponent);
			//----------------------------------------------------------
			/// Force Refresh Render States
			///
			/// Ignores all caching and force changes render states
			//----------------------------------------------------------
			void ForceRefreshRenderStates();
			//----------------------------------------------------------
			/// Backup Mesh Buffers
			///
			/// Copy all active mesh buffers into memory to be
			/// restored at a later date. This should only be called
			/// if the GL context is about to be lost
			//----------------------------------------------------------
			void BackupMeshBuffers();
			//----------------------------------------------------------
			/// Restore Mesh Buffers
			///
			/// Copy all backups from memory into buffers. This is
			/// called after a context is recreated having been lost
			//----------------------------------------------------------
			void RestoreMeshBuffers();
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
			
		private:
			s32 GetLocationForVertexSemantic(moRendering::SEMANTIC ineSemantic);
            bool ApplyVertexAttributePointr(Rendering::IMeshBuffer* inpBuffer,
                                            GLuint inudwLocation, GLint indwSize, GLenum ineType, GLboolean inbNormalized, GLsizei indwStride, const GLvoid* inpOffset);
            
            Rendering::ILightComponent* mpLightComponent;
            
            Core::CColour mNewClearColour;
			Core::CColour mClearColour;
            Core::CColour mCurrentEmissive;
            Core::CColour mCurrentAmbient;
            Core::CColour mCurrentDiffuse;
            Core::CColour mCurrentSpecular;
            bool mbEmissiveSet;
            bool mbAmbientSet;
            bool mbDiffuseSet;
            bool mbSpecularSet;
            
			Core::CMatrix4x4 mmatProj;
			Core::CMatrix4x4 mmatViewProj;
            Core::CMatrix4x4 mmatView;
            Core::CVector3 mvCameraPos;
            
			CTextureManager mTexManager;
            CCubemapManager mCubemapManager;
			CShaderManager mShaderManager;
			
            CRenderCapabilities* mpRenderCapabilities;
            
			GLint mwPosAttributeLocation;
			GLint mwColAttributeLocation;
			GLint mwNormAttributeLocation;
			GLint mwTexAttributeLocation;
			GLint mwWeiAttributeLocation;
			GLint mwJIAttributeLocation;
			
            GLint mdwMaxVertAttribs;
            
            bool* mpbLastVertexAttribState;
            bool* mpbCurrentVertexAttribState;
            
			GLint mGLCurrentShaderProgram;
			
			GLint mmatWVPHandle;
            GLint mmatWorldHandle;
			GLint mmatNormalHandle;
            GLint mEmissiveHandle;
			GLint mAmbientHandle;
			GLint mDiffuseHandle;
			GLint mSpecularHandle;
			GLint mJointsHandle;
            
            GLint mLightPosHandle;
            GLint mLightDirHandle;
            GLint mLightColHandle;
            GLint mAttenConHandle;
            GLint mAttenLinHandle;
            GLint mAttenQuadHandle;
            GLint mShadowMapTexHandle;
            GLint mShadowToleranceHandle;
            GLint mLightMatrixHandle;
            GLint mCameraPosHandle;
            GLint mCubemapHandle;
            std::pair<GLint, u32>* mpaTextureHandles;
            
			CRenderTarget* mpDefaultRenderTarget;
            
            Core::CVector2 mvCachedScissorPos;
            Core::CVector2 mvCachedScissorSize;
            
            const Rendering::CMaterial* mpCurrentMaterial;
            
            u32 mudwViewWidth;
            u32 mudwViewHeight;
            
            u32 mudwNumBoundTextures;
            
            bool mbInvalidateAllCaches;
            
            struct VertexAttribSet
            {
                Rendering::IMeshBuffer * pBuffer;
                GLint size;
                GLenum type;
                GLboolean normalised;
                GLsizei   stride;
                GLvoid*   offset;
            };
            VertexAttribSet * mpVertexAttribs;
            
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
            
            Rendering::AlphaBlend mSrcBlendFunc;
            Rendering::AlphaBlend mDstBlendFunc;
            Rendering::CullFace meCurrentCullFace;
            Rendering::DepthFunction meDepthFunc;
            
#ifdef TARGET_OS_IPHONE
            EAGLContext* mContext;
#elif defined TARGET_ANDROID
            DYNAMIC_ARRAY<CMeshBuffer*> mMeshBuffers;
#endif
		};
	}
}
#endif
