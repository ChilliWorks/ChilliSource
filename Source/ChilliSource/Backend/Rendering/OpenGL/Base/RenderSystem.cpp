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
#include <ChilliSource/Backend/Rendering/OpenGL/Shader/Shader.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Texture/Cubemap.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Texture/Texture.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Texture/TextureUnitSystem.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>
#include <ChilliSource/Core/Event/Event.h>
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
        
        namespace
        {
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Vertex attribute semantic ID
            ///
            /// @return Vertex attribute variable name
            //----------------------------------------------------------
            const char* GetAttribNameForVertexSemantic(Rendering::VertexDataSemantic in_semantic)
            {
                switch(in_semantic)
                {
                    case Rendering::VertexDataSemantic::k_position:
                        return "a_position";
                    case Rendering::VertexDataSemantic::k_normal:
                        return  "a_normal";
                    case Rendering::VertexDataSemantic::k_uv:
                        return "a_texCoord";
                    case Rendering::VertexDataSemantic::k_colour:
                        return "a_colour";
                    case Rendering::VertexDataSemantic::k_weight:
                        return "a_weights";
                    case Rendering::VertexDataSemantic::k_jointIndex:
                        return "a_jointIndices";
                }

                CS_LOG_FATAL("No such vertex semantic type");
                return "";
            }
        }
		//----------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------
		RenderSystem::RenderSystem(Rendering::RenderCapabilities* in_renderCapabilities)
		: mpDefaultRenderTarget(nullptr), mpCurrentMaterial(nullptr), m_currentShader(nullptr), mbInvalidateAllCaches(true), mdwMaxVertAttribs(0),
        mbEmissiveSet(false), mbAmbientSet(false), mbDiffuseSet(false), mbSpecularSet(false), mudwNumBoundTextures(0), mpLightComponent(nullptr),
        mbBlendFunctionLocked(false), mbInvalidateLigthingCache(true),
        mpRenderCapabilities(static_cast<RenderCapabilities*>(in_renderCapabilities)), m_hasContextBeenBackedUp(false)
		{

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
            
            m_hasContext = true;
            
            m_screen = Core::Application::Get()->GetSystem<Core::Screen>();
            CS_ASSERT((m_screen->GetResolution().x > 0.0f && m_screen->GetResolution().y > 0.0f), "Cannot create and OpenGL ES view with size ZERO");
            
            m_textureUnitSystem = Core::Application::Get()->GetSystem<TextureUnitSystem>();
            CS_ASSERT(m_textureUnitSystem, "Cannot find required system: Texture Unit System.");

            CS_ASSERT(mpRenderCapabilities, "Cannot find required system: Render Capabilities.");
            mpRenderCapabilities->DetermineCapabilities();
            
            m_textureUniformNames.clear();
            for(u32 i=0; i<mpRenderCapabilities->GetNumTextureUnits(); ++i)
            {
                m_textureUniformNames.push_back("u_texture" + Core::ToString(i));
            }
            
            ForceRefreshRenderStates();
			
            OnScreenResolutionChanged(m_screen->GetResolution());
            m_resolutionChangeConnection = m_screen->GetResolutionChangedEvent().OpenConnection(Core::MakeDelegate(this, &RenderSystem::OnScreenResolutionChanged));
            
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
                m_contextRestorer.Backup();
                
                m_hasContextBeenBackedUp = true;
            }
            m_hasContext = false;
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
                m_contextRestorer.Restore();
                m_currentShader = nullptr;
                mpCurrentMaterial = nullptr;
                m_hasContextBeenBackedUp = false;
            }
            
            m_hasContext = true;
#endif
		}
        //----------------------------------------------------------
        //----------------------------------------------------------
        bool RenderSystem::HasContext() const
        {
            return m_hasContext;
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
            
            //Casting away constness for the time being as we need to set the shader variables. This will
            //all change when the new renderer comes in.
            Shader* shader = (Shader*)(in_material->GetShader(in_shaderPass).get());
            CS_ASSERT(shader != nullptr, "Cannot render with null shader");
            
            shader->SetUniform("u_cameraPos", mvCameraPos, Shader::UniformNotFoundPolicy::k_failSilent);
            
            bool hasMaterialChanged = mbInvalidateAllCaches == true || mpCurrentMaterial == nullptr || mpCurrentMaterial != in_material.get() || mpCurrentMaterial->IsCacheValid() == false;
            if(hasMaterialChanged == true)
            {
                mudwNumBoundTextures = 0;
                mpCurrentMaterial = in_material.get();
                
                //Bind this materials shader
                ApplyRenderStates(mpCurrentMaterial);
                
                //Don't bind the shader if we don't need to!
                bool hasShaderChanged = mbInvalidateAllCaches || shader != m_currentShader;
                if(hasShaderChanged == true)
                {
                    shader->Bind();
                    m_currentShader = shader;
                    
                    mbEmissiveSet = false;
                    mbAmbientSet = false;
                    mbDiffuseSet = false;
                    mbSpecularSet = false;
                    mbInvalidateLigthingCache = true;
                }
                
                //Set all the custom shader variables
                if(mpCurrentMaterial->IsVariableCacheValid() == false || hasShaderChanged == true)
                {
                    ApplyShaderVariables(mpCurrentMaterial, m_currentShader);
                }
                
                ApplyTextures(mpCurrentMaterial, m_currentShader);
                ApplyLightingValues(mpCurrentMaterial, m_currentShader);
                ApplyLighting(mpLightComponent, m_currentShader);
                
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
            CS_ASSERT(m_currentShader != nullptr,  "Cannot set joints without binding shader");
            
            //Remove the final column from the joint matrix data as it is always going to be [0 0 0 1].
            std::vector<Core::Vector4> jointVectors;
            for (const auto& joint : inaJoints)
            {
                jointVectors.push_back(Core::Vector4(joint.m[0], joint.m[4], joint.m[8], joint.m[12]));
                jointVectors.push_back(Core::Vector4(joint.m[1], joint.m[5], joint.m[9], joint.m[13]));
                jointVectors.push_back(Core::Vector4(joint.m[2], joint.m[6], joint.m[10], joint.m[14]));
            }
            
            m_currentShader->SetUniform("u_joints", jointVectors);
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
		/// Apply Shader Variables
		//----------------------------------------------------------
		void RenderSystem::ApplyShaderVariables(const Rendering::Material* inMaterial, Shader* out_shader)
		{
			//Get and set all the custom shader variables
			for(auto it = inMaterial->m_floatVars.begin(); it!= inMaterial->m_floatVars.end(); ++it)
			{
                out_shader->SetUniform(it->first, it->second);
			}
			for(auto it = inMaterial->m_vec2Vars.begin(); it!= inMaterial->m_vec2Vars.end(); ++it)
			{
                out_shader->SetUniform(it->first, it->second);
			}
			for(auto it = inMaterial->m_vec3Vars.begin(); it!= inMaterial->m_vec3Vars.end(); ++it)
			{
				out_shader->SetUniform(it->first, it->second);
			}
			for(auto it = inMaterial->m_vec4Vars.begin(); it!= inMaterial->m_vec4Vars.end(); ++it)
			{
				out_shader->SetUniform(it->first, it->second);
			}
			for(auto it = inMaterial->m_mat4Vars.begin(); it!= inMaterial->m_mat4Vars.end(); ++it)
			{
				out_shader->SetUniform(it->first, it->second);
			}
			for(auto it = inMaterial->m_colourVars.begin(); it!= inMaterial->m_colourVars.end(); ++it)
			{
				out_shader->SetUniform(it->first, it->second);
			}
		}
        //----------------------------------------------------------
        /// Apply Textures
        //----------------------------------------------------------
        void RenderSystem::ApplyTextures(const Rendering::Material* inMaterial, Shader* out_shader)
        {
            //Cubemap takes precedence over texture
            if(inMaterial->GetCubemap() != nullptr)
            {
                //TODO: Once we change the render system this hack will not be required as we will be dealing
                //with a list of commands and texture handles
                Cubemap* cubemap = (Cubemap*)inMaterial->GetCubemap().get();
                cubemap->Bind(mudwNumBoundTextures);
                out_shader->SetUniform("u_cubemap", (s32)mudwNumBoundTextures);
                ++mudwNumBoundTextures;
            }
            
            //Cannot bind more textures than the number of texture units
            u32 numTextureUnitsAvailable = mpRenderCapabilities->GetNumTextureUnits() - mudwNumBoundTextures;
            u32 numTexturesToBind = std::min(inMaterial->GetNumTextures(), numTextureUnitsAvailable);
            
            for(u32 i=0; i<numTexturesToBind; ++i)
            {
                //TODO: Once we change the render system this hack will not be required as we will be dealing
                //with a list of commands and texture handles
                Texture* texture = (Texture*)inMaterial->GetTexture(i).get();
                texture->Bind(mudwNumBoundTextures);
                out_shader->SetUniform(m_textureUniformNames[i], (s32)mudwNumBoundTextures);
                ++mudwNumBoundTextures;
            }
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void RenderSystem::ApplyLightingValues(const Rendering::Material* inMaterial, Shader* out_shader)
        {
            if(mbInvalidateAllCaches || mbEmissiveSet == false || mCurrentEmissive != inMaterial->GetEmissive())
            {
                mbEmissiveSet = true;
                mCurrentEmissive = inMaterial->GetEmissive();
                out_shader->SetUniform("u_emissive", mCurrentEmissive, Shader::UniformNotFoundPolicy::k_failSilent);
            }
            if(mbInvalidateAllCaches || mbAmbientSet == false || mCurrentAmbient != inMaterial->GetAmbient())
            {
                mbAmbientSet = true;
                mCurrentAmbient = inMaterial->GetAmbient();
                out_shader->SetUniform("u_ambient", mCurrentAmbient, Shader::UniformNotFoundPolicy::k_failSilent);
            }
            if(mbInvalidateAllCaches || mbDiffuseSet == false || mCurrentDiffuse != inMaterial->GetDiffuse())
            {
                mbDiffuseSet = true;
                mCurrentDiffuse = inMaterial->GetDiffuse();
                out_shader->SetUniform("u_diffuse", mCurrentDiffuse, Shader::UniformNotFoundPolicy::k_failSilent);
            }
            if(mbInvalidateAllCaches || mbSpecularSet == false || mCurrentSpecular != inMaterial->GetSpecular())
            {
                mbSpecularSet = true;
                mCurrentSpecular = inMaterial->GetSpecular();
                out_shader->SetUniform("u_specular", mCurrentSpecular, Shader::UniformNotFoundPolicy::k_failSilent);
            }
        }
        //----------------------------------------------------------
        /// Apply Lighting
        //----------------------------------------------------------
        void RenderSystem::ApplyLighting(Rendering::LightComponent* inpLightComponent, Shader* out_shader)
        {
            if(mbInvalidateLigthingCache == false || inpLightComponent == nullptr)
                return;
            
            mbInvalidateLigthingCache = false;
            inpLightComponent->CalculateLightingValues();
            inpLightComponent->SetCacheValid();
            
            if(inpLightComponent->IsA(Rendering::DirectionalLightComponent::InterfaceID))
            {
                Rendering::DirectionalLightComponent* pLightComponent = (Rendering::DirectionalLightComponent*)inpLightComponent;
                out_shader->SetUniform("u_lightDir", pLightComponent->GetDirection());
  
                if(pLightComponent->GetShadowMapPtr() != nullptr)
                {
                    out_shader->SetUniform("u_shadowTolerance", pLightComponent->GetShadowTolerance(), Shader::UniformNotFoundPolicy::k_failSilent);
                    
                    //If we have used all the texture units then we cannot bind the shadow map
                    if(mudwNumBoundTextures <= mpRenderCapabilities->GetNumTextureUnits())
                    {
                        pLightComponent->GetShadowMapPtr()->Bind(mudwNumBoundTextures);
                        out_shader->SetUniform("u_shadowMap", (s32)mudwNumBoundTextures);
                        ++mudwNumBoundTextures;
                    }
                    else
                    {
                        CS_LOG_WARNING("Failed to bind shadow map. Not enough texture units");
                    }
                }
            }
            else if(inpLightComponent->IsA(Rendering::PointLightComponent::InterfaceID))
            {
                Rendering::PointLightComponent* pLightComponent = (Rendering::PointLightComponent*)inpLightComponent;
                out_shader->SetUniform("u_attenuationConstant", pLightComponent->GetConstantAttenuation(), Shader::UniformNotFoundPolicy::k_failSilent);
                out_shader->SetUniform("u_attenuationLinear", pLightComponent->GetLinearAttenuation(), Shader::UniformNotFoundPolicy::k_failSilent);
                out_shader->SetUniform("u_attenuationQuadratic", pLightComponent->GetQuadraticAttenuation(), Shader::UniformNotFoundPolicy::k_failSilent);
            }
            
            out_shader->SetUniform("u_lightPos", inpLightComponent->GetWorldPosition(), Shader::UniformNotFoundPolicy::k_failSilent);
            out_shader->SetUniform("u_lightCol", inpLightComponent->GetColour(), Shader::UniformNotFoundPolicy::k_failSilent);
            out_shader->SetUniform("u_lightMat", inpLightComponent->GetLightMatrix(), Shader::UniformNotFoundPolicy::k_failSilent);
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
            
            m_attributeCache.clear();
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
            m_contextRestorer.AddMeshBuffer(pBuffer);
#endif
			return pBuffer;
		}
        //----------------------------------------------------------
        /// Render Vertex Buffer
        //----------------------------------------------------------
        void RenderSystem::RenderVertexBuffer(Rendering::MeshBuffer* inpBuffer, u32 inudwOffset, u32 inudwNumVerts, const Core::Matrix4x4& inmatWorld)
		{
#ifdef CS_ENABLE_DEBUGSTATS
            Core::Application::Get()->GetDebugStats()->AddToEvent("DrawCalls", 1u);
			Core::Application::Get()->GetDebugStats()->AddToEvent("Verts", inudwNumVerts);
#endif
            
			//Set the new model view matrix based on the camera view matrix and the object matrix
            static Core::Matrix4x4 matWorldViewProj;
            Core::Matrix4x4::Multiply(&inmatWorld, &mmatViewProj, &matWorldViewProj);
            m_currentShader->SetUniform("u_wvpMat", matWorldViewProj, Shader::UniformNotFoundPolicy::k_failSilent);
            m_currentShader->SetUniform("u_worldMat", inmatWorld, Shader::UniformNotFoundPolicy::k_failSilent);
            if(m_currentShader->HasUniform("u_normalMat"))
            {
                m_currentShader->SetUniform("u_normalMat", inmatWorld.Inverse().GetTranspose());
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
            Core::Application::Get()->GetDebugStats()->AddToEvent("DrawCalls", 1u);
            Core::Application::Get()->GetDebugStats()->AddToEvent("Verts", inpBuffer->GetVertexCount());
#endif

			//Set the new model view matrix based on the camera view matrix and the object matrix
            static Core::Matrix4x4 matWorldViewProj;
            Core::Matrix4x4::Multiply(&inmatWorld, &mmatViewProj, &matWorldViewProj);
            m_currentShader->SetUniform("u_wvpMat", matWorldViewProj, Shader::UniformNotFoundPolicy::k_failSilent);
            m_currentShader->SetUniform("u_worldMat", inmatWorld, Shader::UniformNotFoundPolicy::k_failSilent);
            if(m_currentShader->HasUniform("u_normalMat"))
            {
                m_currentShader->SetUniform("u_normalMat", inmatWorld.Inverse().GetTranspose());
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
            }
        }
        void RenderSystem::ApplyVertexAttributePointr(Rendering::MeshBuffer* inpBuffer,
                                                       const char* in_attribName, GLint indwSize, GLenum ineType, GLboolean inbNormalized,
                                                       GLsizei indwStride, const GLvoid* inpOffset)
        {
            auto it = m_attributeCache.find(in_attribName);
            
            if(it != m_attributeCache.end())
            {
                if(it->second.pBuffer == inpBuffer &&
                   it->second.size == indwSize &&
                   it->second.type == ineType &&
                   it->second.normalised == inbNormalized &&
                   it->second.stride == indwStride &&
                   it->second.offset == inpOffset)
                    return;
                
                it->second.pBuffer = inpBuffer;
                it->second.size = indwSize;
                it->second.type = ineType;
                it->second.normalised = inbNormalized;
                it->second.stride = indwStride;
                it->second.offset = inpOffset;
            }
            else
            {
                VertexAttribSet set;
                set.pBuffer = inpBuffer;
                set.size = indwSize;
                set.type = ineType;
                set.normalised = inbNormalized;
                set.stride = indwStride;
                set.offset = inpOffset;
                m_attributeCache.insert(std::make_pair(in_attribName, set));
            }
  
            m_currentShader->SetAttribute(in_attribName, indwSize, ineType, inbNormalized, indwStride, inpOffset);
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
                for(auto& entry : m_attributeCache)
                {
                    if(entry.second.pBuffer == inpBuffer)
                        entry.second.pBuffer = nullptr;
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
                //Track the active attribute channels
                mpbCurrentVertexAttribState[i] = true;
                
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
                const char* attribName = GetAttribNameForVertexSemantic(Element.eSemantic);
                
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
                
                ApplyVertexAttributePointr(inpBuffer, attribName, uwNumComponents, eType, bNormalise, dwVertSize, (const GLvoid*)(s32)uwElementOffset);
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
		/// Force Refresh Render States
		//----------------------------------------------------------
		void RenderSystem::ForceRefreshRenderStates()
		{
			//clear the cache.
            m_textureUnitSystem->Clear();
            RenderTarget::ClearCache();
            
            //Set the default blend function and alpha function
            SetDepthFunction(Rendering::DepthTestComparison::k_lequal);
            
			//we're using pre-multiplied alpha and multipass rendering and therefore require the add blend equation
            glBlendEquation(GL_FUNC_ADD);
            
            mbInvalidateAllCaches = true;
		}
        //----------------------------------------------------------
		/// Remove Buffer
		//----------------------------------------------------------
		void RenderSystem::RemoveBuffer(Rendering::MeshBuffer* inpBuffer)
		{
#ifdef CS_TARGETPLATFORM_ANDROID
            m_contextRestorer.RemoveMeshBuffer((MeshBuffer*)inpBuffer);
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
        void RenderSystem::OnScreenResolutionChanged(const Core::Vector2& in_resolution)
        {
            ResizeFrameBuffer((u32)in_resolution.x, (u32)in_resolution.y);
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
            
            m_resolutionChangeConnection = nullptr;
		}
	}
}
