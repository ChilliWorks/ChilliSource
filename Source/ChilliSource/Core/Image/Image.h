/*
 *  Image.h
 *  moFlo
 *
 *  Created by Tag Games on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_CORE_IMAGE_H_
#define _MOFLO_CORE_IMAGE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceOld.h>

namespace ChilliSource
{
	namespace Core
	{
		enum class ImageCompression
		{
            k_none,
            k_PVR4Bpp,
            k_PVR2Bpp,
            k_ETC1
		};
		
		class Image : public ResourceOld
		{
		public:
			CS_DECLARE_NAMEDTYPE(Image);
			enum class Format
			{
				k_RGBA4444,
				k_RGBA8888,
				k_RGB888,
				k_RGB565,
				k_LumA88,
				k_Lum8,
                k_Depth16,
                k_Depth32,
                k_default
			};
			
			///@param inpData - The passed pointer becomes owned by the Image and should have been allocated using malloc
			Image();
			~Image();
		
            //----------------------------------------------------------------
            /// Get Format BPP
            ///
            /// @param Format
            /// @return Bits per pixel
            //----------------------------------------------------------------
            static u8 GetFormatBPP(Format ineFormat);
			//----------------------------------------------------------------
			/// Is A
			///
			/// Is the object of the given interface type
			/// @param Interface type to query
			/// @return Whether the object is of given type
			//----------------------------------------------------------------
			virtual bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//----------------------------------------------------------------
			/// Get Format
			///
			/// @return Image format (RGB, RGBA, etc)
			//----------------------------------------------------------------
			Format GetFormat() const;
			//----------------------------------------------------------------
			/// Get Width
			///
			/// @return Image width
			//----------------------------------------------------------------
			u32 GetWidth() const;
			//----------------------------------------------------------------
			/// Get Height
			///
			/// @return Image height
			//----------------------------------------------------------------
			u32 GetHeight() const;
			//----------------------------------------------------------------
			/// Get Data Length
			///
			/// @return Return size of the image in bytes
			//----------------------------------------------------------------
			u32 GetDataLength() const;
			//----------------------------------------------------------------
			/// Get Data
			///
			/// @return Raw image data
			//----------------------------------------------------------------
			const u8* GetData() const;
			//----------------------------------------------------------------
			/// Get Name
			///
			/// @return Image label
			//----------------------------------------------------------------
			const std::string& GetName() const;
			//----------------------------------------------------------------
			/// Set Name
			///
			/// @param Image label
			//----------------------------------------------------------------
			void SetName(const std::string& instrName);
			//----------------------------------------------------------------
			/// Get Compression
			///
			/// @return image compression type
			//----------------------------------------------------------------
			ImageCompression GetCompression() const;
			
			//--------------------------------------------------
			/// Unpack PVR Data
			///
			/// Unpack the texture data in accordance with the
			/// PVR format
			//--------------------------------------------------
			void UnpackPVRData();
            
            //--------------------------------------------------
			/// Unpack Legacy PVRTC Data - Unsupported older format
			///
			/// Unpack the texture data in accordance with the
			/// PVR format
			//--------------------------------------------------
			void UnpackLegacyPVRTCData(void* inpData);
            
            //--------------------------------------------------
			/// Unpack PVRTC Data
			///
			/// Unpack the texture data in accordance with the
			/// PVR format
			//--------------------------------------------------
			void UnpackPVRTCData();
            
			//--------------------------------------------------
			/// Set Width
			///
			/// @param Width of the image in pixels
			//--------------------------------------------------
			void SetWidth(u32 inudwWidth);
			//--------------------------------------------------
			/// Set Height
			///
			/// @param Height of the image in pixels
			//--------------------------------------------------
			void SetHeight(u32 inudwHeight);
			//--------------------------------------------------
			/// Set Data
			///
			/// @param Image data in bytes
			//--------------------------------------------------
			void SetData(u8* inpData);
			//--------------------------------------------------
			/// Set Format
			///
			/// @param Image format
			//--------------------------------------------------
			void SetFormat(Format ineFormat);
			//--------------------------------------------------
			/// Set Compression
			///
			/// @param image compression type
			//--------------------------------------------------
			void SetCompression(ImageCompression ineCompression);
			//--------------------------------------------------
			/// Set Data Length
			///
			/// @param data length
			//--------------------------------------------------
			void SetDataLength(u32 inudwDataLength);
		protected:
			Format meFormat;
			u8* mpRawData;
			u8* mpImageData;
			
			u32 mudwWidth;
			u32 mudwHeight;
			
			u32 mudwDataLength;
		
			std::string mImageName;
			
			ImageCompression meCompression;
			
			bool mbHasAlpha;
		};
		
	}

}
#endif
