//
//  FormattedLabel.cpp
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

#include <ChilliSource/GUI/Label/FormattedLabel.h>

#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(FormattedLabel)

        //---Mark-up samples
        // IMPROVEMENT: [img= image.png/atlaskey x=200 y=200]
        // IMPROVEMENT: [url= http://www.example.com LinkText]
        // [var= MyVariableName]
        //--------------------------------------------------
        /// Constructor 
        ///
        /// Default
        //--------------------------------------------------
        FormattedLabel::FormattedLabel()
        {
            
        }
        //--------------------------------------------------
        /// Constructor 
        ///
        /// @param Param dictionary
        //--------------------------------------------------
        FormattedLabel::FormattedLabel(const Core::ParamDictionary& insParam) : Label(insParam)
        {
            
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void FormattedLabel::SetText(const std::string& instrText, const Core::ParamDictionary& insParams,bool inbOverwriteText)
        {
			mCachedChars.clear();
			
            //Let us find the variable names in the string
            if(inbOverwriteText)
                Label::SetText("");
            
            Core::StringUtils::InsertVariables(instrText, insParams, Text);
        }
    }
}
