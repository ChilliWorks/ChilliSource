//
//  FormattedLabel.cpp
//  moFloTest
//
//  Created by Scott Downie on 14/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <moFlo/GUI/FormattedLabel.h>

#include <moFlo/Core/StringUtils.h>

namespace moFlo
{
    namespace GUI
    {
		DEFINE_META_CLASS(CFormattedLabel)

        //---Mark-up samples
        // [img= image.png/spritesheetkey x=200 y=200]
        // [url= http://www.example.com LinkText]
        // [var= MyVariableName]
        //--------------------------------------------------
        /// Constructor 
        ///
        /// Default
        //--------------------------------------------------
        CFormattedLabel::CFormattedLabel()
        {
            
        }
        //--------------------------------------------------
        /// Constructor 
        ///
        /// @param Param dictionary
        //--------------------------------------------------
        CFormattedLabel::CFormattedLabel(const Core::ParamDictionary& insParam) : CLabel(insParam)
        {
            
        }
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
        //-------------------------------------------------------
        void CFormattedLabel::SetText(const UTF8String& instrText, const Core::ParamDictionary& insParams,bool inbOverwriteText)
        {
			mCachedChars.clear();
			
            //Let us find the variable names in the string
            if(inbOverwriteText)
                CLabel::SetText("");
            
            Core::CStringUtils::InsertVariables(instrText, insParams, Text);
        }
    }
}
