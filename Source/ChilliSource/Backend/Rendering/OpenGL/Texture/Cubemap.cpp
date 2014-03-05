/*
 *  Cubemap.cpp
 *  moFlo
 *
 *  Created by Tag Games on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Rendering/OpenGL/Texture/Cubemap.h>

#include <ChilliSource/Backend/Rendering/OpenGL/Texture/CubemapManager.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Texture/Texture.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/ImageFormatConverter.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>

namespace ChilliSource
{
	namespace OpenGL
	{
        namespace
        {
            //--------------------------------------------------
            /// Format Converter
            ///
            /// @param MoFlow image format
            /// @return GL image format
            //--------------------------------------------------
            GLenum FormatConverter(Core::Image::Format ineFormat)
            {
                switch(ineFormat)
                {
                    default:
                    case Core::Image::Format::k_RGBA8888:
                        return GL_RGBA;
                        break;
                    case Core::Image::Format::k_RGB888:
                        return GL_RGB;
                        break;
                    case Core::Image::Format::k_RGBA4444:
                        return GL_RGBA;
                        break;
                    case Core::Image::Format::k_RGB565:
                        return GL_RGB;
                        break;
                    case Core::Image::Format::k_LumA88:
                        return GL_LUMINANCE_ALPHA;
                        break;
                    case Core::Image::Format::k_Lum8:
                        return GL_LUMINANCE;
                        break;
                };
            }
            //--------------------------------------------------
            /// Type Converter
            ///
            /// @param MoFlow image format
            /// @return GL image type
            //--------------------------------------------------
            GLenum TypeConverter(Core::Image::Format ineFormat)
            {
                switch(ineFormat)
                {
                    default:
                    case Core::Image::Format::k_RGBA8888:
                        return GL_UNSIGNED_BYTE;
                        break;
                    case Core::Image::Format::k_RGB888:
                        return GL_UNSIGNED_BYTE;
                        break;
                    case Core::Image::Format::k_RGBA4444:
                        return GL_UNSIGNED_SHORT_4_4_4_4;
                        break;
                    case Core::Image::Format::k_RGB565:
                        return GL_UNSIGNED_SHORT_5_6_5;
                        break;
                    case Core::Image::Format::k_LumA88:
                        return GL_UNSIGNED_BYTE;
                        break;
                    case Core::Image::Format::k_Lum8:
                        return GL_UNSIGNED_BYTE;
                        break;
                };
            }
            //--------------------------------------------------
            /// Cubemap Image 2D
            ///
            /// @param Array of 6 images
            //--------------------------------------------------
            void CubemapImage2D(const std::vector<Core::ResourceSPtr>& inapSourceImages)
            {
                for(u32 i=0; i<6; ++i)
                {
                    Core::Image* pSourceImage = (Core::Image*)inapSourceImages[i].get();
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, FormatConverter(pSourceImage->GetFormat()), pSourceImage->GetWidth(), pSourceImage->GetHeight(), 0,
                                 FormatConverter(pSourceImage->GetFormat()), TypeConverter(pSourceImage->GetFormat()), pSourceImage->GetData());
                }
            }
            //--------------------------------------------------
            /// Cubemap Compressed Image 2D
            ///
            /// @param Format
            /// @param Array of 6 images
            //--------------------------------------------------
            void CubemapCompressedImage2D(GLenum inFormat, const std::vector<Core::ResourceSPtr>& inapSourceImages)
            {
                for(u32 i=0; i<6; ++i)
                {
                    Core::Image* pSourceImage = (Core::Image*)inapSourceImages[i].get();
                    glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, inFormat, pSourceImage->GetWidth(), pSourceImage->GetHeight(), 0, pSourceImage->GetDataLength(), pSourceImage->GetData());
                }
            }
        }
        
        //--------------------------------------------------
		/// Cosntructor
		//--------------------------------------------------
        Cubemap::Cubemap(CubemapManager* inpManager)
        :mpCubemapManager(inpManager),
        meSFilter(Rendering::Texture::Filter::k_linear), meTFilter(Rendering::Texture::Filter::k_linear),
        meSWrapMode(Rendering::Texture::WrapMode::k_clamp), meTWrapMode(Rendering::Texture::WrapMode::k_clamp),
        mbHasMipMaps(false), mbHasTextureFilterModeChanged(true), meImageFormat(Core::Image::Format::k_RGBA8888),
        mpRenderCapabilities(nullptr)
        {
            mpRenderCapabilities = Core::Application::Get()->GetSystem<Rendering::RenderCapabilities>();
        }
		//--------------------------------------------------
		/// Init
		//--------------------------------------------------
		void Cubemap::Init(const std::vector<Core::ResourceSPtr>& inapSourceImages, bool inbWithMipsMaps)
		{
            CS_ASSERT(inapSourceImages.size() == 6, "Cubemaps must have 6 face textures");
            
			glGenTextures(1, &mGLTextureID);
			Bind();
			
            mbHasMipMaps = inbWithMipsMaps;
            mbHasTextureFilterModeChanged = true;
            
            UpdateTextureParameters();

            Core::Image* pSourceImage = (Core::Image*)inapSourceImages[0].get();
            meImageFormat = pSourceImage->GetFormat();
            
            switch(pSourceImage->GetCompression())
            {
                default:
                case Core::ImageCompression::k_none:
                    CubemapImage2D(inapSourceImages);
                    break;
                case Core::ImageCompression::k_ETC1:
#ifdef CS_TARGETPLATFORM_ANDROID
                    CubemapCompressedImage2D(GL_ETC1_RGB8_OES, inapSourceImages);
#endif
                    break;
                case Core::ImageCompression::k_PVR2Bpp:
#ifndef CS_TARGETPLATFORM_IOS
                    CS_LOG_FATAL("PVR compressed textures are only supported on iOS.");
#else
                    switch(pSourceImage->GetFormat())
                    {
                        default:
                        case Core::Image::Format::k_RGBA8888:
                            CubemapCompressedImage2D(GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG, inapSourceImages);
                            break;
                        case Core::Image::Format::k_RGB888:
                            CubemapCompressedImage2D(GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, inapSourceImages);                            
                            break;
                    };
#endif
                    break;
                case Core::ImageCompression::k_PVR4Bpp:
#ifndef CS_TARGETPLATFORM_IOS
                    CS_LOG_FATAL("PVR compressed textures are only supported on iOS.");
#else
                    switch(pSourceImage->GetFormat())
                    {
                        default:
                        case Core::Image::Format::k_RGBA8888:
                            CubemapCompressedImage2D(GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, inapSourceImages);
                            break;
                        case Core::Image::Format::k_RGB888:
                            CubemapCompressedImage2D(GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG, inapSourceImages);
                            break;
                    };
#endif
                    break;
            };
            
            if(inbWithMipsMaps)
			{
                glGenerateMipmap(GL_TEXTURE_CUBE_MAP); 
			}
		}
		//--------------------------------------------------
		/// Is A
		//--------------------------------------------------
		bool Cubemap::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Cubemap::InterfaceID;
		}
		//--------------------------------------------------
		/// Bind
		//--------------------------------------------------
		void Cubemap::Bind(u32 inSlot)
		{
			if(inSlot > mpRenderCapabilities->GetNumTextureUnits())
			{
				CS_LOG_FATAL("Attempting to bind to texture unit not supported on this device: " + Core::ToString(inSlot));
			}
			
            Texture::SetActiveTextureSlot(inSlot);
            Texture::Bind(GL_TEXTURE_CUBE_MAP, inSlot, mGLTextureID, (u8*)this);

            //Check if the filter params have changed
            if(mbHasTextureFilterModeChanged)
            {
                UpdateTextureParameters();
            }
		}
        //--------------------------------------------------
		/// Set Filter
		///
		/// Set the filtering mode
		/// @param S filter mode
		/// @param T filter mode
		//--------------------------------------------------
		void Cubemap::SetFilter(Rendering::Texture::Filter ineSFilter, Rendering::Texture::Filter ineTFilter)
		{
			meSFilter = ineSFilter;
			meTFilter = ineTFilter;
            
            mbHasTextureFilterModeChanged = true;
		}
		//--------------------------------------------------
		/// Set Wrap Mode
		///
		/// Set the texture wrap mode
		/// @param S wrap mode
		/// @param T wrap mode
		//--------------------------------------------------
		void Cubemap::SetWrapMode(Rendering::Texture::WrapMode inSWrapMode, Rendering::Texture::WrapMode inTWrapMode)
		{
			meSWrapMode = inSWrapMode;
			meTWrapMode = inTWrapMode;
            
            mbHasTextureFilterModeChanged = true;
		}
        //---------------------------------------------------
        /// Update Texture Parameters
        //---------------------------------------------------
        void Cubemap::UpdateTextureParameters()
        {
            mbHasTextureFilterModeChanged = false;
            
            switch(meSWrapMode)
            {
                default:
                case Rendering::Texture::WrapMode::k_clamp:
                    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
                    break;
                case Rendering::Texture::WrapMode::k_repeat:
                    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_REPEAT);
                    break;
            };
            switch(meTWrapMode)
            {
                default:
                case Rendering::Texture::WrapMode::k_clamp:
                    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
                    break;
                case Rendering::Texture::WrapMode::k_repeat:
                    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_REPEAT);
                    break;
            };
            
            if(!mbHasMipMaps)
            {
                switch (meSFilter)
                {
                    default:
                    case Rendering::Texture::Filter::k_point:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,  GL_NEAREST);
                        break;
                    case Rendering::Texture::Filter::k_linear:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,  GL_LINEAR);
                        break;
                }
                switch (meTFilter)
                {
                    default:
                    case Rendering::Texture::Filter::k_point:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                        break;
                    case Rendering::Texture::Filter::k_linear:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        break;
                }
            }
            else
            {
                switch (meSFilter)
                {
                    default:
                    case Rendering::Texture::Filter::k_point:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,  GL_NEAREST_MIPMAP_NEAREST);
                        break;
                    case Rendering::Texture::Filter::k_linear:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_NEAREST);
                        break;
                }
                switch (meTFilter)
                {
                    default:
                    case Rendering::Texture::Filter::k_point:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                        break;
                    case Rendering::Texture::Filter::k_linear:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        break;
                }
            }
        }
		//--------------------------------------------------
		/// Unbind
		//--------------------------------------------------
		void Cubemap::Unbind()
		{
            Texture::Unbind((u8*)this);
		}
		//--------------------------------------------------
		/// Get Texture ID
		//--------------------------------------------------
		GLuint Cubemap::GetTextureID() const 
		{
			return mGLTextureID;
		}
        //--------------------------------------------------
        /// Has Mip Maps
        //--------------------------------------------------
        bool Cubemap::HasMipMaps() const
        {
            return mbHasMipMaps;
        }
        //--------------------------------------------------
        /// Get Image Format
        //--------------------------------------------------
        Core::Image::Format Cubemap::GetImageFormat() const
        {
            return meImageFormat;
        }
        //--------------------------------------------------
        /// Destructor
        //--------------------------------------------------
        Cubemap::~Cubemap()
        {
            mpCubemapManager->RemoveRestorableCubemap(this);
            Texture::Reset(mGLTextureID, (u8*)this);
        }
	}
}
