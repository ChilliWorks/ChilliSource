//
//  PointLightComponent.h
//  Chilli Source
//  Created by Scott Downie on 31/01/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef _CHILLISOURCE_RENDERING_COMPONENTS_POINTLIGHTCOMPONENT_H_
#define _CHILLISOURCE_RENDERING_COMPONENTS_POINTLIGHTCOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Lighting/LightComponent.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class PointLightComponent : public LightComponent
		{
		public:
			CS_DECLARE_NAMEDTYPE(PointLightComponent);
			
            //----------------------------------------------------------
            /// Constructor
            //----------------------------------------------------------
			PointLightComponent();
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
            //----------------------------------------------------------
			/// Set Radius
			///
			/// Sets the radius of the point light. This radius is used
            /// to calculate the attenuation constants and the lights
            /// maximum influence distance. Assuming r is the radius,
            /// and L is the Light Attenuation, the following is the
            /// equation used for attenuation:
            ///
            ///   kC = 1
            ///   kL = 2/r
            ///   kQ = 1/(r*r)
            ///   L = 1 / (kC + kL*d + kQ*d*d)
            ///
            /// @param The lights radius.
			//----------------------------------------------------------
			void SetRadius(f32 infRadius);
            //----------------------------------------------------------
			/// Set Min Light Influence
			///
			/// Sets the minimum light influence. This is used to
            /// calculate the Range of Influence of the light. Increasing
            /// the Min Light Influence will reduce the range of the light,
            /// reducing the number of objects that will be influenced
            /// by it. This value should never be 0 or below as this will
            /// mean that all objects will be influenced by the light,
            /// regardless of distance. Assuming L is the Min Light
            /// Influence, I is the light intensity and r is the light radius,
            /// the following is used to calculate d, the Range of Influence:
            ///
            ///  d = r * (sqrt(I / L) + 1)
            ///
            /// @param The minimum light influence.
			//----------------------------------------------------------
			void SetMinLightInfluence(f32 infMinLightInfluence);
            //----------------------------------------------------------
			/// Get Radius
			///
			/// @return Radius of emission
			//----------------------------------------------------------
			f32 GetRadius() const;
            //----------------------------------------------------------
			/// Get Min Light Influence
			///
			/// @return The light attenuation factor at which the light
            /// is considered to no longer be influencing an object.
			//----------------------------------------------------------
			f32 GetMinLightInfluence() const;
            //----------------------------------------------------------
			/// Get Constant Attenuation
			///
			/// @return Constant attenuation factor
			//----------------------------------------------------------
			f32 GetConstantAttenuation() const;
            //----------------------------------------------------------
			/// Get Linear Attenuation
			///
			/// @return Linear attenuation factor
			//----------------------------------------------------------
			f32 GetLinearAttenuation() const;
            //----------------------------------------------------------
			/// Get Quadratic Attenuation
			///
			/// @return Quadratic attenuation factor
			//----------------------------------------------------------
			f32 GetQuadraticAttenuation() const;
            //----------------------------------------------------------
			/// Get Range Of Influence
			///
			/// @return The range in which the light has influence on
            /// objects. This is calculated based on the Min Light
            /// Influence and the radius of the light.
			//----------------------------------------------------------
			f32 GetRangeOfInfluence() const;
            //----------------------------------------------------------
            /// Get Light Matrix
            ///
            /// @return Matrix to transform into light space
            //----------------------------------------------------------
            const Core::Matrix4& GetLightMatrix() const override;
            //----------------------------------------------------
			/// Triggered when the component is attached to
			/// an entity on the scene
            ///
            /// @author S Downie
			//----------------------------------------------------
            void OnAddedToScene() override;
			//----------------------------------------------------
			/// Triggered when the component is removed from
			/// an entity on the scene
            ///
            /// @author S Downie
			//----------------------------------------------------
            void OnRemovedFromScene() override;
            //----------------------------------------------------
            /// On Entity Transform Changed
            ///
            /// Triggered when the entity transform changes
            /// and invalidates the light VP matrix
            //----------------------------------------------------
            void OnEntityTransformChanged();
            //----------------------------------------------------------
            /// Calculate Lighting Values
            ///
            /// Calculates the attenuation constants and the range
            /// of influence based on the light radius and the
            /// minimum light influence value.
            //----------------------------------------------------------
            void CalculateLightingValues();
        private:
            
            Core::EventConnectionUPtr m_transformChangedConnection;
            
            f32 mfConstantAttenuation;
            f32 mfLinearAttenuation;
            f32 mfQuadraticAttenuation;
            f32 mfRadius;
            f32 mfMinLightInfluence;
            f32 mfRangeOfInfluence;
            
            mutable bool mbMatrixCacheValid;
		};
    }
}

#endif