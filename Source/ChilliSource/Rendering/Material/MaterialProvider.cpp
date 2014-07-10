//
//  MaterialProvider.cpp
//  Chilli Source
//  Created by Scott Downie on 22/11/2010.
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

#include <ChilliSource/Rendering/Material/MaterialProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/XML/XMLUtils.h>
#include <ChilliSource/Rendering/Base/BlendMode.h>
#include <ChilliSource/Rendering/Base/CullFace.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Shader/Shader.h>
#include <ChilliSource/Rendering/Texture/Cubemap.h>
#include <ChilliSource/Rendering/Texture/CubemapResourceOptions.h>
#include <ChilliSource/Rendering/Texture/TextureResourceOptions.h>

namespace ChilliSource
{
	namespace Rendering
	{
		namespace
		{
			const std::string k_materialExtension("csmaterial");
            
            //----------------------------------------------------------------------------
            /// The resource types supported by materials
            //----------------------------------------------------------------------------
            enum class ResourceType
            {
                k_shader,
                k_texture,
                k_cubemap
            };
            //-------------------------------------------------------------------------
            /// Holds the description of a generic resource load from the supported types
            ///
            /// @author S Downie
            //-------------------------------------------------------------------------
            struct ChainedLoadDesc
            {
                ResourceType m_type;
                std::string m_filePath;
                Core::StorageLocation m_location;
                ShaderPass m_pass;
                bool m_shouldMipMap;
                Texture::FilterMode m_filterMode;
                Texture::WrapMode m_wrapModeU;
                Texture::WrapMode m_wrapModeV;
            };
            //----------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param String describing mode
            ///
            /// @return Blend mode
            //----------------------------------------------------------------------------
            BlendMode ConvertStringToBlendMode(const std::string& in_blendMode)
            {
                if(in_blendMode == "Zero") return BlendMode::k_zero;
                if(in_blendMode == "One") return BlendMode::k_one;
                if(in_blendMode == "SourceColour") return BlendMode::k_sourceCol;
                if(in_blendMode == "OneMinusSourceColour") return BlendMode::k_oneMinusSourceCol;
                if(in_blendMode == "SourceAlpha") return BlendMode::k_sourceAlpha;
                if(in_blendMode == "OneMinusSourceAlpha") return BlendMode::k_oneMinusSourceAlpha;
                if(in_blendMode == "DestAlpha")	return BlendMode::k_destAlpha;
                if(in_blendMode == "OneMinusDestAlpha") return BlendMode::k_oneMinusDestAlpha;
                
                return BlendMode::k_one;
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
            /// @author Ian Copland
            ///
            /// @param A string describing the texture wrap mode.
            ///
            /// @return The wrap mode. If the string was not a valid wrap mode this will
            /// default to clamp.
            //----------------------------------------------------------------------------
            Texture::WrapMode ConvertStringToWrapMode(const std::string& in_wrapModeString)
            {
                std::string lowerWrapModeString = in_wrapModeString;
                CSCore::StringUtils::ToLowerCase(lowerWrapModeString);
                
                if(lowerWrapModeString == "clamp")
                {
                    return Texture::WrapMode::k_clamp;
                }
                if(lowerWrapModeString == "repeat")
                {
                    return Texture::WrapMode::k_repeat;
                }
                
                CS_LOG_ERROR("Invalid WrapMode: " + in_wrapModeString);
                return Texture::WrapMode::k_clamp;
            }
            //----------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param A string describing the texture filter mode.
            ///
            /// @return The filter mode. If the string was not a valid filter mode this will
            /// default to bilinear.
            //----------------------------------------------------------------------------
            Texture::FilterMode ConvertStringToFilterMode(const std::string& in_filterModeString)
            {
                std::string lowerFilterModeString = in_filterModeString;
                CSCore::StringUtils::ToLowerCase(lowerFilterModeString);
                
                if(lowerFilterModeString == "nearestneighbour")
                {
                    return Texture::FilterMode::k_nearestNeighbour;
                }
                if(lowerFilterModeString == "bilinear")
                {
                    return Texture::FilterMode::k_bilinear;
                }
                
                CS_LOG_ERROR("Invalid FilterMode: " + in_filterModeString);
                return Texture::FilterMode::k_bilinear;
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
                    out_shaders.push_back({"Shaders/Sprite.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_ambient});
                    return;
                }
                if(in_materialType == "Static")
                {
                    out_shaders.push_back({"Shaders/Static.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_ambient});
                    return;
                }
                if(in_materialType == "StaticAmbient")
                {
                    out_shaders.push_back({"Shaders/StaticAmbient.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_ambient});
                    return;
                }
                if(in_materialType == "StaticBlinn")
                {
                    out_shaders.push_back({"Shaders/StaticAmbient.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_ambient});
                    out_shaders.push_back({"Shaders/StaticBlinnDirectional.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_directional});
                    out_shaders.push_back({"Shaders/StaticBlinnPoint.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_point});
                    return;
                }
                if(in_materialType == "StaticBlinnShadowed")
                {
                    if (in_renderCapabilities->IsShadowMappingSupported() == true)
                    {
                        out_shaders.push_back({"Shaders/StaticAmbient.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_ambient});
                        out_shaders.push_back({"Shaders/StaticBlinnShadowedDirectional.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_directional});
                        out_shaders.push_back({"Shaders/StaticBlinnPoint.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_point});
                    }
                    else
                    {
                        out_shaders.push_back({"Shaders/StaticAmbient.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_ambient});
                        out_shaders.push_back({"Shaders/StaticBlinnDirectional.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_directional});
                        out_shaders.push_back({"Shaders/StaticBlinnPoint.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_point});
                    }
                    return;
                }
                if(in_materialType == "StaticBlinnPerVertex")
                {
                    out_shaders.push_back({"Shaders/StaticAmbient.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_ambient});
                    out_shaders.push_back({"Shaders/StaticBlinnPerVertexDirectional.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_directional});
                    out_shaders.push_back({"Shaders/StaticBlinnPerVertexPoint.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_point});
                    return;
                }
                if(in_materialType == "StaticBlinnPerVertexShadowed")
                {
                    if (in_renderCapabilities->IsShadowMappingSupported() == true)
                    {
                        out_shaders.push_back({"Shaders/StaticAmbient.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_ambient});
                        out_shaders.push_back({"Shaders/StaticBlinnPerVertexShadowedDirectional.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_directional});
                        out_shaders.push_back({"Shaders/StaticBlinnPerVertexPoint.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_point});
                    }
                    else
                    {
                        out_shaders.push_back({"Shaders/StaticAmbient.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_ambient});
                        out_shaders.push_back({"Shaders/StaticBlinnPerVertexDirectional.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_directional});
                        out_shaders.push_back({"Shaders/StaticBlinnPerVertexPoint.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_point});
                    }
                    return;
                }
                if(in_materialType == "Animated")
                {
                    out_shaders.push_back({"Shaders/Animated.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_ambient});
                    return;
                }
                if(in_materialType == "AnimatedAmbient")
                {
                    out_shaders.push_back({"Shaders/AnimatedAmbient.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_ambient});
                    return;
                }
                if(in_materialType == "AnimatedBlinn")
                {
                    out_shaders.push_back({"Shaders/AnimatedAmbient.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_ambient});
                    out_shaders.push_back({"Shaders/AnimatedBlinnDirectional.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_directional});
                    out_shaders.push_back({"Shaders/AnimatedBlinnPoint.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_point});
                    return;
                }
                if(in_materialType == "AnimatedBlinnShadowed")
                {
                    if (in_renderCapabilities->IsShadowMappingSupported() == true)
                    {
                        out_shaders.push_back({"Shaders/AnimatedAmbient.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_ambient});
                        out_shaders.push_back({"Shaders/AnimatedBlinnShadowedDirectional.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_directional});
                        out_shaders.push_back({"Shaders/AnimatedBlinnPoint.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_point});
                    }
                    else
                    {
                        out_shaders.push_back({"Shaders/AnimatedAmbient.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_ambient});
                        out_shaders.push_back({"Shaders/AnimatedBlinnDirectional.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_directional});
                        out_shaders.push_back({"Shaders/AnimatedBlinnPoint.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_point});
                    }
                    return;
                }
                if(in_materialType == "AnimatedBlinnPerVertex")
                {
                    out_shaders.push_back({"Shaders/AnimatedAmbient.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_ambient});
                    out_shaders.push_back({"Shaders/AnimatedBlinnPerVertexDirectional.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_directional});
                    out_shaders.push_back({"Shaders/AnimatedBlinnPerVertexPoint.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_point});
                    return;
                }
                if(in_materialType == "AnimatedBlinnPerVertexShadowed")
                {
                    if (in_renderCapabilities->IsShadowMappingSupported() == true)
                    {
                        out_shaders.push_back({"Shaders/AnimatedAmbient.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_ambient});
                        out_shaders.push_back({"Shaders/AnimatedBlinnPerVertexShadowedDirectional.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_directional});
                        out_shaders.push_back({"Shaders/AnimatedBlinnPerVertexPoint.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_point});
                    }
                    else
                    {
                        out_shaders.push_back({"Shaders/AnimatedAmbient.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_ambient});
                        out_shaders.push_back({"Shaders/AnimatedBlinnPerVertexDirectional.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_directional});
                        out_shaders.push_back({"Shaders/AnimatedBlinnPerVertexPoint.csshader", Core::StorageLocation::k_chilliSource, ShaderPass::k_point});
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
            void ParseRenderStates(Core::XML::Node* in_rootElement, Material* out_material)
            {
                
                Core::XML::Node* renderStatesEl = Core::XMLUtils::GetFirstChildElement(in_rootElement, "RenderStates");
                if(renderStatesEl)
                {
                    //---Depth Writing
                    Core::XML::Node* depthWriteStateEl =  Core::XMLUtils::GetFirstChildElement(renderStatesEl, "DepthWrite");
                    if(depthWriteStateEl)
                    {
                        out_material->SetDepthWriteEnabled(Core::XMLUtils::GetAttributeValue(depthWriteStateEl, "enabled", true));
                    }
                    //---Depth Testing
                    Core::XML::Node* depthTestStateEl = Core::XMLUtils::GetFirstChildElement(renderStatesEl, "DepthTest");
                    if(depthTestStateEl)
                    {
                        out_material->SetDepthTestEnabled(Core::XMLUtils::GetAttributeValue<bool>(depthTestStateEl, "enabled", true));
                    }
                    //---Transparency
                    Core::XML::Node* transparentStateEl = Core::XMLUtils::GetFirstChildElement(renderStatesEl, "Transparency");
                    if(transparentStateEl)
                    {
                        out_material->SetTransparencyEnabled(Core::XMLUtils::GetAttributeValue<bool>(transparentStateEl, "enabled", false));
                    }
                    //---Culling
                    Core::XML::Node* cullingStateEl = Core::XMLUtils::GetFirstChildElement(renderStatesEl, "Culling");
                    if(cullingStateEl)
                    {
                        out_material->SetFaceCullingEnabled(Core::XMLUtils::GetAttributeValue<bool>(cullingStateEl, "enabled", true));
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
            void ParseAlphaBlendFunction(Core::XML::Node* in_rootElement, Material* out_material)
            {
                Core::XML::Node* blendFuncEl = Core::XMLUtils::GetFirstChildElement(in_rootElement, "BlendFunc");
                if(blendFuncEl)
                {
                    const std::string srcFuncString = Core::XMLUtils::GetAttributeValue<std::string>(blendFuncEl, "src", "One");
                    BlendMode srcFunc = ConvertStringToBlendMode(srcFuncString);
                    
                    const std::string dstFuncString = Core::XMLUtils::GetAttributeValue<std::string>(blendFuncEl, "dst", "One");
                    BlendMode dstFunc = ConvertStringToBlendMode(dstFuncString);
                    
                    out_material->SetBlendModes(srcFunc, dstFunc);
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
            void ParseCullFunction(Core::XML::Node* in_rootElement, Material* out_material)
            {
                Core::XML::Node* cullFaceEl = Core::XMLUtils::GetFirstChildElement(in_rootElement, "Culling");
                if(cullFaceEl)
                {
                    const std::string& cullFaceString = Core::XMLUtils::GetAttributeValue<std::string>(cullFaceEl, "face", "Front");
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
            void ParseSurface(Core::XML::Node* in_rootElement, Material* out_material)
            {
                Core::XML::Node* lightingEl = Core::XMLUtils::GetFirstChildElement(in_rootElement, "Lighting");
                if(lightingEl)
                {
                    //---Emissive
                    Core::XML::Node* emissiveEl = Core::XMLUtils::GetFirstChildElement(lightingEl, "Emissive");
                    if(emissiveEl)
                    {
                        out_material->SetEmissive(Core::XMLUtils::GetAttributeValue<Core::Colour>(emissiveEl, "value", Core::Colour::k_white));
                    }
                    //---Ambient Lighting
                    Core::XML::Node* ambientEl = Core::XMLUtils::GetFirstChildElement(lightingEl, "Ambient");
                    if(ambientEl)
                    {
                        out_material->SetAmbient(Core::XMLUtils::GetAttributeValue<Core::Colour>(ambientEl, "value", Core::Colour::k_white));
                    }
                    //---Diffuse Lighting
                    Core::XML::Node* diffuseEl = Core::XMLUtils::GetFirstChildElement(lightingEl, "Diffuse");
                    if(diffuseEl)
                    {
                        out_material->SetDiffuse(Core::XMLUtils::GetAttributeValue<Core::Colour>(diffuseEl, "value", Core::Colour::k_white));
                    }
                    //---Specular Lighting
                    Core::Colour specular(1.0f, 1.0f, 1.0f, 0.0f);
                    Core::XML::Node* specularEl = Core::XMLUtils::GetFirstChildElement(lightingEl, "Specular");
                    if(specularEl)
                    {
                        specular = Core::XMLUtils::GetAttributeValue<Core::Colour>(specularEl, "value", Core::Colour::k_white);
                    }
                    
                    //---Intensity
                    Core::XML::Node* shininessEl = Core::XMLUtils::GetFirstChildElement(lightingEl, "Shininess");
                    if(shininessEl)
                    {
                        specular.a = Core::XMLUtils::GetAttributeValue<f32>(shininessEl, "value", 0.0f);
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
            void ParseShaders(Core::XML::Node* in_rootElement, std::vector<MaterialProvider::ShaderDesc>& out_shaderFiles, Material* out_material)
            {
                const u32 numShaderNodes = 3;
                const std::pair<std::string, ShaderPass> shaderNodes[numShaderNodes] =
                {
                    std::make_pair("AmbientLightPass", ShaderPass::k_ambient),
                    std::make_pair("DirectionalLightPass", ShaderPass::k_directional),
                    std::make_pair("PointLightPass", ShaderPass::k_point)
                };
                
                Core::XML::Node* shadersEl = Core::XMLUtils::GetFirstChildElement(in_rootElement, "Shaders");
                if(shadersEl)
                {
                    for(u32 i=0; i<numShaderNodes; ++i)
                    {
                        //Overwrite any of the default files for this material type with specified custom ones
                        Core::XML::Node* shaderEl = Core::XMLUtils::GetFirstChildElement(shadersEl, shaderNodes[i].first);
                        if(shaderEl)
                        {
                            for(u32 udwShaderFilesIndex = 0; udwShaderFilesIndex<out_shaderFiles.size(); ++udwShaderFilesIndex)
                            {
                                if(out_shaderFiles[udwShaderFilesIndex].m_pass == shaderNodes[i].second)
                                {
                                    out_shaderFiles[udwShaderFilesIndex].m_location = Core::ParseStorageLocation(Core::XMLUtils::GetAttributeValue<std::string>(shaderEl, "location", "Package"));
                                    out_shaderFiles[udwShaderFilesIndex].m_filePath = Core::XMLUtils::GetAttributeValue<std::string>(shaderEl, "file-name", "");
                                    break;
                                }
                            }
                        }
                    }
                    
                    //---Get the shader variables
                    Core::XML::Node* shaderVarEl = Core::XMLUtils::GetFirstChildElement(shadersEl, "Var");
                    while(shaderVarEl)
                    {
                        //Get the variable type
                        std::string strType = Core::XMLUtils::GetAttributeValue<std::string>(shaderVarEl, "type", "");
                        //Get the variable name
                        std::string strName = Core::XMLUtils::GetAttributeValue<std::string>(shaderVarEl, "name", "");
                        //Add the variable to the material
                        if(strType == "Float")
                        {
                            out_material->SetShaderVar(strName, Core::XMLUtils::GetAttributeValue<f32>(shaderVarEl, "value", 0.0f));
                        }
                        else if(strType == "Vec2")
                        {
                            out_material->SetShaderVar(strName, Core::XMLUtils::GetAttributeValue<Core::Vector2>(shaderVarEl, "value", Core::Vector2::k_zero));
                        }
                        else if(strType == "Vec3")
                        {
                            out_material->SetShaderVar(strName, Core::XMLUtils::GetAttributeValue<Core::Vector3>(shaderVarEl, "value", Core::Vector3::k_zero));
                        }
                        else if(strType == "Vec4")
                        {
                            out_material->SetShaderVar(strName, Core::XMLUtils::GetAttributeValue<Core::Vector4>(shaderVarEl, "value", Core::Vector4::k_zero));
                        }
                        else if(strType == "Colour")
                        {
                            out_material->SetShaderVar(strName, Core::XMLUtils::GetAttributeValue<Core::Colour>(shaderVarEl, "value", Core::Colour::k_white));
                        }
                        else if(strType == "Matrix")
                        {
                            out_material->SetShaderVar(strName, Core::XMLUtils::GetAttributeValue<Core::Matrix4>(shaderVarEl, "value", Core::Matrix4::k_identity));
                        }
                        //Move on to the next variable
                        shaderVarEl =  Core::XMLUtils::GetNextSiblingElement(shaderVarEl, "Var");
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
            void ParseTextures(Core::XML::Node* in_rootElement, std::vector<MaterialProvider::TextureDesc>& out_textureFiles)
            {
                Core::XML::Node* texturesEl = Core::XMLUtils::GetFirstChildElement(in_rootElement, "Textures");
                if(texturesEl)
                {
                    //---Texture
                    Core::XML::Node* textureEl = Core::XMLUtils::GetFirstChildElement(texturesEl, "Texture");
                    while(textureEl)
                    {
                        MaterialProvider::TextureDesc desc;
                        desc.m_location = Core::ParseStorageLocation(Core::XMLUtils::GetAttributeValue<std::string>(textureEl, "location", "Package"));
                        desc.m_filePath = Core::XMLUtils::GetAttributeValue<std::string>(textureEl, "image-name", "");
                        desc.m_shouldMipMap = Core::XMLUtils::GetAttributeValue<bool>(textureEl, "mipmapped", false);
                        desc.m_filterMode = ConvertStringToFilterMode(Core::XMLUtils::GetAttributeValue<std::string>(textureEl, "filter-mode", "Bilinear"));
                        desc.m_wrapModeU = ConvertStringToWrapMode(Core::XMLUtils::GetAttributeValue<std::string>(textureEl, "wrap-mode-u", "Clamp"));
                        desc.m_wrapModeV = ConvertStringToWrapMode(Core::XMLUtils::GetAttributeValue<std::string>(textureEl, "wrap-mode-v", "Clamp"));
                        out_textureFiles.push_back(desc);
                        
                        textureEl =  Core::XMLUtils::GetNextSiblingElement(textureEl, "Texture");
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
            void ParseCubemaps(Core::XML::Node* in_rootElement, std::vector<MaterialProvider::TextureDesc>& out_cubemapFiles)
            {
                Core::XML::Node* cubemapEl = Core::XMLUtils::GetFirstChildElement(in_rootElement, "Cubemap");
                if(cubemapEl)
                {
                    MaterialProvider::TextureDesc desc;
                    desc.m_location = Core::ParseStorageLocation(Core::XMLUtils::GetAttributeValue<std::string>(cubemapEl, "location", "Package"));
                    desc.m_filePath = Core::XMLUtils::GetAttributeValue<std::string>(cubemapEl, "base-name", "");
                    desc.m_shouldMipMap = Core::XMLUtils::GetAttributeValue<bool>(cubemapEl, "mipmapped", false);
                    desc.m_filterMode = ConvertStringToFilterMode(Core::XMLUtils::GetAttributeValue<std::string>(cubemapEl, "filter-mode", "Bilinear"));
                    desc.m_wrapModeU = ConvertStringToWrapMode(Core::XMLUtils::GetAttributeValue<std::string>(cubemapEl, "wrap-mode-u", "Clamp"));
                    desc.m_wrapModeV = ConvertStringToWrapMode(Core::XMLUtils::GetAttributeValue<std::string>(cubemapEl, "wrap-mode-v", "Clamp"));
                    out_cubemapFiles.push_back(desc);
                }
            }
            //----------------------------------------------------------------------------
            /// Load the resource from the given descs at the given index. On
            /// completion this will recursively kick off the next load if one
            /// is required, otherwise will call the delegate
            ///
            /// @author S Downie
            ///
            /// @param Index of desc to load
            /// @param Descs
            /// @param Completion delegate
            /// @param [Out] Material
            //----------------------------------------------------------------------------
            void LoadResourcesChained(u32 in_loadIndex, const std::vector<ChainedLoadDesc>& in_descs, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const MaterialSPtr& out_material)
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                
                switch(in_descs[in_loadIndex].m_type)
                {
                    case ResourceType::k_shader:
                    {
                        resourcePool->LoadResourceAsync<Shader>(in_descs[in_loadIndex].m_location, in_descs[in_loadIndex].m_filePath, [in_loadIndex, in_descs, in_delegate, out_material](const ShaderCSPtr& in_shader)
                        {
                            if(in_shader->GetLoadState() == Core::Resource::LoadState::k_loaded)
                            {
                                out_material->SetShader(in_descs[in_loadIndex].m_pass, in_shader);
                                
                                u32 newLoadIndex = in_loadIndex + 1;
                                
                                if(newLoadIndex < in_descs.size())
                                {
                                    LoadResourcesChained(newLoadIndex, in_descs, in_delegate, out_material);
                                }
                                else
                                {
                                    out_material->SetLoadState(Core::Resource::LoadState::k_loaded);
									Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_material));
                                    return;
                                }
                            }
                            else
                            {
                                out_material->SetLoadState(Core::Resource::LoadState::k_failed);
								Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_material));
                                return;
                            }
                        });
                        break;
                    }
                    case ResourceType::k_texture:
                    {
                        auto options(std::make_shared<TextureResourceOptions>(in_descs[in_loadIndex].m_shouldMipMap, in_descs[in_loadIndex].m_filterMode, in_descs[in_loadIndex].m_wrapModeU, in_descs[in_loadIndex].m_wrapModeV, true));
                        resourcePool->LoadResourceAsync<Texture>(in_descs[in_loadIndex].m_location, in_descs[in_loadIndex].m_filePath, options, [in_loadIndex, in_descs, in_delegate, out_material](const TextureCSPtr& in_texture)
                        {
                            if(in_texture->GetLoadState() == Core::Resource::LoadState::k_loaded)
                            {
                                out_material->AddTexture(in_texture);
                                
                                u32 newLoadIndex = in_loadIndex + 1;
                                
                                if(newLoadIndex < in_descs.size())
                                {
                                    LoadResourcesChained(newLoadIndex, in_descs, in_delegate, out_material);
                                }
                                else
                                {
                                    out_material->SetLoadState(Core::Resource::LoadState::k_loaded);
									Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_material));
                                    return;
                                }
                            }
                            else
                            {
                                out_material->SetLoadState(Core::Resource::LoadState::k_failed);
								Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_material));
                                return;
                            }
                        });
                        break;
                    }
                    case ResourceType::k_cubemap:
                    {
                        auto options(std::make_shared<CubemapResourceOptions>(in_descs[in_loadIndex].m_shouldMipMap, in_descs[in_loadIndex].m_filterMode, in_descs[in_loadIndex].m_wrapModeU, in_descs[in_loadIndex].m_wrapModeV, true));
                        resourcePool->LoadResourceAsync<Cubemap>(in_descs[in_loadIndex].m_location, in_descs[in_loadIndex].m_filePath, options, [in_loadIndex, in_descs, in_delegate, out_material](const CubemapCSPtr& in_cubemap)
                        {
                            if(in_cubemap->GetLoadState() == Core::Resource::LoadState::k_loaded)
                            {
                                out_material->SetCubemap(in_cubemap);

                                u32 newLoadIndex = in_loadIndex + 1;

                                if(newLoadIndex < in_descs.size())
                                {
                                    LoadResourcesChained(newLoadIndex, in_descs, in_delegate, out_material);
                                }
                                else
                                {
                                    out_material->SetLoadState(Core::Resource::LoadState::k_loaded);
									Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_material));
                                    return;
                                }
                            }
                            else
                            {
                                out_material->SetLoadState(Core::Resource::LoadState::k_failed);
								Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_material));
                                return;
                            }
                        });
                        break;
                    }
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
        Core::InterfaceIDType MaterialProvider::GetResourceType() const
        {
            return Material::InterfaceID;
        }
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool MaterialProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
		{
			return in_extension == k_materialExtension;
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		void MaterialProvider::CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceSPtr& out_resource)
		{
            std::vector<ShaderDesc> shaderFiles;
            std::vector<TextureDesc> textureFiles;
            std::vector<TextureDesc> cubemapFiles;
            
            if(BuildMaterialFromFile(in_location, in_filePath, shaderFiles, textureFiles, cubemapFiles, (Material*)out_resource.get()) == false)
            {
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                return;
            }
            
            MaterialSPtr material = std::static_pointer_cast<Material>(out_resource);
            
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            
            for(u32 i=0; i<shaderFiles.size(); ++i)
            {
                if(shaderFiles[i].m_filePath.empty() == false)
                {
                    ShaderCSPtr shader = resourcePool->LoadResource<Shader>(shaderFiles[i].m_location, shaderFiles[i].m_filePath);
                    if(shader == nullptr)
                    {
                        out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                        return;
                    }
                    material->SetShader(shaderFiles[i].m_pass, shader);
                }
            }
            
            for(u32 i=0; i<textureFiles.size(); ++i)
            {
                if(textureFiles[i].m_filePath.empty() == false)
                {
                    auto options(std::make_shared<TextureResourceOptions>(textureFiles[i].m_shouldMipMap, textureFiles[i].m_filterMode, textureFiles[i].m_wrapModeU, textureFiles[i].m_wrapModeV, true));
                    TextureCSPtr texture = resourcePool->LoadResource<Texture>(textureFiles[i].m_location, textureFiles[i].m_filePath, options);
                    if(texture == nullptr)
                    {
                        out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                        return;
                    }
                    material->AddTexture(texture);
                }
            }
            
            CS_ASSERT(cubemapFiles.size() <= 1, "Currently only 1 cubemap is supported");
            for(u32 i=0; i<cubemapFiles.size(); ++i)
            {
                if(cubemapFiles[i].m_filePath.empty() == false)
                {
                    auto options(std::make_shared<CubemapResourceOptions>(cubemapFiles[i].m_shouldMipMap, cubemapFiles[i].m_filterMode, cubemapFiles[i].m_wrapModeU, cubemapFiles[i].m_wrapModeV, true));
                    CubemapCSPtr cubemap = resourcePool->LoadResource<Cubemap>(cubemapFiles[i].m_location, cubemapFiles[i].m_filePath, options);
                    if(cubemap == nullptr)
                    {
                        out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                        return;
                    }
                    material->SetCubemap(cubemap);
                }
            }
            
            out_resource->SetLoadState(Core::Resource::LoadState::k_loaded);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		void MaterialProvider::CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
		{
			auto task = std::bind(&MaterialProvider::BuildMaterialTask, this, in_location, in_filePath, in_delegate, out_resource);
			Core::Application::Get()->GetTaskScheduler()->ScheduleTask(task);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		void MaterialProvider::BuildMaterialTask(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
		{
            std::vector<ShaderDesc> shaderFiles;
            std::vector<TextureDesc> textureFiles;
            std::vector<TextureDesc> cubemapFiles;
			if(BuildMaterialFromFile(in_location, in_filePath, shaderFiles, textureFiles, cubemapFiles, (Material*)out_resource.get()) == false)
            {
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
				Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                return;
            }
            
            CS_ASSERT(cubemapFiles.size() <= 1, "Currently only 1 cubemap is supported");
            
            MaterialSPtr material = std::static_pointer_cast<Material>(out_resource);
            
            std::vector<ChainedLoadDesc> resourceFiles;
            resourceFiles.reserve(shaderFiles.size() + textureFiles.size() + cubemapFiles.size());
            
            for(const auto& shaderDesc : shaderFiles)
            {
                ChainedLoadDesc desc;
                desc.m_filePath = shaderDesc.m_filePath;
                desc.m_location = shaderDesc.m_location;
                desc.m_pass = shaderDesc.m_pass;
                desc.m_type = ResourceType::k_shader;
                resourceFiles.push_back(desc);
            }
            
            for(const auto& textureDesc : textureFiles)
            {
                ChainedLoadDesc desc;
                desc.m_filePath = textureDesc.m_filePath;
                desc.m_location = textureDesc.m_location;
                desc.m_shouldMipMap = textureDesc.m_shouldMipMap;
                desc.m_filterMode = textureDesc.m_filterMode;
                desc.m_wrapModeU = textureDesc.m_wrapModeU;
                desc.m_wrapModeV = textureDesc.m_wrapModeV;
                desc.m_type = ResourceType::k_texture;
                resourceFiles.push_back(desc);
            }
            
            for(const auto& cubemapDesc : cubemapFiles)
            {
                ChainedLoadDesc desc;
                desc.m_filePath = cubemapDesc.m_filePath;
                desc.m_location = cubemapDesc.m_location;
                desc.m_shouldMipMap = cubemapDesc.m_shouldMipMap;
                desc.m_filterMode = cubemapDesc.m_filterMode;
                desc.m_wrapModeU = cubemapDesc.m_wrapModeU;
                desc.m_wrapModeV = cubemapDesc.m_wrapModeV;
                desc.m_type = ResourceType::k_cubemap;
                resourceFiles.push_back(desc);
            }
            
            LoadResourcesChained(0, resourceFiles, in_delegate, material);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool MaterialProvider::BuildMaterialFromFile(Core::StorageLocation in_location, const std::string& in_filePath,
                                                    std::vector<ShaderDesc>& out_shaderFiles,
                                                    std::vector<TextureDesc>& out_textureFiles,
                                                    std::vector<TextureDesc>& out_cubemapFiles,
                                                    Material* out_material)
		{
			//Load the XML file
            Core::XMLUPtr xml = Core::XMLUtils::ReadDocument(in_location, in_filePath);
            Core::XML::Node* rootElement = Core::XMLUtils::GetFirstChildElement(xml->GetDocument());
            
            if(rootElement == nullptr || Core::XMLUtils::GetName(rootElement) != "Material")
            {
                return false;
            }
			
            std::string materialType = Core::XMLUtils::GetAttributeValue<std::string>(rootElement, "type", "Static");
            GetShaderFilesForMaterialType(materialType, m_renderCapabilities, out_shaderFiles);

            ParseRenderStates(rootElement, out_material);
            ParseAlphaBlendFunction(rootElement, out_material);
            ParseCullFunction(rootElement, out_material);
            ParseSurface(rootElement, out_material);
            ParseShaders(rootElement, out_shaderFiles, out_material);
            ParseTextures(rootElement, out_textureFiles);
            ParseCubemaps(rootElement, out_cubemapFiles);
            
            return true;
		}
	}
}
