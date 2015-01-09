//
//  RenderSystem.cpp
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

#include <CSBackend/Rendering/OpenGL/Base/RenderSystem.h>

#include <CSBackend/Rendering/OpenGL/Base/GLError.h>
#include <CSBackend/Rendering/OpenGL/Base/MeshBuffer.h>
#include <CSBackend/Rendering/OpenGL/Base/RenderCapabilities.h>
#include <CSBackend/Rendering/OpenGL/Base/RenderTarget.h>
#include <CSBackend/Rendering/OpenGL/Shader/Shader.h>
#include <CSBackend/Rendering/OpenGL/Texture/Cubemap.h>
#include <CSBackend/Rendering/OpenGL/Texture/Texture.h>
#include <CSBackend/Rendering/OpenGL/Texture/TextureUnitSystem.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
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

namespace CSBackend
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
            const char* GetAttribNameForVertexSemantic(CSRendering::VertexDataSemantic in_semantic)
            {
                switch(in_semantic)
                {
                    case CSRendering::VertexDataSemantic::k_position:
                        return "a_position";
                    case CSRendering::VertexDataSemantic::k_normal:
                        return  "a_normal";
                    case CSRendering::VertexDataSemantic::k_uv:
                        return "a_texCoord";
                    case CSRendering::VertexDataSemantic::k_colour:
                        return "a_colour";
                    case CSRendering::VertexDataSemantic::k_weight:
                        return "a_weights";
                    case CSRendering::VertexDataSemantic::k_jointIndex:
                        return "a_jointIndices";
                }
                
                CS_LOG_FATAL("No such vertex semantic type");
                return "";
            }
            //----------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The blend mode enum in the engine.
            ///
            /// @return The equivalent opengl blend mode.
            //----------------------------------------------------------
            GLenum BlendModeToGL(CSRendering::BlendMode in_blendMode)
            {
				switch(in_blendMode)
				{
					case CSRendering::BlendMode::k_zero:
						return GL_ZERO;
					case CSRendering::BlendMode::k_one:
						return GL_ONE;
					case CSRendering::BlendMode::k_sourceCol:
						return GL_SRC_COLOR;
					case CSRendering::BlendMode::k_oneMinusSourceCol:
						return GL_ONE_MINUS_SRC_COLOR;
					case CSRendering::BlendMode::k_sourceAlpha:
						return GL_SRC_ALPHA;
					case CSRendering::BlendMode::k_oneMinusSourceAlpha:
						return GL_ONE_MINUS_SRC_ALPHA;
					case CSRendering::BlendMode::k_destAlpha:
						return GL_DST_ALPHA;
					case CSRendering::BlendMode::k_oneMinusDestAlpha:
						return GL_ONE_MINUS_DST_ALPHA;
                    default:
                        return GL_SRC_ALPHA;
				};
            }
        }
		//----------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------
		RenderSystem::RenderSystem(CSRendering::RenderCapabilities* in_renderCapabilities)
		: mpCurrentMaterial(nullptr), m_currentShader(nullptr), mbInvalidateAllCaches(true), mdwMaxVertAttribs(0),
        mbEmissiveSet(false), mbAmbientSet(false), mbDiffuseSet(false), mbSpecularSet(false), mudwNumBoundTextures(0), mpLightComponent(nullptr),
        mbBlendFunctionLocked(false), mbInvalidateLightingCache(true),
		mpRenderCapabilities(static_cast<RenderCapabilities*>(in_renderCapabilities)), m_hasContextBeenBackedUp(false), mpbLastVertexAttribState(nullptr), mpbCurrentVertexAttribState(nullptr)
		{
            
		}
        //----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool RenderSystem::IsA(CSCore::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == RenderSystem::InterfaceID || inInterfaceID == CSRendering::RenderSystem::InterfaceID;
		}
        //----------------------------------------------------------
        //----------------------------------------------------------
        void RenderSystem::Init()
		{
            m_hasContext = true;
            
            m_screen = CSCore::Application::Get()->GetSystem<CSCore::Screen>();
            CS_ASSERT((m_screen->GetResolution().x > 0.0f && m_screen->GetResolution().y > 0.0f), "Cannot create and OpenGL ES view with size ZERO");
            
            m_textureUnitSystem = CSCore::Application::Get()->GetSystem<TextureUnitSystem>();
            CS_ASSERT(m_textureUnitSystem, "Cannot find required system: Texture Unit System.");
            
            CS_ASSERT(mpRenderCapabilities, "Cannot find required system: Render Capabilities.");
            mpRenderCapabilities->DetermineCapabilities();
            
            m_textureUniformNames.clear();
            for(u32 i=0; i<mpRenderCapabilities->GetNumTextureUnits(); ++i)
            {
                m_textureUniformNames.push_back("u_texture" + CSCore::ToString(i));
            }
            
            ForceRefreshRenderStates();
			
            OnScreenResolutionChanged(m_screen->GetResolution());
            m_resolutionChangeConnection = m_screen->GetResolutionChangedEvent().OpenConnection(CSCore::MakeDelegate(this, &RenderSystem::OnScreenResolutionChanged));
			m_displayModeChangeConnection = m_screen->GetDisplayModeChangedEvent().OpenConnection(CSCore::MakeDelegate(this, &RenderSystem::OnDisplayModeChanged));
            
            m_hasContextBeenBackedUp = false;
		}
        //----------------------------------------------------------
        //----------------------------------------------------------
        void RenderSystem::Resume()
        {
			mbInvalidateAllCaches = true;
            
            RestoreContext();
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
		void RenderSystem::OnDisplayModeChanged(CSCore::Screen::DisplayMode in_mode)
        {
			ForceRefreshRenderStates();
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
        void RenderSystem::SetLight(CSRendering::LightComponent* inpLightComponent)
        {
            if(inpLightComponent == mpLightComponent && mbInvalidateAllCaches == false)
            {
                if(mpLightComponent != nullptr)
                {
                    mbInvalidateLightingCache = mpLightComponent->IsCacheValid() == false;
                }
                
                return;
            }
            
            mpLightComponent = inpLightComponent;
            mbInvalidateLightingCache = true;
        }
        //----------------------------------------------------------
		//----------------------------------------------------------
		void RenderSystem::ApplyMaterial(const CSRendering::MaterialCSPtr& in_material, CSRendering::ShaderPass in_shaderPass)
		{
            //TODO: We can remove alot of these cache checks one we move to our render command buffer
            //as the apply material function will only be called once per "batch" rather than for each
            //element in the batch as it is now.
            
            CS_ASSERT(in_material != nullptr, "Cannot apply null material");
            
            //Casting away constness for the time being as we need to set the shader variables. This will
            //all change when the new renderer comes in.
            Shader* shader = (Shader*)(in_material->GetShader(in_shaderPass).get());
            CS_ASSERT(shader != nullptr, "Cannot render with null shader");
            
            bool hasMaterialChanged = mbInvalidateAllCaches == true || mpCurrentMaterial == nullptr || mpCurrentMaterial != in_material.get() || mpCurrentMaterial->IsCacheValid() == false || shader != m_currentShader;
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
                    mbInvalidateLightingCache = true;
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
                const_cast<CSRendering::Material*>(mpCurrentMaterial)->SetCacheValid();
            }
            
            shader->SetUniform("u_cameraPos", mvCameraPos, Shader::UniformNotFoundPolicy::k_failSilent);
		}
        //----------------------------------------------------------
        /// Apply Joints
        //----------------------------------------------------------
        void RenderSystem::ApplyJoints(const std::vector<CSCore::Matrix4>& inaJoints)
        {
            CS_ASSERT(m_currentShader != nullptr,  "Cannot set joints without binding shader");
            
            //Remove the final column from the joint matrix data as it is always going to be [0 0 0 1].
            std::vector<CSCore::Vector4> jointVectors;
            for (const auto& joint : inaJoints)
            {
                jointVectors.push_back(CSCore::Vector4(joint.m[0], joint.m[4], joint.m[8], joint.m[12]));
                jointVectors.push_back(CSCore::Vector4(joint.m[1], joint.m[5], joint.m[9], joint.m[13]));
                jointVectors.push_back(CSCore::Vector4(joint.m[2], joint.m[6], joint.m[10], joint.m[14]));
            }
            
            m_currentShader->SetUniform("u_joints", jointVectors);
        }
        //----------------------------------------------------------
		/// Apply Render States
		//----------------------------------------------------------
		void RenderSystem::ApplyRenderStates(const CSRendering::Material* inMaterial)
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
		void RenderSystem::ApplyShaderVariables(const CSRendering::Material* inMaterial, Shader* out_shader)
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
        void RenderSystem::ApplyTextures(const CSRendering::Material* inMaterial, Shader* out_shader)
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
        void RenderSystem::ApplyLightingValues(const CSRendering::Material* inMaterial, Shader* out_shader)
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
        void RenderSystem::ApplyLighting(CSRendering::LightComponent* inpLightComponent, Shader* out_shader)
        {
            if(mbInvalidateLightingCache == false || inpLightComponent == nullptr)
                return;
            
            mbInvalidateLightingCache = false;
            inpLightComponent->CalculateLightingValues();
            inpLightComponent->SetCacheValid();
            
            if(inpLightComponent->IsA(CSRendering::DirectionalLightComponent::InterfaceID))
            {
                CSRendering::DirectionalLightComponent* pLightComponent = (CSRendering::DirectionalLightComponent*)inpLightComponent;
                out_shader->SetUniform("u_lightDir", pLightComponent->GetDirection(), Shader::UniformNotFoundPolicy::k_failSilent);
                
                if(pLightComponent->GetShadowMapPtr() != nullptr)
                {
                    out_shader->SetUniform("u_shadowTolerance", pLightComponent->GetShadowTolerance(), Shader::UniformNotFoundPolicy::k_failSilent);
                    
                    //If we have used all the texture units then we cannot bind the shadow map
                    if(mudwNumBoundTextures <= mpRenderCapabilities->GetNumTextureUnits())
                    {
                        pLightComponent->GetShadowMapPtr()->Bind(mudwNumBoundTextures);
                        out_shader->SetUniform("u_shadowMap", (s32)mudwNumBoundTextures, Shader::UniformNotFoundPolicy::k_failSilent);
                        ++mudwNumBoundTextures;
                    }
                    else
                    {
                        CS_LOG_WARNING("Failed to bind shadow map. Not enough texture units");
                    }
                }
            }
            else if(inpLightComponent->IsA(CSRendering::PointLightComponent::InterfaceID))
            {
                CSRendering::PointLightComponent* pLightComponent = (CSRendering::PointLightComponent*)inpLightComponent;
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
		void RenderSystem::ApplyCamera(const CSCore::Vector3& invPosition, const CSCore::Matrix4& inmatView, const CSCore::Matrix4& inmatProj, const CSCore::Colour& inClearCol)
		{
			//Set the new view matrix based on the camera position
			mmatView = inmatView;
			mmatProj = inmatProj;
            mvCameraPos = invPosition;
			mmatViewProj = mmatView * mmatProj;
			
			//Set the clear colour
            mNewClearColour = inClearCol;
		}
		//----------------------------------------------------------
		/// Begin Frame
		//----------------------------------------------------------
		void RenderSystem::BeginFrame(CSRendering::RenderTarget* inpActiveRenderTarget)
		{
			if (inpActiveRenderTarget != nullptr)
			{
				inpActiveRenderTarget->Bind();
			}
            else
            {
                glViewport(0, 0, mudwViewWidth, mudwViewHeight);
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
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while beginning frame.");
		}
		//----------------------------------------------------------
		/// Create Render Target
		//----------------------------------------------------------
		CSRendering::RenderTarget* RenderSystem::CreateRenderTarget(u32 inudwWidth, u32 inudwHeight)
		{
			RenderTarget* pDefaultRenderTarget = new RenderTarget();
			pDefaultRenderTarget->Init(inudwWidth, inudwHeight);
			
			return pDefaultRenderTarget;
		}
		//----------------------------------------------------------
		/// Create Buffer
		//----------------------------------------------------------
		CSRendering::MeshBuffer* RenderSystem::CreateBuffer(CSRendering::BufferDescription &inDesc)
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
        void RenderSystem::RenderVertexBuffer(CSRendering::MeshBuffer* inpBuffer, u32 inudwOffset, u32 inudwNumVerts, const CSCore::Matrix4& inmatWorld)
		{
			//Set the new model view matrix based on the camera view matrix and the object matrix
            static CSCore::Matrix4 matWorldViewProj;
			matWorldViewProj = inmatWorld * mmatViewProj;
            m_currentShader->SetUniform("u_wvpMat", matWorldViewProj, Shader::UniformNotFoundPolicy::k_failSilent);
            m_currentShader->SetUniform("u_worldMat", inmatWorld, Shader::UniformNotFoundPolicy::k_failSilent);
            if(m_currentShader->HasUniform("u_normalMat"))
            {
                m_currentShader->SetUniform("u_normalMat", CSCore::Matrix4::Transpose(CSCore::Matrix4::Inverse(inmatWorld)));
            }
            
			EnableVertexAttributeForSemantic(inpBuffer);
			glDrawArrays(GetPrimitiveType(inpBuffer->GetPrimitiveType()), inudwOffset, inudwNumVerts);
            
            mbInvalidateAllCaches = false;
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while rendering vertex buffer.");
		}
        //----------------------------------------------------------
        /// Render Buffer
        //----------------------------------------------------------
        void RenderSystem::RenderBuffer(CSRendering::MeshBuffer* inpBuffer, u32 inudwOffset, u32 inudwNumIndices, const CSCore::Matrix4& inmatWorld)
		{
			//Set the new model view matrix based on the camera view matrix and the object matrix
            static CSCore::Matrix4 matWorldViewProj;
			matWorldViewProj = inmatWorld * mmatViewProj;
            m_currentShader->SetUniform("u_wvpMat", matWorldViewProj, Shader::UniformNotFoundPolicy::k_failSilent);
            m_currentShader->SetUniform("u_worldMat", inmatWorld, Shader::UniformNotFoundPolicy::k_failSilent);
            if(m_currentShader->HasUniform("u_normalMat"))
            {
                m_currentShader->SetUniform("u_normalMat", CSCore::Matrix4::Transpose(CSCore::Matrix4::Inverse(inmatWorld)));
            }
            
			//Render the buffer contents
			EnableVertexAttributeForSemantic(inpBuffer);
			glDrawElements(GetPrimitiveType(inpBuffer->GetPrimitiveType()), inudwNumIndices, GL_UNSIGNED_SHORT, (GLvoid*)inudwOffset);
            
            mbInvalidateAllCaches = false;
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while rendering buffer.");
		}
		//----------------------------------------------------------
		/// End Frame
		//----------------------------------------------------------
		void RenderSystem::EndFrame(CSRendering::RenderTarget* inpActiveRenderTarget)
		{
#ifdef CS_TARGETPLATFORM_WINDOWS
			if (inpActiveRenderTarget == nullptr)
			{
				CSBackend::Windows::SFMLWindow::Get()->Display();
			}
#endif
            
#ifdef CS_TARGETPLATFORM_ANDROID
            if (inpActiveRenderTarget != nullptr)
            {
                Texture* colourTexture = static_cast<Texture*>(inpActiveRenderTarget->GetColourTexture().get());
                if (colourTexture != nullptr)
                {
                    colourTexture->UpdateRestorationData();
                }

                Texture* depthTexture = static_cast<Texture*>(inpActiveRenderTarget->GetDepthTexture().get());
                if (depthTexture != nullptr)
                {
                    depthTexture->UpdateRestorationData();
                }
            }
#endif
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
            if(msCurrentRenderLocks.mbIsAlphaBlendingEnabled == false)
            {
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
            else if (mbInvalidateAllCaches == true)
            {
                msCurrentRenderFlags.mbIsAlphaBlendingEnabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while enabling/disabling blending.");
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
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while enabling/disabling depth test.");
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
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while enabling/disabling colour writing.");
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
            if(msCurrentRenderLocks.mbIsDepthWriteEnabled == false)
            {
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
            else if (mbInvalidateAllCaches == true)
            {
                msCurrentRenderFlags.mbIsDepthWriteEnabled ? glDepthMask(GL_TRUE) : glDepthMask(GL_FALSE);
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while enabling/disabling depth writing.");
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
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while enabling/disabling face culling.");
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
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while enabling/disabling scissor testing.");
        }
        //----------------------------------------------------------
        /// Set Scissor Region
        //---------------------------------------------------------
        void RenderSystem::SetScissorRegion(const CSCore::Vector2& invPosition, const CSCore::Vector2& invSize)
        {
			if(mbInvalidateAllCaches || mvCachedScissorPos != invPosition || mvCachedScissorSize != invSize)
            {
                mvCachedScissorPos = invPosition;
                mvCachedScissorSize = invSize;
				glScissor((GLsizei)invPosition.x, (GLsizei)invPosition.y, (GLsizei)invSize.x, (GLsizei)invSize.y);
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while setting scissor region.");
        }
        //----------------------------------------------------------
        /// Set Cull Face
        //----------------------------------------------------------
        void RenderSystem::SetCullFace(CSRendering::CullFace ineCullface)
		{
			if(mbInvalidateAllCaches || meCurrentCullFace != ineCullface)
            {
                switch (ineCullface)
                {
                    case CSRendering::CullFace::k_front:
                        glCullFace(GL_FRONT);
                        break;
                    case CSRendering::CullFace::k_back:
                        glCullFace(GL_BACK);
                        break;
                }
                
                meCurrentCullFace = ineCullface;
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while setting face culling.");
        }
        //----------------------------------------------------------
        /// Set Depth Function
        //----------------------------------------------------------
        void RenderSystem::SetDepthFunction(CSRendering::DepthTestComparison ineFunc)
        {
            if(mbInvalidateAllCaches || meDepthFunc != ineFunc)
            {
                switch (ineFunc)
                {
                    case CSRendering::DepthTestComparison::k_less:
                        glDepthFunc(GL_LESS);
                        break;
                    case CSRendering::DepthTestComparison::k_equal:
                        glDepthFunc(GL_EQUAL);
                        break;
                    case CSRendering::DepthTestComparison::k_lequal:
                        glDepthFunc(GL_LEQUAL);
                        break;
                }
                
                meDepthFunc = ineFunc;
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while setting depth function.");
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
		void RenderSystem::SetBlendFunction(CSRendering::BlendMode ineSrcFunc, CSRendering::BlendMode ineDstFunc)
		{
            if(mbBlendFunctionLocked == false)
            {
                if(mbInvalidateAllCaches || ineSrcFunc != mSrcBlendFunc || ineDstFunc != mDstBlendFunc)
                {
                    mSrcBlendFunc = ineSrcFunc;
                    mDstBlendFunc = ineDstFunc;
                    
                    GLenum srcFunc = BlendModeToGL(mSrcBlendFunc);
                    GLenum dstFunc = BlendModeToGL(mDstBlendFunc);
                    glBlendFunc(srcFunc, dstFunc);
                }
            }
            else if (mbInvalidateAllCaches == true)
            {
                GLenum srcFunc = BlendModeToGL(mSrcBlendFunc);
                GLenum dstFunc = BlendModeToGL(mDstBlendFunc);
                glBlendFunc(srcFunc, dstFunc);
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while setting blend function.");
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
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while creating the attribute state cache.");
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void RenderSystem::ApplyVertexAttributePointer(CSRendering::MeshBuffer* inpBuffer,
                                                       const char* in_attribName, GLint indwSize, GLenum ineType, GLboolean inbNormalized,
                                                       GLsizei indwStride, const GLvoid* inpOffset)
        {
            auto it = m_attributeCache.find(in_attribName);
            
            if(it != m_attributeCache.end())
            {
                //TODO: This needs fixed. At the moment it will always cache the given info which causes problems if the shader doesn't actually contain the
                //attribute. In this case the attribute will not actually get bound and therefore will cause weird issues when the same attribute is then used
                //with a shader that does actually need it since the cache beleives it to already be set and wont set it again.
                
                //                if(it->second.pBuffer == inpBuffer &&
                //                   it->second.size == indwSize &&
                //                   it->second.type == ineType &&
                //                   it->second.normalised == inbNormalized &&
                //                   it->second.stride == indwStride &&
                //                   it->second.offset == inpOffset)
                //                    return;
                
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
		void RenderSystem::EnableVertexAttributeForSemantic(CSRendering::MeshBuffer* inpBuffer)
		{
            if(mbInvalidateAllCaches || mdwMaxVertAttribs == 0)
            {
                CreateAttribStateCache();
            }
            
            // If mesh buffer has changed we need to reset all its vertex attributes
            if(((CSBackend::OpenGL::MeshBuffer*)inpBuffer)->IsCacheValid() == false)
            {
                for(auto& entry : m_attributeCache)
                {
                    if(entry.second.pBuffer == inpBuffer)
                        entry.second.pBuffer = nullptr;
                }
                ((CSBackend::OpenGL::MeshBuffer*)inpBuffer)->SetCacheValid();
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
				CS_LOG_FATAL("OpenGL ES 2.0: Shader exceeds maximum vertex attributes " + CSCore::ToString(mdwMaxVertAttribs));
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
                const CSRendering::VertexElement &Element = inpBuffer->GetVertexDeclaration().GetElementAtIndex(i);
                
                // Get parameters for vertex attribute pointer call
                const char* attribName = GetAttribNameForVertexSemantic(Element.eSemantic);
                
				const GLint         uwNumComponents = Element.NumDataTypesPerType();
                GLenum              eType = GL_FLOAT;
                GLboolean           bNormalise = GL_FALSE;
                const GLsizei       dwVertSize = inpBuffer->GetVertexDeclaration().GetTotalSize();
                const u16           uwElementOffset = inpBuffer->GetVertexDeclaration().GetElementOffset(Element);
                
                
                // Specific settings for those that differ from default
                if(Element.eSemantic == CSRendering::VertexDataSemantic::k_colour)
                {
                    eType = GL_UNSIGNED_BYTE;
                    bNormalise = GL_TRUE;
                }
                else if(Element.eSemantic == CSRendering::VertexDataSemantic::k_jointIndex)
                {
                    eType = GL_UNSIGNED_BYTE;
                }
                
                ApplyVertexAttributePointer(inpBuffer, attribName, uwNumComponents, eType, bNormalise, dwVertSize, (const GLvoid*)(s32)uwElementOffset);
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while enabling vertex attributes for semantics.");
		}
		//------------------------------------------------------------
		/// Get Primitive Type
		//------------------------------------------------------------
		s32 RenderSystem::GetPrimitiveType(CSRendering::PrimitiveType inType)
		{
			switch(inType)
			{
				case CSRendering::PrimitiveType::k_tri:
					return GL_TRIANGLES;
				case CSRendering::PrimitiveType::k_triStrip:
					return GL_TRIANGLE_STRIP;
				case CSRendering::PrimitiveType::k_line:
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
			mbInvalidateAllCaches = true;

			//clear the cache.
            m_textureUnitSystem->Clear();
            RenderTarget::ClearCache();
            
            //Set the default blend function and alpha function
            SetDepthFunction(CSRendering::DepthTestComparison::k_lequal);
            
			//we're using pre-multiplied alpha and multipass rendering and therefore require the add blend equation
            glBlendEquation(GL_FUNC_ADD);
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while forcing refresh of render states.");
		}
        //----------------------------------------------------------
		/// Remove Buffer
		//----------------------------------------------------------
		void RenderSystem::RemoveBuffer(CSRendering::MeshBuffer* inpBuffer)
		{
#ifdef CS_TARGETPLATFORM_ANDROID
            m_contextRestorer.RemoveMeshBuffer((MeshBuffer*)inpBuffer);
#endif
		}
        //----------------------------------------------------------
        //----------------------------------------------------------
        void RenderSystem::OnScreenResolutionChanged(const CSCore::Vector2& in_resolution)
        {
            mudwViewHeight = (u32)in_resolution.y;
            mudwViewWidth = (u32)in_resolution.x;
        }
        //----------------------------------------------------------
		//----------------------------------------------------------
		void RenderSystem::Destroy()
		{
			if (mpbCurrentVertexAttribState != nullptr)
			{
				free(mpbCurrentVertexAttribState);
			}
			if (mpbLastVertexAttribState != nullptr)
			{
				free(mpbLastVertexAttribState);
			}
            
            m_resolutionChangeConnection = nullptr;
			m_displayModeChangeConnection = nullptr;
		}
	}
}
