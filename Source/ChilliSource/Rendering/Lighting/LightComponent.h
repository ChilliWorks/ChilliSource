/*
 *  LightComponent.h
 *  moFlo
 *
 *  Created by Scott Downie on 08/12/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_RENDERING_COMPONENTS_LIGHTCOMPONENT_H_
#define _MOFLOW_RENDERING_COMPONENTS_LIGHTCOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Entity/Component.h>
#include <ChilliSource/Core/Math/Matrix4x4.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class LightComponent : public Core::IComponent
		{
		public:
			DECLARE_NAMED_INTERFACE(LightComponent);
            
            //----------------------------------------------------------
            /// Constructor
            //----------------------------------------------------------
			LightComponent();
            //----------------------------------------------------------
			/// Set Colour
			///
			/// @param Colour of the light
			//----------------------------------------------------------
			void SetColour(const Core::CColour &inColour);
            //----------------------------------------------------------
			/// Get Colour
			///
			/// @return Colour of the light with intensity applied
			//----------------------------------------------------------
            const Core::CColour& GetColour() const;
            //----------------------------------------------------------
			/// Get World Position
			///
			/// @return Position in world space
			//----------------------------------------------------------
			const Core::CVector3& GetWorldPosition() const;
            //----------------------------------------------------------
			/// Set Intensity
			///
			/// @param Intensity scaler
			//----------------------------------------------------------
			void SetIntensity(f32 infIntensity);
            //----------------------------------------------------------
			/// Get Intensity
			///
			/// @return Intensity scaler
			//----------------------------------------------------------
			f32 GetIntensity() const;
            //----------------------------------------------------------
            /// Get Light Matrix
            ///
            /// @return Matrix to transform into light space
            //----------------------------------------------------------
            virtual const Core::CMatrix4x4& GetLightMatrix() const = 0;
            //----------------------------------------------------------
            /// Is Cache Valid
            ///
            /// @return Have any lighting values changed since the last
            /// render
            //----------------------------------------------------------
            bool IsCacheValid() const;
            //----------------------------------------------------------
            /// Calculate Lighting Values
            ///
            /// This calculates any lighting values that are based upon
            /// other lighting values. This is called whenever the light
            /// cache is invalidated.
            //----------------------------------------------------------
            virtual void CalculateLightingValues() {};
            //----------------------------------------------------------
            /// Set Cache Valid
            ///
            /// After rendering the cache is validated
            //----------------------------------------------------------
            void SetCacheValid();
            
        protected:
            
            mutable Core::CMatrix4x4 mmatLight;
            
            Core::CColour mColour;
            mutable Core::CColour mIntenseColour;
            
            f32 mfIntensity;
            
            bool mbCacheValid;
		};
    }
}

#endif