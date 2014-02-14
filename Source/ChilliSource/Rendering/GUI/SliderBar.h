//
//  Slider.h
//  moFloTest
//
//  Created by Scott Downie on 09/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//


#ifndef _MO_FLO_GUI_SLIDER_BAR_H_
#define _MO_FLO_GUI_SLIDER_BAR_H_

#include <ChilliSource/Rendering/GUI/GUIView.h>

namespace moFlo
{
	namespace GUI
	{
		class CSliderBar : public CGUIView
		{
		public:

			DECLARE_META_CLASS(CSliderBar)

			CSliderBar();
			CSliderBar(const Core::ParamDictionary& insParams);

			virtual ~CSliderBar(){}

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