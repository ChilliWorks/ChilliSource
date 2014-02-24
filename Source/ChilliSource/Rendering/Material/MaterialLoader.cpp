//
//  MaterialLoader.cpp
//  MoFlow
//
//  Created by Scott Downie on 22/11/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>

#include <ChilliSource/Rendering/Shader/ShaderManager.h>
#include <ChilliSource/Rendering/Shader/Shader.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/CubemapManager.h>
#include <ChilliSource/Rendering/Texture/Cubemap.h>
#include <ChilliSource/Rendering/Material/MaterialLoader.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>

#include <ChilliSource/Core/XML/XMLUtils.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace ChilliSource
{
	namespace Rendering
	{
		namespace
		{
			const std::string kstrMaterialExtension("momaterial");
		}
		
		//-------------------------------------------------------------------------
		/// Constructor
		//-------------------------------------------------------------------------
		CMaterialLoader::CMaterialLoader(IRenderCapabilities* inpRenderCapabilities)
			: mpRenderCapabilities(inpRenderCapabilities)
		{
			MOFLOW_ASSERT(mpRenderCapabilities, "Material loader is missing required system: Render Capabilities.");
		}
		//-------------------------------------------------------------------------
		/// Is A
		//-------------------------------------------------------------------------
		bool CMaterialLoader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource of Kind
		//----------------------------------------------------------------------------
		bool CMaterialLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CMaterial::InterfaceID);
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource From File With Extension
		//----------------------------------------------------------------------------
		bool CMaterialLoader::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
		{
			return (inExtension == kstrMaterialExtension);
		}
		//----------------------------------------------------------------------------
		/// Create Resource From File
		//----------------------------------------------------------------------------
		bool CMaterialLoader::CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)  
		{
            DYNAMIC_ARRAY<std::pair<ShaderPass, std::pair<Core::StorageLocation, std::string> > > aShaderFiles;
            DYNAMIC_ARRAY<TextureDesc> aTextureFiles;
            DYNAMIC_ARRAY<TextureDesc> aCubemapFiles;
            if(BuildMaterialFromFile(ineStorageLocation, inFilePath, aShaderFiles, aTextureFiles, aCubemapFiles, outpResource) == true)
			{
                MaterialPtr pMaterial = SHARED_PTR_CAST<CMaterial>(outpResource);
                
                IShaderManager* pShaderManager = GET_RESOURCE_MANAGER(IShaderManager);
                if(pShaderManager != NULL)
                {
                    for(u32 i=0; i<aShaderFiles.size(); ++i)
                    {
                        if(aShaderFiles[i].second.second.empty() == false)
                        {
                            ShaderPtr pShader = pShaderManager->GetShaderFromFile(aShaderFiles[i].second.first, aShaderFiles[i].second.second);
                            pMaterial->SetShaderProgram(aShaderFiles[i].first, pShader);
                        }
                    }
                }
                
                ITextureManager* pTextureManager = GET_RESOURCE_MANAGER(ITextureManager);
                if(pTextureManager != NULL)
                {
                    for(u32 i=0; i<aTextureFiles.size(); ++i)
                    {
                        pMaterial->AddTexture(pTextureManager->GetTextureFromFile(aTextureFiles[i].meLocation, aTextureFiles[i].mstrFile, Core::CImage::Format::k_default, aTextureFiles[i].mbMipMapped));
                    }
                }
                
                ICubemapManager* pCubemapManager = GET_RESOURCE_MANAGER(ICubemapManager);
                if(pCubemapManager != NULL)
                {
                    for(u32 i=0; i<aCubemapFiles.size(); ++i)
                    {
                        pMaterial->SetCubemap(pCubemapManager->GetCubemapFromFile(aCubemapFiles[i].meLocation, aCubemapFiles[i].mstrFile, Core::CImage::Format::k_default, aCubemapFiles[i].mbMipMapped));
                    }
                }
                
                pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
                
                return true;
            }
            
            return false;
		}
		//----------------------------------------------------------------------------
		/// Async Create Resource From File
		//----------------------------------------------------------------------------
		bool CMaterialLoader::AsyncCreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)  
		{
			//Start the material building task.
			Core::Task3<Core::StorageLocation, const std::string&, Core::ResourcePtr&> BuildMaterialTask(this, &CMaterialLoader::BuildMaterialTask, ineStorageLocation, inFilePath, outpResource);
			Core::CTaskScheduler::ScheduleTask(BuildMaterialTask);
			
			return true;
		}
		//----------------------------------------------------------------------------
		/// Build Material Task
		//----------------------------------------------------------------------------
		void CMaterialLoader::BuildMaterialTask(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)
		{
			//build the material
            DYNAMIC_ARRAY<std::pair<ShaderPass, std::pair<Core::StorageLocation, std::string> > > aShaderFiles;
            DYNAMIC_ARRAY<TextureDesc> aTextureFiles;
            DYNAMIC_ARRAY<TextureDesc> aCubemapFiles;
			if(BuildMaterialFromFile(ineStorageLocation, inFilePath, aShaderFiles, aTextureFiles, aCubemapFiles, outpResource) == true)
			{
                MaterialPtr pMaterial = SHARED_PTR_CAST<CMaterial>(outpResource);
                
                IShaderManager* pShaderManager = GET_RESOURCE_MANAGER(IShaderManager);
                if(pShaderManager != NULL)
                {
                    for(u32 i=0; i<aShaderFiles.size(); ++i)
                    {
                        ShaderPtr pShader = pShaderManager->AsyncGetShaderFromFile(aShaderFiles[i].second.first, aShaderFiles[i].second.second);
                        pMaterial->SetShaderProgram(aShaderFiles[i].first, pShader);
                        pShader->WaitTilLoaded();
                    }
                }
                
                ITextureManager* pTextureManager = GET_RESOURCE_MANAGER(ITextureManager);
                if(pTextureManager != NULL)
                {
                    for(u32 i=0; i<aTextureFiles.size(); ++i)
                    {
                        TexturePtr pTexture = pTextureManager->AsyncGetTextureFromFile(aTextureFiles[i].meLocation, aTextureFiles[i].mstrFile, Core::CImage::Format::k_default, aTextureFiles[i].mbMipMapped);
                        pMaterial->AddTexture(pTexture);
                        pTexture->WaitTilLoaded();
                    }
                }
                
                ICubemapManager* pCubemapManager = GET_RESOURCE_MANAGER(ICubemapManager);
                if(pCubemapManager != NULL)
                {
                    for(u32 i=0; i<aCubemapFiles.size(); ++i)
                    {
                        CubemapPtr pCubemap = pCubemapManager->AsyncGetCubemapFromFile(aCubemapFiles[i].meLocation, aCubemapFiles[i].mstrFile, Core::CImage::Format::k_default, aCubemapFiles[i].mbMipMapped);
                        pMaterial->SetCubemap(pCubemap);
                        pCubemap->WaitTilLoaded();
                    }
                }
                
                pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
                
                Core::CTaskScheduler::ScheduleMainThreadTask(Core::Task1<Core::ResourcePtr&>(this, &CMaterialLoader::SetLoadedTask, outpResource));
			}
		}
		//----------------------------------------------------------------------------
		/// Set Loaded Task
		//----------------------------------------------------------------------------
		void CMaterialLoader::SetLoadedTask(Core::ResourcePtr& outpResource)
		{
			outpResource->SetLoaded(true);
		}
		//----------------------------------------------------------------------------
		/// Build Material From File
		//----------------------------------------------------------------------------
		bool CMaterialLoader::BuildMaterialFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath,
                                                    DYNAMIC_ARRAY<std::pair<ShaderPass, std::pair<Core::StorageLocation, std::string> > >& outaShaderFiles,
                                                    DYNAMIC_ARRAY<TextureDesc>& outaTextureFiles,
                                                    DYNAMIC_ARRAY<TextureDesc>& outaCubemapFiles,
                                                    Core::ResourcePtr& outpResource)
		{
            const u32 kudwNumShaderNodes = 3;
            const std::pair<std::string, ShaderPass> kaShaderNodes[kudwNumShaderNodes] =
            {
                std::make_pair("AmbientLightPass", ShaderPass::k_ambient),
                std::make_pair("DirectionalLightPass", ShaderPass::k_directional),
                std::make_pair("PointLightPass", ShaderPass::k_point)
            };
            
			CMaterial* pMaterial = (CMaterial*)(outpResource.get());
			
			//Load the XML file
			TiXmlDocument Document(inFilePath);
			Document.LoadFile(ineStorageLocation);
			
			//Get the root element. This is the material namespace
			TiXmlElement * pRoot = Document.RootElement();
			
			if (pRoot && pRoot->ValueStr() == "Material") 
			{
                std::string strMaterialType = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pRoot, "type", "Static");
                GetShaderFilesForMaterialType(strMaterialType, outaShaderFiles);
                
                //Get the render states - transparency, lighting etc
				TiXmlElement * pRenderStatesEl = Core::XMLUtils::FirstChildElementWithName(pRoot, "RenderStates");
				if(pRenderStatesEl) 
				{
					//---Depth Writing
					TiXmlElement * pDepthWriteStateEl = Core::XMLUtils::FirstChildElementWithName(pRenderStatesEl, "DepthWrite");
					if(pDepthWriteStateEl)
					{
						pMaterial->SetDepthWriteEnabled(Core::XMLUtils::GetAttributeValueOrDefault<bool>(pDepthWriteStateEl, "enabled", true));
					}
					//---Depth Testing
					TiXmlElement * pDepthTestStateEl = Core::XMLUtils::FirstChildElementWithName(pRenderStatesEl, "DepthTest");
					if(pDepthTestStateEl)
					{
						pMaterial->SetDepthTestEnabled(Core::XMLUtils::GetAttributeValueOrDefault<bool>(pDepthTestStateEl, "enabled", true));
					}
					//---Transparency
					TiXmlElement * pTransparentStateEl = Core::XMLUtils::FirstChildElementWithName(pRenderStatesEl, "Transparency");
					if(pTransparentStateEl)
					{
						pMaterial->SetTransparent(Core::XMLUtils::GetAttributeValueOrDefault<bool>(pTransparentStateEl, "enabled", false));
					}
					//---Culling
					TiXmlElement * pCullingStateEl = Core::XMLUtils::FirstChildElementWithName(pRenderStatesEl, "Culling");
					if(pCullingStateEl)
					{
						pMaterial->SetCullingEnabled(Core::XMLUtils::GetAttributeValueOrDefault<bool>(pCullingStateEl, "enabled", true));
					}
				}
				//Get the alpha blend function
				TiXmlElement * pBlendFuncEl = Core::XMLUtils::FirstChildElementWithName(pRoot, "BlendFunc");
				if(pBlendFuncEl)
				{
					const std::string strSrcFunc = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pBlendFuncEl, "src", "One");
					AlphaBlend eSrcFunc = ConvertStringToBlendFunction(strSrcFunc);
					
					const std::string strDstFunc = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pBlendFuncEl, "dst", "One");
					AlphaBlend eDstFunc = ConvertStringToBlendFunction(strDstFunc);
					
					if(eSrcFunc != AlphaBlend::k_unknown && eDstFunc != AlphaBlend::k_unknown)
					{
						pMaterial->SetBlendFunction(eSrcFunc, eDstFunc);
					}
					else
					{
						ERROR_LOG("Material: No blend function exists for the following " + strSrcFunc + ", " + strDstFunc);
					}
				}
                //Get the cull face
				TiXmlElement * pCullFaceEl = Core::XMLUtils::FirstChildElementWithName(pRoot, "Culling");
				if(pCullFaceEl)
				{
                    const std::string& strCullFace = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pCullFaceEl, "face", "Front");
                    CullFace eCullFace = ConvertStringToCullFace(strCullFace);
                    pMaterial->SetCullFace(eCullFace);
				}
                
				//Get the lighting values
				TiXmlElement * pLightingEl = Core::XMLUtils::FirstChildElementWithName(pRoot, "Lighting");
				if(pLightingEl) 
				{
					//---Emissive
					TiXmlElement * pEmissiveEl = Core::XMLUtils::FirstChildElementWithName(pLightingEl, "Emissive");
					if(pEmissiveEl)
					{
						pMaterial->mEmissive = Core::XMLUtils::GetAttributeValueOrDefault<Core::CColour>(pEmissiveEl, "value", Core::CColour::WHITE);
					}
                    //---Ambient Lighting
					TiXmlElement * pAmbientEl = Core::XMLUtils::FirstChildElementWithName(pLightingEl, "Ambient");
					if(pAmbientEl)
					{
						pMaterial->mAmbient = Core::XMLUtils::GetAttributeValueOrDefault<Core::CColour>(pAmbientEl, "value", Core::CColour::WHITE);
					}
					//---Diffuse Lighting
					TiXmlElement * pDiffuseEl = Core::XMLUtils::FirstChildElementWithName(pLightingEl, "Diffuse");
					if(pDiffuseEl)
					{
						pMaterial->mDiffuse = Core::XMLUtils::GetAttributeValueOrDefault<Core::CColour>(pDiffuseEl, "value", Core::CColour::WHITE);
					}
					//---Specular Lighting
					TiXmlElement * pSpecularEl = Core::XMLUtils::FirstChildElementWithName(pLightingEl, "Specular");
					if(pSpecularEl)
					{
						pMaterial->mSpecular = Core::XMLUtils::GetAttributeValueOrDefault<Core::CColour>(pSpecularEl, "value", Core::CColour::WHITE);
					}
					//---Intensity
					TiXmlElement * pShininessEl = Core::XMLUtils::FirstChildElementWithName(pLightingEl, "Shininess");
					if(pShininessEl)
					{
						pMaterial->mSpecular.a = Core::XMLUtils::GetAttributeValueOrDefault<f32>(pShininessEl, "value", 0.0f);
					}
				}
                
                //Get the shaders
				TiXmlElement * pShadersEl = Core::XMLUtils::FirstChildElementWithName(pRoot, "Shaders");
				if(pShadersEl)
				{
                    for(u32 i=0; i<kudwNumShaderNodes; ++i)
                    {
                        //Overwrite any of the default files for this material type with specified custom ones
                        TiXmlElement * pShaderEl = Core::XMLUtils::FirstChildElementWithName(pShadersEl, kaShaderNodes[i].first);
                        if(pShaderEl)
                        {
                            for(u32 udwShaderFilesIndex = 0; udwShaderFilesIndex<outaShaderFiles.size(); ++udwShaderFilesIndex)
                            {
                                if(outaShaderFiles[udwShaderFilesIndex].first == kaShaderNodes[i].second)
                                {
                                    outaShaderFiles[udwShaderFilesIndex].second.first = ChilliSource::Core::CStringConverter::ParseStorageLocation(Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pShaderEl, "location", "Package"));
                                    outaShaderFiles[udwShaderFilesIndex].second.second = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pShaderEl, "file-name", "");
                                    break;
                                }
                            }
                        }
                    }
                    
					//---Get the shader variables
					TiXmlElement* pShaderVarEl = Core::XMLUtils::FirstChildElementWithName(pShadersEl, "Var");
					while(pShaderVarEl)
					{
						//Get the variable type
						std::string strType = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pShaderVarEl, "type", "");
						//Get the variable name
						std::string strName = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pShaderVarEl, "name", "");
						//Add the variable to the material
						if(strType == "Float")
						{
							pMaterial->mMapFloatShaderVars.insert(std::make_pair(strName, Core::XMLUtils::GetAttributeValueOrDefault<f32>(pShaderVarEl, "value", 0.0f)));
						}
						else if(strType == "Vec2")
						{
							pMaterial->mMapVec2ShaderVars.insert(std::make_pair(strName, Core::XMLUtils::GetAttributeValueOrDefault<Core::CVector2>(pShaderVarEl, "value", Core::CVector2::ZERO)));
						}
						else if(strType == "Vec3")
						{
							pMaterial->mMapVec3ShaderVars.insert(std::make_pair(strName, Core::XMLUtils::GetAttributeValueOrDefault<Core::CVector3>(pShaderVarEl, "value", Core::CVector3::ZERO)));
						}
						else if(strType == "Vec4")
						{
							pMaterial->mMapVec4ShaderVars.insert(std::make_pair(strName, Core::XMLUtils::GetAttributeValueOrDefault<Core::CVector4>(pShaderVarEl, "value", Core::CVector4::ZERO)));
						}
						else if(strType == "Colour")
						{
							pMaterial->mMapColShaderVars.insert(std::make_pair(strName, Core::XMLUtils::GetAttributeValueOrDefault<Core::CColour>(pShaderVarEl, "value", Core::CColour::WHITE)));
						}
						else if(strType == "Matrix")
						{
							pMaterial->mMapMat4ShaderVars.insert(std::make_pair(strName, Core::XMLUtils::GetAttributeValueOrDefault<Core::CMatrix4x4>(pShaderVarEl, "value", Core::CMatrix4x4::IDENTITY)));
						}
						else if(strType == "MatrixArray")
						{
							pMaterial->mMapMat4ArrayShaderVars.insert(std::make_pair(strName, DYNAMIC_ARRAY<Core::CMatrix4x4>()));
						}
						//Move on to the next variable
						pShaderVarEl =  Core::XMLUtils::NextSiblingElementWithName(pShaderVarEl);
					}
				}
				//Get the textures
				TiXmlElement * pTextureEl = Core::XMLUtils::FirstChildElementWithName(pRoot, "Textures");
				if(pTextureEl) 
				{
					//---Texture
					TiXmlElement * pTexEl = Core::XMLUtils::FirstChildElementWithName(pTextureEl, "Texture");
					while(pTexEl)
					{
                        TextureDesc desc;
                        desc.meLocation = Core::CStringConverter::ParseStorageLocation(Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pTexEl, "location", "Package"));
                        desc.mstrFile = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pTexEl, "image-name", "");
                        desc.mbMipMapped = Core::XMLUtils::GetAttributeValueOrDefault<bool>(pTexEl, "mipmapped", false);
                        outaTextureFiles.push_back(desc);

						pTexEl =  Core::XMLUtils::NextSiblingElementWithName(pTexEl);
					}
				}
                //Get the Cubemap
				TiXmlElement * pCubemapEl = Core::XMLUtils::FirstChildElementWithName(pRoot, "Cubemap");
				if(pCubemapEl)
				{
                    TextureDesc desc;
                    desc.meLocation = Core::CStringConverter::ParseStorageLocation(Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pCubemapEl, "location", "Package"));
                    desc.mstrFile = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pCubemapEl, "base-name", "");
                    desc.mbMipMapped = Core::XMLUtils::GetAttributeValueOrDefault<bool>(pCubemapEl, "mipmapped", false);
                    outaCubemapFiles.push_back(desc);
				}
				
				return true;
			}
			else
			{
				return false;
			}
		}
        //----------------------------------------------------------------------------
		/// Get Shader Files For Material Type
		//----------------------------------------------------------------------------
        void CMaterialLoader::GetShaderFilesForMaterialType(const std::string& instrType, DYNAMIC_ARRAY<std::pair<ShaderPass, std::pair<Core::StorageLocation, std::string> > >& outaShaderFiles) const
        {
            if(instrType == "Sprite")
            {
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "Core/Sprite")));
                return;
            }
            if(instrType == "Static")
            {
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "Core/Static")));
                return;
            }
            if(instrType == "StaticAmbient")
            {
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "Core/StaticAmbient")));
                return;
            }
            if(instrType == "StaticBlinn")
            {
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "Core/StaticAmbient")));
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_directional, std::make_pair(Core::StorageLocation::k_package, "Core/StaticBlinnDirectional")));
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_point, std::make_pair(Core::StorageLocation::k_package, "Core/StaticBlinnPoint")));
                return;
            }
            if(instrType == "StaticBlinnShadowed")
            {
            	if (mpRenderCapabilities->IsShadowMappingSupported() == true)
            	{
            		outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "Core/StaticAmbient")));
                	outaShaderFiles.push_back(std::make_pair(ShaderPass::k_directional, std::make_pair(Core::StorageLocation::k_package, "Core/StaticBlinnShadowedDirectional")));
                	outaShaderFiles.push_back(std::make_pair(ShaderPass::k_point, std::make_pair(Core::StorageLocation::k_package, "Core/StaticBlinnPoint")));
            	}
            	else
            	{
            		outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "Core/StaticAmbient")));
					outaShaderFiles.push_back(std::make_pair(ShaderPass::k_directional, std::make_pair(Core::StorageLocation::k_package, "Core/StaticBlinnDirectional")));
					outaShaderFiles.push_back(std::make_pair(ShaderPass::k_point, std::make_pair(Core::StorageLocation::k_package, "Core/StaticBlinnPoint")));
            	}
                return;
            }
            if(instrType == "StaticBlinnPerVertex")
            {
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "Core/StaticAmbient")));
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_directional, std::make_pair(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexDirectional")));
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_point, std::make_pair(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexPoint")));
                return;
            }
            if(instrType == "StaticBlinnPerVertexShadowed")
            {
            	if (mpRenderCapabilities->IsShadowMappingSupported() == true)
            	{
            		outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "Core/StaticAmbient")));
                	outaShaderFiles.push_back(std::make_pair(ShaderPass::k_directional, std::make_pair(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexShadowedDirectional")));
                	outaShaderFiles.push_back(std::make_pair(ShaderPass::k_point, std::make_pair(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexPoint")));
            	}
            	else
            	{
            		outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "Core/StaticAmbient")));
					outaShaderFiles.push_back(std::make_pair(ShaderPass::k_directional, std::make_pair(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexDirectional")));
					outaShaderFiles.push_back(std::make_pair(ShaderPass::k_point, std::make_pair(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexPoint")));
            	}
                return;
            }
            if(instrType == "Animated")
            {
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "Core/Animated")));
                return;
            }
            if(instrType == "AnimatedAmbient")
            {
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedAmbient")));
                return;
            }
            if(instrType == "AnimatedBlinn")
            {
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedAmbient")));
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_directional, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedBlinnDirectional")));
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_point, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedBlinnPoint")));
                return;
            }
            if(instrType == "AnimatedBlinnShadowed")
            {
            	if (mpRenderCapabilities->IsShadowMappingSupported() == true)
            	{
            		outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedAmbient")));
            		outaShaderFiles.push_back(std::make_pair(ShaderPass::k_directional, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedBlinnShadowedDirectional")));
            		outaShaderFiles.push_back(std::make_pair(ShaderPass::k_point, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedBlinnPoint")));
            	}
            	else
            	{
            		outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedAmbient")));
					outaShaderFiles.push_back(std::make_pair(ShaderPass::k_directional, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedBlinnDirectional")));
					outaShaderFiles.push_back(std::make_pair(ShaderPass::k_point, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedBlinnPoint")));
            	}
                return;
            }
            if(instrType == "AnimatedBlinnPerVertex")
            {
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedAmbient")));
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_directional, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexDirectional")));
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_point, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexPoint")));
                return;
            }
            if(instrType == "AnimatedBlinnPerVertexShadowed")
            {
            	if (mpRenderCapabilities->IsShadowMappingSupported() == true)
            	{
            		outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedAmbient")));
                	outaShaderFiles.push_back(std::make_pair(ShaderPass::k_directional, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexShadowedDirectional")));
                	outaShaderFiles.push_back(std::make_pair(ShaderPass::k_point, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexPoint")));
            	}
            	else
            	{
            		outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedAmbient")));
					outaShaderFiles.push_back(std::make_pair(ShaderPass::k_directional, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexDirectional")));
					outaShaderFiles.push_back(std::make_pair(ShaderPass::k_point, std::make_pair(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexPoint")));
            	}
                return;
            }
            if(instrType == "Custom")
            {
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_ambient, std::make_pair(Core::StorageLocation::k_package, "")));
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_directional, std::make_pair(Core::StorageLocation::k_package, "")));
                outaShaderFiles.push_back(std::make_pair(ShaderPass::k_point, std::make_pair(Core::StorageLocation::k_package, "")));
            }
        }
		//----------------------------------------------------------------------------
		/// Convert String To Blend Function
		//----------------------------------------------------------------------------
		AlphaBlend CMaterialLoader::ConvertStringToBlendFunction(const std::string &instrFunc)
		{
			if(instrFunc == "Zero")					return AlphaBlend::k_zero;
			if(instrFunc == "One")					return AlphaBlend::k_one;
			if(instrFunc == "SourceColour")			return AlphaBlend::k_sourceCol;
			if(instrFunc == "OneMinusSourceColour")	return AlphaBlend::k_oneMinusSourceCol;
			if(instrFunc == "SourceAlpha")			return AlphaBlend::k_sourceAlpha;
			if(instrFunc == "OneMinusSourceAlpha")	return AlphaBlend::k_oneMinusSourceAlpha;
			if(instrFunc == "DestAlpha")			return AlphaBlend::k_destAlpha;
			if(instrFunc == "OneMinusDestAlpha")	return AlphaBlend::k_oneMinusDestAlpha;
			
			//No blend function found
			return AlphaBlend::k_unknown;
		}
        //----------------------------------------------------------------------------
		/// Convert String To Cull Face
		//----------------------------------------------------------------------------
		CullFace CMaterialLoader::ConvertStringToCullFace(const std::string &instrFace)
		{
			if(instrFace == "Front")    return CullFace::k_front;
			if(instrFace == "Back")		return CullFace::k_back;

			return CullFace::k_front;
		}
	}
}
