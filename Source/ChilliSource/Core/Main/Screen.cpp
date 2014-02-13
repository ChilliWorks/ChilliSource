/*
*  Screen.cpp
*  moFlo
*
*  Created by Scott Downie on 12/10/2010.
*  Copyright 2010 Tag Games. All rights reserved.
*
*/

#include <ChilliSource/Core/Screen.h>

namespace moFlo
{
	namespace Core
	{
		CVector2 CScreen::mvRawDims;
		CVector2 CScreen::mvOrientedDims;
		ScreenOrientation CScreen::mOrientation = ORIENTATION_UNDEFINED; 
		bool CScreen::mbIsHighDefinition = false;
        f32 CScreen::mfDensityScale = kfDefaultNormalResScreenDensity;
        f32 CScreen::mfInverseDensityScale = kfDefaultNormalResScreenDensity;

		//----------------------------------------------------------------
		/// Set Raw Dimensions
		///
		/// @param Screen size in default orientation
		//----------------------------------------------------------------
		void CScreen::SetRawDimensions(const CVector2 & invRawScreenDimensions)
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
		const CVector2& CScreen::GetRawDimensions() 
		{
			return mvRawDims;
		}
		//----------------------------------------------------------------
		/// Get Oriented Dimensions
		///
		/// @return Vector containing the width and height of the screen
		/// in the current orientation
		//----------------------------------------------------------------
		const CVector2& CScreen::GetOrientedDimensions() 
		{
			return mvOrientedDims;
		}
		//----------------------------------------------------------------
		/// Get Oriented Width
		///
		/// @return Width of the screen
		//----------------------------------------------------------------
		const u32 CScreen::GetOrientedWidth()
		{
			return (u32)mvOrientedDims.x;
		}
		//----------------------------------------------------------------
		/// Get Oriented Height
		///
		/// @return Height of the screen
		//----------------------------------------------------------------
		const u32 CScreen::GetOrientedHeight()
		{
			return (u32)mvOrientedDims.y;
		}
		//----------------------------------------------------------------
		/// Is Portrait Orientation
		///
		/// @param Screen orientation
		/// @return True if the orientation is portrait up or down 
		//----------------------------------------------------------------
		bool CScreen::IsPortraitOrientation(ScreenOrientation inOrientation)
		{
			return inOrientation == PORTRAIT_UP || inOrientation == PORTRAIT_DOWN;
		}
		//----------------------------------------------------------------
		/// Is Landscape Orientation
		///
		/// @param Screen orientation
		/// @return True if the orientation is landscape left or right 
		//----------------------------------------------------------------
		bool CScreen::IsLandscapeOrientation(ScreenOrientation inOrientation)
		{
			return inOrientation == LANDSCAPE_LEFT || inOrientation == LANDSCAPE_RIGHT;
		}
		//----------------------------------------------------------------
		/// Have Same Aspect Ratio
		///
		/// @param Screen orientation
		/// @param Screen orientation
		/// @return True if the orientations are both landscape or portrait
		//----------------------------------------------------------------
		bool CScreen::HaveSameAspectRatio(ScreenOrientation inOrientation1, ScreenOrientation inOrientation2)
		{
			return IsPortraitOrientation(inOrientation1) == IsPortraitOrientation(inOrientation2);
		}
		//----------------------------------------------------------------
		/// Set Orientation
		///
		/// Change the screen orientation
		/// @param New screen orientation
		//----------------------------------------------------------------
		void CScreen::SetOrientation(ScreenOrientation ineTargetOrientation)
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
		ScreenOrientation CScreen::GetOrientation() 
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
        f32 CScreen::GetDensity()
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
        f32 CScreen::GetInverseDensity()
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
        void CScreen::SetDensity(f32 infDensityScalar)
        {
            mfDensityScale = infDensityScalar;
            mfInverseDensityScale = 1.0f/infDensityScalar;
        }
	}
}

