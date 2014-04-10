/*
 *  OpenGLES_2_0.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 27/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderSystem.h>

#include <ChilliSource/Backend/Rendering/OpenGL/Base/MeshBuffer.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderCapabilities.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderTarget.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Texture/Cubemap.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Texture/Texture.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Rendering/Lighting/AmbientLightComponent.h>
#include <ChilliSource/Rendering/Lighting/DirectionalLightComponent.h>
#include <ChilliSource/Rendering/Lighting/LightComponent.h>
#include <ChilliSource/Rendering/Lighting/PointLightComponent.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Base/CullFace.h>
#include <ChilliSource/Rendering/Base/DepthTestComparison.h>
#include <ChilliSource/Rendering/Base/BlendMode.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <UIKit/UIKit.h>
#else
#include <ChilliSource/Core/Base/Application.h>
#endif

#ifdef CS_ENABLE_DEBUGSTATS
#include <ChilliSource/Debugging/Base/DebugStats.h>
#endif

namespace ChilliSource
{
	namespace OpenGL
	{
        CS_DEFINE_NAMEDTYPE(RenderSystem);
        
		//----------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------
		RenderSystem::RenderSystem(Rendering::RenderCapabilities* in_renderCapabilities)
		: mpDefaultRenderTarget(nullptr), mpCurrentMaterial(nullptr), mbInvalidateAllCaches(true), mdwMaxVertAttribs(0), mpVertexAttribs(nullptr),
        mbEmissiveSet(false), mbAmbientSet(false), mbDiffuseSet(false), mbSpecularSet(false), mudwNumBoundTextures(0), mpLightComponent(nullptr),
        mbBlendFunctionLocked(false), mpaTextureHandles(nullptr), mbInvalidateLigthingCache(true),
        mpRenderCapabilities(static_cast<RenderCapabilities*>(in_renderCapabilities)), m_hasContextBeenBackedUp(false)
		{
			//Register the GL texture and shader managers
            Core::ResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mTexManager);
            Core::ResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mCubemapManager);
            Core::ResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mShaderManager);
		}
        //----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool RenderSystem::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == RenderSystem::InterfaceID || inInterfaceID == Rendering::RenderSystem::InterfaceID || inInterfaceID == Core::IComponentProducer::InterfaceID;
		}
        //----------------------------------------------------------
        //----------------------------------------------------------
        void RenderSystem::Init()
		{
#ifdef CS_TARGETPLATFORM_IOS
            //Create the context with the specified GLES version
			mContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
            
			//Set the current context
            if(!mContext || ![EAGLContext setCurrentContext: mContext])
            {
                CS_LOG_FATAL("Cannot Create OpenGL ES 2.0 Context");
            }
#endif
#ifdef CS_TARGETPLATFORM_WINDOWS
			GLenum GlewError = glewInit();
			if (GLEW_OK != GlewError)
			{
				//Problem: glewInit failed, something is seriously wrong.
				CS_LOG_FATAL("Glew Error On Init: " + std::string((const char*)glewGetErrorString(GlewError)));
			}
#endif
            
            CS_ASSERT((Core::Screen::GetRawDimensions() > Core::Vector2::ZERO), "Cannot create and OpenGL ES view with size ZERO");
            

            CS_ASSERT(mpRenderCapabilities, "Cannot find required system: Render Capabilities.");
            mpRenderCapabilities->DetermineCapabilities();
            
            m_textureUniformNames.reserve(mpRenderCapabilities->GetNumTextureUnits());
            for(u32 i=0; i<mpRenderCapabilities->GetNumTextureUnits(); ++i)
            {
                m_textureUniformNames.push_back(std::string("uTexture" + Core::ToString(i)));
            }
            
            ForceRefreshRenderStates();
			
            mShaderManager.SetRenderSystem(this);
            OnScreenOrientationChanged((u32)Core::Screen::GetRawDimensions().x, (u32)Core::Screen::GetRawDimensions().y);
            
            m_hasContextBeenBackedUp = false;
		}
        //----------------------------------------------------------
        //----------------------------------------------------------
        void RenderSystem::Resume()
        {
			mbInvalidateAllCaches = true;
			
#ifdef CS_TARGETPLATFORM_IOS
            if([EAGLContext currentContext] != mContext)
            {
                [EAGLContext setCurrentContext:mContext];
            }
#endif
            RestoreContext();
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void RenderSystem::Suspend()
        {
        	BackupContext();
        }
        //----------------------------------------------------------
        /// Backup Context
        //----------------------------------------------------------
        void RenderSystem::BackupContext()
        {
#ifdef CS_TARGETPLATFORM_ANDROID
            if(m_hasContextBeenBackedUp == false)
            {
                //Context is about to be lost do a data backup
                BackupMeshBuffers();
                mTexManager.Backup();
                m_hasContextBeenBackedUp = true;
            }
#endif
        }
        //----------------------------------------------------------
		/// Restore Context
		//----------------------------------------------------------
		void RenderSystem::RestoreContext()
		{
#ifdef CS_TARGETPLATFORM_ANDROID
            if(m_hasContextBeenBackedUp == true)
            {
                ForceRefreshRenderStates();
                RestoreMeshBuffers();
                mGLCurrentShaderProgram = 0;
                mShaderManager.Restore();
                mTexManager.Restore();
                mCubemapManager.Restore();
                m_hasContextBeenBackedUp = false;
            }
#endif
		}
        //----------------------------------------------------------
        /// Set Light
        //----------------------------------------------------------
        void RenderSystem::SetLight(Rendering::LightComponent* inpLightComponent)
        {
            if(inpLightComponent == mpLightComponent && mbInvalidateAllCaches == false)
            {
                if(mpLightComponent != nullptr)
                {
                    mbInvalidateLigthingCache = mpLightComponent->IsCacheValid() == false;
                }
                
                return;
            }
            
            mpLightComponent = inpLightComponent;
            mbInvalidateLigthingCache = true;
        }
        //----------------------------------------------------------
		//----------------------------------------------------------
		void RenderSystem::ApplyMaterial(const Rendering::MaterialCSPtr& in_material, Rendering::ShaderPass in_shaderPass)
		{
            //TODO: We can remove alot of these cache checks one we move to our render command buffer
            //as the apply material function will only be called once per "batch" rather than for each
            //element in the batch as it is now.
            
            CS_ASSERT(in_material != nullptr, "Cannot apply null material");
            
            Shader* shader = static_cast<Shader*>(in_material->GetShader(in_shaderPass).get());
            CS_ASSERT(shader != nullptr, "Cannot render with null shader");
            
            GLuint shaderProgram = shader->GetProgramID();
            CS_ASSERT(shaderProgram != 0, "Cannot render with no GL shader program");
            
            bool hasMaterialChanged = mbInvalidateAllCaches == true || mpCurrentMaterial == nullptr || mpCurrentMaterial != in_material.get() || mpCurrentMaterial->IsCacheValid() == false;
            if(hasMaterialChanged == true)
            {
                mudwNumBoundTextures = 0;
                mpCurrentMaterial = in_material.get();
                
                //Bind this materials shader
                ApplyRenderStates(mpCurrentMaterial);
                
                //Don't bind the shader if we don't need to!
                bool hasShaderChanged = mbInvalidateAllCaches || shaderProgram != mGLCurrentShaderProgram;
                if(hasShaderChanged == true)
                {
                    glUseProgram(shaderProgram);
                    mGLCurrentShaderProgram = shaderProgram;
                    
                    GetAttributeLocations(shader);
                    GetUniformLocations(shader);
                    
                    mbEmissiveSet = false;
                    mbAmbientSet = false;
                    mbDiffuseSet = false;
                    mbSpecularSet = false;
                    mbInvalidateLigthingCache = true;
                }
                
                //Get and set all the custom shader variables
                if(mpCurrentMaterial->IsVariableCacheValid() == false || hasShaderChanged == true)
                {
                    ApplyShaderVariables(mpCurrentMaterial, shader);
                }
                
                ApplyTextures(mpCurrentMaterial);
                ApplyLightingValues(mpCurrentMaterial);
                ApplyLighting(mpLightComponent);
                
                //TODO: Once we change the render system to be a list of commands this hack
                //will not be required
                const_cast<Rendering::Material*>(mpCurrentMaterial)->SetCacheValid();
            }
		}
        //----------------------------------------------------------
        /// Apply Joints
        //----------------------------------------------------------
        void RenderSystem::ApplyJoints(const std::vector<Core::Matrix4x4>& inaJoints)
        {
            if(mJointsHandle != -1)
            {
                //remove the final column from the joint matrix data as it is always going to be [0 0 0 1].
                std::vector<Core::Vector4> aJointVectors;
                for (std::vector<Core::Matrix4x4>::const_iterator it = inaJoints.begin(); it != inaJoints.end(); ++it)
                {
                    aJointVectors.push_back(Core::Vector4(it->m[0], it->m[4], it->m[8], it->m[12]));
                    aJointVectors.push_back(Core::Vector4(it->m[1], it->m[5], it->m[9], it->m[13]));
                    aJointVectors.push_back(Core::Vector4(it->m[2], it->m[6], it->m[10], it->m[14]));
                }
                
                glUniform4fv(mJointsHandle, aJointVectors.size(), (GLfloat*)&(aJointVectors[0]));
            }
        }
        //----------------------------------------------------------
		/// Apply Render States
		//----------------------------------------------------------
		void RenderSystem::ApplyRenderStates(const Rendering::Material* inMaterial)
        {
            EnableAlphaBlending(inMaterial->IsTransparencyEnabled());
            SetBlendFunction(inMaterial->GetSourceBlendMode(), inMaterial->GetDestBlendMode());

            EnableFaceCulling(inMaterial->IsFaceCullingEnabled());
            SetCullFace(inMaterial->GetCullFace());
            
            EnableColourWriting(inMaterial->IsColourWriteEnabled());
            EnableDepthWriting(inMaterial->IsDepthWriteEnabled());
            EnableDepthTesting(inMaterial->IsDepthTestEnabled());
        }
        //----------------------------------------------------------
		/// Get Attribute Locations
		//----------------------------------------------------------
		void RenderSystem::GetAttributeLocations(Shader* inpShader)
        {
            //Get the handles to the shader attributes
            
            GLint uwAttrib = inpShader->GetAttributeLocation("avPosition");
            if(uwAttrib != mwPosAttributeLocation)
            {
                mwPosAttributeLocation = uwAttrib;
            }
            uwAttrib = inpShader->GetAttributeLocation("avColour");
            if(uwAttrib != mwColAttributeLocation)
            {
                mwColAttributeLocation = uwAttrib;
            }
            uwAttrib = inpShader->GetAttributeLocation("avNormal");
            if(uwAttrib != mwNormAttributeLocation)
            {
                mwNormAttributeLocation = uwAttrib;
            }
            uwAttrib = inpShader->GetAttributeLocation("avTexCoord");
            if(uwAttrib != mwTexAttributeLocation)
            {
                mwTexAttributeLocation = uwAttrib;
            }
            uwAttrib = inpShader->GetAttributeLocation("avWeights");
            if(uwAttrib != mwWeiAttributeLocation)
            {
                mwWeiAttributeLocation = uwAttrib;
            }
            uwAttrib = inpShader->GetAttributeLocation("avJointIndices");
            if(uwAttrib != mwJIAttributeLocation)
            {
                mwJIAttributeLocation = uwAttrib;
            }
        }
        //----------------------------------------------------------
		/// Get Uniform Locations
		//----------------------------------------------------------
		void RenderSystem::GetUniformLocations(Shader* in_shader)
        {
            //Get the required handles to the shader variables (Uniform)
            mmatWVPHandle = in_shader->GetUniformLocation("umatWorldViewProj");
            mmatWorldHandle = in_shader->GetUniformLocation("umatWorld");
            mmatNormalHandle = in_shader->GetUniformLocation("umatNormal");
        
            mEmissiveHandle = in_shader->GetUniformLocation("uvEmissive");
            mAmbientHandle = in_shader->GetUniformLocation("uvAmbient");
            mDiffuseHandle = in_shader->GetUniformLocation("uvDiffuse");
            mSpecularHandle = in_shader->GetUniformLocation("uvSpecular");
    
            mJointsHandle = in_shader->GetUniformLocation("uavJoints");
            
            mLightPosHandle = in_shader->GetUniformLocation("uvLightPos");
            mLightDirHandle = in_shader->GetUniformLocation("uvLightDir");
            mLightColHandle = in_shader->GetUniformLocation("uvLightCol");
            mAttenConHandle = in_shader->GetUniformLocation("ufAttenuationConstant");
            mAttenLinHandle = in_shader->GetUniformLocation("ufAttenuationLinear");
            mAttenQuadHandle = in_shader->GetUniformLocation("ufAttenuationQuadratic");
            mCameraPosHandle = in_shader->GetUniformLocation("uvCameraPos");
            
            mLightMatrixHandle = in_shader->GetUniformLocation("umatLight");
            mShadowToleranceHandle = in_shader->GetUniformLocation("ufShadowTolerance");
            mShadowMapTexHandle = in_shader->GetUniformLocation("uShadowMapTexture");
            
            mCubemapHandle = in_shader->GetUniformLocation("uCubemap");
            
            if(mpaTextureHandles == nullptr)
            {
                mpaTextureHandles = (std::pair<GLint, u32>*)calloc(mpRenderCapabilities->GetNumTextureUnits(), sizeof(std::pair<GLint, u32>));
            }
            
            for(u32 i=0; i<mpRenderCapabilities->GetNumTextureUnits(); ++i)
            {
                mpaTextureHandles[i].first = in_shader->GetUniformLocation(m_textureUniformNames[i].c_str());
            }
        }
        //----------------------------------------------------------
		/// Apply Shader Variables
		//----------------------------------------------------------
		void RenderSystem::ApplyShaderVariables(const Rendering::Material* inMaterial, Shader* in_shader)
		{
			//Get and set all the custom shader variables
			for(auto it = inMaterial->m_floatVars.begin(); it!= inMaterial->m_floatVars.end(); ++it)
			{
				GLint VarHandle = in_shader->GetUniformLocation(it->first.c_str());
				glUniform1f(VarHandle, it->second);
			}
			for(auto it = inMaterial->m_vec2Vars.begin(); it!= inMaterial->m_vec2Vars.end(); ++it)
			{
				GLint VarHandle = in_shader->GetUniformLocation(it->first.c_str());
				glUniform2fv(VarHandle, 1, (GLfloat*)(&it->second));
			}
			for(auto it = inMaterial->m_vec3Vars.begin(); it!= inMaterial->m_vec3Vars.end(); ++it)
			{
				GLint VarHandle = in_shader->GetUniformLocation(it->first.c_str());
				glUniform3fv(VarHandle, 1, (GLfloat*)(&it->second));
			}
			for(auto it = inMaterial->m_vec4Vars.begin(); it!= inMaterial->m_vec4Vars.end(); ++it)
			{
				GLint VarHandle = in_shader->GetUniformLocation(it->first.c_str());
				glUniform4fv(VarHandle, 1, (GLfloat*)(&it->second));
			}
			for(auto it = inMaterial->m_mat4Vars.begin(); it!= inMaterial->m_mat4Vars.end(); ++it)
			{
				GLint VarHandle = in_shader->GetUniformLocation(it->first.c_str());
				glUniformMatrix4fv(VarHandle, 1, GL_FALSE, (GLfloat*)(it->second.m));
			}
			for(auto it = inMaterial->m_colourVars.begin(); it!= inMaterial->m_colourVars.end(); ++it)
			{
				GLint VarHandle = in_shader->GetUniformLocation(it->first.c_str());
				glUniform4fv(VarHandle, 1, (GLfloat*)(&it->second));
			}
		}
        //----------------------------------------------------------
        /// Apply Textures
        //----------------------------------------------------------
        void RenderSystem::ApplyTextures(const Rendering::Material* inMaterial)
        {
            //Cubemap takes precedence over texture
            if(mCubemapHandle >= 0 && inMaterial->GetCubemap() != nullptr)
            {
                inMaterial->GetCubemap()->Bind(mudwNumBoundTextures);
                glUniform1i(mCubemapHandle, mudwNumBoundTextures);
                ++mudwNumBoundTextures;
            }
            
            //Cannot bind more textures than the number of texture units
            u32 numTextureUnitsAvailable = mpRenderCapabilities->GetNumTextureUnits() - mudwNumBoundTextures;
            u32 numTexturesToBind = std::min(inMaterial->GetNumTextures(), numTextureUnitsAvailable);
            
            for(u32 i=0; i<numTexturesToBind; ++i)
            {
                if(mpaTextureHandles[i].first != -1)
                {
                    inMaterial->GetTexture(i)->Bind(mudwNumBoundTextures);
                    if(mpaTextureHandles[i].second != mudwNumBoundTextures || mbInvalidateAllCaches)
                    {
                        glUniform1i(mpaTextureHandles[i].first, mudwNumBoundTextures);
                        mpaTextureHandles[i].second = mudwNumBoundTextures;
                    }
                    ++mudwNumBoundTextures;
                }
            }
        }
        //----------------------------------------------------------
        /// Apply Lighting Values
        //----------------------------------------------------------
        void RenderSystem::ApplyLightingValues(const Rendering::Material* inMaterial)
        {
            if(mEmissiveHandle != -1 && (mbInvalidateAllCaches || mbEmissiveSet == false || mCurrentEmissive != inMaterial->GetEmissive()))
            {
                mbEmissiveSet = true;
                mCurrentEmissive = inMaterial->GetEmissive();
                glUniform4f(mEmissiveHandle, mCurrentEmissive.r, mCurrentEmissive.g, mCurrentEmissive.b, mCurrentEmissive.a);
            }
            if(mAmbientHandle != -1 && (mbInvalidateAllCaches || mbAmbientSet == false || mCurrentAmbient != inMaterial->GetAmbient()))
            {
                mbAmbientSet = true;
                mCurrentAmbient = inMaterial->GetAmbient();
                glUniform4f(mAmbientHandle, mCurrentAmbient.r, mCurrentAmbient.g, mCurrentAmbient.b, mCurrentAmbient.a);
            }
            if(mDiffuseHandle != -1 && (mbInvalidateAllCaches || mbDiffuseSet == false || mCurrentDiffuse != inMaterial->GetDiffuse()))
            {
                mbDiffuseSet = true;
                mCurrentDiffuse = inMaterial->GetDiffuse();
                glUniform4f(mDiffuseHandle, mCurrentDiffuse.r, mCurrentDiffuse.g, mCurrentDiffuse.b, mCurrentDiffuse.a);
            }
            if(mSpecularHandle != -1 && (mbInvalidateAllCaches || mbSpecularSet == false || mCurrentSpecular != inMaterial->GetSpecular()))
            {
                mbSpecularSet = true;
                mCurrentSpecular = inMaterial->GetSpecular();
                glUniform4f(mSpecularHandle, mCurrentSpecular.r, mCurrentSpecular.g, mCurrentSpecular.b, mCurrentSpecular.a);
            }
        }
        //----------------------------------------------------------
        /// Apply Lighting
        //----------------------------------------------------------
        void RenderSystem::ApplyLighting(Rendering::LightComponent* inpLightComponent)
        {
            if(mbInvalidateLigthingCache == false || inpLightComponent == nullptr)
                return;
            
            mbInvalidateLigthingCache = false;
            inpLightComponent->CalculateLightingValues();
            inpLightComponent->SetCacheValid();
            
            if(inpLightComponent->IsA(Rendering::DirectionalLightComponent::InterfaceID))
            {
                Rendering::DirectionalLightComponent* pLightComponent = (Rendering::DirectionalLightComponent*)inpLightComponent;
                
                if(mLightDirHandle >= 0)
                {
                    Core::Vector3 vDirection = pLightComponent->GetDirection();
                    glUniform3fv(mLightDirHandle, 1, (const GLfloat*)&vDirection);
                }
                if(pLightComponent->GetShadowMapPtr() != nullptr)
                {
                    if(mShadowToleranceHandle >= 0)
                    {
                        glUniform1f(mShadowToleranceHandle, pLightComponent->GetShadowTolerance());
                    }
                    
                    //If we have used all the texture units then we cannot bind the shadow map
                    if(mShadowMapTexHandle >= 0 && mudwNumBoundTextures <= mpRenderCapabilities->GetNumTextureUnits())
                    {
                        pLightComponent->GetShadowMapPtr()->Bind(mudwNumBoundTextures);
                        glUniform1i(mShadowMapTexHandle, mudwNumBoundTextures);
                        ++mudwNumBoundTextures;
                    }
                }
            }
            else if(inpLightComponent->IsA(Rendering::PointLightComponent::InterfaceID))
            {
                Rendering::PointLightComponent* pLightComponent = (Rendering::PointLightComponent*)inpLightComponent;
                
                if(mAttenConHandle >= 0)
                {
                    glUniform1f(mAttenConHandle, pLightComponent->GetConstantAttenuation());
                }
                if(mAttenLinHandle >= 0)
                {
                    glUniform1f(mAttenLinHandle, pLightComponent->GetLinearAttenuation());
                }
                if(mAttenQuadHandle >= 0)
                {
                    glUniform1f(mAttenQuadHandle, pLightComponent->GetQuadraticAttenuation());
                }
            }
            
            if(mLightPosHandle >= 0)
            {
                glUniform3fv(mLightPosHandle, 1, (const GLfloat*)&inpLightComponent->GetWorldPosition());
            }

            if(mLightColHandle >= 0)
            {
                Core::Colour colour = inpLightComponent->GetColour();
                glUniform4fv(mLightColHandle, 1, (const GLfloat*)&colour);
            }
            if(mLightMatrixHandle >= 0)
            {
                glUniformMatrix4fv(mLightMatrixHandle, 1, GL_FALSE, (GLfloat*)inpLightComponent->GetLightMatrix().m);
            }
        }
		//----------------------------------------------------------
		/// Apply Camera
		//----------------------------------------------------------
		void RenderSystem::ApplyCamera(const Core::Vector3& invPosition, const Core::Matrix4x4& inmatView, const Core::Matrix4x4& inmatProj, const Core::Colour& inClearCol)
		{
			//Set the new view matrix based on the camera position
			mmatView = inmatView;
			mmatProj = inmatProj;
            mvCameraPos = invPosition;
            Core::Matrix4x4::Multiply(&mmatView, &mmatProj, &mmatViewProj);
			
			//Set the clear colour
            mNewClearColour = inClearCol;
		}
		//----------------------------------------------------------
		/// Get Default Render Target
		//----------------------------------------------------------
		Rendering::RenderTarget* RenderSystem::GetDefaultRenderTarget()
		{
			return mpDefaultRenderTarget;
		}
		//----------------------------------------------------------
		/// Begin Frame
		//----------------------------------------------------------
		void RenderSystem::BeginFrame(Rendering::RenderTarget* inpActiveRenderTarget)
		{
#ifdef CS_TARGETPLATFORM_IOS
            //Sometimes iOS steals the context and doesn't return it.
            if([EAGLContext currentContext] != mContext)
            {
                [EAGLContext setCurrentContext:mContext];
            }
#endif
            
#ifdef CS_ENABLE_DEBUG
			CheckForGLErrors();
#endif
			if (inpActiveRenderTarget != nullptr)
			{
				inpActiveRenderTarget->Bind();
			}

			EnableColourWriting(true);
            EnableDepthWriting(true);
            
            if(mbInvalidateAllCaches || mClearColour != mNewClearColour)
            {
                mClearColour = mNewClearColour;
                glClearColor(mClearColour.r, mClearColour.g, mClearColour.b, mClearColour.a);
            }
            
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            memset(mpVertexAttribs, 0, sizeof(VertexAttribSet) * mdwMaxVertAttribs);
		}
		//----------------------------------------------------------
		/// Create Render Target
		//----------------------------------------------------------
		Rendering::RenderTarget* RenderSystem::CreateRenderTarget(u32 inudwWidth, u32 inudwHeight)
		{
			RenderTarget* pDefaultRenderTarget = new RenderTarget();
			pDefaultRenderTarget->Init(inudwWidth, inudwHeight);
			
			return pDefaultRenderTarget;
		}
		//----------------------------------------------------------
		/// Create Buffer
		//----------------------------------------------------------
		Rendering::MeshBuffer* RenderSystem::CreateBuffer(Rendering::BufferDescription &inDesc)
		{
			MeshBuffer* pBuffer = new MeshBuffer(inDesc);
			pBuffer->SetOwningRenderSystem(this);
            
#ifdef CS_TARGETPLATFORM_ANDROID
			mMeshBuffers.push_back(pBuffer);
#endif
			return pBuffer;
		}
        //----------------------------------------------------------
        /// Render Vertex Buffer
        //----------------------------------------------------------
        void RenderSystem::RenderVertexBuffer(Rendering::MeshBuffer* inpBuffer, u32 inudwOffset, u32 inudwNumVerts, const Core::Matrix4x4& inmatWorld)
		{
#ifdef CS_ENABLE_DEBUGSTATS
            DebugStats::AddToEvent("DrawCalls", 1u);
			DebugStats::AddToEvent("Verts", inudwNumVerts);
#endif
            
			//Set the new model view matrix based on the camera view matrix and the object matrix
            static Core::Matrix4x4 matWorldViewProj;
            Core::Matrix4x4::Multiply(&inmatWorld, &mmatViewProj, &matWorldViewProj);
			glUniformMatrix4fv(mmatWVPHandle, 1, GL_FALSE, (GLfloat*)matWorldViewProj.m);
			
            if(mmatWorldHandle != -1)
            {
                glUniformMatrix4fv(mmatWorldHandle, 1, GL_FALSE, (GLfloat*)inmatWorld.m);
            }
            
            if(mmatNormalHandle != -1)
            {
                glUniformMatrix4fv(mmatNormalHandle, 1, GL_FALSE, (GLfloat*)inmatWorld.Inverse().GetTranspose().m);
            }
            
            if(mCameraPosHandle != -1)
            {
                glUniform3f(mCameraPosHandle, mvCameraPos.x, mvCameraPos.y, mvCameraPos.z);
            }
			
			EnableVertexAttributeForSemantic(inpBuffer);
			glDrawArrays(GetPrimitiveType(inpBuffer->GetPrimitiveType()), inudwOffset, inudwNumVerts);
            
            mbInvalidateAllCaches = false;
		}
        //----------------------------------------------------------
        /// Render Buffer
        //----------------------------------------------------------
        void RenderSystem::RenderBuffer(Rendering::MeshBuffer* inpBuffer, u32 inudwOffset, u32 inudwNumIndices, const Core::Matrix4x4& inmatWorld)
		{
#ifdef CS_ENABLE_DEBUGSTATS
            DebugStats::AddToEvent("DrawCalls", 1u);
#endif

			//Set the new model view matrix based on the camera view matrix and the object matrix
            static Core::Matrix4x4 matWorldViewProj;
            Core::Matrix4x4::Multiply(&inmatWorld, &mmatViewProj, &matWorldViewProj);
			glUniformMatrix4fv(mmatWVPHandle, 1, GL_FALSE, (GLfloat*)matWorldViewProj.m);
            
            if(mmatWorldHandle != -1)
            {
                glUniformMatrix4fv(mmatWorldHandle, 1, GL_FALSE, (GLfloat*)inmatWorld.m);
            }
            
            if(mmatNormalHandle != -1)
            {
                glUniformMatrix4fv(mmatNormalHandle, 1, GL_FALSE, (GLfloat*)inmatWorld.Inverse().GetTranspose().m);
            }
            
            if(mCameraPosHandle != -1)
            {
                glUniform3f(mCameraPosHandle, mvCameraPos.x, mvCameraPos.y, mvCameraPos.z);
            }
			
			//Render the buffer contents
			EnableVertexAttributeForSemantic(inpBuffer);
			glDrawElements(GetPrimitiveType(inpBuffer->GetPrimitiveType()), inudwNumIndices, GL_UNSIGNED_SHORT, (GLvoid*)inudwOffset);
            
            mbInvalidateAllCaches = false;
		}
		//----------------------------------------------------------
		/// End Frame
		//----------------------------------------------------------
		void RenderSystem::EndFrame(Rendering::RenderTarget* inpActiveRenderTarget)
		{
#ifdef CS_TARGETPLATFORM_IOS
            if (mpDefaultRenderTarget != nullptr && mpDefaultRenderTarget == inpActiveRenderTarget)
            {
                RenderTarget::PresentDefaultRenderTarget(mContext, mpDefaultRenderTarget);
            }
#else
            glViewport(0, 0, mudwViewWidth, mudwViewHeight);
#endif
#ifdef CS_TARGETPLATFORM_WINDOWS
			Windows::GLFWManager::Get()->SwapBuffers();
#endif
		}
		//----------------------------------------------------------
		/// On Screen Orientation Changed
		//----------------------------------------------------------
        void RenderSystem::OnScreenOrientationChanged(u32 inudwWidth, u32 inudwHeight)
		{
            ResizeFrameBuffer(inudwWidth, inudwHeight);
		}
		//----------------------------------------------------------
		/// Resize Frame Buffer
		//----------------------------------------------------------
		void RenderSystem::ResizeFrameBuffer(u32 inudwWidth, u32 inudwHeight)
		{
#ifdef CS_TARGETPLATFORM_IOS
            if(mudwViewWidth != inudwWidth || mudwViewHeight != inudwHeight)
            {
                if(mpDefaultRenderTarget)
                {
                    RenderTarget::DestroyDefaultRenderTarget(mContext, mpDefaultRenderTarget);
                }
                
                //Create a default frame buffer for on-screen rendering
                mpDefaultRenderTarget = RenderTarget::CreateDefaultRenderTarget(mContext, inudwWidth, inudwHeight);
            }
#else
            if(mbInvalidateAllCaches || mudwViewWidth != inudwWidth || mudwViewHeight != inudwHeight)
            {
                glViewport(0, 0, inudwWidth, inudwHeight);
            }
#endif
            
            mudwViewHeight = inudwHeight;
            mudwViewWidth = inudwWidth;
		}
        //----------------------------------------------------------
		/// Lock Alpha Blending
		//----------------------------------------------------------
        void RenderSystem::LockAlphaBlending()
        {
            msCurrentRenderLocks.mbIsAlphaBlendingEnabled = true;
        }
        //----------------------------------------------------------
		/// Unlock Alpha Blending
		//----------------------------------------------------------
        void RenderSystem::UnlockAlphaBlending()
        {
            msCurrentRenderLocks.mbIsAlphaBlendingEnabled = false;
        }
		//----------------------------------------------------------
		/// Enable Alpha Blending
		//----------------------------------------------------------
		void RenderSystem::EnableAlphaBlending(bool inbIsEnabled)
		{
            if(mbInvalidateAllCaches != true && msCurrentRenderLocks.mbIsAlphaBlendingEnabled == true)
            {
                return;
            }
            
			//Turn it on
			if((mbInvalidateAllCaches || !msCurrentRenderFlags.mbIsAlphaBlendingEnabled) && inbIsEnabled)
			{
				msCurrentRenderFlags.mbIsAlphaBlendingEnabled = true;
				glEnable(GL_BLEND);
			}
			//Get it off
			else if((mbInvalidateAllCaches || msCurrentRenderFlags.mbIsAlphaBlendingEnabled) && !inbIsEnabled)
			{
				msCurrentRenderFlags.mbIsAlphaBlendingEnabled = false;
				glDisable(GL_BLEND);
			}
		}
		//----------------------------------------------------------
		/// Enable Depth Testing
		//----------------------------------------------------------
		void RenderSystem::EnableDepthTesting(bool inbIsEnabled)
		{
			//Turn it on
			if((mbInvalidateAllCaches || !msCurrentRenderFlags.mbIsDepthTestEnabled) && inbIsEnabled)
			{
				msCurrentRenderFlags.mbIsDepthTestEnabled = true;
				glEnable(GL_DEPTH_TEST);
			}
			//Get it off
			else if((mbInvalidateAllCaches || msCurrentRenderFlags.mbIsDepthTestEnabled) && !inbIsEnabled)
			{
				msCurrentRenderFlags.mbIsDepthTestEnabled = false;
				glDisable(GL_DEPTH_TEST);
			}
		}
        //----------------------------------------------------------
		/// Enable Colour Writing
		//----------------------------------------------------------
		void RenderSystem::EnableColourWriting(bool inbIsEnabled)
		{
			//Turn it on
			if((mbInvalidateAllCaches || !msCurrentRenderFlags.mbIsColourWriteEnabled) && inbIsEnabled)
			{
				msCurrentRenderFlags.mbIsColourWriteEnabled = true;
				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			}
			//Get it off
			else if((mbInvalidateAllCaches || msCurrentRenderFlags.mbIsColourWriteEnabled) && !inbIsEnabled)
			{
				msCurrentRenderFlags.mbIsColourWriteEnabled = false;
				glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			}
		}
        //----------------------------------------------------------
		/// Lock Depth Writing
		//----------------------------------------------------------
        void RenderSystem::LockDepthWriting()
        {
            msCurrentRenderLocks.mbIsDepthWriteEnabled = true;
        }
        //----------------------------------------------------------
		/// Unlock Depth Writing
		//----------------------------------------------------------
        void RenderSystem::UnlockDepthWriting()
        {
            msCurrentRenderLocks.mbIsDepthWriteEnabled = false;
        }
		//----------------------------------------------------------
		/// Enable Depth Writing
		//----------------------------------------------------------
		void RenderSystem::EnableDepthWriting(bool inbIsEnabled)
		{
            if(mbInvalidateAllCaches != true && msCurrentRenderLocks.mbIsDepthWriteEnabled == true)
            {
                return;
            }
            
			//Turn it on
			if((mbInvalidateAllCaches || !msCurrentRenderFlags.mbIsDepthWriteEnabled) && inbIsEnabled)
			{
				msCurrentRenderFlags.mbIsDepthWriteEnabled = true;
				glDepthMask(GL_TRUE);
			}
			//Get it off
			else if((mbInvalidateAllCaches || msCurrentRenderFlags.mbIsDepthWriteEnabled) && !inbIsEnabled)
			{
				msCurrentRenderFlags.mbIsDepthWriteEnabled = false;
				glDepthMask(GL_FALSE);
			}
		}
		//----------------------------------------------------------
		/// Enable Face Culling
		//----------------------------------------------------------
		void RenderSystem::EnableFaceCulling(bool inbIsEnabled)
		{
			//Turn it on
			if((mbInvalidateAllCaches || !msCurrentRenderFlags.mbIsCullingEnabled) && inbIsEnabled)
			{
				msCurrentRenderFlags.mbIsCullingEnabled = true;
				glEnable(GL_CULL_FACE);
			}
			//Get it off
			else if((mbInvalidateAllCaches || msCurrentRenderFlags.mbIsCullingEnabled) && !inbIsEnabled)
			{
				msCurrentRenderFlags.mbIsCullingEnabled = false;
				glDisable(GL_CULL_FACE);
			}
		}
        //----------------------------------------------------------
        /// Enable Scissor Testing
        //----------------------------------------------------------
        void RenderSystem::EnableScissorTesting(bool inbIsEnabled)
        {
            //Turn it on
			if((mbInvalidateAllCaches || !msCurrentRenderFlags.mbIsScissorTestingEnabled) && inbIsEnabled)
			{
				msCurrentRenderFlags.mbIsScissorTestingEnabled = true;
				//Get the effects on!
				glEnable(GL_SCISSOR_TEST);
			}
			//Get it off
			else if((mbInvalidateAllCaches || msCurrentRenderFlags.mbIsScissorTestingEnabled) && !inbIsEnabled)
			{
				glDisable(GL_SCISSOR_TEST);
				msCurrentRenderFlags.mbIsScissorTestingEnabled = false;
			}
        }
        //----------------------------------------------------------
        /// Set Scissor Region
        //---------------------------------------------------------
        void RenderSystem::SetScissorRegion(const Core::Vector2& invPosition, const Core::Vector2& invSize)
        {
            if(mbInvalidateAllCaches || mvCachedScissorPos != invPosition || mvCachedScissorSize != invSize)
            {
                mvCachedScissorPos = invPosition;
                mvCachedScissorSize = invSize;
				glScissor((GLsizei)invPosition.x, (GLsizei)invPosition.y, (GLsizei)invSize.x, (GLsizei)invSize.y);
            }
        }
        //----------------------------------------------------------
        /// Set Cull Face
        //----------------------------------------------------------
        void RenderSystem::SetCullFace(Rendering::CullFace ineCullface)
		{
			if(mbInvalidateAllCaches || meCurrentCullFace != ineCullface)
            {
                switch (ineCullface)
                {
                    case Rendering::CullFace::k_front:
                        glCullFace(GL_FRONT);
                        break;
                    case Rendering::CullFace::k_back:
                        glCullFace(GL_BACK);
                        break;
                }
                
                meCurrentCullFace = ineCullface;
            }
        }
        //----------------------------------------------------------
        /// Set Depth Function
        //----------------------------------------------------------
        void RenderSystem::SetDepthFunction(Rendering::DepthTestComparison ineFunc)
        {
            if(mbInvalidateAllCaches || meDepthFunc != ineFunc)
            {
                switch (ineFunc)
                {
                    case Rendering::DepthTestComparison::k_less:
                        glDepthFunc(GL_LESS);
                        break;
                    case Rendering::DepthTestComparison::k_equal:
                        glDepthFunc(GL_EQUAL);
                        break;
                    case Rendering::DepthTestComparison::k_lequal:
                        glDepthFunc(GL_LEQUAL);
                        break;
                }
                
                meDepthFunc = ineFunc;
            }
        }
        //----------------------------------------------------------
		/// Lock Blend Function
		//----------------------------------------------------------
        void RenderSystem::LockBlendFunction()
        {
            mbBlendFunctionLocked = true;
        }
        //----------------------------------------------------------
		/// Unlock Blend Function
		//----------------------------------------------------------
        void RenderSystem::UnlockBlendFunction()
        {
            mbBlendFunctionLocked = false;
        }
		//----------------------------------------------------------
		/// Set Blend Function
		//----------------------------------------------------------
		void RenderSystem::SetBlendFunction(Rendering::BlendMode ineSrcFunc, Rendering::BlendMode ineDstFunc)
		{
            if(mbInvalidateAllCaches != true && mbBlendFunctionLocked == true)
            {
                return;
            }
            
			if(mbInvalidateAllCaches || ineSrcFunc != mSrcBlendFunc || ineDstFunc != mDstBlendFunc)
			{
				mSrcBlendFunc = ineSrcFunc;
				mDstBlendFunc = ineDstFunc;
				
				GLenum SrcFunc = GL_SRC_ALPHA;
				switch(ineSrcFunc)
				{
					case Rendering::BlendMode::k_zero:
						SrcFunc = GL_ZERO;
						break;
					case Rendering::BlendMode::k_one:
						SrcFunc = GL_ONE;
						break;
					case Rendering::BlendMode::k_sourceCol:
						SrcFunc = GL_SRC_COLOR;
						break;
					case Rendering::BlendMode::k_oneMinusSourceCol:
						SrcFunc = GL_ONE_MINUS_SRC_COLOR;
						break;
					case Rendering::BlendMode::k_sourceAlpha:
						SrcFunc = GL_SRC_ALPHA;
						break;
					case Rendering::BlendMode::k_oneMinusSourceAlpha:
						SrcFunc = GL_ONE_MINUS_SRC_ALPHA;
						break;
					case Rendering::BlendMode::k_destAlpha:
						SrcFunc = GL_DST_ALPHA;
						break;
					case Rendering::BlendMode::k_oneMinusDestAlpha:
						SrcFunc = GL_ONE_MINUS_DST_ALPHA;
						break;
				};
                
				GLenum DstFunc = GL_ONE_MINUS_SRC_ALPHA;
                switch(ineDstFunc)
				{
					case Rendering::BlendMode::k_zero:
						DstFunc = GL_ZERO;
						break;
					case Rendering::BlendMode::k_one:
						DstFunc = GL_ONE;
						break;
					case Rendering::BlendMode::k_sourceCol:
						DstFunc = GL_SRC_COLOR;
						break;
					case Rendering::BlendMode::k_oneMinusSourceCol:
						DstFunc = GL_ONE_MINUS_SRC_COLOR;
						break;
					case Rendering::BlendMode::k_sourceAlpha:
						DstFunc = GL_SRC_ALPHA;
						break;
					case Rendering::BlendMode::k_oneMinusSourceAlpha:
						DstFunc = GL_ONE_MINUS_SRC_ALPHA;
						break;
					case Rendering::BlendMode::k_destAlpha:
						DstFunc = GL_DST_ALPHA;
						break;
					case Rendering::BlendMode::k_oneMinusDestAlpha:
						DstFunc = GL_ONE_MINUS_DST_ALPHA;
						break;
				};
				
				glBlendFunc(SrcFunc, DstFunc);
			}
		}
        //----------------------------------------------------------
        /// Create Attrib State Cache
        //----------------------------------------------------------
        void RenderSystem::CreateAttribStateCache()
        {
            if(mdwMaxVertAttribs == 0)
            {
                glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &mdwMaxVertAttribs);
                mpbCurrentVertexAttribState = (bool*)calloc(mdwMaxVertAttribs, sizeof(bool));
                mpbLastVertexAttribState = (bool*)calloc(mdwMaxVertAttribs, sizeof(bool));
                mpVertexAttribs = (VertexAttribSet*) calloc(mdwMaxVertAttribs, sizeof(VertexAttribSet));
            }
        }
        
        s32 RenderSystem::GetLocationForVertexSemantic(Rendering::VertexDataSemantic ineSemantic)
        {
            //Determine what client states are required and get their currently bound locations
            switch(ineSemantic)
            {
                case Rendering::VertexDataSemantic::k_position:
                    return mwPosAttributeLocation;
                case Rendering::VertexDataSemantic::k_normal:
                    return  mwNormAttributeLocation;
                case Rendering::VertexDataSemantic::k_uv:
                    return mwTexAttributeLocation;
                case Rendering::VertexDataSemantic::k_colour:
                    return mwColAttributeLocation;
                case Rendering::VertexDataSemantic::k_weight:
                    return mwWeiAttributeLocation;
                case Rendering::VertexDataSemantic::k_jointIndex:
                    return mwJIAttributeLocation;
                default:
                    return -1;
            }
        }
        bool RenderSystem::ApplyVertexAttributePointr(Rendering::MeshBuffer* inpBuffer,
                                                       GLuint inudwLocation, GLint indwSize, GLenum ineType, GLboolean inbNormalized,
                                                       GLsizei indwStride, const GLvoid* inpOffset)
        {
            if(mpVertexAttribs[inudwLocation].pBuffer == inpBuffer &&
               mpVertexAttribs[inudwLocation].size == indwSize &&
               mpVertexAttribs[inudwLocation].type == ineType &&
               mpVertexAttribs[inudwLocation].normalised == inbNormalized &&
               mpVertexAttribs[inudwLocation].stride == indwStride &&
               mpVertexAttribs[inudwLocation].offset ==(void*)indwStride)
                return false;
            
            mpVertexAttribs[inudwLocation].pBuffer = inpBuffer;
            mpVertexAttribs[inudwLocation].size = indwSize;
            mpVertexAttribs[inudwLocation].type = ineType;
            mpVertexAttribs[inudwLocation].normalised = inbNormalized;
            mpVertexAttribs[inudwLocation].stride = indwStride;
            mpVertexAttribs[inudwLocation].offset =(void*)indwStride;
            
            glVertexAttribPointer(inudwLocation, indwSize, ineType, inbNormalized, indwStride, inpOffset);
            return true;
        }
		//------------------------------------------------------------
		/// Enable Vertex Attribute For Semantic
		//------------------------------------------------------------
		void RenderSystem::EnableVertexAttributeForSemantic(Rendering::MeshBuffer* inpBuffer)
		{
            if(mbInvalidateAllCaches || mdwMaxVertAttribs == 0)
            {
                CreateAttribStateCache();
            }
            
            
            // If mesh buffer has changed we need to reset all its vertex attributes
            if(((ChilliSource::OpenGL::MeshBuffer*)inpBuffer)->IsCacheValid() == false)
            {
                for(s32 i =0; i < mdwMaxVertAttribs; i++)
                {
                    if(mpVertexAttribs[i].pBuffer == inpBuffer)
                        mpVertexAttribs[i].pBuffer = nullptr;
                }
                ((ChilliSource::OpenGL::MeshBuffer*)inpBuffer)->SetCacheValid();
            }
            
            //Check if the total size of the vertex declaration has changed
            //if so then invalidate the cache
            const u32 nElements = inpBuffer->GetVertexDeclaration().GetNumElements();
            
            // Calculate active attribute count, used to prevent them exceeding the max.
            u32 udwAttributeCount = 0;
            
			//Enable all the client states required to match the vertex layout
			for(u32 i=0; i<nElements; ++i)
			{
				const Rendering::VertexElement &Element = inpBuffer->GetVertexDeclaration().GetElementAtIndex(i);
                GLint dwLocation = GetLocationForVertexSemantic(Element.eSemantic);
                
                // Check next vertex attribute if no location available
                if(dwLocation < 0)
                    continue;
                
                //Track the active attribute channels
                mpbCurrentVertexAttribState[dwLocation] = true;
                
                //Track to ensure we don't exceed maximum.
                udwAttributeCount++;
			}
            
			//Check we don't exceed the GL limits of this device
			if(udwAttributeCount > (u32)mdwMaxVertAttribs)
			{
				CS_LOG_FATAL("OpenGL ES 2.0: Shader exceeds maximum vertex attributes " + Core::ToString(mdwMaxVertAttribs));
			}
			
            // Enable and disable the vertex attribs that have changed
			for(s32 i=0; i<mdwMaxVertAttribs; ++i)
			{
                if(mbInvalidateAllCaches || mpbCurrentVertexAttribState[i] != mpbLastVertexAttribState[i])
                {
                    mpbCurrentVertexAttribState[i] ? glEnableVertexAttribArray(i) : glDisableVertexAttribArray(i);
                    
                    mpbLastVertexAttribState[i] = mpbCurrentVertexAttribState[i];
                }
			}
            
            // Apply vertex attribute pointers if needed
            for(u32 i=0; i<nElements; ++i)
			{
                const Rendering::VertexElement &Element = inpBuffer->GetVertexDeclaration().GetElementAtIndex(i);
                
                // Get parameters for vertex attribute pointer call
                const GLint         dwLocation = GetLocationForVertexSemantic(Element.eSemantic);
                
                // Check next vertex attribute if no location available
                if(dwLocation < 0)
                    continue;
                
				const GLint         uwNumComponents = Element.NumDataTypesPerType();
                GLenum              eType = GL_FLOAT;
                GLboolean           bNormalise = GL_FALSE;
                const GLsizei       dwVertSize = inpBuffer->GetVertexDeclaration().GetTotalSize();
                const u16           uwElementOffset = inpBuffer->GetVertexDeclaration().GetElementOffset(Element);
                
                
                // Specific settings for those that differ from default
                if(Element.eSemantic == Rendering::VertexDataSemantic::k_colour)
                {
                    eType = GL_UNSIGNED_BYTE;
                    bNormalise = GL_TRUE;
                }
                else if(Element.eSemantic == Rendering::VertexDataSemantic::k_jointIndex)
                {
                    eType = GL_UNSIGNED_BYTE;
                }
                
                ApplyVertexAttributePointr(inpBuffer, dwLocation, uwNumComponents, eType, bNormalise, dwVertSize, (const GLvoid*)(s32)uwElementOffset);
            }
		}
		//------------------------------------------------------------
		/// Get Primitive Type
		//------------------------------------------------------------
		s32 RenderSystem::GetPrimitiveType(Rendering::PrimitiveType inType)
		{
			switch(inType)
			{
				case Rendering::PrimitiveType::k_tri:
					return GL_TRIANGLES;
				case Rendering::PrimitiveType::k_triStrip:
					return GL_TRIANGLE_STRIP;
				case Rendering::PrimitiveType::k_line:
					return GL_LINES;
				default:
					CS_LOG_ERROR("Invalid primitive type OpenGLES");
					return -1;
			}
		}
        //----------------------------------------------------------
        /// Get Path To Shaders
        //----------------------------------------------------------
        std::string RenderSystem::GetPathToShaders() const
        {
            return "Shaders/OpenGL/";
        }
		//----------------------------------------------------------
		/// Force Refresh Render States
		//----------------------------------------------------------
		void RenderSystem::ForceRefreshRenderStates()
		{
			//clear the cache.
			Texture::ClearCache();
            RenderTarget::ClearCache();
            
            //Set the default blend function and alpha function
            SetDepthFunction(Rendering::DepthTestComparison::k_lequal);
            
			//we're using pre-multiplied alpha and multipass rendering and therefore require the add blend equation
            glBlendEquation(GL_FUNC_ADD);
            
            mbInvalidateAllCaches = true;
		}
		//----------------------------------------------------------
		/// Backup Mesh Buffers
		//----------------------------------------------------------
		void RenderSystem::BackupMeshBuffers()
		{
#ifdef CS_TARGETPLATFORM_ANDROID
			for(std::vector<MeshBuffer*>::iterator it = mMeshBuffers.begin(); it != mMeshBuffers.end(); ++it)
			{
				(*it)->Backup();
			}
#endif
		}
		//----------------------------------------------------------
		/// Restore Mesh Buffers
		//----------------------------------------------------------
		void RenderSystem::RestoreMeshBuffers()
		{
#ifdef CS_TARGETPLATFORM_ANDROID
			for(std::vector<MeshBuffer*>::iterator it = mMeshBuffers.begin(); it != mMeshBuffers.end(); ++it)
			{
				(*it)->Restore();
			}
#endif
		}
        //----------------------------------------------------------
		/// Remove Buffer
		//----------------------------------------------------------
		void RenderSystem::RemoveBuffer(Rendering::MeshBuffer* inpBuffer)
		{
#ifdef CS_TARGETPLATFORM_ANDROID
			for(std::vector<MeshBuffer*>::iterator it = mMeshBuffers.begin(); it != mMeshBuffers.end(); ++it)
			{
				if ((*it) == inpBuffer)
				{
					mMeshBuffers.erase(it);
					break;
				}
			}
#endif
		}
		//----------------------------------------------------------
		/// CheckForGLErrors
		//----------------------------------------------------------
		void RenderSystem::CheckForGLErrors()
		{
			//get an array of all the errors that have occurred
			std::vector<GLenum> errorArray;
			GLenum currentError = glGetError();
			while (currentError != GL_NO_ERROR)
			{
				errorArray.push_back(currentError);
				currentError = glGetError();
			}
            
			//print out the meaning of each error found
			for (std::vector<GLenum>::iterator it = errorArray.begin(); it != errorArray.end(); ++it)
			{
				switch (*it)
				{
                    case GL_NO_ERROR:
                        CS_LOG_ERROR("GL_NO_ERROR -> Somethings gone wrong, this should not be getting reported as an error.");
                        break;
                    case GL_INVALID_ENUM:
                        CS_LOG_ERROR("GL_INVALID_ENUM -> An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.");
                        break;
                    case GL_INVALID_VALUE:
                        CS_LOG_ERROR("GL_INVALID_VALUE -> A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.");
                        break;
                    case GL_INVALID_OPERATION:
                        CS_LOG_ERROR("GL_INVALID_OPERATION -> The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.");
                        break;
                    case GL_INVALID_FRAMEBUFFER_OPERATION:
                        CS_LOG_ERROR("GL_INVALID_FRAMEBUFFER_OPERATION -> The command is trying to render to or read from the framebuffer while the currently bound framebuffer is not framebuffer complete (i.e. the return value from glCheckFramebufferStatus is not GL_FRAMEBUFFER_COMPLETE). The offending command is ignored and has no other side effect than to set the error flag.");
                        break;
                    case GL_OUT_OF_MEMORY:
                        CS_LOG_ERROR("GL_OUT_OF_MEMORY -> There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.");
                        break;
                    default:
                        CS_LOG_ERROR("Something's gone wrong, unknown GL error code.");
                        break;
				}
			}
		}
        //----------------------------------------------------------
		//----------------------------------------------------------
		void RenderSystem::Destroy()
		{
#ifdef CS_TARGETPLATFORM_IOS
            if(mpDefaultRenderTarget)
            {
                RenderTarget::DestroyDefaultRenderTarget(mContext, mpDefaultRenderTarget);
            }
            
			//Release the context
			if(mContext)
			{
				if([EAGLContext currentContext] == mContext)
				{
					[EAGLContext setCurrentContext:nil];
				}
				
				[mContext release];
				mContext = nullptr;
			}
#endif
            
            free(mpbCurrentVertexAttribState);
            free(mpbLastVertexAttribState);
            free(mpVertexAttribs);
            free(mpaTextureHandles);
		}
	}
}
