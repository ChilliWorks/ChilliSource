//
//  Slider.cpp
//  Chilli Source
//  Created by Scott Downie on 09/06/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <ChilliSource/GUI/SliderBar/SliderBar.h>

#include <ChilliSource/Core/Math/MathUtils.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(SliderBar)

        //----------------------------------------------
        /// Constructor
        ///
        /// Default
        //----------------------------------------------
        SliderBar::SliderBar() : mfSliderValue(0.0f)
        {
            
        }
        //----------------------------------------------
        /// Constructor
        ///
        /// From Params
        //----------------------------------------------
        SliderBar::SliderBar(const Core::ParamDictionary& insParams) : GUIView(insParams), mfSliderValue(0.0f)
        {
            
        }
        //----------------------------------------------
        /// Get Value
        ///
        /// @return Normalised value of the slider pos
        //----------------------------------------------
        f32 SliderBar::GetValue() const
        {
            return Core::MathUtils::Clamp(mfSliderValue, 0.0f, 1.0f);
        }
        //----------------------------------------------
        /// Set Value
        ///
        /// @param Normalised value of the slider pos
        //----------------------------------------------
        void SliderBar::SetValue(f32 infValue)
        {
            mfSliderValue=Core::MathUtils::Clamp(infValue, 0.0f, 1.0f);
        }		
		
    }
}