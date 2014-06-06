//
//  FormattedLabel.cpp
//  moFloTest
//
//  Created by Scott Downie on 14/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
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
