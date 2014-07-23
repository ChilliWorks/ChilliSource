//
//  CubeMap.cpp
//  Chilli Source
//  Created by Scott Downie on 15/07/2013.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2013 Tag Games Limited
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

#include <CSBackend/Rendering/OpenGL/Texture/Cubemap.h>

#include <CSBackend/Rendering/OpenGL/Base/GLError.h>
#include <CSBackend/Rendering/OpenGL/Base/RenderCapabilities.h>
#include <CSBackend/Rendering/OpenGL/Base/RenderSystem.h>
#include <CSBackend/Rendering/OpenGL/Texture/TextureUnitSystem.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/Image/ImageCompression.h>

namespace CSBackend
{
	namespace OpenGL
	{
        namespace
        {
            //---------------------------------------------------
            /// Apply the currently set wrap mode to the cubemap.
            /// Requires that the cubemap is bound.
            ///
            /// @author S Downie
            ///
            /// @param Horizontal wrap mode
            /// @param Vertical wrap mode
            //---------------------------------------------------
            void ApplyWrapMode(CSRendering::Texture::WrapMode in_smode, CSRendering::Texture::WrapMode in_tmode)
            {
                switch(in_smode)
                {
                    case CSRendering::Texture::WrapMode::k_clamp:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                        break;
                    case CSRendering::Texture::WrapMode::k_repeat:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
                        break;
                };
                switch(in_tmode)
                {
                    case CSRendering::Texture::WrapMode::k_clamp:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                        break;
                    case CSRendering::Texture::WrapMode::k_repeat:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
                        break;
                };
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while applying cubemap wrap mode.");
            }
            //---------------------------------------------------
            /// Apply the currently set filter mode to the cubemap.
            /// Requires that the cubemap is bound.
            ///
            /// @author S Downie
            ///
            /// @param Filter mode
            /// @param Whether mip-mapping is enabled
            //---------------------------------------------------
            void ApplyFilterMode(CSRendering::Texture::FilterMode in_mode, bool in_hasMipMaps)
            {
                if(in_hasMipMaps == false)
                {
                    switch(in_mode)
                    {
                        case CSRendering::Texture::FilterMode::k_nearestNeighbour:
                            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                            break;
                        case CSRendering::Texture::FilterMode::k_bilinear:
                            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            break;
                    }
                }
                else
                {
                    switch(in_mode)
                    {
                        case CSRendering::Texture::FilterMode::k_nearestNeighbour:
                            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                            break;
                        case CSRendering::Texture::FilterMode::k_bilinear:
                            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            break;
                    }
                }
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while applying cubemap filter mode.");
            }
            //---------------------------------------------------
            /// Uploads image data with no compression in the
            /// given format
            ///
            /// @author S Downie
            ///
            /// @param Cubemap face index
            /// @param Format
            /// @param Image width
            /// @param Image height
            /// @param Image data
            //---------------------------------------------------
            void UploadImageDataNoCompression(u32 in_faceIdx, CSCore::ImageFormat in_format, u32 in_imageWidth, u32 in_imageHeight, const u8* in_imageData)
            {
                switch(in_format)
                {
                    default:
                    case CSCore::ImageFormat::k_RGBA8888:
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + in_faceIdx, 0, GL_RGBA, in_imageWidth, in_imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, in_imageData);
                        break;
                    case CSCore::ImageFormat::k_RGB888:
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + in_faceIdx, 0, GL_RGB, in_imageWidth, in_imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, in_imageData);
                        break;
                    case CSCore::ImageFormat::k_RGBA4444:
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + in_faceIdx, 0, GL_RGBA, in_imageWidth, in_imageHeight, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, in_imageData);
                        break;
                    case CSCore::ImageFormat::k_RGB565:
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + in_faceIdx, 0, GL_RGB, in_imageWidth, in_imageHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, in_imageData);
                        break;
                    case CSCore::ImageFormat::k_LumA88:
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + in_faceIdx, 0, GL_LUMINANCE_ALPHA, in_imageWidth, in_imageHeight, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, in_imageData);
                        break;
                    case CSCore::ImageFormat::k_Lum8:
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + in_faceIdx, 0, GL_LUMINANCE, in_imageWidth, in_imageHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, in_imageData);
                        break;
                    case CSCore::ImageFormat::k_Depth16:
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + in_faceIdx, 0, GL_DEPTH_COMPONENT, in_imageWidth, in_imageHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, in_imageData);
                        break;
                    case CSCore::ImageFormat::k_Depth32:
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + in_faceIdx, 0, GL_DEPTH_COMPONENT, in_imageWidth, in_imageHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, in_imageData);
                        break;
                };
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while uploading uncompressed cubemap data.");
            }
            //---------------------------------------------------
            /// Uploads image data with ETC1 compression. ETC1
            /// only supports RGB images and will aseert on any
            /// other format. It is currently only supported on
            /// Android and will assert on other platforms
            ///
            /// @author S Downie
            ///
            /// @param Cubemap face index
            /// @param Format
            /// @param Image width
            /// @param Image height
            /// @param Image data
            /// @param Image data size in bytes
            //---------------------------------------------------
            void UploadImageDataETC1(u32 in_faceIdx, CSCore::ImageFormat in_format, u32 in_imageWidth, u32 in_imageHeight, const u8* in_imageData, u32 in_imageDataSize)
            {
#ifndef CS_TARGETPLATFORM_ANDROID
                CS_LOG_FATAL("ETC1 compression is only supported on Android");
#endif
                CS_ASSERT(in_format == CSCore::ImageFormat::k_RGB888, "ETC1 only supports RGB image format");
                
#ifdef CS_TARGETPLATFORM_ANDROID
                glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + in_faceIdx, 0, GL_ETC1_RGB8_OES, in_imageWidth, in_imageHeight, 0, in_imageDataSize, in_imageData);
#endif
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while uploading ETC1 cubemap data.");
            }
            //---------------------------------------------------
            /// Uploads image data with PVR 2 bits per pixel compression.
            /// PVR only supports RGB and RGBA images and will aseert on any
            /// other format. It is currently only supported on
            /// iOS and will assert on other platforms
            ///
            /// @author S Downie
            ///
            /// @param Cubemap face index
            /// @param Format
            /// @param Image width
            /// @param Image height
            /// @param Image data
            /// @param Image data size in bytes
            //---------------------------------------------------
            void UploadImageDataPVR2(u32 in_faceIdx, CSCore::ImageFormat in_format, u32 in_imageWidth, u32 in_imageHeight, const u8* in_imageData, u32 in_imageDataSize)
            {
#ifndef CS_TARGETPLATFORM_IOS
                CS_LOG_FATAL("PVR compression is only supported on iOS");
#endif
                
#ifdef CS_TARGETPLATFORM_IOS
                switch(in_format)
                {
                    default:
                        CS_LOG_FATAL("PVR compression only supports RGB and RGBA image formats");
                        break;
                    case CSCore::ImageFormat::k_RGBA8888:
                        glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + in_faceIdx, 0, GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG, in_imageWidth, in_imageHeight, 0, in_imageDataSize, in_imageData);
                        break;
                    case CSCore::ImageFormat::k_RGB888:
                        glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + in_faceIdx, 0, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, in_imageWidth, in_imageHeight, 0, in_imageDataSize, in_imageData);
                        break;
                };
#endif
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while uploading PVR2 cubemap data.");
            }
            //---------------------------------------------------
            /// Uploads image data with PVR 4 bits per pixel compression.
            /// PVR only supports RGB and RGBA images and will aseert on any
            /// other format. It is currently only supported on
            /// iOS and will assert on other platforms
            ///
            /// @author S Downie
            ///
            /// @param Cubemap face index
            /// @param Format
            /// @param Image width
            /// @param Image height
            /// @param Image data
            /// @param Image data size in bytes
            //---------------------------------------------------
            void UploadImageDataPVR4(u32 in_faceIdx, CSCore::ImageFormat in_format, u32 in_imageWidth, u32 in_imageHeight, const u8* in_imageData, u32 in_imageDataSize)
            {
#ifndef CS_TARGETPLATFORM_IOS
                CS_LOG_FATAL("PVR compression is only supported on iOS");
#endif
                
#ifdef CS_TARGETPLATFORM_IOS
                switch(in_format)
                {
                    default:
                        CS_LOG_FATAL("PVR compression only supports RGB and RGBA image formats");
                        break;
                    case CSCore::ImageFormat::k_RGBA8888:
                        glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + in_faceIdx, 0, GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, in_imageWidth, in_imageHeight, 0, in_imageDataSize, in_imageData);
                        break;
                    case CSCore::ImageFormat::k_RGB888:
                        glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + in_faceIdx, 0, GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG, in_imageWidth, in_imageHeight, 0, in_imageDataSize, in_imageData);
                        break;
                };
#endif
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while uploading PVR4 cubemap data.");
            }
        }
        
        CS_DEFINE_NAMEDTYPE(Cubemap);
        //---------------------------------------------------------------
		//---------------------------------------------------------------
		Cubemap::Cubemap()
		{
            m_renderCapabilities = CSCore::Application::Get()->GetSystem<CSRendering::RenderCapabilities>();
            CS_ASSERT(m_renderCapabilities, "Cannot find required system: Render Capabilities.");
            
            m_texUnitSystem = CSCore::Application::Get()->GetSystem<TextureUnitSystem>();
            CS_ASSERT(m_renderCapabilities, "Cannot find required system: Texture Unit System.");
		}
        //--------------------------------------------------
		//--------------------------------------------------
		bool Cubemap::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == Cubemap::InterfaceID || in_interfaceId == CSRendering::Cubemap::InterfaceID;
		}
		//--------------------------------------------------
		//--------------------------------------------------
		void Cubemap::Bind(u32 in_texUnit)
		{
			CS_ASSERT(in_texUnit < m_renderCapabilities->GetNumTextureUnits(), "Attempting to bind to texture unit not supported on this device: " + CSCore::ToString(in_texUnit));
			
            m_texUnitSystem->Bind(this, in_texUnit);
            
            if(m_hasFilterModeChanged == true)
            {
                ApplyFilterMode(m_filterMode, m_hasMipMaps);
                m_hasFilterModeChanged = false;
            }
            
            if(m_hasWrapModeChanged == true)
            {
                ApplyWrapMode(m_sWrapMode, m_tWrapMode);
                m_hasWrapModeChanged = false;
            }
		}
        //--------------------------------------------------
        /// GL makes a copy of the data so we can just
        /// let the incoming data delete itself
        //--------------------------------------------------
        void Cubemap::Build(const std::array<CSRendering::Texture::Descriptor, 6>& in_descs, std::array<CSRendering::Texture::TextureDataUPtr, 6>&& in_datas, bool in_mipMap, bool in_restoreCubemapDataEnabled)
        {
            Destroy();
            
            glGenTextures(1, &m_cubemapHandle);
            Bind();
            
            if(in_mipMap == true)
            {
                glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
            }
            m_hasMipMaps = in_mipMap;
            
            for(u32 i=0; i<in_descs.size(); ++i)
            {
                CS_ASSERT(in_descs[i].m_width <= m_renderCapabilities->GetMaxTextureSize() && in_descs[i].m_height <= m_renderCapabilities->GetMaxTextureSize(),
                          "OpenGL does not support textures of this size on this device (" + CSCore::ToString(in_descs[i].m_width) + ", " + CSCore::ToString(in_descs[i].m_height) + ")");
                
                m_widths[i] = in_descs[i].m_width;
                m_heights[i] = in_descs[i].m_height;
                m_formats[i] = in_descs[i].m_format;
                m_compressions[i] = in_descs[i].m_compression;
                
                switch(m_compressions[i])
                {
                    case CSCore::ImageCompression::k_none:
                        UploadImageDataNoCompression(i, m_formats[i], m_widths[i], m_heights[i], in_datas[i].get());
                        break;
                    case CSCore::ImageCompression::k_ETC1:
                        UploadImageDataETC1(i, m_formats[i], m_widths[i], m_heights[i], in_datas[i].get(), in_descs[i].m_dataSize);
                        break;
                    case CSCore::ImageCompression::k_PVR2Bpp:
                        UploadImageDataPVR2(i, m_formats[i], m_widths[i], m_heights[i], in_datas[i].get(), in_descs[i].m_dataSize);
                        break;
                    case CSCore::ImageCompression::k_PVR4Bpp:
                        UploadImageDataPVR4(i, m_formats[i], m_widths[i], m_heights[i], in_datas[i].get(), in_descs[i].m_dataSize);
                        break;
                };
            }
            
#ifdef CS_TARGETPLATFORM_ANDROID
            if (GetStorageLocation() == CSCore::StorageLocation::k_none && in_restoreCubemapDataEnabled == true)
            {
            	m_restoreCubemapDataEnabled = true;

                for(u32 i = 0; i < in_descs.size(); ++i)
                {
                    m_restorationDataSizes[i] = in_descs[i].m_dataSize;
                    m_restorationDatas[i] = std::move(in_datas[i]);
                }
            }
#endif
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while building cubemap.");
        }
        //--------------------------------------------------
		//--------------------------------------------------
		void Cubemap::Unbind()
		{
            m_texUnitSystem->Unbind(this);
		}
		//--------------------------------------------------
		//--------------------------------------------------
		GLuint Cubemap::GetCubemapHandle() const
		{
			return m_cubemapHandle;
		}
        //--------------------------------------------------
		//--------------------------------------------------
		bool Cubemap::HasMipMaps() const
		{
			return m_hasMipMaps;
		}
		//--------------------------------------------------
		//--------------------------------------------------
		CSCore::ImageFormat Cubemap::GetFormat(Face in_face) const
		{
			return m_formats[(u32)in_face];
		}
		//--------------------------------------------------
		//--------------------------------------------------
		void Cubemap::SetFilterMode(CSRendering::Texture::FilterMode in_mode)
		{
            m_filterMode = in_mode;
            
            m_hasFilterModeChanged = true;
		}
		//--------------------------------------------------
		//--------------------------------------------------
		void Cubemap::SetWrapMode(CSRendering::Texture::WrapMode in_smode, CSRendering::Texture::WrapMode in_tmode)
		{
			m_sWrapMode = in_smode;
			m_tWrapMode = in_tmode;
            
            m_hasWrapModeChanged = true;
		}
#ifdef CS_TARGETPLATFORM_ANDROID
        //--------------------------------------------------
        //--------------------------------------------------
        void Cubemap::Restore()
        {
            CS_ASSERT(GetStorageLocation() == CSCore::StorageLocation::k_none, "Cannot restore texture that was loaded from file. This should be handled using RefreshResource().");
            
            std::array<CSRendering::Texture::Descriptor, 6> descs;
            for (u32 i = 0; i < descs.size(); ++i)
            {
                descs[i].m_width = m_widths[i];
                descs[i].m_height = m_heights[i];
                descs[i].m_format = m_formats[i];
                descs[i].m_compression = m_compressions[i];
                descs[i].m_dataSize = m_restorationDataSizes[i];
            }
            
            CSRendering::Texture::WrapMode sWrap = m_sWrapMode;
            CSRendering::Texture::WrapMode tWrap = m_tWrapMode;
            CSRendering::Texture::FilterMode filterMode = m_filterMode;
            
            Build(descs, std::move(m_restorationDatas), m_hasMipMaps, m_restoreCubemapDataEnabled);
            SetWrapMode(sWrap, tWrap);
            SetFilterMode(filterMode);
        }
#endif
        //--------------------------------------------------
        //--------------------------------------------------
        void Cubemap::Destroy()
        {
            m_hasFilterModeChanged = true;
            m_hasWrapModeChanged = true;
            m_hasMipMaps = false;
            
            m_sWrapMode = CSRendering::Texture::WrapMode::k_clamp;
            m_tWrapMode = CSRendering::Texture::WrapMode::k_clamp;
            
            //If the context has already been destroyed then the cubemap has already been destroyed
            bool hasContext = static_cast<RenderSystem*>(CSCore::Application::Get()->GetRenderSystem())->HasContext();
            if(hasContext == true && m_cubemapHandle > 0)
            {
                Unbind();
                glDeleteTextures(1, &m_cubemapHandle);
            }
            
            m_cubemapHandle = 0;
            
#ifdef CS_TARGETPLATFORM_ANDROID
            m_restoreCubemapDataEnabled = false;

            for (u32 i = 0; i < m_restorationDataSizes.size(); ++i)
            {
                m_restorationDataSizes[i] = 0;
            }
            
            for (u32 i = 0; i < m_restorationDatas.size(); ++i)
            {
                m_restorationDatas[i].reset();
            }
#endif
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while destroying a cubemap.");
        }
		//--------------------------------------------------
		//--------------------------------------------------
		Cubemap::~Cubemap()
		{
            Destroy();
		}
	}
}
