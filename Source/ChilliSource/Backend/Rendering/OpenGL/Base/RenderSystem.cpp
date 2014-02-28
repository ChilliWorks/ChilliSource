/*
 *  OpenGLES_2_0.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 27/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderSystem.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Texture/Texture.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Texture/Cubemap.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/MeshBuffer.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderCapabilities.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderTarget.h>

#include <ChilliSource/Rendering/Lighting/LightComponent.h>
#include <ChilliSource/Rendering/Lighting/AmbientLightComponent.h>
#include <ChilliSource/Rendering/Lighting/DirectionalLightComponent.h>
#include <ChilliSource/Rendering/Lighting/PointLightComponent.h>

#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>

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
        CS_DEFINE_NAMEDTYPE(CRenderSystem);
        
        bool gbIsMapBufferAvailable = true;
        
		//----------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------
		CRenderSystem::CRenderSystem()
		: mpDefaultRenderTarget(nullptr), mpCurrentMaterial(nullptr), mbInvalidateAllCaches(true), mdwMaxVertAttribs(0), mpVertexAttribs(nullptr),
        mbEmissiveSet(false), mbAmbientSet(false), mbDiffuseSet(false), mbSpecularSet(false), mudwNumBoundTextures(0), mSrcBlendFunc(Rendering::AlphaBlend::k_unknown), mDstBlendFunc(Rendering::AlphaBlend::k_unknown),
        meCurrentCullFace(Rendering::CullFace::k_front), meDepthFunc(Rendering::DepthFunction::k_less), mpLightComponent(nullptr), mbBlendFunctionLocked(false), mpaTextureHandles(nullptr), mbInvalidateLigthingCache(true),
        mpRenderCapabilities(nullptr)
		{
			//Register the GL texture and shader managers
            Core::ResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mTexManager);
            Core::ResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mCubemapManager);
            Core::ResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mShaderManager);
            
#ifdef CS_TARGETPLATFORM_IOS
            //Create the context with the specified GLES version
			mContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
            
			//Set the current context
            if(!mContext || ![EAGLContext setCurrentContext: mContext])
            {
                CS_LOG_FATAL("Cannot Create OpenGL ES 2.0 Context");
            }
#endif
		}
        //----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool CRenderSystem::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return	(inInterfaceID == RenderSystem::InterfaceID) || (inInterfaceID == Core::IComponentProducer::InterfaceID);
		}
        //----------------------------------------------------------
        /// Init
        //----------------------------------------------------------
        bool CRenderSystem::Init(u32 inudwWidth, u32 inudwHeight)
		{
            CS_ASSERT((inudwWidth > 0 && inudwHeight > 0), "Cannot create and OpenGL ES view with size ZERO");
#ifdef TARGET_WINDOWS
			GLenum GlewError = glewInit();
			if(GLEW_OK != GlewError)
			{
				//Problem: glewInit failed, something is seriously wrong.
				CS_LOG_FATAL("Glew Error On Init: " + std::string((const char*)glewGetErrorString(GlewError)));
			}
#endif
#ifdef TARGET_ANDROID
            //Check for map buffer support
            gbIsMapBufferAvailable = CRenderSystem::CheckForOpenGLExtension("GL_OES_mapbuffer");
#endif
            
            mpRenderCapabilities = Core::Application::GetSystemImplementing<CRenderCapabilities>();
            CS_ASSERT(mpRenderCapabilities, "Cannot find required system: Render Capabilities.");
            mpRenderCapabilities->CalculateCapabilities();
            
            ForceRefreshRenderStates();
			
            mShaderManager.SetRenderSystem(this);
            OnScreenOrientationChanged(inudwWidth, inudwHeight);
            
            mudwViewWidth = inudwWidth;
            mudwViewHeight = inudwHeight;
            
            return true;
		}
        //----------------------------------------------------------
        /// Resume
        //----------------------------------------------------------
        void CRenderSystem::Resume()
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
        /// Suspend
        //----------------------------------------------------------
        void CRenderSystem::Suspend()
        {
        	BackupContext();
        }
        //----------------------------------------------------------
        /// Backup Context
        //----------------------------------------------------------
        void CRenderSystem::BackupContext()
        {
#ifdef TARGET_ANDROID
        	//Context is about to be lost do a data backup
        	BackupMeshBuffers();
        	mTexManager.Backup();
#endif
        }
        //----------------------------------------------------------
		/// Restore Context
		//----------------------------------------------------------
		void CRenderSystem::RestoreContext()
		{
#ifdef TARGET_ANDROID
            ForceRefreshRenderStates();
            RestoreMeshBuffers();
            mGLCurrentShaderProgram = 0;
            mShaderManager.Restore();
            mTexManager.Restore();
            mCubemapManager.Restore();
#endif
		}
        //----------------------------------------------------------
        /// Set Light
        //----------------------------------------------------------
        void CRenderSystem::SetLight(Rendering::LightComponent* inpLightComponent)
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
		/// Apply Material
		//----------------------------------------------------------
		void CRenderSystem::ApplyMaterial(const Rendering::Material& inMaterial)
		{
			//Enable the hardware states
            if(mbInvalidateAllCaches || !mpCurrentMaterial || mpCurrentMaterial != &inMaterial || !mpCurrentMaterial->IsCacheValid())
            {
                mudwNumBoundTextures = 0;
                mpCurrentMaterial = &inMaterial;
                
                //Bind this materials shader
                if(inMaterial.GetActiveShaderProgram() != nullptr)
                {
                    ApplyRenderStates(inMaterial);
                    
                    ShaderSPtr pShader = std::static_pointer_cast<CShader>(inMaterial.GetActiveShaderProgram());
                    GLuint GLShaderProgram = pShader->GetProgramID();
                    
                    if(GLShaderProgram != 0)
                    {
                        //Don't bind the shader if we don't need to!
                        bool bShaderChanged = mbInvalidateAllCaches || GLShaderProgram != mGLCurrentShaderProgram;
                        if(bShaderChanged == true)
                        {
                            glUseProgram(GLShaderProgram);
                            mGLCurrentShaderProgram = GLShaderProgram;
                            
                            GetAttributeLocations(pShader);
                            GetUniformLocations(inMaterial);
                            
                            mbEmissiveSet = false;
                            mbAmbientSet = false;
                            mbDiffuseSet = false;
                            mbSpecularSet = false;
                            mbInvalidateLigthingCache = true;
                        }
                        
                        //Get and set all the custom shader variables
                        if(inMaterial.IsVariableCacheValid() == false || bShaderChanged)
                        {
                            ApplyShaderVariables(inMaterial, GLShaderProgram);
                        }
                        
                        ApplyTextures(inMaterial);
                        ApplyLightingValues(inMaterial);
                        ApplyLighting(inMaterial, mpLightComponent);
                    }
                    
                    mpCurrentMaterial->SetCacheValid();
                }
            }
		}
        //----------------------------------------------------------
        /// Apply Joints
        //----------------------------------------------------------
        void CRenderSystem::ApplyJoints(const std::vector<Core::Matrix4x4>& inaJoints)
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
		void CRenderSystem::ApplyRenderStates(const Rendering::Material& inMaterial)
        {
            EnableAlphaBlending(inMaterial.IsTransparent());
            SetBlendFunction(inMaterial.GetSourceBlendFunction(), inMaterial.GetDestBlendFunction());

            EnableFaceCulling(inMaterial.IsCullingEnabled());
            SetCullFace(inMaterial.GetCullFace());
            
            EnableColourWriting(inMaterial.IsColourWriteEnabled());
            EnableDepthWriting(inMaterial.IsDepthWriteEnabled());
            EnableDepthTesting(inMaterial.IsDepthTestEnabled());
            EnableScissorTesting(inMaterial.IsScissoringEnabled());
            SetScissorRegion(inMaterial.GetScissoringRegionPosition(), inMaterial.GetScissoringRegionSize());
        }
        //----------------------------------------------------------
		/// Get Attribute Locations
		//----------------------------------------------------------
		void CRenderSystem::GetAttributeLocations(const ShaderSPtr& inpShader)
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
		void CRenderSystem::GetUniformLocations(const Rendering::Material &inMaterial)
        {
            ShaderSPtr pShader = std::static_pointer_cast<CShader>(inMaterial.GetActiveShaderProgram());
            
            //Get the required handles to the shader variables (Uniform)
            mmatWVPHandle = pShader->GetUniformLocation("umatWorldViewProj");
            mmatWorldHandle = pShader->GetUniformLocation("umatWorld");
            mmatNormalHandle = pShader->GetUniformLocation("umatNormal");
        
            mEmissiveHandle = pShader->GetUniformLocation("uvEmissive");
            mAmbientHandle = pShader->GetUniformLocation("uvAmbient");
            mDiffuseHandle = pShader->GetUniformLocation("uvDiffuse");
            mSpecularHandle = pShader->GetUniformLocation("uvSpecular");
    
            mJointsHandle = pShader->GetUniformLocation("uavJoints");
            
            mLightPosHandle = pShader->GetUniformLocation("uvLightPos");
            mLightDirHandle = pShader->GetUniformLocation("uvLightDir");
            mLightColHandle = pShader->GetUniformLocation("uvLightCol");
            mAttenConHandle = pShader->GetUniformLocation("ufAttenuationConstant");
            mAttenLinHandle = pShader->GetUniformLocation("ufAttenuationLinear");
            mAttenQuadHandle = pShader->GetUniformLocation("ufAttenuationQuadratic");
            mCameraPosHandle = pShader->GetUniformLocation("uvCameraPos");
            
            mLightMatrixHandle = pShader->GetUniformLocation("umatLight");
            mShadowToleranceHandle = pShader->GetUniformLocation("ufShadowTolerance");
            mShadowMapTexHandle = pShader->GetUniformLocation("uShadowMapTexture");
            
            mCubemapHandle = pShader->GetUniformLocation("uCubemap");
            
            if(mpaTextureHandles == nullptr)
            {
                mpaTextureHandles = (std::pair<GLint, u32>*)calloc(mpRenderCapabilities->GetNumTextureUnits(), sizeof(std::pair<GLint, u32>));
            }
            
            if(inMaterial.GetTextures().empty() == false)
            {
                CS_ASSERT(inMaterial.GetTextures().size() <= mpRenderCapabilities->GetNumTextureUnits(), "RenderSystem::ApplyMaterial -> Trying to bind more textures than there area texture units");
                
                for(u32 i=0; i<inMaterial.GetTextures().size(); ++i)
                {
                    mpaTextureHandles[i].first = pShader->GetUniformLocation(std::string("uTexture"+Core::ToString(i)).c_str());
                }
            }
        }
        //----------------------------------------------------------
		/// Apply Shader Variables
		//----------------------------------------------------------
		void CRenderSystem::ApplyShaderVariables(const Rendering::Material &inMaterial, GLuint inShaderProg)
		{
            ShaderSPtr pShader = std::static_pointer_cast<CShader>(inMaterial.GetActiveShaderProgram());
			//Get and set all the custom shader variables
			for(Rendering::MapStringToFloat::const_iterator it = inMaterial.mMapFloatShaderVars.begin(); it!= inMaterial.mMapFloatShaderVars.end(); ++it)
			{
				GLint VarHandle = pShader->GetUniformLocation(it->first.c_str());
				glUniform1f(VarHandle, it->second);
			}
			for(Rendering::MapStringToVec2::const_iterator it = inMaterial.mMapVec2ShaderVars.begin(); it!= inMaterial.mMapVec2ShaderVars.end(); ++it)
			{
				GLint VarHandle = pShader->GetUniformLocation(it->first.c_str());
				glUniform2fv(VarHandle, 1, (GLfloat*)(&it->second));
			}
			for(Rendering::MapStringToVec3::const_iterator it = inMaterial.mMapVec3ShaderVars.begin(); it!= inMaterial.mMapVec3ShaderVars.end(); ++it)
			{
				GLint VarHandle = pShader->GetUniformLocation(it->first.c_str());
				glUniform3fv(VarHandle, 1, (GLfloat*)(&it->second));
			}
			for(Rendering::MapStringToVec4::const_iterator it = inMaterial.mMapVec4ShaderVars.begin(); it!= inMaterial.mMapVec4ShaderVars.end(); ++it)
			{
				GLint VarHandle = pShader->GetUniformLocation(it->first.c_str());
				glUniform4fv(VarHandle, 1, (GLfloat*)(&it->second));
			}
			for(Rendering::MapStringToMat4::const_iterator it = inMaterial.mMapMat4ShaderVars.begin(); it!= inMaterial.mMapMat4ShaderVars.end(); ++it)
			{
				GLint VarHandle = pShader->GetUniformLocation(it->first.c_str());
				glUniformMatrix4fv(VarHandle, 1, GL_FALSE, (GLfloat*)(it->second.m));
			}
			for(Rendering::MapStringToMat4Array::const_iterator it = inMaterial.mMapMat4ArrayShaderVars.begin(); it!= inMaterial.mMapMat4ArrayShaderVars.end(); ++it)
			{
				GLint VarHandle = pShader->GetUniformLocation(it->first.c_str());
				glUniformMatrix4fv(VarHandle, it->second.size(), GL_FALSE, (GLfloat*)(it->second[0].m));
			}
			for(Rendering::MapStringToCol::const_iterator it = inMaterial.mMapColShaderVars.begin(); it!= inMaterial.mMapColShaderVars.end(); ++it)
			{
				GLint VarHandle = pShader->GetUniformLocation(it->first.c_str());
				glUniform4fv(VarHandle, 1, (GLfloat*)(&it->second));
			}
		}
        //----------------------------------------------------------
        /// Apply Textures
        //----------------------------------------------------------
        void CRenderSystem::ApplyTextures(const Rendering::Material &inMaterial)
        {
            for(u32 i=0; i<inMaterial.GetTextures().size(); ++i)
            {
                if(mpaTextureHandles[i].first != -1)
                {
                    inMaterial.GetTextures()[i]->Bind(mudwNumBoundTextures);
                    if(mpaTextureHandles[i].second != mudwNumBoundTextures || mbInvalidateAllCaches)
                    {
                        glUniform1i(mpaTextureHandles[i].first, mudwNumBoundTextures);
                        mpaTextureHandles[i].second = mudwNumBoundTextures;
                    }
                    ++mudwNumBoundTextures;
                }
            }
            
            if(mCubemapHandle >= 0 && inMaterial.GetCubemap() != nullptr)
            {
                inMaterial.GetCubemap()->Bind(mudwNumBoundTextures);
                glUniform1i(mCubemapHandle, mudwNumBoundTextures);
                ++mudwNumBoundTextures;
            }
        }
        //----------------------------------------------------------
        /// Apply Lighting Values
        //----------------------------------------------------------
        void CRenderSystem::ApplyLightingValues(const Rendering::Material &inMaterial)
        {
            if(mEmissiveHandle != -1 && (mbInvalidateAllCaches || mbEmissiveSet == false || mCurrentEmissive != inMaterial.GetEmissive()))
            {
                mbEmissiveSet = true;
                mCurrentEmissive = inMaterial.GetEmissive();
                glUniform4f(mEmissiveHandle, mCurrentEmissive.r, mCurrentEmissive.g, mCurrentEmissive.b, mCurrentEmissive.a);
            }
            if(mAmbientHandle != -1 && (mbInvalidateAllCaches || mbAmbientSet == false || mCurrentAmbient != inMaterial.GetAmbient()))
            {
                mbAmbientSet = true;
                mCurrentAmbient = inMaterial.GetAmbient();
                glUniform4f(mAmbientHandle, mCurrentAmbient.r, mCurrentAmbient.g, mCurrentAmbient.b, mCurrentAmbient.a);
            }
            if(mDiffuseHandle != -1 && (mbInvalidateAllCaches || mbDiffuseSet == false || mCurrentDiffuse != inMaterial.GetDiffuse()))
            {
                mbDiffuseSet = true;
                mCurrentDiffuse = inMaterial.GetDiffuse();
                glUniform4f(mDiffuseHandle, mCurrentDiffuse.r, mCurrentDiffuse.g, mCurrentDiffuse.b, mCurrentDiffuse.a);
            }
            if(mSpecularHandle != -1 && (mbInvalidateAllCaches || mbSpecularSet == false || mCurrentSpecular != inMaterial.GetSpecular()))
            {
                mbSpecularSet = true;
                mCurrentSpecular = inMaterial.GetSpecular();
                glUniform4f(mSpecularHandle, mCurrentSpecular.r, mCurrentSpecular.g, mCurrentSpecular.b, mCurrentSpecular.a);
            }
        }
        //----------------------------------------------------------
        /// Apply Lighting
        //----------------------------------------------------------
        void CRenderSystem::ApplyLighting(const Rendering::Material &inMaterial, Rendering::LightComponent* inpLightComponent)
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
                    glUniform3fv(mLightDirHandle, 1, (const GLfloat*)&pLightComponent->GetDirection());
                }
                if(pLightComponent->GetShadowMapPtr() != nullptr)
                {
                    if(mShadowToleranceHandle >= 0)
                    {
                        glUniform1f(mShadowToleranceHandle, pLightComponent->GetShadowTolerance());
                    }
                    
                    if(mShadowMapTexHandle >= 0)
                    {
                        CS_ASSERT(mudwNumBoundTextures <= mpRenderCapabilities->GetNumTextureUnits(), "RenderSystem::ApplyMaterial -> Trying to bind more textures than there area texture units");
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
                glUniform4fv(mLightColHandle, 1, (const GLfloat*)&inpLightComponent->GetColour());
            }
            if(mLightMatrixHandle >= 0)
            {
                glUniformMatrix4fv(mLightMatrixHandle, 1, GL_FALSE, (GLfloat*)inpLightComponent->GetLightMatrix().m);
            }
        }
		//----------------------------------------------------------
		/// Apply Camera
		//----------------------------------------------------------
		void CRenderSystem::ApplyCamera(const Core::Vector3& invPosition, const Core::Matrix4x4& inmatView, const Core::Matrix4x4& inmatProj, const Core::Colour& inClearCol)
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
		Rendering::RenderTarget* CRenderSystem::GetDefaultRenderTarget()
		{
			return mpDefaultRenderTarget;
		}
		//----------------------------------------------------------
		/// Begin Frame
		//----------------------------------------------------------
		void CRenderSystem::BeginFrame(Rendering::RenderTarget* inpActiveRenderTarget)
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
		Rendering::RenderTarget* CRenderSystem::CreateRenderTarget(u32 inudwWidth, u32 inudwHeight)
		{
			CRenderTarget* pDefaultRenderTarget = new CRenderTarget();
			pDefaultRenderTarget->Init(inudwWidth, inudwHeight);
			
			return pDefaultRenderTarget;
		}
		//----------------------------------------------------------
		/// Create Buffer
		//----------------------------------------------------------
		Rendering::MeshBuffer* CRenderSystem::CreateBuffer(Rendering::BufferDescription &inDesc)
		{
			CMeshBuffer* pBuffer = new CMeshBuffer(inDesc);
			pBuffer->SetMapBufferAvailable(gbIsMapBufferAvailable);
			pBuffer->SetOwningRenderSystem(this);
            
#ifdef TARGET_ANDROID
			mMeshBuffers.push_back(pBuffer);
#endif
			return pBuffer;
		}
        //----------------------------------------------------------
        /// Render Vertex Buffer
        //----------------------------------------------------------
        void CRenderSystem::RenderVertexBuffer(Rendering::MeshBuffer* inpBuffer, u32 inudwOffset, u32 inudwNumVerts, const Core::Matrix4x4& inmatWorld)
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
        void CRenderSystem::RenderBuffer(Rendering::MeshBuffer* inpBuffer, u32 inudwOffset, u32 inudwNumIndices, const Core::Matrix4x4& inmatWorld)
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
		void CRenderSystem::EndFrame(Rendering::RenderTarget* inpActiveRenderTarget)
		{
#ifdef CS_TARGETPLATFORM_IOS
            if (mpDefaultRenderTarget != nullptr && mpDefaultRenderTarget == inpActiveRenderTarget)
            {
                CRenderTarget::PresentDefaultRenderTarget(mContext, mpDefaultRenderTarget);
            }
#else
            glViewport(0, 0, mudwViewWidth, mudwViewHeight);
#endif
#ifdef TARGET_WINDOWS
			glfwSwapBuffers();
#endif
		}
		//----------------------------------------------------------
		/// On Screen Orientation Changed
		//----------------------------------------------------------
        void CRenderSystem::OnScreenOrientationChanged(u32 inudwWidth, u32 inudwHeight)
		{
            ResizeFrameBuffer(inudwWidth, inudwHeight);
		}
		//----------------------------------------------------------
		/// Resize Frame Buffer
		//----------------------------------------------------------
		void CRenderSystem::ResizeFrameBuffer(u32 inudwWidth, u32 inudwHeight)
		{
#ifdef CS_TARGETPLATFORM_IOS
            if(mudwViewWidth != inudwWidth || mudwViewHeight != inudwHeight)
            {
                if(mpDefaultRenderTarget)
                {
                    CRenderTarget::DestroyDefaultRenderTarget(mContext, mpDefaultRenderTarget);
                }
                
                //Create a default frame buffer for on-screen rendering
                mpDefaultRenderTarget = CRenderTarget::CreateDefaultRenderTarget(mContext, inudwWidth, inudwHeight);
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
        void CRenderSystem::LockAlphaBlending()
        {
            msCurrentRenderLocks.mbIsAlphaBlendingEnabled = true;
        }
        //----------------------------------------------------------
		/// Unlock Alpha Blending
		//----------------------------------------------------------
        void CRenderSystem::UnlockAlphaBlending()
        {
            msCurrentRenderLocks.mbIsAlphaBlendingEnabled = false;
        }
		//----------------------------------------------------------
		/// Enable Alpha Blending
		//----------------------------------------------------------
		void CRenderSystem::EnableAlphaBlending(bool inbIsEnabled)
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
		void CRenderSystem::EnableDepthTesting(bool inbIsEnabled)
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
		void CRenderSystem::EnableColourWriting(bool inbIsEnabled)
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
        void CRenderSystem::LockDepthWriting()
        {
            msCurrentRenderLocks.mbIsDepthWriteEnabled = true;
        }
        //----------------------------------------------------------
		/// Unlock Depth Writing
		//----------------------------------------------------------
        void CRenderSystem::UnlockDepthWriting()
        {
            msCurrentRenderLocks.mbIsDepthWriteEnabled = false;
        }
		//----------------------------------------------------------
		/// Enable Depth Writing
		//----------------------------------------------------------
		void CRenderSystem::EnableDepthWriting(bool inbIsEnabled)
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
		void CRenderSystem::EnableFaceCulling(bool inbIsEnabled)
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
        void CRenderSystem::EnableScissorTesting(bool inbIsEnabled)
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
        void CRenderSystem::SetScissorRegion(const Core::Vector2& invPosition, const Core::Vector2& invSize)
        {
            if(mbInvalidateAllCaches || mvCachedScissorPos != invPosition || mvCachedScissorSize != invSize)
            {
                mvCachedScissorPos = invPosition;
                mvCachedScissorSize = invSize;
                glScissor(invPosition.x, invPosition.y, invSize.x, invSize.y);
            }
        }
        //----------------------------------------------------------
        /// Set Cull Face
        //----------------------------------------------------------
        void CRenderSystem::SetCullFace(Rendering::CullFace ineCullface)
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
        void CRenderSystem::SetDepthFunction(Rendering::DepthFunction ineFunc)
        {
            if(mbInvalidateAllCaches || meDepthFunc != ineFunc)
            {
                switch (ineFunc)
                {
                    case Rendering::DepthFunction::k_less:
                        glDepthFunc(GL_LESS);
                        break;
                    case Rendering::DepthFunction::k_equal:
                        glDepthFunc(GL_EQUAL);
                        break;
                    case Rendering::DepthFunction::k_lequal:
                        glDepthFunc(GL_LEQUAL);
                        break;
                }
                
                meDepthFunc = ineFunc;
            }
        }
        //----------------------------------------------------------
		/// Lock Blend Function
		//----------------------------------------------------------
        void CRenderSystem::LockBlendFunction()
        {
            mbBlendFunctionLocked = true;
        }
        //----------------------------------------------------------
		/// Unlock Blend Function
		//----------------------------------------------------------
        void CRenderSystem::UnlockBlendFunction()
        {
            mbBlendFunctionLocked = false;
        }
		//----------------------------------------------------------
		/// Set Blend Function
		//----------------------------------------------------------
		void CRenderSystem::SetBlendFunction(Rendering::AlphaBlend ineSrcFunc, Rendering::AlphaBlend ineDstFunc)
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
					case Rendering::AlphaBlend::k_zero:
						SrcFunc = GL_ZERO;
						break;
					case Rendering::AlphaBlend::k_one:
						SrcFunc = GL_ONE;
						break;
					case Rendering::AlphaBlend::k_sourceCol:
						SrcFunc = GL_SRC_COLOR;
						break;
					case Rendering::AlphaBlend::k_oneMinusSourceCol:
						SrcFunc = GL_ONE_MINUS_SRC_COLOR;
						break;
					case Rendering::AlphaBlend::k_sourceAlpha:
						SrcFunc = GL_SRC_ALPHA;
						break;
					case Rendering::AlphaBlend::k_oneMinusSourceAlpha:
						SrcFunc = GL_ONE_MINUS_SRC_ALPHA;
						break;
					case Rendering::AlphaBlend::k_destAlpha:
						SrcFunc = GL_DST_ALPHA;
						break;
					case Rendering::AlphaBlend::k_oneMinusDestAlpha:
						SrcFunc = GL_ONE_MINUS_DST_ALPHA;
						break;
					case Rendering::AlphaBlend::k_unknown:
					default:
						CS_LOG_ERROR("Open GL ES Unknown blend function");
						break;
				};
                
				GLenum DstFunc = GL_ONE_MINUS_SRC_ALPHA;
                switch(ineDstFunc)
				{
					case Rendering::AlphaBlend::k_zero:
						DstFunc = GL_ZERO;
						break;
					case Rendering::AlphaBlend::k_one:
						DstFunc = GL_ONE;
						break;
					case Rendering::AlphaBlend::k_sourceCol:
						DstFunc = GL_SRC_COLOR;
						break;
					case Rendering::AlphaBlend::k_oneMinusSourceCol:
						DstFunc = GL_ONE_MINUS_SRC_COLOR;
						break;
					case Rendering::AlphaBlend::k_sourceAlpha:
						DstFunc = GL_SRC_ALPHA;
						break;
					case Rendering::AlphaBlend::k_oneMinusSourceAlpha:
						DstFunc = GL_ONE_MINUS_SRC_ALPHA;
						break;
					case Rendering::AlphaBlend::k_destAlpha:
						DstFunc = GL_DST_ALPHA;
						break;
					case Rendering::AlphaBlend::k_oneMinusDestAlpha:
						DstFunc = GL_ONE_MINUS_DST_ALPHA;
						break;
					case Rendering::AlphaBlend::k_unknown:
					default:
						CS_LOG_ERROR("Open GL ES Unknown blend function");
						break;
				};
				
				glBlendFunc(SrcFunc, DstFunc);
			}
		}
        //----------------------------------------------------------
        /// Create Attrib State Cache
        //----------------------------------------------------------
        void CRenderSystem::CreateAttribStateCache()
        {
            if(mdwMaxVertAttribs == 0)
            {
                glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &mdwMaxVertAttribs);
                mpbCurrentVertexAttribState = (bool*)calloc(mdwMaxVertAttribs, sizeof(bool));
                mpbLastVertexAttribState = (bool*)calloc(mdwMaxVertAttribs, sizeof(bool));
                mpVertexAttribs = (VertexAttribSet*) calloc(mdwMaxVertAttribs, sizeof(VertexAttribSet));
            }
        }
        
        s32 CRenderSystem::GetLocationForVertexSemantic(Rendering::VertexDataSemantic ineSemantic)
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
        bool CRenderSystem::ApplyVertexAttributePointr(Rendering::MeshBuffer* inpBuffer,
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
		void CRenderSystem::EnableVertexAttributeForSemantic(Rendering::MeshBuffer* inpBuffer)
		{
            if(mbInvalidateAllCaches || mdwMaxVertAttribs == 0)
            {
                CreateAttribStateCache();
            }
            
            
            // If mesh buffer has changed we need to reset all its vertex attributes
            if(((ChilliSource::OpenGL::CMeshBuffer*)inpBuffer)->IsCacheValid() == false)
            {
                for(u32 i =0; i < mdwMaxVertAttribs; i++)
                {
                    if(mpVertexAttribs[i].pBuffer == inpBuffer)
                        mpVertexAttribs[i].pBuffer = nullptr;
                }
                ((ChilliSource::OpenGL::CMeshBuffer*)inpBuffer)->SetCacheValid();
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
		s32 CRenderSystem::GetPrimitiveType(Rendering::PrimitiveType inType)
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
        std::string CRenderSystem::GetPathToShaders() const
        {
            return "Shaders/OpenGL/";
        }
		//----------------------------------------------------------
		/// Check for OpenGL Extension
		//----------------------------------------------------------
		bool CRenderSystem::CheckForOpenGLExtension(const std::string& instrExtension)
		{
			std::string strExt = (const char*)glGetString(GL_EXTENSIONS);
            return strExt.find(instrExtension) != strExt.npos;
		}
		//----------------------------------------------------------
		/// Force Refresh Render States
		//----------------------------------------------------------
		void CRenderSystem::ForceRefreshRenderStates()
		{
			//clear the cache.
			CTexture::ClearCache();
            CRenderTarget::ClearCache();
            
            //Set the default blend function and alpha function
			SetBlendFunction(Rendering::AlphaBlend::k_sourceAlpha, Rendering::AlphaBlend::k_oneMinusSourceAlpha);
            SetDepthFunction(Rendering::DepthFunction::k_lequal);
            
			//we're using pre-multiplied alpha and therefore require a different blend mode
            glBlendEquation(GL_FUNC_ADD);
            glCullFace(GL_FRONT);
            
            mbInvalidateAllCaches = true;
		}
		//----------------------------------------------------------
		/// Backup Mesh Buffers
		//----------------------------------------------------------
		void CRenderSystem::BackupMeshBuffers()
		{
#ifdef TARGET_ANDROID
			for(std::vector<CMeshBuffer*>::iterator it = mMeshBuffers.begin(); it != mMeshBuffers.end(); ++it)
			{
				(*it)->Backup();
			}
#endif
		}
		//----------------------------------------------------------
		/// Restore Mesh Buffers
		//----------------------------------------------------------
		void CRenderSystem::RestoreMeshBuffers()
		{
#ifdef TARGET_ANDROID
			for(std::vector<CMeshBuffer*>::iterator it = mMeshBuffers.begin(); it != mMeshBuffers.end(); ++it)
			{
				(*it)->Restore();
			}
#endif
		}
        //----------------------------------------------------------
		/// Remove Buffer
		//----------------------------------------------------------
		void CRenderSystem::RemoveBuffer(Rendering::MeshBuffer* inpBuffer)
		{
#ifdef TARGET_ANDROID
			for(std::vector<CMeshBuffer*>::iterator it = mMeshBuffers.begin(); it != mMeshBuffers.end(); ++it)
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
		void CRenderSystem::CheckForGLErrors()
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
		/// Destroy
		//----------------------------------------------------------
		void CRenderSystem::Destroy()
		{
#ifdef CS_TARGETPLATFORM_IOS
            if(mpDefaultRenderTarget)
            {
                CRenderTarget::DestroyDefaultRenderTarget(mContext, mpDefaultRenderTarget);
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
		}
		//----------------------------------------------------------------
		/// Destructor
		//----------------------------------------------------------
		CRenderSystem::~CRenderSystem()
		{
            Destroy();
            free(mpbCurrentVertexAttribState);
            free(mpbLastVertexAttribState);
            free(mpVertexAttribs);
            free(mpaTextureHandles);
		}
	}
}
