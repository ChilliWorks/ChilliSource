//
//  FormattedLabel.h
//  moFloTest
//
//  Created by Scott Downie on 14/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_GUI_FORMATTED_LABEL_H_
#define _MO_FLO_GUI_FORMATTED_LABEL_H_

#include <ChilliSource/Rendering/GUI/Label.h>

namespace moFlo
{
	namespace GUI
	{
		//============================================================
		/// Formatted Label
		///
		/// Uses a mark-up language to format the string via embedded
		/// images, hyperlinks etc
		//============================================================
		class CFormattedLabel : public CLabel
		{
		public:

			DECLARE_META_CLASS(CFormattedLabel)

			CFormattedLabel();
			CFormattedLabel(const Core::ParamDictionary& insParams);

            //-------------------------------------------------------
			/// Set Text
			///
			/// Set the text string and a param dictionary that
			/// contains any string variables that require
			/// inserting into the string.
			///
			/// For instance "My string contains [var =a] variable
			/// and [var= b] variable called a and b
			///
			/// @param Text string
			/// @param Params
            /// @param Bool Overwrite
			//-------------------------------------------------------
			void SetText(const UTF8String& instrText, const Core::ParamDictionary& insParams,bool inbOverwriteText = false);
		};
	}
}

#endif