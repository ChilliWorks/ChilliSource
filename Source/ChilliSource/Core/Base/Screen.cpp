/*
*  Screen.cpp
*  moFlo
*
*  Created by Scott Downie on 12/10/2010.
*  Copyright 2010 Tag Games. All rights reserved.
*
*/

#include <ChilliSource/Core/Base/Screen.h>

namespace ChilliSource
{
	namespace Core
	{
		Vector2 Screen::mvRawDims;
		Vector2 Screen::mvOrientedDims;
		ScreenOrientation Screen::mOrientation = ScreenOrientation::k_undefined;
		bool Screen::mbIsHighDefinition = false;
        f32 Screen::mfDensityScale = kfDefaultNormalResScreenDensity;
        f32 Screen::mfInverseDensityScale = kfDefaultNormalResScreenDensity;

		//----------------------------------------------------------------
		/// Set Raw Dimensions
		///
		/// @param Screen size in default orientation
		//----------------------------------------------------------------
		void Screen::SetRawDimensions(const Vector2 & invRawScreenDimensions)
		{
			mvRawDims = invRawScreenDimensions;

			if(IsPortraitOrientation(mOrientation))
			{
				mvOrientedDims = mvRawDims;
			}
			else
			{
				mvOrientedDims.x = mvRawDims.y;
				mvOrientedDims.y = mvRawDims.x;
			}
		}
		//----------------------------------------------------------------
		/// Get Raw Dimensions
		///
		/// @return Vector containing the width and height of the screen
		//----------------------------------------------------------------
		const Vector2& Screen::GetRawDimensions() 
		{
			return mvRawDims;
		}
		//----------------------------------------------------------------
		/// Get Oriented Dimensions
		///
		/// @return Vector containing the width and height of the screen
		/// in the current orientation
		//----------------------------------------------------------------
		const Vector2& Screen::GetOrientedDimensions() 
		{
			return mvOrientedDims;
		}
		//----------------------------------------------------------------
		/// Get Oriented Width
		///
		/// @return Width of the screen
		//----------------------------------------------------------------
		const u32 Screen::GetOrientedWidth()
		{
			return (u32)mvOrientedDims.x;
		}
		//----------------------------------------------------------------
		/// Get Oriented Height
		///
		/// @return Height of the screen
		//----------------------------------------------------------------
		const u32 Screen::GetOrientedHeight()
		{
			return (u32)mvOrientedDims.y;
		}
		//----------------------------------------------------------------
		/// Is Portrait Orientation
		///
		/// @param Screen orientation
		/// @return True if the orientation is portrait up or down 
		//----------------------------------------------------------------
		bool Screen::IsPortraitOrientation(ScreenOrientation inOrientation)
		{
			return inOrientation == ScreenOrientation::k_portraitUp || inOrientation == ScreenOrientation::k_portraitDown;
		}
		//----------------------------------------------------------------
		/// Is Landscape Orientation
		///
		/// @param Screen orientation
		/// @return True if the orientation is landscape left or right 
		//----------------------------------------------------------------
		bool Screen::IsLandscapeOrientation(ScreenOrientation inOrientation)
		{
			return inOrientation == ScreenOrientation::k_landscapeLeft || inOrientation == ScreenOrientation::k_landscapeRight;
		}
		//----------------------------------------------------------------
		/// Have Same Aspect Ratio
		///
		/// @param Screen orientation
		/// @param Screen orientation
		/// @return True if the orientations are both landscape or portrait
		//----------------------------------------------------------------
		bool Screen::HaveSameAspectRatio(ScreenOrientation inOrientation1, ScreenOrientation inOrientation2)
		{
			return IsPortraitOrientation(inOrientation1) == IsPortraitOrientation(inOrientation2);
		}
		//----------------------------------------------------------------
		/// Set Orientation
		///
		/// Change the screen orientation
		/// @param New screen orientation
		//----------------------------------------------------------------
		void Screen::SetOrientation(ScreenOrientation ineTargetOrientation)
		{
			if(mOrientation != ineTargetOrientation)
			{
				mOrientation = ineTargetOrientation;

				if(IsPortraitOrientation(mOrientation))
				{
					mvOrientedDims = mvRawDims;
				}
				else
				{
					mvOrientedDims.x = mvRawDims.y;
					mvOrientedDims.y = mvRawDims.x;
				}
			}
		}
		//----------------------------------------------------------------
		/// Get Orientation
		///
		/// @return Screen orientation
		//----------------------------------------------------------------
		ScreenOrientation Screen::GetOrientation() 
		{
			return mOrientation;
		}
        //----------------------------------------------------------------
        /// Get Density
        ///
        /// The density scale factor used to convert from DIPS (density
        /// independant pixels) to actual physical pixels. For example
        /// the retina screen has a resolution of 960x640 but will be 
        /// reported as having a screen size of 480x320 DIPS and a
        /// density scalar of 2.0
        ///
        /// @return The density scale factor of the screen
        //----------------------------------------------------------------
        f32 Screen::GetDensity()
        {
            return mfDensityScale;
        }
        //----------------------------------------------------------------
        /// Get Inverse Density
        ///
        /// The density scale factor used to convert from DIPS (density
        /// independant pixels) to actual physical pixels. For example
        /// the retina screen has a resolution of 960x640 but will be
        /// reported as having a screen size of 480x320 DIPS and a
        /// density scalar of 2.0
        ///
        /// @return The inverse of density scale factor of the screen
        //----------------------------------------------------------------
        f32 Screen::GetInverseDensity()
        {
            return mfInverseDensityScale;
        }
        //----------------------------------------------------------------
        /// Set Density
        ///
        /// The density scale factor used to convert from DIPS (density
        /// independant pixels) to actual physical pixels. For example
        /// the retina screen has a resolution of 960x640 but will be 
        /// reported as having a screen size of 480x320 DIPS and a
        /// density scalar of 2.0
        ///
        /// @param The density scale factor of the screen
        //----------------------------------------------------------------
        void Screen::SetDensity(f32 infDensityScalar)
        {
            mfDensityScale = infDensityScalar;
            mfInverseDensityScale = 1.0f/infDensityScalar;
        }
	}
}

