/*
 *  GLTexture.cpp
 *  moFlo
 *
 *  Created by Tag Games on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Rendering/OpenGL/Texture/Texture.h>

#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderTarget.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderCapabilities.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Texture/TextureManager.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/ImageFormatConverter.h>
#include <ChilliSource/Core/Math/MathUtils.h>

namespace ChilliSource
{
	namespace OpenGL
	{
        Texture::TextureUnit* Texture::paTextureUnits = nullptr;
        u32 Texture::udwCurrentActiveSlot = 0;
		
		//---------------------------------------------------------------
		/// Constructor
		///
		/// @param Source image to create texture from
		//---------------------------------------------------------------
		Texture::Texture(TextureManager* inpTextureManager)
        : meSFilter(Filter::k_linear), meTFilter(Filter::k_linear), meSWrapMode(WrapMode::k_clamp), meTWrapMode(WrapMode::k_clamp), mGLTexID(0),
        mbHasMipMaps(false), mbHasTextureFilterModeChanged(true), mdwTextureSlot(0), meImageFormat(Core::Image::Format::k_RGBA8888), mpTextureManager(inpTextureManager),
        mpRenderCapabilities(nullptr)
		{
            mpRenderCapabilities = Core::Application::Get()->GetSystem<Rendering::RenderCapabilities>();
            CS_ASSERT(mpRenderCapabilities, "Cannot find required system: Render Capabilities.");
		}
		//--------------------------------------------------
		/// Init
		///
		/// @param Width
		/// @param Height
		//--------------------------------------------------
		void Texture::Init(u32 inudwWidth, u32 inudwHeight, Core::Image::Format ineFormat)
		{
            ErrorCheck(inudwWidth, inudwHeight);
			
			SetWidth(inudwWidth);
			SetHeight(inudwHeight);
            
            meImageFormat = ineFormat;
            
			glGenTextures(1, &mGLTexID);
            Bind();

            mbHasTextureFilterModeChanged = true;
            
            UpdateTextureParameters();
			
			switch (ineFormat)
            {
                default:
                case Core::Image::Format::k_RGBA8888:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, inudwWidth, inudwHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
                    break;
                case Core::Image::Format::k_RGB888:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, inudwWidth, inudwHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
                    break;
                case Core::Image::Format::k_RGBA4444:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, inudwWidth, inudwHeight, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, nullptr);
                    break;
                case Core::Image::Format::k_RGB565:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, inudwWidth, inudwHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, nullptr);
                    break;
                case Core::Image::Format::k_LumA88:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, inudwWidth, inudwHeight, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, nullptr);
                    break;
                case Core::Image::Format::k_Lum8:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, inudwWidth, inudwHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, nullptr);
                    break;
                case Core::Image::Format::k_Depth16:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, inudwWidth, inudwHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, nullptr);
                    break;
                case Core::Image::Format::k_Depth32:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, inudwWidth, inudwHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);
                    break;
            };
		}
		//--------------------------------------------------
		/// Init
		///
		/// @param Source image
		/// @param Whether to create mip maps
		//--------------------------------------------------
		void Texture::Init(Core::Image * pSourceImage)
		{
            meImageFormat = pSourceImage->GetFormat();
            
            ErrorCheck(pSourceImage->GetWidth(), pSourceImage->GetHeight());
			
			SetWidth(pSourceImage->GetWidth());
			SetHeight(pSourceImage->GetHeight());
			
			glGenTextures(1, &mGLTexID);
            Bind();
            
            mbHasTextureFilterModeChanged = true;
			mbHasMipMaps = false;
            
            UpdateTextureParameters();
			
			//We must decide here whether to compress the texture or not. We use PVR compression
			switch(pSourceImage->GetCompression())
			{
				default:
				case Core::ImageCompression::k_none:
					switch(pSourceImage->GetFormat())
                    {
                        default:
                        case Core::Image::Format::k_RGBA8888:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pSourceImage->GetWidth(), pSourceImage->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pSourceImage->GetData());
                            break;
                        case Core::Image::Format::k_RGB888:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pSourceImage->GetWidth(), pSourceImage->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, pSourceImage->GetData());
                            break;
                        case Core::Image::Format::k_RGBA4444:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pSourceImage->GetWidth(), pSourceImage->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, pSourceImage->GetData());
                            break;
                        case Core::Image::Format::k_RGB565:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pSourceImage->GetWidth(), pSourceImage->GetHeight(), 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pSourceImage->GetData());
                            break;
                        case Core::Image::Format::k_LumA88:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, pSourceImage->GetWidth(), pSourceImage->GetHeight(), 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, pSourceImage->GetData());
                            break;
                        case Core::Image::Format::k_Lum8:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, pSourceImage->GetWidth(), pSourceImage->GetHeight(), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pSourceImage->GetData());
                            break;
                    };
					break;
				case Core::ImageCompression::k_ETC1:
#ifdef CS_TARGETPLATFORM_ANDROID
					glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, pSourceImage->GetWidth(), pSourceImage->GetHeight(), 0, pSourceImage->GetDataLength(), pSourceImage->GetData());
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
                            glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG, pSourceImage->GetWidth(), pSourceImage->GetHeight(), 0, pSourceImage->GetDataLength(), pSourceImage->GetData());
                            break;
                        case Core::Image::Format::k_RGB888:
                            glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, pSourceImage->GetWidth(), pSourceImage->GetHeight(), 0, pSourceImage->GetDataLength(), pSourceImage->GetData());
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
                            glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, pSourceImage->GetWidth(), pSourceImage->GetHeight(), 0, pSourceImage->GetDataLength(), pSourceImage->GetData());
                            break;
                        case Core::Image::Format::k_RGB888:
                            glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG, pSourceImage->GetWidth(), pSourceImage->GetHeight(), 0, pSourceImage->GetDataLength(), pSourceImage->GetData());
                            break;
                    };
#endif
					break;
			};
            
            if(mbHasMipMaps)
			{
                glGenerateMipmap(GL_TEXTURE_2D); 
			}
		}
        //----------------------------------------------------------------
        /// Create Image From Texture
        ///
        /// @param Image To Initialize from this Texture
        //----------------------------------------------------------------
        bool Texture::CreateImage(Core::ImageSPtr& outpImage)
        {           
            GLuint udwFrameBufferID;
            
            //Generate Buffers
            glGenFramebuffers(1, &udwFrameBufferID);
 
            //Make sure Texture unbound (see glFramebufferTexture2D specs)
            //Unbinding all texture units since cache can have been trashed in previous call
            for(u32 i = 0; i < mpRenderCapabilities->GetNumTextureUnits(); ++i)
            {
                if(paTextureUnits[i].pObjectID != nullptr)
                {
                    glActiveTexture(GL_TEXTURE0 + i);
                    udwCurrentActiveSlot = paTextureUnits[i].dwTextureSlot;
                    glBindTexture(GL_TEXTURE_2D, 0);
                    paTextureUnits[i].dwTextureSlot = -1;
                    paTextureUnits[i].pObjectID = nullptr;
                }
            }
            
            //Bind Buffers
            glBindFramebuffer(GL_FRAMEBUFFER, udwFrameBufferID);
            
            //attach texture
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mGLTexID, 0);

            GLuint udwCheck = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if(udwCheck != GL_FRAMEBUFFER_COMPLETE)
            {
                CS_LOG_ERROR("Framebuffer Not Complete!");
                return false;
            }
            
            //read the data from the texture
            u32 udwSize = GetWidth() * GetHeight() * 4;
            u8* pData = (u8*)malloc(udwSize);
            if (!pData)
                return false;
            
            glReadPixels(0, 0, GetWidth(), GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, pData);
            
            outpImage = Core::ImageSPtr(new Core::Image());
            outpImage->SetCompression(Core::ImageCompression::k_none);
            outpImage->SetDataLength(udwSize);
            outpImage->SetFormat(Core::Image::Format::k_RGBA8888);
            outpImage->SetWidth(GetWidth());
            outpImage->SetHeight(GetHeight());
            outpImage->SetData((u8*)pData);
            
            //convert the RGBA8888 data taken from the texture to the intended format.
            switch (meImageFormat)
            {
                case Core::Image::Format::k_RGBA8888:
                    Core::ImageFormatConverter::RGBA8888ToRGB888(outpImage.get());
                    break;
                case Core::Image::Format::k_RGBA4444:
                    Core::ImageFormatConverter::RGBA8888ToRGBA4444(outpImage.get());
                    break;
                case Core::Image::Format::k_RGB565:
                    Core::ImageFormatConverter::RGBA8888ToRGB565(outpImage.get());
                    break;
                case Core::Image::Format::k_LumA88:
                    Core::ImageFormatConverter::RGBA8888ToLUMA88(outpImage.get());
                    break;
                case Core::Image::Format::k_Lum8:
                    Core::ImageFormatConverter::RGBA8888ToLUM8(outpImage.get());
                    break;
                case Core::Image::Format::k_Depth32:
                    Core::ImageFormatConverter::RGBA8888ToDepth32(outpImage.get());
                    break;
                case Core::Image::Format::k_Depth16:
                    Core::ImageFormatConverter::RGBA8888ToDepth16(outpImage.get());
                    break;
                default:
                    break;
            }

            RenderTarget::ClearCache();
            ClearCache();

            glBindFramebuffer(GL_FRAMEBUFFER, 0); // Hack to resolve implementation issue for glDeleteFramebuffers not resetting bound buffer to zero
            glDeleteFramebuffers(1, &udwFrameBufferID);
            
            return true;
        }
		//--------------------------------------------------
		/// Clear Cache
		///
		/// Flush the currently bound texture cache
		//--------------------------------------------------
		void Texture::ClearCache()
		{
            if (paTextureUnits != nullptr)
            {
                Rendering::RenderCapabilities* pRenderCapabilities = Core::Application::Get()->GetSystem<Rendering::RenderCapabilities>();
                CS_ASSERT(pRenderCapabilities, "Cannot find required system: Render Capabilities.");
                
                for(u32 i = 0; i < pRenderCapabilities->GetNumTextureUnits(); ++i)
                {
                    //If there is a texture at that slot we must unbind him
                    if(paTextureUnits[i].pObjectID)
                    {
                        paTextureUnits[i].dwTextureSlot = -1;
                        paTextureUnits[i].pObjectID = nullptr;
                    }
                }
            }

	        udwCurrentActiveSlot = 0;
	        glActiveTexture(GL_TEXTURE0);
		}
        //---------------------------------------------------
        /// Error Check
        ///
        /// Throw a warning if the texture is non power of
        /// two or if it exceeds the maximum texture size
        ///
        /// @param Width
        /// @param Height
        //---------------------------------------------------
        void Texture::ErrorCheck(u32 inudwWidth, u32 inudwHeight)
        {
            Rendering::RenderCapabilities* pRenderCapabilities = Core::Application::Get()->GetSystem<Rendering::RenderCapabilities>();
            CS_ASSERT(pRenderCapabilities, "Cannot find required system: Render Capabilities.");
            
			if(inudwWidth > pRenderCapabilities->GetMaxTextureSize() || inudwHeight > pRenderCapabilities->GetMaxTextureSize())
			{
				CS_LOG_ERROR("OpenGL ES does not support textures of this size on this hardware - Empty texture");
			}
        }
		//--------------------------------------------------
		/// Is A
		///
		/// @param Interface ID type
		/// @return Whether the object is of this type
		//--------------------------------------------------
		bool Texture::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Texture::InterfaceID;
		}
		//--------------------------------------------------
		/// Bind
		///
		/// @param Texture unit to bind texture to (default 0)
		//--------------------------------------------------
		void Texture::Bind(u32 inSlot)
		{
			if(inSlot > mpRenderCapabilities->GetNumTextureUnits())
			{
				CS_LOG_FATAL("Attempting to bind to texture unit not supported on this device: " + Core::ToString(inSlot));
			}
			
            SetActiveTextureSlot(inSlot);
            Bind(GL_TEXTURE_2D, inSlot, mGLTexID, (u8*)this);
            
            //Check if the filter params have changed
            if(mbHasTextureFilterModeChanged)
            {
                UpdateTextureParameters();
            }
		}
        //---------------------------------------------------
        /// Bind
        //---------------------------------------------------
        void Texture::Bind(GLenum inType, u32 inudwSlot, GLint inTextureID, u8* inpObjectID)
        {
            if(!paTextureUnits)
			{
                Rendering::RenderCapabilities* pRenderCapabilities = Core::Application::Get()->GetSystem<Rendering::RenderCapabilities>();
                CS_ASSERT(pRenderCapabilities, "Cannot find required system: Render Capabilities.");
                
                //Create the available texture unit slots
                paTextureUnits = new TextureUnit[pRenderCapabilities->GetNumTextureUnits()];
			}
            
            if(paTextureUnits[inudwSlot].pObjectID != inpObjectID)
			{
                //If there is a texture at that slot we must unbind him
                if(paTextureUnits[inudwSlot].pObjectID)
                {
                    paTextureUnits[inudwSlot].dwTextureSlot = -1;
                }
                
                //Bind the texture to the active slot
                glBindTexture(inType, inTextureID);
                paTextureUnits[inudwSlot].pObjectID = inpObjectID;
                paTextureUnits[inudwSlot].dwTextureSlot = inudwSlot;
            }
        }
        //---------------------------------------------------
        /// Set Active Texture Slot
        //---------------------------------------------------
        void Texture::SetActiveTextureSlot(u32 inudwSlot)
        {
            if(inudwSlot != udwCurrentActiveSlot)
            {
                udwCurrentActiveSlot = inudwSlot;
                glActiveTexture(GL_TEXTURE0 + inudwSlot);
            }
        }
        //---------------------------------------------------
        /// Update Texture Parameters
        ///
        /// Update the texture filter and repeat modes
        //---------------------------------------------------
        void Texture::UpdateTextureParameters()
        {
            mbHasTextureFilterModeChanged = false;
            
            switch(meSWrapMode)
            {
                default:
                case WrapMode::k_clamp:
                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
                    break;
                case WrapMode::k_repeat:
                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
                    break;
            };
            switch(meTWrapMode)
            {
                default:
                case WrapMode::k_clamp:
                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
                    break;
                case WrapMode::k_repeat:
                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
                    break;
            };
            
            if(!mbHasMipMaps)
            {
                switch (meSFilter)
                {
                    default:
                    case Filter::k_point:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_NEAREST);
                        break;
                    case Filter::k_linear:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR);
                        break;
                }
                switch (meTFilter)
                {
                    default:
                    case Filter::k_point:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                        break;
                    case Filter::k_linear:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        break;
                }
            }
            else
            {
                switch (meSFilter)
                {
                    default:
                    case Filter::k_point:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_NEAREST_MIPMAP_NEAREST);
                        break;
                    case Filter::k_linear:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_NEAREST);
                        break;
                }
                switch (meTFilter)
                {
                    default:
                    case Filter::k_point:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                        break;
                    case Filter::k_linear:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        break;
                }
            }
        }
        //--------------------------------------------------
		/// Unbind
		///
		/// Unbind this texture from a slot if it is bound
		//--------------------------------------------------
		void Texture::Unbind()
		{
            Unbind((u8*)this);
		}
		//--------------------------------------------------
		/// Unbind
		///
		/// Unbind the object with the given ID
        ///
        /// @param Object ID
		//--------------------------------------------------
		void Texture::Unbind(u8* inpObjectID)
		{
            Rendering::RenderCapabilities* pRenderCapabilities = Core::Application::Get()->GetSystem<Rendering::RenderCapabilities>();
            CS_ASSERT(pRenderCapabilities, "Cannot find required system: Render Capabilities.");
            
            for (u32 i = 0; i < pRenderCapabilities->GetNumTextureUnits(); ++i)
            {
                //If there is a texture at that slot we must unbind him
                if(paTextureUnits[i].pObjectID == inpObjectID)
                {
                    glActiveTexture(GL_TEXTURE0 + i);
                    udwCurrentActiveSlot = paTextureUnits[i].dwTextureSlot;
                    glBindTexture(GL_TEXTURE_2D, 0);
                    paTextureUnits[i].dwTextureSlot = -1;
                    paTextureUnits[i].pObjectID = nullptr;
                    break;
                }
            }
		}
		//--------------------------------------------------
		/// Get Texture ID
		///
		/// @return The GL generated texture handle
		//--------------------------------------------------
		GLuint Texture::GetTextureID() const 
		{
			return mGLTexID;
		}
        //--------------------------------------------------
		/// Has Mip Maps
		///
		/// @return Whether or not this was told to use
		///			mip maps
		//--------------------------------------------------
		bool Texture::HasMipMaps() const
		{
			return mbHasMipMaps;
		}
		//--------------------------------------------------
		/// Get Image Format
		///
		/// @return The format of the image used to create
		///			the texture.
		//--------------------------------------------------
		Core::Image::Format Texture::GetImageFormat() const
		{
			return meImageFormat;
		}
		//--------------------------------------------------
		/// Set Filter
		///
		/// Set the filtering mode
		/// @param S filter mode
		/// @param T filter mode
		//--------------------------------------------------
		void Texture::SetFilter(Filter ineSFilter, Filter ineTFilter)
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
		void Texture::SetWrapMode(WrapMode inSWrapMode, WrapMode inTWrapMode)
		{
			meSWrapMode = inSWrapMode;
			meTWrapMode = inTWrapMode;
            
            mbHasTextureFilterModeChanged = true;
		}
		//--------------------------------------------------
        /// Reset
        ///
        /// Restores the texture to it's state prior to
        /// calling init. Init will need to be called
        /// again before the texture can be used.
        ///
        /// @param Object ID
        //--------------------------------------------------
        void Texture::Reset(GLuint& inTextureID, u8* inpObjectID)
        {
            if(inTextureID > 0)
            {
                Rendering::RenderCapabilities* pRenderCapabilities = Core::Application::Get()->GetSystem<Rendering::RenderCapabilities>();
				if(pRenderCapabilities == nullptr)
					return;

                for(u32 i = 0; i < pRenderCapabilities->GetNumTextureUnits(); ++i)
                {
                    //If there is a texture at that slot we must unbind him
                    if(paTextureUnits[i].pObjectID == inpObjectID)
                    {
                        paTextureUnits[i].dwTextureSlot = -1;
                        paTextureUnits[i].pObjectID = nullptr;
                        break;
                    }
                    
                    glDeleteTextures(1, &inTextureID);
                    inTextureID = 0;
                }
            }
        }
        //--------------------------------------------------
        /// Reset
        ///
        /// Restores the texture to it's state prior to
        /// calling init. Init will need to be called
        /// again before the texture can be used.
        //--------------------------------------------------
        void Texture::Reset()
        {
        	Reset(mGLTexID, (u8*)this);
        }
		//--------------------------------------------------
		/// Destructor
		///
		//--------------------------------------------------
		Texture::~Texture()
		{
			mpTextureManager->RemoveRestorableTexture(this);
			Reset(mGLTexID, (u8*)this);
		}
	}
}
