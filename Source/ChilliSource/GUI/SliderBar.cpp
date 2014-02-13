//
//  Slider.cpp
//  moFloTest
//
//  Created by Scott Downie on 09/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <moFlo/GUI/SliderBar.h>

#include <moFlo/Core/Math/MathUtils.h>

namespace moFlo
{
    namespace GUI
    {
		DEFINE_META_CLASS(CSliderBar)

        //----------------------------------------------
        /// Constructor
        ///
        /// Default
        //----------------------------------------------
        CSliderBar::CSliderBar() : mfSliderValue(0.0f)
        {
            
        }
        //----------------------------------------------
        /// Constructor
        ///
        /// From Params
        //----------------------------------------------
        CSliderBar::CSliderBar(const Core::ParamDictionary& insParams) : CGUIView(insParams), mfSliderValue(0.0f)
        {
            
        }
        //----------------------------------------------
        /// Get Value
        ///
        /// @return Normalised value of the slider pos
        //----------------------------------------------
        f32 CSliderBar::GetValue() const
        {
            return Core::CMathUtils::Clamp(mfSliderValue, 0.0f, 1.0f);
        }
        //----------------------------------------------
        /// Set Value
        ///
        /// @param Normalised value of the slider pos
        //----------------------------------------------
        void CSliderBar::SetValue(f32 infValue)
        {
            mfSliderValue=Core::CMathUtils::Clamp(infValue, 0.0f, 1.0f);
        }		
		
    }
}