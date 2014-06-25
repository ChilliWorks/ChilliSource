//
//  FormattedLabel.h
//  Chilli Source
//  Created by Scott Downie on 14/06/2011.
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

#ifndef _CHILLISOURCE_GUI_FORMATTED_LABEL_H_
#define _CHILLISOURCE_GUI_FORMATTED_LABEL_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Label/Label.h>

namespace ChilliSource
{
	namespace GUI
	{
		//============================================================
		/// Formatted Label
		///
		/// Uses a mark-up language to format the string via embedded
		/// images, hyperlinks etc
		//============================================================
		class FormattedLabel : public Label
		{
		public:

			DECLARE_META_CLASS(FormattedLabel)

			FormattedLabel();
			FormattedLabel(const Core::ParamDictionary& insParams);

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
			/// @param Text string (UTF-8)
			/// @param Params
            /// @param Bool Overwrite
			//-------------------------------------------------------
			void SetText(const std::string& instrText, const Core::ParamDictionary& insParams,bool inbOverwriteText = false);
		};
	}
}

#endif