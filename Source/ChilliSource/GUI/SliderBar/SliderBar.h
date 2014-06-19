//
//  Slider.h
//  moFloTest
//
//  Created by Scott Downie on 09/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//


#ifndef _CHILLISOURCE_GUI_SLIDER_BAR_H_
#define _CHILLISOURCE_GUI_SLIDER_BAR_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Base/GUIView.h>

namespace ChilliSource
{
	namespace GUI
	{
		class SliderBar : public GUIView
		{
		public:

			DECLARE_META_CLASS(SliderBar)

			SliderBar();
			SliderBar(const Core::ParamDictionary& insParams);

			virtual ~SliderBar(){}

			//----------------------------------------------
			/// Get Value
			///
			/// @return Normalised value of the slider pos
			//----------------------------------------------
			f32 GetValue() const;
			//----------------------------------------------
			/// Get Value
			///
			/// @param Normalised value of the slider pos
			//----------------------------------------------			
			void SetValue(f32 infValue);

		protected:

			f32 mfSliderValue;
		};
	}
}

#endif