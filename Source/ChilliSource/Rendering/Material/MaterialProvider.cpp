//
//  MaterialProvider.cpp
//  Chilli Source
//
//  Created by Scott Downie on 22/11/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Material/MaterialProvider.h>

#include <ChilliSource/Core/Base/Application.h>

#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/XML/XMLUtils.h>
#include <ChilliSource/Rendering/Shader/ShaderManager.h>
#include <ChilliSource/Rendering/Shader/Shader.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/CubemapManager.h>
#include <ChilliSource/Rendering/Texture/Cubemap.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>


namespace ChilliSource
{
	namespace Rendering
	{
		namespace
		{
			const std::string k_materialExtension("momaterial");
            
            //----------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param String describing function
            ///
            /// @return Blend type
            //----------------------------------------------------------------------------
            AlphaBlend ConvertStringToBlendFunction(const std::string& in_blendFunc)
            {
                if(in_blendFunc == "Zero") return AlphaBlend::k_zero;
                if(in_blendFunc == "One") return AlphaBlend::k_one;
                if(in_blendFunc == "SourceColour") return AlphaBlend::k_sourceCol;
                if(in_blendFunc == "OneMinusSourceColour") return AlphaBlend::k_oneMinusSourceCol;
                if(in_blendFunc == "SourceAlpha") return AlphaBlend::k_sourceAlpha;
                if(in_blendFunc == "OneMinusSourceAlpha") return AlphaBlend::k_oneMinusSourceAlpha;
                if(in_blendFunc == "DestAlpha")	return AlphaBlend::k_destAlpha;
                if(in_blendFunc == "OneMinusDestAlpha") return AlphaBlend::k_oneMinusDestAlpha;
                
                //No blend function found
                return AlphaBlend::k_unknown;
            }
            //----------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param String describing cull function
            ///
            /// @return Cull face type
            //----------------------------------------------------------------------------
            CullFace ConvertStringToCullFace(const std::string& in_cullFace)
            {
                if(in_cullFace == "Front") return CullFace::k_front;
                if(in_cullFace == "Back") return CullFace::k_back;
                
                return CullFace::k_front;
            }
            //----------------------------------------------------------------------------
            /// Each material type has associated shaders. This function will build a
            /// list of the paths to the shaders that are required by the material type
            /// for each pass
            ///
            /// @author S Downie
            ///
            /// @param Material type
            /// @param Render capabilities
            /// @param [Out] List of shader descriptions with pass and location
            //----------------------------------------------------------------------------
            void GetShaderFilesForMaterialType(const std::string& in_materialType, RenderCapabilities* in_renderCapabilities, std::vector<MaterialProvider::ShaderDesc>& out_shaders)
            {
                if(in_materialType == "Sprite")
                {
                    out_shaders.push_back({"Core/Sprite", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                    return;
                }
                if(in_materialType == "Static")
                {
                    out_shaders.push_back({"Core/Static", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                    return;
                }
                if(in_materialType == "StaticAmbient")
                {
                    out_shaders.push_back({"Core/StaticAmbient", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                    return;
                }
                if(in_materialType == "StaticBlinn")
                {
                    out_shaders.push_back({"Core/StaticAmbient", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                    out_shaders.push_back({"Core/StaticBlinnDirectional", Core::StorageLocation::k_package, ShaderPass::k_directional});
                    out_shaders.push_back({"Core/StaticBlinnPoint", Core::StorageLocation::k_package, ShaderPass::k_point});
                    return;
                }
                if(in_materialType == "StaticBlinnShadowed")
                {
                    if (in_renderCapabilities->IsShadowMappingSupported() == true)
                    {
                        out_shaders.push_back({"Core/StaticAmbient", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                        out_shaders.push_back({"Core/StaticBlinnShadowedDirectional", Core::StorageLocation::k_package, ShaderPass::k_directional});
                        out_shaders.push_back({"Core/StaticBlinnPoint", Core::StorageLocation::k_package, ShaderPass::k_point});
                    }
                    else
                    {
                        out_shaders.push_back({"Core/StaticAmbient", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                        out_shaders.push_back({"Core/StaticBlinnDirectional", Core::StorageLocation::k_package, ShaderPass::k_directional});
                        out_shaders.push_back({"Core/StaticBlinnPoint", Core::StorageLocation::k_package, ShaderPass::k_point});
                    }
                    return;
                }
                if(in_materialType == "StaticBlinnPerVertex")
                {
                    out_shaders.push_back({"Core/StaticAmbient", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                    out_shaders.push_back({"Core/StaticBlinnPerVertexDirectional", Core::StorageLocation::k_package, ShaderPass::k_directional});
                    out_shaders.push_back({"Core/StaticBlinnPerVertexPoint", Core::StorageLocation::k_package, ShaderPass::k_point});
                    return;
                }
                if(in_materialType == "StaticBlinnPerVertexShadowed")
                {
                    if (in_renderCapabilities->IsShadowMappingSupported() == true)
                    {
                        out_shaders.push_back({"Core/StaticAmbient", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                        out_shaders.push_back({"Core/StaticBlinnPerVertexShadowedDirectional", Core::StorageLocation::k_package, ShaderPass::k_directional});
                        out_shaders.push_back({"Core/StaticBlinnPerVertexPoint", Core::StorageLocation::k_package, ShaderPass::k_point});
                    }
                    else
                    {
                        out_shaders.push_back({"Core/StaticAmbient", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                        out_shaders.push_back({"Core/StaticBlinnPerVertexDirectional", Core::StorageLocation::k_package, ShaderPass::k_directional});
                        out_shaders.push_back({"Core/StaticBlinnPerVertexPoint", Core::StorageLocation::k_package, ShaderPass::k_point});
                    }
                    return;
                }
                if(in_materialType == "Animated")
                {
                    out_shaders.push_back({"Core/Animated", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                    return;
                }
                if(in_materialType == "AnimatedAmbient")
                {
                    out_shaders.push_back({"Core/AnimatedAmbient", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                    return;
                }
                if(in_materialType == "AnimatedBlinn")
                {
                    out_shaders.push_back({"Core/AnimatedAmbient", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                    out_shaders.push_back({"Core/AnimatedBlinnDirectional", Core::StorageLocation::k_package, ShaderPass::k_directional});
                    out_shaders.push_back({"Core/AnimatedBlinnPoint", Core::StorageLocation::k_package, ShaderPass::k_point});
                    return;
                }
                if(in_materialType == "AnimatedBlinnShadowed")
                {
                    if (in_renderCapabilities->IsShadowMappingSupported() == true)
                    {
                        out_shaders.push_back({"Core/AnimatedAmbient", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                        out_shaders.push_back({"Core/AnimatedBlinnShadowedDirectional", Core::StorageLocation::k_package, ShaderPass::k_directional});
                        out_shaders.push_back({"Core/AnimatedBlinnPoint", Core::StorageLocation::k_package, ShaderPass::k_point});
                    }
                    else
                    {
                        out_shaders.push_back({"Core/AnimatedAmbient", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                        out_shaders.push_back({"Core/AnimatedBlinnDirectional", Core::StorageLocation::k_package, ShaderPass::k_directional});
                        out_shaders.push_back({"Core/AnimatedBlinnPoint", Core::StorageLocation::k_package, ShaderPass::k_point});
                    }
                    return;
                }
                if(in_materialType == "AnimatedBlinnPerVertex")
                {
                    out_shaders.push_back({"Core/AnimatedAmbient", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                    out_shaders.push_back({"Core/AnimatedBlinnPerVertexDirectional", Core::StorageLocation::k_package, ShaderPass::k_directional});
                    out_shaders.push_back({"Core/AnimatedBlinnPerVertexPoint", Core::StorageLocation::k_package, ShaderPass::k_point});
                    return;
                }
                if(in_materialType == "AnimatedBlinnPerVertexShadowed")
                {
                    if (in_renderCapabilities->IsShadowMappingSupported() == true)
                    {
                        out_shaders.push_back({"Core/AnimatedAmbient", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                        out_shaders.push_back({"Core/AnimatedBlinnPerVertexShadowedDirectional", Core::StorageLocation::k_package, ShaderPass::k_directional});
                        out_shaders.push_back({"Core/AnimatedBlinnPerVertexPoint", Core::StorageLocation::k_package, ShaderPass::k_point});
                    }
                    else
                    {
                        out_shaders.push_back({"Core/AnimatedAmbient", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                        out_shaders.push_back({"Core/AnimatedBlinnPerVertexDirectional", Core::StorageLocation::k_package, ShaderPass::k_directional});
                        out_shaders.push_back({"Core/AnimatedBlinnPerVertexPoint", Core::StorageLocation::k_package, ShaderPass::k_point});
                    }
                    return;
                }
                if(in_materialType == "Custom")
                {
                    out_shaders.push_back({"", Core::StorageLocation::k_package, ShaderPass::k_ambient});
                    out_shaders.push_back({"", Core::StorageLocation::k_package, ShaderPass::k_directional});
                    out_shaders.push_back({"", Core::StorageLocation::k_package, ShaderPass::k_point});
                    return;
                }
                
                CS_LOG_FATAL("Invalid material type: " + in_materialType);
            }
            //----------------------------------------------------------------------------
            /// Parse the render states from the XML element to the material
            ///
            /// @author S Downie
            ///
            /// @param Root element
            /// @param [Out] Material to populate
            //----------------------------------------------------------------------------
            void ParseRenderStates(TiXmlElement* in_rootElement, Material* out_material)
            {
                TiXmlElement* renderStatesEl = Core::XMLUtils::FirstChildElementWithName(in_rootElement, "RenderStates");
                if(renderStatesEl)
                {
                    //---Depth Writing
                    TiXmlElement* depthWriteStateEl = Core::XMLUtils::FirstChildElementWithName(renderStatesEl, "DepthWrite");
                    if(depthWriteStateEl)
                    {
                        out_material->SetDepthWriteEnabled(Core::XMLUtils::GetAttributeValueOrDefault<bool>(depthWriteStateEl, "enabled", true));
                    }
                    //---Depth Testing
                    TiXmlElement* depthTestStateEl = Core::XMLUtils::FirstChildElementWithName(renderStatesEl, "DepthTest");
                    if(depthTestStateEl)
                    {
                        out_material->SetDepthTestEnabled(Core::XMLUtils::GetAttributeValueOrDefault<bool>(depthTestStateEl, "enabled", true));
                    }
                    //---Transparency
                    TiXmlElement* transparentStateEl = Core::XMLUtils::FirstChildElementWithName(renderStatesEl, "Transparency");
                    if(transparentStateEl)
                    {
                        out_material->SetTransparent(Core::XMLUtils::GetAttributeValueOrDefault<bool>(transparentStateEl, "enabled", false));
                    }
                    //---Culling
                    TiXmlElement* cullingStateEl = Core::XMLUtils::FirstChildElementWithName(renderStatesEl, "Culling");
                    if(cullingStateEl)
                    {
                        out_material->SetCullingEnabled(Core::XMLUtils::GetAttributeValueOrDefault<bool>(cullingStateEl, "enabled", true));
                    }
                }
            }
            //----------------------------------------------------------------------------
            /// Parse the alpha blend function from the XML element to the material
            ///
            /// @author S Downie
            ///
            /// @param Root element
            /// @param [Out] Material to populate
            //----------------------------------------------------------------------------
            void ParseAlphaBlendFunction(TiXmlElement* in_rootElement, Material* out_material)
            {
                TiXmlElement* blendFuncEl = Core::XMLUtils::FirstChildElementWithName(in_rootElement, "BlendFunc");
                if(blendFuncEl)
                {
                    const std::string srcFuncString = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(blendFuncEl, "src", "One");
                    AlphaBlend srcFunc = ConvertStringToBlendFunction(srcFuncString);
                    
                    const std::string dstFuncString = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(blendFuncEl, "dst", "One");
                    AlphaBlend dstFunc = ConvertStringToBlendFunction(dstFuncString);
                    
                    CS_ASSERT(srcFunc != AlphaBlend::k_unknown && dstFunc != AlphaBlend::k_unknown, "Unknown blend function in material");
       
                    out_material->SetBlendFunction(srcFunc, dstFunc);
                }
            }
            //----------------------------------------------------------------------------
            /// Parse the cull function from the XML element to the material
            ///
            /// @author S Downie
            ///
            /// @param Root element
            /// @param [Out] Material to populate
            //----------------------------------------------------------------------------
            void ParseCullFunction(TiXmlElement* in_rootElement, Material* out_material)
            {
                TiXmlElement* cullFaceEl = Core::XMLUtils::FirstChildElementWithName(in_rootElement, "Culling");
                if(cullFaceEl)
                {
                    const std::string& cullFaceString = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(cullFaceEl, "face", "Front");
                    CullFace cullFace = ConvertStringToCullFace(cullFaceString);
                    out_material->SetCullFace(cullFace);
                }
            }
            //----------------------------------------------------------------------------
            /// Parse the surface date such as shininess etc from the XML element
            /// to the material
            ///
            /// @author S Downie
            ///
            /// @param Root element
            /// @param [Out] Material to populate
            //----------------------------------------------------------------------------
            void ParseSurface(TiXmlElement* in_rootElement, Material* out_material)
            {
                TiXmlElement* lightingEl = Core::XMLUtils::FirstChildElementWithName(in_rootElement, "Lighting");
                if(lightingEl)
                {
                    //---Emissive
                    TiXmlElement* emissiveEl = Core::XMLUtils::FirstChildElementWithName(lightingEl, "Emissive");
                    if(emissiveEl)
                    {
                        out_material->SetEmissive(Core::XMLUtils::GetAttributeValueOrDefault<Core::Colour>(emissiveEl, "value", Core::Colour::k_white));
                    }
                    //---Ambient Lighting
                    TiXmlElement* ambientEl = Core::XMLUtils::FirstChildElementWithName(lightingEl, "Ambient");
                    if(ambientEl)
                    {
                        out_material->SetAmbient(Core::XMLUtils::GetAttributeValueOrDefault<Core::Colour>(ambientEl, "value", Core::Colour::k_white));
                    }
                    //---Diffuse Lighting
                    TiXmlElement* diffuseEl = Core::XMLUtils::FirstChildElementWithName(lightingEl, "Diffuse");
                    if(diffuseEl)
                    {
                        out_material->SetDiffuse(Core::XMLUtils::GetAttributeValueOrDefault<Core::Colour>(diffuseEl, "value", Core::Colour::k_white));
                    }
                    //---Specular Lighting
                    Core::Colour specular(1.0f, 1.0f, 1.0f, 0.0f);
                    TiXmlElement* specularEl = Core::XMLUtils::FirstChildElementWithName(lightingEl, "Specular");
                    if(specularEl)
                    {
                        specular = Core::XMLUtils::GetAttributeValueOrDefault<Core::Colour>(specularEl, "value", Core::Colour::k_white);
                    }
                    
                    //---Intensity
                    TiXmlElement* shininessEl = Core::XMLUtils::FirstChildElementWithName(lightingEl, "Shininess");
                    if(shininessEl)
                    {
                        specular.a = Core::XMLUtils::GetAttributeValueOrDefault<f32>(shininessEl, "value", 0.0f);
                    }
                    
                    out_material->SetSpecular(specular);
                }
            }
            //----------------------------------------------------------------------------
            /// Parse the shader file paths
            ///
            /// @author S Downie
            ///
            /// @param Root element
            /// @param [Out] Shader files to populate
            /// @param [Out] Material to populate
            //----------------------------------------------------------------------------
            void ParseShaders(TiXmlElement* in_rootElement, std::vector<MaterialProvider::ShaderDesc>& out_shaderFiles, Material* out_material)
            {
                const u32 numShaderNodes = 3;
                const std::pair<std::string, ShaderPass> shaderNodes[numShaderNodes] =
                {
                    std::make_pair("AmbientLightPass", ShaderPass::k_ambient),
                    std::make_pair("DirectionalLightPass", ShaderPass::k_directional),
                    std::make_pair("PointLightPass", ShaderPass::k_point)
                };
                
                TiXmlElement* shadersEl = Core::XMLUtils::FirstChildElementWithName(in_rootElement, "Shaders");
                if(shadersEl)
                {
                    for(u32 i=0; i<numShaderNodes; ++i)
                    {
                        //Overwrite any of the default files for this material type with specified custom ones
                        TiXmlElement* shaderEl = Core::XMLUtils::FirstChildElementWithName(shadersEl, shaderNodes[i].first);
                        if(shaderEl)
                        {
                            for(u32 udwShaderFilesIndex = 0; udwShaderFilesIndex<out_shaderFiles.size(); ++udwShaderFilesIndex)
                            {
                                if(out_shaderFiles[udwShaderFilesIndex].m_pass == shaderNodes[i].second)
                                {
                                    out_shaderFiles[udwShaderFilesIndex].m_location = ChilliSource::Core::ParseStorageLocation(Core::XMLUtils::GetAttributeValueOrDefault<std::string>(shaderEl, "location", "Package"));
                                    out_shaderFiles[udwShaderFilesIndex].m_relativePath = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(shaderEl, "file-name", "");
                                    break;
                                }
                            }
                        }
                    }
                    
                    //---Get the shader variables
                    TiXmlElement* shaderVarEl = Core::XMLUtils::FirstChildElementWithName(shadersEl, "Var");
                    while(shaderVarEl)
                    {
                        //Get the variable type
                        std::string strType = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(shaderVarEl, "type", "");
                        //Get the variable name
                        std::string strName = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(shaderVarEl, "name", "");
                        //Add the variable to the material
                        if(strType == "Float")
                        {
                            out_material->mMapFloatShaderVars.insert(std::make_pair(strName, Core::XMLUtils::GetAttributeValueOrDefault<f32>(shaderVarEl, "value", 0.0f)));
                        }
                        else if(strType == "Vec2")
                        {
                            out_material->mMapVec2ShaderVars.insert(std::make_pair(strName, Core::XMLUtils::GetAttributeValueOrDefault<Core::Vector2>(shaderVarEl, "value", Core::Vector2::ZERO)));
                        }
                        else if(strType == "Vec3")
                        {
                            out_material->mMapVec3ShaderVars.insert(std::make_pair(strName, Core::XMLUtils::GetAttributeValueOrDefault<Core::Vector3>(shaderVarEl, "value", Core::Vector3::ZERO)));
                        }
                        else if(strType == "Vec4")
                        {
                            out_material->mMapVec4ShaderVars.insert(std::make_pair(strName, Core::XMLUtils::GetAttributeValueOrDefault<Core::Vector4>(shaderVarEl, "value", Core::Vector4::ZERO)));
                        }
                        else if(strType == "Colour")
                        {
                            out_material->mMapColShaderVars.insert(std::make_pair(strName, Core::XMLUtils::GetAttributeValueOrDefault<Core::Colour>(shaderVarEl, "value", Core::Colour::k_white)));
                        }
                        else if(strType == "Matrix")
                        {
                            out_material->mMapMat4ShaderVars.insert(std::make_pair(strName, Core::XMLUtils::GetAttributeValueOrDefault<Core::Matrix4x4>(shaderVarEl, "value", Core::Matrix4x4::IDENTITY)));
                        }
                        else if(strType == "MatrixArray")
                        {
                            out_material->mMapMat4ArrayShaderVars.insert(std::make_pair(strName, std::vector<Core::Matrix4x4>()));
                        }
                        //Move on to the next variable
                        shaderVarEl =  Core::XMLUtils::NextSiblingElementWithName(shaderVarEl);
                    }
                }
            }
            //----------------------------------------------------------------------------
            /// Parse the texture file paths
            ///
            /// @author S Downie
            ///
            /// @param Root element
            /// @param [Out] Texture files to populate
            //----------------------------------------------------------------------------
            void ParseTextures(TiXmlElement* in_rootElement, std::vector<MaterialProvider::TextureDesc>& out_textureFiles)
            {
                TiXmlElement* texturesEl = Core::XMLUtils::FirstChildElementWithName(in_rootElement, "Textures");
                if(texturesEl)
                {
                    //---Texture
                    TiXmlElement* textureEl = Core::XMLUtils::FirstChildElementWithName(texturesEl, "Texture");
                    while(textureEl)
                    {
                        MaterialProvider::TextureDesc desc;
                        desc.m_location = Core::ParseStorageLocation(Core::XMLUtils::GetAttributeValueOrDefault<std::string>(textureEl, "location", "Package"));
                        desc.m_filePath = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(textureEl, "image-name", "");
                        desc.m_shouldMipMap = Core::XMLUtils::GetAttributeValueOrDefault<bool>(textureEl, "mipmapped", false);
                        out_textureFiles.push_back(desc);
                        
                        textureEl =  Core::XMLUtils::NextSiblingElementWithName(textureEl);
                    }
                }
            }
            //----------------------------------------------------------------------------
            /// Parse the cubemap file paths
            ///
            /// @author S Downie
            ///
            /// @param Root element
            /// @param [Out] Cubemap files to populate
            //----------------------------------------------------------------------------
            void ParseCubemaps(TiXmlElement* in_rootElement, std::vector<MaterialProvider::TextureDesc>& out_cubemapFiles)
            {
                TiXmlElement* cubemapEl = Core::XMLUtils::FirstChildElementWithName(in_rootElement, "Cubemap");
                if(cubemapEl)
                {
                    MaterialProvider::TextureDesc desc;
                    desc.m_location = Core::ParseStorageLocation(Core::XMLUtils::GetAttributeValueOrDefault<std::string>(cubemapEl, "location", "Package"));
                    desc.m_filePath = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(cubemapEl, "base-name", "");
                    desc.m_shouldMipMap = Core::XMLUtils::GetAttributeValueOrDefault<bool>(cubemapEl, "mipmapped", false);
                    out_cubemapFiles.push_back(desc);
                }
            }
		}
        
        CS_DEFINE_NAMEDTYPE(MaterialProvider);
        
        //-------------------------------------------------------------------------
        //-------------------------------------------------------------------------
        MaterialProviderUPtr MaterialProvider::Create(RenderCapabilities* in_renderCapabilities)
        {
            return MaterialProviderUPtr(new MaterialProvider(in_renderCapabilities));
        }
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
		MaterialProvider::MaterialProvider(RenderCapabilities* in_renderCapabilities)
        : m_renderCapabilities(in_renderCapabilities)
		{
			CS_ASSERT(m_renderCapabilities, "Material loader is missing required system: Render Capabilities.");
		}
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
		bool MaterialProvider::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == MaterialProvider::InterfaceID;
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool MaterialProvider::CanCreateResourceOfKind(Core::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == Material::InterfaceID);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool MaterialProvider::CanCreateResourceFromFileWithExtension(const std::string& in_extension) const
		{
			return (in_extension == k_materialExtension);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool MaterialProvider::CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceSPtr& out_resource)
		{
            out_resource->SetLoaded(false);
            
            std::vector<ShaderDesc> shaderFiles;
            std::vector<TextureDesc> textureFiles;
            std::vector<TextureDesc> cubemapFiles;
            
            if(BuildMaterialFromFile(in_location, in_filePath, shaderFiles, textureFiles, cubemapFiles, out_resource) == false)
            {
                return false;
            }
            
            MaterialSPtr material = std::static_pointer_cast<Material>(out_resource);
            
            ShaderManager* pShaderManager = GET_RESOURCE_MANAGER(ShaderManager);
            CS_ASSERT(pShaderManager != nullptr, "Shader manager must be created");
            for(u32 i=0; i<shaderFiles.size(); ++i)
            {
                if(shaderFiles[i].m_relativePath.empty() == false)
                {
                    ShaderSPtr shader = pShaderManager->GetShaderFromFile(shaderFiles[i].m_location, shaderFiles[i].m_relativePath);
                    CS_ASSERT(shader != nullptr, "Material cannot load shader with path: " + shaderFiles[i].m_relativePath);
                    
                    material->SetShaderProgram(shaderFiles[i].m_pass, shader);
                }
            }
            
            TextureManager* textureManager = GET_RESOURCE_MANAGER(TextureManager);
            CS_ASSERT(textureManager != nullptr, "Texture manager must be created");
            for(u32 i=0; i<textureFiles.size(); ++i)
            {
                TextureSPtr texture = std::static_pointer_cast<Texture>(textureManager->GetResourceFromFile(textureFiles[i].m_location, textureFiles[i].m_filePath));
                CS_ASSERT(texture != nullptr, "Material cannot load texture with path: " + textureFiles[i].m_filePath);
                
                material->AddTexture(texture);
                if(textureFiles[i].m_shouldMipMap == true)
                {
                    //TODO: Generate mipmaps
                }
            }
            
            CubemapManager* pCubemapManager = GET_RESOURCE_MANAGER(CubemapManager);
            CS_ASSERT(pCubemapManager != nullptr, "Cubemap manager must be created");
            CS_ASSERT(cubemapFiles.size() <= 1, "Currently only 1 cubemap is supported");
            for(u32 i=0; i<cubemapFiles.size(); ++i)
            {
                CubemapSPtr cubemap = std::static_pointer_cast<Cubemap>(pCubemapManager->GetResourceFromFile(cubemapFiles[i].m_location, cubemapFiles[i].m_filePath));
                CS_ASSERT(cubemap != nullptr, "Material cannot load cubemap with path: " + cubemapFiles[i].m_filePath);
                
                material->SetCubemap(cubemap);
                if(cubemapFiles[i].m_shouldMipMap == true)
                {
                    //TODO: Generate mipmaps
                }
            }
            
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            
            material->SetLoaded(true);
                
            return true;
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool MaterialProvider::AsyncCreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceSPtr& out_resource)
		{
            out_resource->SetLoaded(false);
            
			//Start the material building task.
			Core::Task<Core::StorageLocation, const std::string&, Core::ResourceSPtr&> BuildMaterialTask(this, &MaterialProvider::BuildMaterialTask, in_location, in_filePath, out_resource);
			Core::TaskScheduler::ScheduleTask(BuildMaterialTask);
			
			return true;
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		void MaterialProvider::BuildMaterialTask(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceSPtr& out_resource)
		{
            //TODO: This can get into a deadlock situation if we block and wait on the texture etc. This will need to be changed
            //once we have callbacks
            
            
			//build the material
            std::vector<ShaderDesc> shaderFiles;
            std::vector<TextureDesc> textureFiles;
            std::vector<TextureDesc> cubemapFiles;
			if(BuildMaterialFromFile(in_location, in_filePath, shaderFiles, textureFiles, cubemapFiles, out_resource) == false)
            {
                return;
            }
            
            MaterialSPtr material = std::static_pointer_cast<Material>(out_resource);
            
            ShaderManager* pShaderManager = GET_RESOURCE_MANAGER(ShaderManager);
            CS_ASSERT(pShaderManager != nullptr, "Shader manager must be created");
            for(u32 i=0; i<shaderFiles.size(); ++i)
            {
                ShaderSPtr shader = pShaderManager->AsyncGetShaderFromFile(shaderFiles[i].m_location, shaderFiles[i].m_relativePath);
                CS_ASSERT(shader != nullptr, "Material cannot load shader with path: " + shaderFiles[i].m_relativePath);
                
                material->SetShaderProgram(shaderFiles[i].m_pass, shader);
                shader->WaitTilLoaded();
            }
            
            TextureManager* textureManager = GET_RESOURCE_MANAGER(TextureManager);
            CS_ASSERT(textureManager != nullptr, "Texture manager must be created");
            for(u32 i=0; i<textureFiles.size(); ++i)
            {
                TextureSPtr texture = std::static_pointer_cast<Texture>(textureManager->AsyncGetResourceFromFile(textureFiles[i].m_location, textureFiles[i].m_filePath));
                CS_ASSERT(texture != nullptr, "Material cannot load texture with path: " + textureFiles[i].m_filePath);
                
                if(textureFiles[i].m_shouldMipMap == true)
                {
                    //TODO: Generate mipmaps
                }
                
                material->AddTexture(texture);
                texture->WaitTilLoaded();
            }
            
            CubemapManager* cubemapManager = GET_RESOURCE_MANAGER(CubemapManager);
            CS_ASSERT(cubemapManager != nullptr, "Cubemap manager must be created");
            CS_ASSERT(cubemapFiles.size() <= 1, "Currently only 1 cubemap is supported");
            for(u32 i=0; i<cubemapFiles.size(); ++i)
            {
                CubemapSPtr cubemap = std::static_pointer_cast<Cubemap>(cubemapManager->AsyncGetResourceFromFile(cubemapFiles[i].m_location, cubemapFiles[i].m_filePath));
                CS_ASSERT(cubemap != nullptr, "Material cannot load cubemap with path: " + cubemapFiles[i].m_filePath);
                
                if(cubemapFiles[i].m_shouldMipMap == true)
                {
                    //TODO: Generate mipmaps
                }
                
                material->SetCubemap(cubemap);
                cubemap->WaitTilLoaded();
            }
            
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            
            out_resource->SetLoaded(true);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool MaterialProvider::BuildMaterialFromFile(Core::StorageLocation in_location, const std::string& in_filePath,
                                                    std::vector<ShaderDesc>& out_shaderFiles,
                                                    std::vector<TextureDesc>& out_textureFiles,
                                                    std::vector<TextureDesc>& out_cubemapFiles,
                                                    Core::ResourceSPtr& out_resource)
		{
			Material* material = (Material*)(out_resource.get());
			
			//Load the XML file
			TiXmlDocument document(in_filePath);
			document.LoadFile(in_location);
			
			TiXmlElement* rootElement = document.RootElement();
            
            if(rootElement == nullptr || rootElement->ValueStr() != "Material")
            {
                return false;
            }
			
            std::string materialType = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(rootElement, "type", "Static");
            GetShaderFilesForMaterialType(materialType, m_renderCapabilities, out_shaderFiles);

            ParseRenderStates(rootElement, material);
            ParseAlphaBlendFunction(rootElement, material);
            ParseCullFunction(rootElement, material);
            ParseSurface(rootElement, material);
            ParseShaders(rootElement, out_shaderFiles, material);
            ParseTextures(rootElement, out_textureFiles);
            ParseCubemaps(rootElement, out_cubemapFiles);
            
            return true;
		}
	}
}
