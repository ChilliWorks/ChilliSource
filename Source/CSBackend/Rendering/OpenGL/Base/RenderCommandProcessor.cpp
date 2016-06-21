//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#include <CSBackend/Rendering/OpenGL/Base/RenderCommandProcessor.h>

#include <CSBackend/Rendering/OpenGL/Model/GLMesh.h>
#include <CSBackend/Rendering/OpenGL/Shader/GLShader.h>
#include <CSBackend/Rendering/OpenGL/Texture/GLTexture.h>

#include <ChilliSource/Rendering/Base/BlendMode.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommandList.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommandQueue.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyCameraRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyMaterialRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyMeshRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/BeginRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/EndRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadMaterialGroupRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadMeshRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadShaderRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadTextureRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/RenderInstanceRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadMaterialGroupRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadMeshRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadShaderRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadTextureRenderCommand.h>

namespace CSBackend
{
    namespace OpenGL
    {
        namespace
        {
            /// TODO: Do properly.
            ///
            GLenum ToGLBlendMode(ChilliSource::BlendMode in_blendMode)
            {
                switch(in_blendMode)
                {
                    case ChilliSource::BlendMode::k_zero:
                        return GL_ZERO;
                    case ChilliSource::BlendMode::k_one:
                        return GL_ONE;
                    case ChilliSource::BlendMode::k_sourceCol:
                        return GL_SRC_COLOR;
                    case ChilliSource::BlendMode::k_oneMinusSourceCol:
                        return GL_ONE_MINUS_SRC_COLOR;
                    case ChilliSource::BlendMode::k_sourceAlpha:
                        return GL_SRC_ALPHA;
                    case ChilliSource::BlendMode::k_oneMinusSourceAlpha:
                        return GL_ONE_MINUS_SRC_ALPHA;
                    case ChilliSource::BlendMode::k_destAlpha:
                        return GL_DST_ALPHA;
                    case ChilliSource::BlendMode::k_oneMinusDestAlpha:
                        return GL_ONE_MINUS_DST_ALPHA;
                    default:
                        return GL_SRC_ALPHA;
                };
            }
        }
        
        //------------------------------------------------------------------------------
        void RenderCommandProcessor::Process(const ChilliSource::RenderCommandQueue& renderCommandQueue) noexcept
        {
            for (const auto& renderCommandList : renderCommandQueue.GetQueue())
            {
                for (const auto& renderCommand : renderCommandList->GetOrderedList())
                {
                    switch (renderCommand->GetType())
                    {
                        case ChilliSource::RenderCommand::Type::k_loadShader:
                            LoadShader(static_cast<const ChilliSource::LoadShaderRenderCommand*>(renderCommand));
                            break;
                        case ChilliSource::RenderCommand::Type::k_loadTexture:
                            LoadTexture(static_cast<const ChilliSource::LoadTextureRenderCommand*>(renderCommand));
                            break;
                        case ChilliSource::RenderCommand::Type::k_loadMaterialGroup:
                            // Do nothing in OpenGL 2.0 / ES 2.0
                            break;
                        case ChilliSource::RenderCommand::Type::k_loadMesh:
                            LoadMesh(static_cast<const ChilliSource::LoadMeshRenderCommand*>(renderCommand));
                            break;
                        case ChilliSource::RenderCommand::Type::k_begin:
                            Begin(static_cast<const ChilliSource::BeginRenderCommand*>(renderCommand));
                            break;
                        case ChilliSource::RenderCommand::Type::k_applyCamera:
                            ApplyCamera(static_cast<const ChilliSource::ApplyCameraRenderCommand*>(renderCommand));
                            break;
                        case ChilliSource::RenderCommand::Type::k_applyMaterial:
                            ApplyMaterial(static_cast<const ChilliSource::ApplyMaterialRenderCommand*>(renderCommand));
                            break;
                        case ChilliSource::RenderCommand::Type::k_applyMesh:
                            ApplyMesh(static_cast<const ChilliSource::ApplyMeshRenderCommand*>(renderCommand));
                            break;
                        case ChilliSource::RenderCommand::Type::k_renderInstance:
                            RenderInstance(static_cast<const ChilliSource::RenderInstanceRenderCommand*>(renderCommand));
                            break;
                        case ChilliSource::RenderCommand::Type::k_end:
                            End();
                            break;
                        case ChilliSource::RenderCommand::Type::k_unloadShader:
                            UnloadShader(static_cast<const ChilliSource::UnloadShaderRenderCommand*>(renderCommand));
                            break;
                        case ChilliSource::RenderCommand::Type::k_unloadTexture:
                            UnloadTexture(static_cast<const ChilliSource::UnloadTextureRenderCommand*>(renderCommand));
                            break;
                        case ChilliSource::RenderCommand::Type::k_unloadMaterialGroup:
                            // Do nothing in OpenGL 2.0 / ES 2.0
                            break;
                        case ChilliSource::RenderCommand::Type::k_unloadMesh:
                            UnloadMesh(static_cast<const ChilliSource::UnloadMeshRenderCommand*>(renderCommand));
                            break;
                        default:
                            CS_LOG_FATAL("Unknown render command.");
                            break;
                    }
                }
            }
        }
        
        //------------------------------------------------------------------------------
        void RenderCommandProcessor::LoadShader(const ChilliSource::LoadShaderRenderCommand* renderCommand) noexcept
        {
            auto renderShader = renderCommand->GetRenderShader();
            
            //TODO: Should be pooled.
            auto glShader = new GLShader(renderCommand->GetVertexShader(), renderCommand->GetFragmentShader());
            
            renderShader->SetExtraData(glShader);
            m_contextState.SetRenderShader(nullptr);
        }
        
        //------------------------------------------------------------------------------
        void RenderCommandProcessor::LoadTexture(const ChilliSource::LoadTextureRenderCommand* renderCommand) noexcept
        {
            auto renderTexture = renderCommand->GetRenderTexture();
            
            //TODO: Should be pooled.
            auto glTexture = new GLTexture(renderCommand->GetTextureData(), renderCommand->GetTextureDataSize(), renderTexture->GetDimensions(), renderTexture->GetImageFormat(),
                                           renderTexture->GetImageCompression(), renderTexture->GetFilterMode(), renderTexture->GetWrapModeS(), renderTexture->GetWrapModeT(),
                                           renderTexture->IsMipmapped());
            
            renderTexture->SetExtraData(glTexture);
            m_contextState.SetRenderTexture(nullptr);
        }
        
        //------------------------------------------------------------------------------
        void RenderCommandProcessor::LoadMesh(const ChilliSource::LoadMeshRenderCommand* renderCommand) noexcept
        {
            auto renderMesh = renderCommand->GetRenderMesh();
            
            //TODO: Should be pooled.
            auto glMesh = new GLMesh(renderMesh->GetPolygonType(), renderMesh->GetVertexFormat(), renderMesh->GetIndexFormat(), renderCommand->GetVertexData(), renderCommand->GetVertexDataSize(),
                                     renderCommand->GetIndexData(), renderCommand->GetIndexDataSize());
            
            renderMesh->SetExtraData(glMesh);
            m_contextState.SetRenderMesh(nullptr);
        }
        
        //------------------------------------------------------------------------------
        void RenderCommandProcessor::Begin(const ChilliSource::BeginRenderCommand* renderCommand) noexcept
        {
            glViewport(0, 0, renderCommand->GetResolution().x, renderCommand->GetResolution().y);
            
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glDepthMask(GL_TRUE);
            
            glClearColor(renderCommand->GetClearColour().r, renderCommand->GetClearColour().g, renderCommand->GetClearColour().b, renderCommand->GetClearColour().a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glBlendEquation(GL_FUNC_ADD);
            glDepthFunc(GL_LEQUAL);
            
            m_contextState.Reset();
        }
        
        //------------------------------------------------------------------------------
        void RenderCommandProcessor::ApplyCamera(const ChilliSource::ApplyCameraRenderCommand* renderCommand) noexcept
        {
            m_contextState.SetCameraState(ContextState::CameraState(renderCommand->GetPosition(), renderCommand->GetViewProjectionMatrix()));
            
            m_contextState.SetRenderShader(nullptr);
            m_contextState.SetRenderTexture(nullptr);
            m_contextState.SetRenderMaterial(nullptr);
            m_contextState.SetRenderMesh(nullptr);
        }
        
        //------------------------------------------------------------------------------
        void RenderCommandProcessor::ApplyMaterial(const ChilliSource::ApplyMaterialRenderCommand* renderCommand) noexcept
        {
            auto renderMaterial = renderCommand->GetRenderMaterial();
            
            auto renderShader = renderMaterial->GetRenderShader();
            if (renderShader != m_contextState.GetRenderShader())
            {
                m_contextState.SetRenderShader(renderShader);
                
                auto glShader = reinterpret_cast<GLShader*>(renderShader->GetExtraData());
                glShader->Bind();
            }
            
            //TODO: Handle textures properly
            auto renderTexture = renderMaterial->GetRenderTextures()[0];
            if (renderTexture != m_contextState.GetRenderTexture())
            {
                m_contextState.SetRenderTexture(renderTexture);
                
                auto glTexture = reinterpret_cast<GLTexture*>(renderTexture->GetExtraData());
                glTexture->Bind();
            }
            
            //TODO: Apply material state properly
            
            if (renderMaterial->IsDepthTestEnabled())
            {
                glEnable(GL_DEPTH_TEST);
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
            }
            
            if (renderMaterial->IsDepthWriteEnabled())
            {
                glDepthMask(GL_TRUE);
            }
            else
            {
                glDepthMask(GL_FALSE);
            }
            
            if (renderMaterial->IsColourWriteEnabled())
            {
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            }
            else
            {
                glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            }
            
            if (renderMaterial->IsFaceCullingEnabled())
            {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }
            
            if (renderMaterial->IsTransparencyEnabled())
            {
                glEnable(GL_BLEND);
                glBlendFunc(ToGLBlendMode(renderMaterial->GetSourceBlendMode()), ToGLBlendMode(renderMaterial->GetDestinationBlendMode()));
            }
            else
            {
                glDisable(GL_BLEND);
            }
            
            GLShader* glShader = static_cast<GLShader*>(renderShader->GetExtraData());
            glShader->SetUniform("u_emissive", renderMaterial->GetEmissiveColour(), GLShader::FailurePolicy::k_silent);
            glShader->SetUniform("u_ambient", renderMaterial->GetAmbientColour(), GLShader::FailurePolicy::k_silent);
            glShader->SetUniform("u_diffuse", renderMaterial->GetDiffuseColour(), GLShader::FailurePolicy::k_silent);
            glShader->SetUniform("u_specular", renderMaterial->GetSpecularColour(), GLShader::FailurePolicy::k_silent);
            
            glShader->SetUniform("u_cameraPos", m_contextState.GetCameraState().GetPosition(), GLShader::FailurePolicy::k_silent);
        }
        
        //------------------------------------------------------------------------------
        void RenderCommandProcessor::ApplyMesh(const ChilliSource::ApplyMeshRenderCommand* renderCommand) noexcept
        {
            auto renderMesh = renderCommand->GetRenderMesh();
            if (renderMesh != m_contextState.GetRenderMesh())
            {
                m_contextState.SetRenderMesh(renderMesh);
                
                auto glMesh = reinterpret_cast<GLMesh*>(renderMesh->GetExtraData());
                auto glShader = reinterpret_cast<GLShader*>(m_contextState.GetRenderShader()->GetExtraData());
                glMesh->Bind(glShader);
            }
        }
        
        //------------------------------------------------------------------------------
        void RenderCommandProcessor::RenderInstance(const ChilliSource::RenderInstanceRenderCommand* renderCommand) noexcept
        {
            GLShader* glShader = static_cast<GLShader*>(m_contextState.GetRenderShader()->GetExtraData());
            glShader->SetUniform("u_wvpMat", renderCommand->GetWorldMatrix() * m_contextState.GetCameraState().GetViewProjectionMatrix(), GLShader::FailurePolicy::k_silent);
            glShader->SetUniform("u_normalMat", ChilliSource::Matrix4::Transpose(ChilliSource::Matrix4::Inverse(renderCommand->GetWorldMatrix())), GLShader::FailurePolicy::k_silent);
            
            glDrawElements(GL_TRIANGLES, m_contextState.GetRenderMesh()->GetNumIndices(), GL_UNSIGNED_SHORT, 0);
        }
        
        //------------------------------------------------------------------------------
        void RenderCommandProcessor::End() noexcept
        {
            m_contextState.Reset();
        }
        
        //------------------------------------------------------------------------------
        void RenderCommandProcessor::UnloadShader(const ChilliSource::UnloadShaderRenderCommand* renderCommand) noexcept
        {
            m_contextState.SetRenderShader(nullptr);
            
            auto renderShader = renderCommand->GetRenderShader();
            auto glShader = reinterpret_cast<GLShader*>(renderShader->GetExtraData());
            
            CS_SAFEDELETE(glShader);
        }
        
        //------------------------------------------------------------------------------
        void RenderCommandProcessor::UnloadTexture(const ChilliSource::UnloadTextureRenderCommand* renderCommand) noexcept
        {
            m_contextState.SetRenderTexture(nullptr);
            
            auto renderTexture = renderCommand->GetRenderTexture();
            auto glTexture = reinterpret_cast<GLTexture*>(renderTexture->GetExtraData());
            
            CS_SAFEDELETE(glTexture);
        }
        
        //------------------------------------------------------------------------------
        void RenderCommandProcessor::UnloadMesh(const ChilliSource::UnloadMeshRenderCommand* renderCommand) noexcept
        {
            m_contextState.SetRenderMesh(nullptr);
            
            auto renderMesh = renderCommand->GetRenderMesh();
            auto glMesh = reinterpret_cast<GLMesh*>(renderMesh->GetExtraData());
            
            CS_SAFEDELETE(glMesh);
        }
    }
}