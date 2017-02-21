//
//  MaterialProvider.cpp
//  ChilliSource
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
#include <ChilliSource/Rendering/Base/TestFunc.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Shader/Shader.h>
#include <ChilliSource/Rendering/Texture/Cubemap.h>
#include <ChilliSource/Rendering/Texture/CubemapResourceOptions.h>
#include <ChilliSource/Rendering/Texture/TextureResourceOptions.h>
#include <ChilliSource/Rendering/Texture/TextureType.h>

namespace ChilliSource
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
            k_texture
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
            StorageLocation m_location;
            bool m_shouldMipMap;
            TextureFilterMode m_filterMode;
            TextureWrapMode m_wrapModeU;
            TextureWrapMode m_wrapModeV;
            RenderPasses m_pass;
            TextureType m_textureType;
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
            constexpr char k_zero[] = "zero";
            constexpr char k_one[] = "one";
            constexpr char k_sourceColour[] = "sourcecolour";
            constexpr char k_oneMinusSourceColour[] = "oneminussourcecolour";
            constexpr char k_sourceAlpha[] = "sourcealpha";
            constexpr char k_oneMinusSourceAlpha[] = "oneminussourcealpha";
            constexpr char k_destColour[] = "destcolour";
            constexpr char k_oneMinusDestColour[] = "oneminusdestcolour";
            constexpr char k_destAlpha[] = "destalpha";
            constexpr char k_oneMinusDestAlpha[] = "oneminusdestalpha";
            
            std::string blendModeLower = in_blendMode;
            StringUtils::ToLowerCase(blendModeLower);
            
            if (blendModeLower == k_zero)
            {
                return BlendMode::k_zero;
            }
            else if (blendModeLower == k_one)
            {
                return BlendMode::k_one;
            }
            else if (blendModeLower == k_sourceColour)
            {
                return BlendMode::k_sourceCol;
            }
            else if (blendModeLower == k_oneMinusSourceColour)
            {
                return BlendMode::k_oneMinusSourceCol;
            }
            else if (blendModeLower == k_sourceAlpha)
            {
                return BlendMode::k_sourceAlpha;
            }
            else if (blendModeLower == k_oneMinusSourceAlpha)
            {
                 return BlendMode::k_oneMinusSourceAlpha;
            }
            else if (blendModeLower == k_destColour)
            {
                return BlendMode::k_destCol;
            }
            else if (blendModeLower == k_oneMinusDestColour)
            {
                return BlendMode::k_oneMinusDestCol;
            }
            else if (blendModeLower == k_destAlpha)
            {
                return BlendMode::k_destAlpha;
            }
            else if (blendModeLower == k_oneMinusDestAlpha)
            {
                return BlendMode::k_oneMinusDestAlpha;
            }
            
            CS_LOG_FATAL("Invalid BlendMode: " + in_blendMode);
            return BlendMode::k_one;
        }
        //----------------------------------------------------------------------------
        /// @author S Downie
        ///
        /// @param String describing func
        ///
        /// @return Depth test func
        //----------------------------------------------------------------------------
        TestFunc ConvertStringToDepthFunc(const std::string& depthFunc)
        {
            std::string depthFuncLower = depthFunc;
            StringUtils::ToLowerCase(depthFuncLower);
            
            if(depthFuncLower == "never")
            {
                return TestFunc::k_never;
            }
            else if(depthFuncLower == "less")
            {
                return TestFunc::k_less;
            }
            else if(depthFuncLower == "lequal")
            {
                return TestFunc::k_lessEqual;
            }
            else if(depthFuncLower == "greater")
            {
                return TestFunc::k_greater;
            }
            else if(depthFuncLower == "gequal")
            {
                return TestFunc::k_greaterEqual;
            }
            else if(depthFuncLower == "equal")
            {
                return TestFunc::k_equal;
            }
            else if(depthFuncLower == "notequal")
            {
                return TestFunc::k_notEqual;
            }
            else if(depthFuncLower == "always")
            {
                return TestFunc::k_always;
            }
            
            CS_LOG_FATAL("Invalid DepthTestFunc: " + depthFunc);
            return TestFunc::k_never;
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
            constexpr char k_front[] = "front";
            constexpr char k_back[] = "back";
            
            std::string cullFaceLower = in_cullFace;
            StringUtils::ToLowerCase(cullFaceLower);
            
            if (cullFaceLower == k_front)
            {
                return CullFace::k_front;
            }
            else if (cullFaceLower == k_back)
            {
                return CullFace::k_back;
            }
            
            CS_LOG_FATAL("Invalid CullFace: " + in_cullFace);
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
        TextureWrapMode ConvertStringToWrapMode(const std::string& in_wrapModeString)
        {
            constexpr char k_clamp[] = "clamp";
            constexpr char k_repeat[] = "repeat";
            
            std::string lowerWrapModeString = in_wrapModeString;
            StringUtils::ToLowerCase(lowerWrapModeString);
            
            if (lowerWrapModeString == k_clamp)
            {
                return TextureWrapMode::k_clamp;
            }
            else if (lowerWrapModeString == k_repeat)
            {
                return TextureWrapMode::k_repeat;
            }
            
            CS_LOG_FATAL("Invalid WrapMode: " + in_wrapModeString);
            return TextureWrapMode::k_clamp;
        }
        //----------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param A string describing the texture filter mode.
        ///
        /// @return The filter mode. If the string was not a valid filter mode this will
        /// default to bilinear.
        //----------------------------------------------------------------------------
        TextureFilterMode ConvertStringToFilterMode(const std::string& in_filterModeString)
        {
            constexpr char k_nearest[] = "nearest";
            constexpr char k_bilinear[] = "bilinear";
            
            std::string lowerFilterModeString = in_filterModeString;
            StringUtils::ToLowerCase(lowerFilterModeString);
            
            if (lowerFilterModeString == k_nearest)
            {
                return TextureFilterMode::k_nearest;
            }
            else if (lowerFilterModeString == k_bilinear)
            {
                return TextureFilterMode::k_bilinear;
            }
            
            CS_LOG_FATAL("Invalid FilterMode: " + in_filterModeString);
            return TextureFilterMode::k_bilinear;
        }
        //----------------------------------------------------------------------------
        /// Returns the shading type for the given material type.
        ///
        /// @param in_materialType - The material type.
        ///
        /// @return The shading type.
        //----------------------------------------------------------------------------
        MaterialShadingType ConvertStringToShadingType(const std::string& in_materialType) noexcept
        {
            constexpr char k_unlit[] = "unlit";
            constexpr char k_blinn[] = "blinn";
            constexpr char k_custom[] = "custom";
            constexpr char k_skybox[] = "skybox";
            
            auto materialTypeLower = in_materialType;
            StringUtils::ToLowerCase(materialTypeLower);
            
            if (materialTypeLower == k_unlit)
            {
                return MaterialShadingType::k_unlit;
            }
            else if (materialTypeLower == k_blinn)
            {
                return MaterialShadingType::k_blinn;
            }
            else if (materialTypeLower == k_custom)
            {
                return MaterialShadingType::k_custom;
            }
            else if (materialTypeLower == k_skybox)
            {
                return MaterialShadingType::k_skybox;
            }
            
            CS_LOG_FATAL("Invalid material type: " + in_materialType);
            return MaterialShadingType::k_unlit;
        }

        ///
        /// @param pass
        ///     The render pass as a string.
        ///
        /// @return The render pass as an type.
        ///
        RenderPasses ConvertStringToRenderPass(const std::string& pass) noexcept
        {
            auto passLower = pass;
            StringUtils::ToLowerCase(passLower);
            
            if (passLower == "shadowmap")
            {
                return RenderPasses::k_shadowMap;
            }
            else if (passLower == "base")
            {
                return RenderPasses::k_base;
            }
            else if (passLower == "directionallight")
            {
                return RenderPasses::k_directionalLight;
            }
            else if (passLower == "directionallightshadows")
            {
                return RenderPasses::k_directionalLightShadows;
            }
            else if (passLower == "pointlight")
            {
                return RenderPasses::k_pointLight;
            }
            else if (passLower == "transparent")
            {
                return RenderPasses::k_transparent;
            }
            else if (passLower == "skybox")
            {
                return RenderPasses::k_skybox;
            }
            
            CS_LOG_FATAL("Invalid render pass: " + pass);
            return RenderPasses::k_base;
        }
        ///
        /// @param type
        ///     The texture type as a string.
        ///
        /// @return The texture type as an type.
        ///
        TextureType ConvertStringToTextureType(const std::string& type) noexcept
        {
            auto typeLower = type;
            StringUtils::ToLowerCase(typeLower);
            
            if (typeLower == "texture")
            {
                return TextureType::k_texture;
            }
            else if (typeLower == "cubemap")
            {
                return TextureType::k_cubemap;
            }
            
            CS_LOG_FATAL("Invalid texture type: " + type);
            return TextureType::k_texture;
        }
        //----------------------------------------------------------------------------
        /// Parse the vertex format from the given string name. If the name doesn't
        /// exist this will assert. Note that materials loaded from file cannot
        /// describe custom vertex formats.
        ///
        /// @author Ian Copland
        ///
        /// @param in_vertexFormatName
        ///
        /// @return The vertex format with the given name.
        //----------------------------------------------------------------------------
        VertexFormat ParseVertexFormat(const std::string& in_vertexFormatName) noexcept
        {
            constexpr char k_sprite[] = "sprite";
            constexpr char k_staticMesh[] = "staticmesh";
            constexpr char k_animatedmesh[] = "animatedmesh";
            
            auto vertexFormatNameTypeLower = in_vertexFormatName;
            StringUtils::ToLowerCase(vertexFormatNameTypeLower);
            
            if (vertexFormatNameTypeLower == k_sprite)
            {
                return VertexFormat::k_sprite;
            }
            else if (vertexFormatNameTypeLower == k_staticMesh)
            {
                return VertexFormat::k_staticMesh;
            }
            else if (vertexFormatNameTypeLower == k_animatedmesh)
            {
                return VertexFormat::k_animatedMesh;
            }
            
            CS_LOG_FATAL("Invalid vertex format: " + in_vertexFormatName);
            return VertexFormat::k_sprite;
        }
        //----------------------------------------------------------------------------
        /// Parse the render states from the XML element to the material
        ///
        /// @author S Downie
        ///
        /// @param Root element
        /// @param [Out] Material to populate
        //----------------------------------------------------------------------------
        void ParseRenderStates(XML::Node* in_rootElement, Material* out_material)
        {
            
            XML::Node* renderStatesEl = XMLUtils::GetFirstChildElement(in_rootElement, "RenderStates");
            if(renderStatesEl)
            {
                //---Depth Writing
                XML::Node* depthWriteStateEl =  XMLUtils::GetFirstChildElement(renderStatesEl, "DepthWrite");
                if(depthWriteStateEl)
                {
                    out_material->SetDepthWriteEnabled(XMLUtils::GetAttributeValue(depthWriteStateEl, "enabled", true));
                }
                //---Depth Testing
                XML::Node* depthTestStateEl = XMLUtils::GetFirstChildElement(renderStatesEl, "DepthTest");
                if(depthTestStateEl)
                {
                    out_material->SetDepthTestEnabled(XMLUtils::GetAttributeValue<bool>(depthTestStateEl, "enabled", true));
                }
                //---Transparency
                XML::Node* transparentStateEl = XMLUtils::GetFirstChildElement(renderStatesEl, "Transparency");
                if(transparentStateEl)
                {
                    out_material->SetTransparencyEnabled(XMLUtils::GetAttributeValue<bool>(transparentStateEl, "enabled", false));
                }
                //---Culling
                XML::Node* cullingStateEl = XMLUtils::GetFirstChildElement(renderStatesEl, "Culling");
                if(cullingStateEl)
                {
                    out_material->SetFaceCullingEnabled(XMLUtils::GetAttributeValue<bool>(cullingStateEl, "enabled", true));
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
        void ParseAlphaBlendFunction(XML::Node* in_rootElement, Material* out_material)
        {
            XML::Node* blendFuncEl = XMLUtils::GetFirstChildElement(in_rootElement, "BlendFunc");
            if(blendFuncEl)
            {
                const std::string srcFuncString = XMLUtils::GetAttributeValue<std::string>(blendFuncEl, "src", "One");
                BlendMode srcFunc = ConvertStringToBlendMode(srcFuncString);
                
                const std::string dstFuncString = XMLUtils::GetAttributeValue<std::string>(blendFuncEl, "dst", "One");
                BlendMode dstFunc = ConvertStringToBlendMode(dstFuncString);
                
                out_material->SetBlendModes(srcFunc, dstFunc);
            }
        }
        //----------------------------------------------------------------------------
        /// Parse the depth test function from the XML element to the material
        ///
        /// @author S Downie
        ///
        /// @param Root element
        /// @param [Out] Material to populate
        //----------------------------------------------------------------------------
        void ParseDepthTestFunction(XML::Node* in_rootElement, Material* out_material)
        {
            XML::Node* depthFuncEl = XMLUtils::GetFirstChildElement(in_rootElement, "DepthTestFunc");
            if(depthFuncEl)
            {
                const std::string funcString = XMLUtils::GetAttributeValue<std::string>(depthFuncEl, "func", "lequal");
                out_material->SetDepthTestFunc(ConvertStringToDepthFunc(funcString));
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
        void ParseCullFunction(XML::Node* in_rootElement, Material* out_material)
        {
            XML::Node* cullFaceEl = XMLUtils::GetFirstChildElement(in_rootElement, "Culling");
            if(cullFaceEl)
            {
                const std::string& cullFaceString = XMLUtils::GetAttributeValue<std::string>(cullFaceEl, "face", "Front");
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
        void ParseSurface(XML::Node* in_rootElement, Material* out_material)
        {
            XML::Node* lightingEl = XMLUtils::GetFirstChildElement(in_rootElement, "Lighting");
            if(lightingEl)
            {
                //---Emissive
                XML::Node* emissiveEl = XMLUtils::GetFirstChildElement(lightingEl, "Emissive");
                if(emissiveEl)
                {
                    out_material->SetEmissive(XMLUtils::GetAttributeValue<Colour>(emissiveEl, "value", Colour::k_white));
                }
                //---Ambient Lighting
                XML::Node* ambientEl = XMLUtils::GetFirstChildElement(lightingEl, "Ambient");
                if(ambientEl)
                {
                    out_material->SetAmbient(XMLUtils::GetAttributeValue<Colour>(ambientEl, "value", Colour::k_white));
                }
                //---Diffuse Lighting
                XML::Node* diffuseEl = XMLUtils::GetFirstChildElement(lightingEl, "Diffuse");
                if(diffuseEl)
                {
                    out_material->SetDiffuse(XMLUtils::GetAttributeValue<Colour>(diffuseEl, "value", Colour::k_white));
                }
                //---Specular Lighting
                Colour specular(1.0f, 1.0f, 1.0f, 0.0f);
                XML::Node* specularEl = XMLUtils::GetFirstChildElement(lightingEl, "Specular");
                if(specularEl)
                {
                    specular = XMLUtils::GetAttributeValue<Colour>(specularEl, "value", Colour::k_white);
                }
                
                //---Intensity
                XML::Node* shininessEl = XMLUtils::GetFirstChildElement(lightingEl, "Shininess");
                if(shininessEl)
                {
                    specular.a = XMLUtils::GetAttributeValue<f32>(shininessEl, "value", 0.0f);
                }
                
                out_material->SetSpecular(specular);
            }
        }
        //----------------------------------------------------------------------------
        /// Parse the shader file paths
        ///
        /// @author Ian Copland
        ///
        /// @param Root element
        /// @param [Out] Shader files to populate
        /// @param [Out] Material to populate
        //----------------------------------------------------------------------------
        void ParseShaders(XML::Node* in_rootElement, std::vector<MaterialProvider::ShaderDesc>& out_shaderFiles, Material* out_material)
        {
            XML::Node* shadersEl = XMLUtils::GetFirstChildElement(in_rootElement, "Shaders");
            if(shadersEl)
            {
                CS_ASSERT(out_material->GetShadingType() == MaterialShadingType::k_custom, "Only custom materials can have shaders.");
                
                //Get the fallback shader type which will populate all the other passes
                out_material->PrepCustomShaders(ParseVertexFormat(XMLUtils::GetAttributeValue<std::string>(shadersEl, "vertex-format", "StaticMesh")), ConvertStringToShadingType(XMLUtils::GetAttributeValue<std::string>(shadersEl, "fallback-type", "Custom")));
                
                XML::Node* shaderEl = XMLUtils::GetFirstChildElement(shadersEl, "Shader");
                while(shaderEl)
                {
                    MaterialProvider::ShaderDesc desc;
                    desc.m_location = ParseStorageLocation(XMLUtils::GetAttributeValue<std::string>(shaderEl, "location", "Package"));
                    desc.m_filePath = XMLUtils::GetAttributeValue<std::string>(shaderEl, "file-name", "");
                    desc.m_pass = ConvertStringToRenderPass(XMLUtils::GetAttributeValue<std::string>(shaderEl, "pass", "Base"));
                    out_shaderFiles.push_back(desc);
                    
                    // Get the shader variables
                    XML::Node* shaderVarEl = XMLUtils::GetFirstChildElement(shaderEl, "Var");
                    while(shaderVarEl)
                    {
                        //Get the variable type
                        std::string strType = XMLUtils::GetAttributeValue<std::string>(shaderVarEl, "type", "");
                        //Get the variable name
                        std::string strName = XMLUtils::GetAttributeValue<std::string>(shaderVarEl, "name", "");
                        //Add the variable to the material
                        if(strType == "Float")
                        {
                            out_material->SetShaderVar(strName, XMLUtils::GetAttributeValue<f32>(shaderVarEl, "value", 0.0f));
                        }
                        else if(strType == "Vec2")
                        {
                            out_material->SetShaderVar(strName, XMLUtils::GetAttributeValue<Vector2>(shaderVarEl, "value", Vector2::k_zero));
                        }
                        else if(strType == "Vec3")
                        {
                            out_material->SetShaderVar(strName, XMLUtils::GetAttributeValue<Vector3>(shaderVarEl, "value", Vector3::k_zero));
                        }
                        else if(strType == "Vec4")
                        {
                            out_material->SetShaderVar(strName, XMLUtils::GetAttributeValue<Vector4>(shaderVarEl, "value", Vector4::k_zero));
                        }
                        else if(strType == "Colour")
                        {
                            out_material->SetShaderVar(strName, XMLUtils::GetAttributeValue<Colour>(shaderVarEl, "value", Colour::k_white));
                        }
                        else if(strType == "Matrix")
                        {
                            out_material->SetShaderVar(strName, XMLUtils::GetAttributeValue<Matrix4>(shaderVarEl, "value", Matrix4::k_identity));
                        }
                        //Move on to the next variable
                        shaderVarEl =  XMLUtils::GetNextSiblingElement(shaderVarEl, "Var");
                    }
                    
                    shaderEl =  XMLUtils::GetNextSiblingElement(shaderEl, "Shader");
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
        void ParseTextures(XML::Node* in_rootElement, std::vector<MaterialProvider::TextureDesc>& out_textureFiles)
        {
            XML::Node* texturesEl = XMLUtils::GetFirstChildElement(in_rootElement, "Textures");
            if(texturesEl)
            {
                //---Texture
                XML::Node* textureEl = XMLUtils::GetFirstChildElement(texturesEl, "Texture");
                while(textureEl)
                {
                    MaterialProvider::TextureDesc desc;
                    desc.m_location = ParseStorageLocation(XMLUtils::GetAttributeValue<std::string>(textureEl, "location", "Package"));
                    desc.m_filePath = XMLUtils::GetAttributeValue<std::string>(textureEl, "file-name", "");
                    desc.m_shouldMipMap = XMLUtils::GetAttributeValue<bool>(textureEl, "mipmapped", false);
                    desc.m_filterMode = ConvertStringToFilterMode(XMLUtils::GetAttributeValue<std::string>(textureEl, "filter-mode", "Bilinear"));
                    desc.m_wrapModeU = ConvertStringToWrapMode(XMLUtils::GetAttributeValue<std::string>(textureEl, "wrap-mode-u", "Clamp"));
                    desc.m_wrapModeV = ConvertStringToWrapMode(XMLUtils::GetAttributeValue<std::string>(textureEl, "wrap-mode-v", "Clamp"));
                    desc.m_type = ConvertStringToTextureType(XMLUtils::GetAttributeValue<std::string>(textureEl, "type", "Texture"));
                    out_textureFiles.push_back(desc);
                    
                    textureEl =  XMLUtils::GetNextSiblingElement(textureEl, "Texture");
                }
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
        void LoadResourcesChained(u32 in_loadIndex, const std::vector<ChainedLoadDesc>& in_descs, const ResourceProvider::AsyncLoadDelegate& in_delegate, const MaterialSPtr& out_material)
        {
            ResourcePool* resourcePool = Application::Get()->GetResourcePool();
            
            switch(in_descs[in_loadIndex].m_type)
            {
                case ResourceType::k_shader:
                {
                    resourcePool->LoadResourceAsync<Shader>(in_descs[in_loadIndex].m_location, in_descs[in_loadIndex].m_filePath, [in_loadIndex, in_descs, in_delegate, out_material](const ShaderCSPtr& in_shader)
                    {
                        if(in_shader->GetLoadState() == Resource::LoadState::k_loaded)
                        {
                            out_material->AddCustomShader(in_shader, in_descs[in_loadIndex].m_pass);
                            
                            u32 newLoadIndex = in_loadIndex + 1;
                            
                            if(newLoadIndex < in_descs.size())
                            {
                                LoadResourcesChained(newLoadIndex, in_descs, in_delegate, out_material);
                            }
                            else
                            {
                                out_material->SetLoadState(Resource::LoadState::k_loaded);
                                Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept
                                {
                                    in_delegate(out_material);
                                });
                                return;
                            }
                        }
                        else
                        {
                            out_material->SetLoadState(Resource::LoadState::k_failed);
                            Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept
                            {
                                in_delegate(out_material);
                            });
                            return;
                        }
                    });
                    break;
                }
                case ResourceType::k_texture:
                {
                    switch(in_descs[in_loadIndex].m_textureType)
                    {
                        case TextureType::k_texture:
                        {
                            auto options(std::make_shared<TextureResourceOptions>(in_descs[in_loadIndex].m_shouldMipMap, in_descs[in_loadIndex].m_filterMode, in_descs[in_loadIndex].m_wrapModeU, in_descs[in_loadIndex].m_wrapModeV));
                            
                            resourcePool->LoadResourceAsync<Texture>(in_descs[in_loadIndex].m_location, in_descs[in_loadIndex].m_filePath, options, [=](const TextureCSPtr& in_texture)
                             {
                                 if(in_texture->GetLoadState() == Resource::LoadState::k_loaded)
                                 {
                                     out_material->AddTexture(in_texture);
                                     
                                     u32 newLoadIndex = in_loadIndex + 1;
                                     
                                     if(newLoadIndex < in_descs.size())
                                     {
                                         LoadResourcesChained(newLoadIndex, in_descs, in_delegate, out_material);
                                     }
                                     else
                                     {
                                         out_material->SetLoadState(Resource::LoadState::k_loaded);
                                         Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept { in_delegate(out_material); });
                                         return;
                                     }
                                 }
                                 else
                                 {
                                     out_material->SetLoadState(Resource::LoadState::k_failed);
                                     Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept { in_delegate(out_material); });
                                     return;
                                 }
                             });
                            break;
                        }
                        case TextureType::k_cubemap:
                        {
                            auto options(std::make_shared<CubemapResourceOptions>(in_descs[in_loadIndex].m_shouldMipMap, in_descs[in_loadIndex].m_filterMode, in_descs[in_loadIndex].m_wrapModeU, in_descs[in_loadIndex].m_wrapModeV));
                            
                            resourcePool->LoadResourceAsync<Cubemap>(in_descs[in_loadIndex].m_location, in_descs[in_loadIndex].m_filePath, options, [=](const CubemapCSPtr& cubemap)
                             {
                                 if(cubemap->GetLoadState() == Resource::LoadState::k_loaded)
                                 {
                                     out_material->AddCubemap(cubemap);
                                     
                                     u32 newLoadIndex = in_loadIndex + 1;
                                     
                                     if(newLoadIndex < in_descs.size())
                                     {
                                         LoadResourcesChained(newLoadIndex, in_descs, in_delegate, out_material);
                                     }
                                     else
                                     {
                                         out_material->SetLoadState(Resource::LoadState::k_loaded);
                                         Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept { in_delegate(out_material); });
                                         return;
                                     }
                                 }
                                 else
                                 {
                                     out_material->SetLoadState(Resource::LoadState::k_failed);
                                     Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept { in_delegate(out_material); });
                                     return;
                                 }
                             });
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    
    CS_DEFINE_NAMEDTYPE(MaterialProvider);
    
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    MaterialProviderUPtr MaterialProvider::Create()
    {
        return MaterialProviderUPtr(new MaterialProvider());
    }
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    bool MaterialProvider::IsA(InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == MaterialProvider::InterfaceID;
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    InterfaceIDType MaterialProvider::GetResourceType() const
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
    void MaterialProvider::CreateResourceFromFile(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceSPtr& out_resource)
    {
        std::vector<ShaderDesc> shaderFiles;
        std::vector<TextureDesc> textureFiles;
        
        if(BuildMaterialFromFile(in_location, in_filePath, shaderFiles, textureFiles, (Material*)out_resource.get()) == false)
        {
            out_resource->SetLoadState(Resource::LoadState::k_failed);
            return;
        }
        
        MaterialSPtr material = std::static_pointer_cast<Material>(out_resource);
        
        ResourcePool* resourcePool = Application::Get()->GetResourcePool();
        
        for(u32 i=0; i<shaderFiles.size(); ++i)
        {
            if(shaderFiles[i].m_filePath.empty() == false)
            {
                ShaderCSPtr shader = resourcePool->LoadResource<Shader>(shaderFiles[i].m_location, shaderFiles[i].m_filePath);
                if(shader == nullptr)
                {
                    out_resource->SetLoadState(Resource::LoadState::k_failed);
                    return;
                }
                
                material->AddCustomShader(shader, shaderFiles[i].m_pass);
            }
        }
        
        for(u32 i=0; i<textureFiles.size(); ++i)
        {
            if(textureFiles[i].m_filePath.empty() == false)
            {
                switch (textureFiles[i].m_type)
                {
                    case TextureType::k_texture:
                    {
                        auto options(std::make_shared<TextureResourceOptions>(textureFiles[i].m_shouldMipMap, textureFiles[i].m_filterMode, textureFiles[i].m_wrapModeU, textureFiles[i].m_wrapModeV));
                        
                        TextureCSPtr texture = resourcePool->LoadResource<Texture>(textureFiles[i].m_location, textureFiles[i].m_filePath, options);
                        if(texture == nullptr)
                        {
                            out_resource->SetLoadState(Resource::LoadState::k_failed);
                            return;
                        }
                        material->AddTexture(texture);
                        break;
                    }
                    case TextureType::k_cubemap:
                    {
                        auto options(std::make_shared<CubemapResourceOptions>(textureFiles[i].m_shouldMipMap, textureFiles[i].m_filterMode, textureFiles[i].m_wrapModeU, textureFiles[i].m_wrapModeV));
                        
                        CubemapCSPtr cubemap = resourcePool->LoadResource<Cubemap>(textureFiles[i].m_location, textureFiles[i].m_filePath, options);
                        if(cubemap == nullptr)
                        {
                            out_resource->SetLoadState(Resource::LoadState::k_failed);
                            return;
                        }
                        material->AddCubemap(cubemap);
                        break;
                    }
                }

            }
        }
        
        out_resource->SetLoadState(Resource::LoadState::k_loaded);
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void MaterialProvider::CreateResourceFromFileAsync(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceProvider::AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
    {
        Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_file, [=](const TaskContext&) noexcept
        {
            BuildMaterialTask(in_location, in_filePath, in_delegate, out_resource);
        });
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void MaterialProvider::BuildMaterialTask(StorageLocation in_location, const std::string& in_filePath, const ResourceProvider::AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
    {
        std::vector<ShaderDesc> shaderFiles;
        std::vector<TextureDesc> textureFiles;
        if(BuildMaterialFromFile(in_location, in_filePath, shaderFiles, textureFiles, (Material*)out_resource.get()) == false)
        {
            out_resource->SetLoadState(Resource::LoadState::k_failed);
            Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept
            {
                in_delegate(out_resource);
            });
            return;
        }
        
        CS_ASSERT(shaderFiles.size() < 2, "Can have zero or one shaders.");
        
        MaterialSPtr material = std::static_pointer_cast<Material>(out_resource);
        
        std::vector<ChainedLoadDesc> resourceFiles;
        resourceFiles.reserve(textureFiles.size());
        
        for(const auto& shaderDesc : shaderFiles)
        {
            ChainedLoadDesc desc;
            desc.m_filePath = shaderDesc.m_filePath;
            desc.m_location = shaderDesc.m_location;
            desc.m_type = ResourceType::k_shader;
            desc.m_pass = shaderDesc.m_pass;
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
            desc.m_textureType = textureDesc.m_type;
            desc.m_type = ResourceType::k_texture;
            resourceFiles.push_back(desc);
        }
        
        LoadResourcesChained(0, resourceFiles, in_delegate, material);
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    bool MaterialProvider::BuildMaterialFromFile(StorageLocation in_location, const std::string& in_filePath,
                                                 std::vector<ShaderDesc>& out_shaderFiles,
                                                 std::vector<TextureDesc>& out_textureFiles,
                                                Material* out_material)
    {
        //Load the XML file
        XMLUPtr xml = XMLUtils::ReadDocument(in_location, in_filePath);
        XML::Node* rootElement = XMLUtils::GetFirstChildElement(xml->GetDocument());
        
        if(rootElement == nullptr || XMLUtils::GetName(rootElement) != "Material")
        {
            return false;
        }
        
        std::string materialType = XMLUtils::GetAttributeValue<std::string>(rootElement, "type", "Static");
        out_material->SetShadingType(ConvertStringToShadingType(materialType));

        ParseRenderStates(rootElement, out_material);
        ParseAlphaBlendFunction(rootElement, out_material);
        ParseCullFunction(rootElement, out_material);
        ParseDepthTestFunction(rootElement, out_material);
        ParseSurface(rootElement, out_material);
        
        ParseShaders(rootElement, out_shaderFiles, out_material);
        ParseTextures(rootElement, out_textureFiles);
        
        return true;
    }
}
