/*
 *  Screen.h
 *  moFlo
 *
 *  Created by Scott Downie on 12/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_SCREEN_H_
#define _MO_FLO_CORE_SCREEN_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>

namespace ChilliSource
{
	namespace Core
	{
		enum class ScreenOrientation
		{
			k_portraitUp     = (1 << 0), //DEFAULT ORIENTATION FOR THE DEVICE
			k_portraitDown	 = (1 << 1), //DEVICE ROTATED 180
			k_landscapeRight = (1 << 2), //DEVICE ROTATED 90 DEGREES CW
			k_landscapeLeft	 = (1 << 3), //DEVICE ROTATED 90 DEGREES CCW
			k_undefined      = (1 << 4)
		};
        
        const f32 kfDefaultNormalResScreenDensity = 1.0f;
        const f32 kfDefaultHighResScreenDensity = 2.0f;
		
		class CScreen
		{
		public:
			//----------------------------------------------------------------
			/// Get Raw Dimensions
			///
			/// @return Vector containing the width and height of the screen
			//----------------------------------------------------------------
			static const CVector2& GetRawDimensions();
			//----------------------------------------------------------------
			/// Get Oriented Dimensions
			///
			/// @return Vector containing the width and height of the screen
			/// in the current orientation
			//----------------------------------------------------------------
			static const CVector2& GetOrientedDimensions();
			//----------------------------------------------------------------
			/// Get Oriented Width
			///
			/// @return Width of the screen
			//----------------------------------------------------------------
			static const u32 GetOrientedWidth();
			//----------------------------------------------------------------
			/// Get Oriented Height
			///
			/// @return Height of the screen
			//----------------------------------------------------------------
			static const u32 GetOrientedHeight();
			//----------------------------------------------------------------
			/// Is Portrait Orientation
			///
			/// @param Screen orientation
			/// @return True if the orientation is portrait up or down 
			//----------------------------------------------------------------
			static bool IsPortraitOrientation(ScreenOrientation inOrientation);
			//----------------------------------------------------------------
			/// Is Landscape Orientation
			///
			/// @param Screen orientation
			/// @return True if the orientation is landscape left or right 
			//----------------------------------------------------------------
			static bool IsLandscapeOrientation(ScreenOrientation inOrientation);
			//----------------------------------------------------------------
			/// Have Same Aspect Ratio
			///
			/// @param Screen orientation
			/// @param Screen orientation
			/// @return True if the orientations are both landscape or portrait
			//----------------------------------------------------------------
			static bool HaveSameAspectRatio(ScreenOrientation inOrientation1, ScreenOrientation inOrientation2);
            //----------------------------------------------------------------
			/// Get Orientation
			///
			/// @return Screen orientation
			//----------------------------------------------------------------
			static ScreenOrientation GetOrientation();
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
            static f32 GetDensity();
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
            static f32 GetInverseDensity();
            //----------------------------------------------------------------
			/// Set Raw Dimensiosn
			///
			/// @param Screen size in default orientation
			//----------------------------------------------------------------
			static void SetRawDimensions(const CVector2 & invRawScreenDimensions);
			//----------------------------------------------------------------
			/// Set Orientation
			///
			/// Change the screen orientation
			/// @param New screen orientation
			//----------------------------------------------------------------
			static void SetOrientation(ScreenOrientation ineTargetOrientation);
		private:
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
            static void SetDensity(f32 infDensityScalar);

			friend class Application;
			friend class CApplicationDelegates;
		private:

			static CVector2 mvRawDims;
			static CVector2 mvOrientedDims;
			static ScreenOrientation mOrientation; 
            static bool mbIsHighDefinition;
            static f32 mfDensityScale;
            static f32 mfInverseDensityScale;
		};
	}
}

#endif
