/*
 *  PngLoader.h
 *  iOSTemplate
 *
 *  Created by Ian Copland on 06/04/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#ifndef _MOFLO_ANDROIDPLATFORM_IMAGELOADING_PNGIMAGE_
#define _MOFLO_ANDROIDPLATFORM_IMAGELOADING_PNGIMAGE_

#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Image/Image.h>

namespace ChilliSource
{
	namespace Android
	{
		//=======================================
		/// PixelRGBA8888
		///
		/// Container for a single RGBA8888 pixel
		//=======================================
		struct PixelRGBA8888
		{
			u8 mbyR;
			u8 mbyG;
			u8 mbyB;
			u8 mbyA;
		};
		//=======================================
		/// PixelRGB888
		///
		/// Container for a single RGB888 pixel
		//=======================================
		struct PixelRGB888
		{
			u8 mbyR;
			u8 mbyG;
			u8 mbyB;
		};
		//=======================================
		/// PixelRGBA4444
		///
		/// Container for a single RGBA4444 pixel
		//=======================================
		struct PixelRGBA4444
		{
			u8 mbyRG;
			u8 mbyBA;
		};
		//=======================================
		/// PixelRGB545
		///
		/// Container for a single RGB545 pixel
		//=======================================
		struct PixelRGB545
		{
			u16 mwData;
		};
		//=======================================
		/// PixelLUMA88
		///
		/// Container for a single LUMA88 pixel
		//=======================================
		struct PixelLUMA88
		{
			u8 mbyLum;
			u8 mbyA;
		};
		//=======================================
		/// PixelLUM8
		///
		/// Container for a single LUM8 pixel
		//=======================================
		struct PixelLUM8
		{
			u8 mbyLum;
		};

		class CPngImage
		{
		public:
			//----------------------------------------------------------------------------------
			/// Constructor
			//----------------------------------------------------------------------------------
			CPngImage();
			//----------------------------------------------------------------------------------
			/// Constructor
			//----------------------------------------------------------------------------------
			CPngImage(Core::StorageLocation ineStorageLocation, std::string instrFilename);
			//----------------------------------------------------------------------------------
			/// Destructor
			//----------------------------------------------------------------------------------
			virtual ~CPngImage();
			//----------------------------------------------------------------------------------
			/// Load PNG
			///
			/// Loads a png from a file within documents or the package.
			///
			/// @param The storage location to load from.
			/// @param The file path.
			//----------------------------------------------------------------------------------
			void Load(Core::StorageLocation ineStorageLocation, std::string instrFilename);
			//----------------------------------------------------------------------------------
			/// Release
			///
			/// Releases the image data
			/// @param  Whether or not to release the image data.
			//----------------------------------------------------------------------------------
			void Release(bool inReleaseImageData = true);
			//----------------------------------------------------------------------------------
			/// Is Loaded
			///
			/// @return whether or not the image is loaded
			//----------------------------------------------------------------------------------
			bool IsLoaded();
			//----------------------------------------------------------------------------------
			/// Get Height
			///
			/// @return the height of the image
			//----------------------------------------------------------------------------------
			s32 GetHeight();
			//----------------------------------------------------------------------------------
			/// Get Width
			///
			/// @return the width of the image
			//----------------------------------------------------------------------------------
			s32 GetWidth();
			//----------------------------------------------------------------------------------
			/// Get Image Data
			///
			/// @return the image data.
			//----------------------------------------------------------------------------------
			u8 * GetImageData();
			//----------------------------------------------------------------------------------
			/// Get Image Format
			///
			/// @return the image format
			//----------------------------------------------------------------------------------
			Core::Image::Format GetImageFormat();
			//----------------------------------------------------------------------------------
			/// Convert Format From RGB888 To LUM8
			///
			/// Changes the image data from RGB888 LUM8
			//----------------------------------------------------------------------------------
			void ConvertFormatFromRGB888ToLUM8();
			//----------------------------------------------------------------------------------
			/// Convert Format From RGB8888 To LUMA88
			///
			/// Changes the image data from RGB8888 LUMA88
			//----------------------------------------------------------------------------------
			void ConvertFormatFromRGB8888ToLUMA88();
			//----------------------------------------------------------------------------------
			/// Convert Format From RGB888 To RGB565
			///
			/// Changes the image data from RGB888 RGB565
			//----------------------------------------------------------------------------------
			void ConvertFormatFromRGB888ToRGB565();
			//----------------------------------------------------------------------------------
			/// Convert Format From RGBA8888 To RGBA4444
			///
			/// Changes the image data from RGBA8888 RGBA4444
			//----------------------------------------------------------------------------------
			void ConvertFormatFromRGBA8888ToRGBA4444();
		protected:
			//----------------------------------------------------------------------------------
			/// Load with lib png
			///
			/// Loads the png data using lib png
			///
			/// @param the stream lib png should use to read the data.
			//----------------------------------------------------------------------------------
			bool LoadWithLibPng(Core::FileStreamSPtr inStream);

			bool mbIsLoaded;
			s32 mdwHeight;
			s32 mdwWidth;
			u8 * mpData;
			Core::Image::Format meFormat;
		};
	}
}

#endif
