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

namespace moFlo
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
            GLenum FormatConverter(Core::CImage::Format ineFormat)
            {
                switch(ineFormat)
                {
                    default:
                    case Core::CImage::Format::k_RGBA8888:
                        return GL_RGBA;
                        break;
                    case Core::CImage::Format::k_RGB888:
                        return GL_RGB;
                        break;
                    case Core::CImage::Format::k_RGBA4444:
                        return GL_RGBA;
                        break;
                    case Core::CImage::Format::k_RGB565:
                        return GL_RGB;
                        break;
                    case Core::CImage::Format::k_LumA88:
                        return GL_LUMINANCE_ALPHA;
                        break;
                    case Core::CImage::Format::k_Lum8:
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
            GLenum TypeConverter(Core::CImage::Format ineFormat)
            {
                switch(ineFormat)
                {
                    default:
                    case Core::CImage::Format::k_RGBA8888:
                        return GL_UNSIGNED_BYTE;
                        break;
                    case Core::CImage::Format::k_RGB888:
                        return GL_UNSIGNED_BYTE;
                        break;
                    case Core::CImage::Format::k_RGBA4444:
                        return GL_UNSIGNED_SHORT_4_4_4_4;
                        break;
                    case Core::CImage::Format::k_RGB565:
                        return GL_UNSIGNED_SHORT_5_6_5;
                        break;
                    case Core::CImage::Format::k_LumA88:
                        return GL_UNSIGNED_BYTE;
                        break;
                    case Core::CImage::Format::k_Lum8:
                        return GL_UNSIGNED_BYTE;
                        break;
                };
            }
            //--------------------------------------------------
            /// Cubemap Image 2D
            ///
            /// @param Array of 6 images
            //--------------------------------------------------
            void CubemapImage2D(const DYNAMIC_ARRAY<Core::ResourcePtr>& inapSourceImages)
            {
                for(u32 i=0; i<6; ++i)
                {
                    Core::CImage* pSourceImage = (Core::CImage*)inapSourceImages[i].get();
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
            void CubemapCompressedImage2D(GLenum inFormat, const DYNAMIC_ARRAY<Core::ResourcePtr>& inapSourceImages)
            {
                for(u32 i=0; i<6; ++i)
                {
                    Core::CImage* pSourceImage = (Core::CImage*)inapSourceImages[i].get();
                    glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, inFormat, pSourceImage->GetWidth(), pSourceImage->GetHeight(), 0, pSourceImage->GetDataLength(), pSourceImage->GetData());
                }
            }
        }
        
        //--------------------------------------------------
		/// Cosntructor
		//--------------------------------------------------
        CCubemap::CCubemap(CCubemapManager* inpManager)
        :mpCubemapManager(inpManager),
        meSFilter(Rendering::ITexture::Filter::k_linear), meTFilter(Rendering::ITexture::Filter::k_linear),
        meSWrapMode(Rendering::ITexture::WrapMode::k_clamp), meTWrapMode(Rendering::ITexture::WrapMode::k_clamp),
        mbHasMipMaps(false), mbHasTextureFilterModeChanged(true), meImageFormat(Core::CImage::Format::k_RGBA8888),
        mpRenderCapabilities(NULL)
        {
            mpRenderCapabilities = Core::CApplication::GetSystemImplementing<Rendering::IRenderCapabilities>();
        }
		//--------------------------------------------------
		/// Init
		//--------------------------------------------------
		void CCubemap::Init(const DYNAMIC_ARRAY<Core::ResourcePtr>& inapSourceImages, bool inbWithMipsMaps)
		{
            MOFLOW_ASSERT(inapSourceImages.size() == 6, "Cubemaps must have 6 face textures");
            
			glGenTextures(1, &mGLTextureID);
			Bind();
			
            mbHasMipMaps = inbWithMipsMaps;
            mbHasTextureFilterModeChanged = true;
            
            UpdateTextureParameters();

            Core::CImage* pSourceImage = (Core::CImage*)inapSourceImages[0].get();
            meImageFormat = pSourceImage->GetFormat();
            
            switch(pSourceImage->GetCompression())
            {
                default:
                case Core::ImageCompression::k_none:
                    CubemapImage2D(inapSourceImages);
                    break;
                case Core::ImageCompression::k_ETC1:
#ifdef TARGET_ANDROID
                    CubemapCompressedImage2D(GL_ETC1_RGB8_OES, inapSourceImages);
#endif
                    break;
                case Core::ImageCompression::k_PVR2Bpp:
#ifndef TARGET_OS_IPHONE
                    FATAL_LOG("PVR compressed textures are only supported on iOS.");
#else
                    switch(pSourceImage->GetFormat())
                    {
                        default:
                        case Core::CImage::Format::k_RGBA8888:
                            CubemapCompressedImage2D(GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG, inapSourceImages);
                            break;
                        case Core::CImage::Format::k_RGB888:
                            CubemapCompressedImage2D(GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, inapSourceImages);                            
                            break;
                    };
#endif
                    break;
                case Core::ImageCompression::k_PVR4Bpp:
#ifndef TARGET_OS_IPHONE
                    FATAL_LOG("PVR compressed textures are only supported on iOS.");
#else
                    switch(pSourceImage->GetFormat())
                    {
                        default:
                        case Core::CImage::Format::k_RGBA8888:
                            CubemapCompressedImage2D(GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, inapSourceImages);
                            break;
                        case Core::CImage::Format::k_RGB888:
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
		bool CCubemap::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == ICubemap::InterfaceID;
		}
		//--------------------------------------------------
		/// Bind
		//--------------------------------------------------
		void CCubemap::Bind(u32 inSlot)
		{
			if(inSlot > mpRenderCapabilities->GetNumTextureUnits())
			{
				FATAL_LOG("Attempting to bind to texture unit not supported on this device: " + STRING_CAST(inSlot));
			}
			
            CTexture::SetActiveTextureSlot(inSlot);
            CTexture::Bind(GL_TEXTURE_CUBE_MAP, inSlot, mGLTextureID, (u8*)this);

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
		void CCubemap::SetFilter(Rendering::ITexture::Filter ineSFilter, Rendering::ITexture::Filter ineTFilter)
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
		void CCubemap::SetWrapMode(Rendering::ITexture::WrapMode inSWrapMode, Rendering::ITexture::WrapMode inTWrapMode)
		{
			meSWrapMode = inSWrapMode;
			meTWrapMode = inTWrapMode;
            
            mbHasTextureFilterModeChanged = true;
		}
        //---------------------------------------------------
        /// Update Texture Parameters
        //---------------------------------------------------
        void CCubemap::UpdateTextureParameters()
        {
            mbHasTextureFilterModeChanged = false;
            
            switch(meSWrapMode)
            {
                default:
                case Rendering::ITexture::WrapMode::k_clamp:
                    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
                    break;
                case Rendering::ITexture::WrapMode::k_repeat:
                    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_REPEAT);
                    break;
            };
            switch(meTWrapMode)
            {
                default:
                case Rendering::ITexture::WrapMode::k_clamp:
                    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
                    break;
                case Rendering::ITexture::WrapMode::k_repeat:
                    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_REPEAT);
                    break;
            };
            
            if(!mbHasMipMaps)
            {
                switch (meSFilter)
                {
                    default:
                    case Rendering::ITexture::Filter::k_point:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,  GL_NEAREST);
                        break;
                    case Rendering::ITexture::Filter::k_linear:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,  GL_LINEAR);
                        break;
                }
                switch (meTFilter)
                {
                    default:
                    case Rendering::ITexture::Filter::k_point:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                        break;
                    case Rendering::ITexture::Filter::k_linear:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        break;
                }
            }
            else
            {
                switch (meSFilter)
                {
                    default:
                    case Rendering::ITexture::Filter::k_point:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,  GL_NEAREST_MIPMAP_NEAREST);
                        break;
                    case Rendering::ITexture::Filter::k_linear:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_NEAREST);
                        break;
                }
                switch (meTFilter)
                {
                    default:
                    case Rendering::ITexture::Filter::k_point:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                        break;
                    case Rendering::ITexture::Filter::k_linear:
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        break;
                }
            }
        }
		//--------------------------------------------------
		/// Unbind
		//--------------------------------------------------
		void CCubemap::Unbind()
		{
            CTexture::Unbind((u8*)this);
		}
		//--------------------------------------------------
		/// Get Texture ID
		//--------------------------------------------------
		GLuint CCubemap::GetTextureID() const 
		{
			return mGLTextureID;
		}
        //--------------------------------------------------
        /// Has Mip Maps
        //--------------------------------------------------
        bool CCubemap::HasMipMaps() const
        {
            return mbHasMipMaps;
        }
        //--------------------------------------------------
        /// Get Image Format
        //--------------------------------------------------
        Core::CImage::Format CCubemap::GetImageFormat() const
        {
            return meImageFormat;
        }
        //--------------------------------------------------
        /// Destructor
        //--------------------------------------------------
        CCubemap::~CCubemap()
        {
            mpCubemapManager->RemoveRestorableCubemap(this);
            CTexture::Reset(mGLTextureID, (u8*)this);
        }
	}
}
