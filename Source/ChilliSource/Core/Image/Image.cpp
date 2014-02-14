/*
 *  Image.cpp
 *  moFlo
 *
 *  Created by Stuart McGaw on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Image/Image.h>

#define PVR_TEXTURE_FLAG_TYPE_MASK	0xff

#define PVRTC_VERSION_MATCHES 0x03525650
#define PVRTC_VERSION_MISMATCH 0x50565203

namespace moFlo
{
	namespace Core
	{
		DEFINE_NAMED_INTERFACE(CImage);
		
		const u8 gcstrPVRTexIdentifier[4] = {'P', 'V', 'R', '!'};
		
		const u32 kPVRTextureFlagTypeRGBA4444 = 16;
		const u32 kPVRTextureFlagTypePVRTC_2 = 24;
		const u32 kPVRTextureFlagTypePVRTC_4 = 25;
		
        //Old Texture Format : Not Supported in newer tools
		struct PVRTexHeader
		{
			u32 udwHeaderLength;
			u32 udwHeight;
			u32 udwWidth;
			u32 udwNumMipmaps;
			u32 udwFlags;
			u32 udwDataLength;
			u32 udwBpp;
			u32 udwBitmaskRed;
			u32 udwBitmaskGreen;
			u32 udwBitmaskBlue;
			u32 udwBitmaskAlpha;
			u32 udwPvrTag;
			u32 udwNumSurfs;
		};
        
        //new Texture Format as in v4 of the tool
        struct PVRTCTexHeader
        {
            u32 udwVersion;
            u32 udwFlags;
            u64 udwPixelFormat;
            u32 udwColourSpace;
            u32 udwChannelType;
            u32 udwHeight;
            u32 udwWidth;
            u32 udwDepth;
            u32 udwNumSurfaces;
            u32 udwNumFaces;
            u32 udwMipMapCount;
            u32 udwMetaDataSize;
        };
		
        enum PVRTCFLAGS
        {
            PVRTCFLAGS_NONE = 0x0,
            PVRTCFLAGS_PRE_MULTIPLIED = 0x02
        };
        
        //Anything outside these is not supported on iDevices
        enum PIXELFORMAT
        {
            PIXELFORMAT_PVRTC_2BPP_RGB  = 0,
            PIXELFORMAT_PVRTC_2BPP_RGBA = 1,
            PIXELFORMAT_PVRTC_4BPP_RGB  = 2,
            PIXELFORMAT_PVRTC_4BPP_RGBA = 3
//            PIXELFORMAT_PVRTC2_2BPP     = 4, //Only supported in iPhone 4S and above GPU's
//            PIXELFORMAT_PVRTC2_4BPP     = 5
        };
        
        enum COLOR_SPACE
        {
            CHANNEL_TYPE_LINEAR_RGB = 0,
            CHANNEL_TYPE_SRGB = 1
        };
        
        enum CHANNEL_TYPE
        {
            CHANNEL_TYPE_UNSIGNED_BYTE_NORMALISED = 0,
            CHANNEL_TYPE_SIGNED_BYTE_NORMALISED = 1,
            CHANNEL_TYPE_UNSIGNED_BYTE = 2,
            CHANNEL_TYPE_SIGNED_BYTE = 3,
            CHANNEL_TYPE_UNSIGNED_SHORT_NORMALISED = 4,
            CHANNEL_TYPE_SIGNED_SHORT_NORMALISED = 5,
            CHANNEL_TYPE_UNSIGNED_SHORT = 6,
            CHANNEL_TYPE_SIGNED_SHORT = 7,
            CHANNEL_TYPE_UNSIGNED_INTEGER_NORMALISED = 8,
            CHANNEL_TYPE_SIGNED_INTEGER_NORMALISED = 9,
            CHANNEL_TYPE_UNSIGNED_INTEGER = 10,
            CHANNEL_TYPE_SIGNED_INTEGER = 11,
            CHANNEL_TYPE_FLOAT = 12
        };

        const u8 kBPP[] = 
        {
            16,
            32,
            24,
            16,
            16,
            8
        };
        //----------------------------------------------------------------
		/// Get Format BPP
		///
		/// @param Format
        /// @return Bits per pixel
		//----------------------------------------------------------------
        u8 CImage::GetFormatBPP(CImage::Format ineFormat)
        {
            return kBPP[ineFormat];
        }
        
		//----------------------------------------------------------------
		/// Constructor
		///
		/// Default
		//----------------------------------------------------------------
		CImage::CImage()
		:meFormat(CImage::RGBA_8888), mudwWidth(0), mudwHeight(0), mpRawData(NULL), meCompression(COMPRESSION_NONE), mbHasAlpha(false), mudwDataLength(0), mpImageData(NULL)
		{
			
		}
		//--------------------------------------------------
		/// Set Width
		///
		/// @param Width of the image in pixels
		//--------------------------------------------------
		void CImage::SetWidth(u32 inudwWidth)
		{
			mudwWidth = inudwWidth;
		}
		//--------------------------------------------------
		/// Set Height
		///
		/// @param Height of the image in pixels
		//--------------------------------------------------
		void CImage::SetHeight(u32 inudwHeight)
		{
			mudwHeight = inudwHeight;
		}
		//--------------------------------------------------
		/// Set Data
		///
		/// @param Image data in bytes
		//--------------------------------------------------
		void CImage::SetData(u8* inpData)
		{
			mpRawData = inpData;
			mpImageData = inpData;
		}
		//--------------------------------------------------
		/// Set Format
		///
		/// @param Image format
		//--------------------------------------------------
		void CImage::SetFormat(Format ineFormat)
		{
			meFormat = ineFormat;
		}
		//--------------------------------------------------
		/// Set Compression
		///
		/// @param image compression type
		//--------------------------------------------------
		void CImage::SetCompression(ImageCompression ineCompression)
		{
			meCompression = ineCompression;
		}
		//--------------------------------------------------
		/// Set Data Length
		///
		/// @param data length
		//--------------------------------------------------
		void CImage::SetDataLength(u32 inudwDataLength)
		{
			mudwDataLength = inudwDataLength;
		}
		//----------------------------------------------------------------
		/// Is A
		///
		/// Is the object of the given interface type
		/// @param Interface type to query
		/// @return Whether the object is of given type
		//----------------------------------------------------------------
		bool CImage::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == CImage::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Format
		///
		/// @return Image format (RGB, RGBA, etc)
		//----------------------------------------------------------------
		CImage::Format CImage::GetFormat() const
		{
			return meFormat;
		}
		//----------------------------------------------------------------
		/// Get Width
		///
		/// @return Image width
		//----------------------------------------------------------------
		u32 CImage::GetWidth() const
		{
			return mudwWidth;
		}
		//----------------------------------------------------------------
		/// Get Height
		///
		/// @return Image height
		//----------------------------------------------------------------
		u32 CImage::GetHeight() const
		{
			return mudwHeight;
		}
		//----------------------------------------------------------------
		/// Get Data
		///
		/// @return Raw image data
		//----------------------------------------------------------------
		const u8* CImage::GetData() const
		{
			return mpImageData;
		}
		//----------------------------------------------------------------
		/// Get Name
		///
		/// @return Image label
		//----------------------------------------------------------------
		const std::string& CImage::GetName() const
		{
			return mImageName;
		}
		//----------------------------------------------------------------
		/// Set Name
		///
		/// @param Image label
		//----------------------------------------------------------------
		void CImage::SetName(const std::string& instrName)
		{
			mImageName = instrName;
		}
		//----------------------------------------------------------------
		/// Get Compression
		///
		/// @return PVR image compression type (NONE, 4BPP, 2BPP)
		//----------------------------------------------------------------
		ImageCompression CImage::GetCompression() const
		{
			return meCompression;
		}
		//----------------------------------------------------------------
		/// Get Data Length
		///
		/// @return Return size of the image in bytes
		//----------------------------------------------------------------
		u32 CImage::GetDataLength() const
		{
			return mudwDataLength;
		}
		//--------------------------------------------------
		/// Unpack PVR Data
		///
		/// Unpack the texture data in accordance with the
		/// PVR format
		//--------------------------------------------------
		void CImage::UnpackPVRData()
		{
			//Get the header data from the image file
			PVRTexHeader* pPVRHeader = reinterpret_cast<PVRTexHeader*>(mpRawData);

			//Check that the header of the PVR data is correct
			//We must swap the endianess for the ARM
			u32 udwPvrTag = pPVRHeader->udwPvrTag;
			
            //We check this to maintain support for the older, unsupported header format
			if (gcstrPVRTexIdentifier[0] != ((udwPvrTag >>  0) & 0xff) ||
				gcstrPVRTexIdentifier[1] != ((udwPvrTag >>  8) & 0xff) ||
				gcstrPVRTexIdentifier[2] != ((udwPvrTag >> 16) & 0xff) ||
				gcstrPVRTexIdentifier[3] != ((udwPvrTag >> 24) & 0xff))
			{
				//Unpack with the updated header format
                UnpackPVRTCData();
			}
            else
            {
                UnpackLegacyPVRTCData(pPVRHeader);
            }
		}
        
        //--------------------------------------------------
        /// Unpack Legacy PVRTC Data - Unsupported older format
        ///
        /// Unpack the texture data in accordance with the
        /// PVR format
        //--------------------------------------------------
        void CImage::UnpackLegacyPVRTCData(void* inpPVRHeader)
        {
            PVRTexHeader* pPVRHeader = static_cast<PVRTexHeader*>(inpPVRHeader);
            
			//Get the flags for the compression format
			u32 udwFlags = pPVRHeader->udwFlags;
			u32 udwFormatFlags = udwFlags & PVR_TEXTURE_FLAG_TYPE_MASK;
			
			//The only compression formats we support are 2 bits per pixel and 4 bits per pixel
			if(udwFormatFlags == kPVRTextureFlagTypePVRTC_4 || udwFormatFlags == kPVRTextureFlagTypePVRTC_2)
			{
				if (udwFormatFlags == kPVRTextureFlagTypePVRTC_4)
				{
					meCompression = COMPRESSION_PVR_4BPP;
				}
				else if(udwFormatFlags == kPVRTextureFlagTypePVRTC_2)
				{
					meCompression = COMPRESSION_PVR_2BPP;
				}
				
				if(pPVRHeader->udwBitmaskAlpha > 0)
				{
					mbHasAlpha = true;
				}
				else
				{
					mbHasAlpha = false;
				}
                
				mudwDataLength = pPVRHeader->udwDataLength;
				
				//Now just use the image data without the header
				//Set the pointer to the actual image data
				mpImageData = mpRawData + sizeof(PVRTexHeader);
			}
        }
        
        //--------------------------------------------------
        /// Unpack PVRTC Data
        ///
        /// Unpack the texture data in accordance with the
        /// PVR format
        //--------------------------------------------------
        void CImage::UnpackPVRTCData()
        {
            //Get the header data from the image file
			PVRTCTexHeader* pPVRTCHeader = reinterpret_cast<PVRTCTexHeader*>(mpRawData);

            //Check the version so determine endianess correctness
            if(pPVRTCHeader->udwVersion == PVRTC_VERSION_MISMATCH)
            {
                //TODO:: Endianess is not correct, need to flip bits in the header data, possibly image data?
                FATAL_LOG("CImage::UnpackPVRTCData >> Endianess Check failed for creating PVR");
            }
            
            u64 udwPFormat = pPVRTCHeader->udwPixelFormat;
            
            u32 udwLow32Bits;
            u32 udwHigh32Bits;
            
            udwLow32Bits  = udwPFormat & 0x00000000ffffffff;
            udwHigh32Bits = udwPFormat >> 32; //Shift to right 32bits
            
            bool bSupported = false;
            
            u32 udwWidth = mudwWidth = pPVRTCHeader->udwWidth;
            u32 udwHeight = mudwHeight = pPVRTCHeader->udwHeight;
            
            
            //Calculate the data size for each texture level and respect the minimum number of blocks
            u32 udwBpp = 4;
            
            
            //Where the most significant 4 bytes have been set to ‘0’ the least significant 4 bytes will contain a 32bit unsigned integer value identifying the pixel format.
            if(udwHigh32Bits == 0)
            {
                if(udwLow32Bits == PIXELFORMAT_PVRTC_2BPP_RGB || udwLow32Bits == PIXELFORMAT_PVRTC_2BPP_RGBA)
                {
                    meCompression = COMPRESSION_PVR_2BPP;
                    bSupported = true;
                    
                    //Pixel by pixel block size for 2bpp
					udwBpp = 2;
                    
                    //Set if Alpha in image
                    if(udwLow32Bits == PIXELFORMAT_PVRTC_2BPP_RGBA)
                    {
                        mbHasAlpha = true;
                    }
                    else
                    {
                        mbHasAlpha = false;
                    }
                }
                else if(udwLow32Bits == PIXELFORMAT_PVRTC_4BPP_RGB || udwLow32Bits == PIXELFORMAT_PVRTC_4BPP_RGBA)
                {
                    meCompression = COMPRESSION_PVR_4BPP;
                    bSupported = true;
                    
                    //Pixel by pixel block size for 4bpp
					udwBpp = 4;
                    
                    //Set if Alpha in image
                    if(udwLow32Bits == PIXELFORMAT_PVRTC_4BPP_RGBA)
                    {
                        mbHasAlpha = true;
                    }
                    else
                    {
                        mbHasAlpha = false;
                    }
                }
                else
                {
                    FATAL_LOG("Unrecognised PixelFormat for image");
                }
            }
            else
            {
                //If the most significant 4 bytes contain a value, the full 8 bytes are used to determine the pixel format. The least significant 4 bytes contain the channel order,
                //each byte containing a single character, or a null character if there are fewer than four channels; for example, {‘r’,‘g’,‘b’,‘a’} or {‘r’,‘g’,‘b’,‘\0’}.
                //The most significant 4 bytes state the bit rate for each channel in the same order, each byte containing a single 8bit unsigned integer value,
                //or zero if there are fewer than four channels; for example, {8,8,8,8} or {5,6,5,0}.
                
                //TODO:: Should never reach here through pvr format
                FATAL_LOG("Unimplemented PixelFormat for image");
            }
            
            if(bSupported)
            {
                mudwDataLength = (udwWidth * udwHeight * udwBpp) >> 3;
				
				//Now just use the image data without the header
				//Set the pointer to the actual image data
				mpImageData = mpRawData + sizeof(PVRTCTexHeader);
            }
            else
            {
                FATAL_LOG("Unimplemented PixelFormat for image");
            }
        }
        
		//---------------------------------------------------------------
		/// Destructor
		///
		//---------------------------------------------------------------
		CImage::~CImage()
		{
			free(mpRawData);
		}
	}
}
