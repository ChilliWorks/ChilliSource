//
//  GUIViewFactory.cpp
//  Chilli Source
//  Created by Scott Downie on 21/04/2011.
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

#include <ChilliSource/GUI/Base/GUIViewFactory.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Container/ParamDictionarySerialiser.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/XML/XMLUtils.h>

#include <ChilliSource/GUI/Button/HighlightButton.h>
#include <ChilliSource/GUI/Button/StretchableHighlightButton.h>
#include <ChilliSource/GUI/Button/ToggleButton.h>
#include <ChilliSource/GUI/Container/ScrollView.h>
#include <ChilliSource/GUI/Container/HorizontalList.h>
#include <ChilliSource/GUI/Container/GridView.h>
#include <ChilliSource/GUI/Container/VerticalList.h>
#include <ChilliSource/GUI/Image/ImageView.h>
#include <ChilliSource/GUI/Image/VerticalStretchableImage.h>
#include <ChilliSource/GUI/Image/StretchableImage.h>
#include <ChilliSource/GUI/Label/Label.h>
#include <ChilliSource/GUI/Label/EditableLabel.h>
#include <ChilliSource/GUI/Label/FormattedLabel.h>
#include <ChilliSource/GUI/ProgressBar/HorizontalProgressBar.h>
#include <ChilliSource/GUI/ProgressBar/HorizontalEndcappedProgressBar.h>
#include <ChilliSource/GUI/ProgressBar/VerticalEndcappedProgressBar.h>
#include <ChilliSource/GUI/ProgressBar/HorizontalClippingProgressBar.h>
#include <ChilliSource/GUI/SliderBar/HorizontalSliderBar.h>
#include <ChilliSource/GUI/SliderBar/VerticalSliderBar.h>


namespace ChilliSource
{
	namespace GUI
	{
		GUIViewFactory::MapDelegateToString GUIViewFactory::mmapDelegateToType;

		//--------------------------------------------------------
		/// Register Defaults
		///
		/// Register default views with the factory
		//--------------------------------------------------------
		void GUIViewFactory::RegisterDefaults()
		{
			Register<GUIView>("GUIView");
			Register<ScrollView, GUIView>("ScrollView");
			Register<ImageView, GUIView>("ImageView");
			Register<Label, GUIView>("Label");
			Register<StretchableImage, GUIView>("StretchableImage");
            Register<VerticalStretchableImage, GUIView>("VerticalStretchableImage");
			Register<HorizontalStretchableImage, GUIView>("HorizontalStretchableImage");
			Register<GridView, GUIView>("GridView");
			Register<EditableLabel, Label>("EditableLabel");
            Register<FormattedLabel, Label>("FormattedLabel");
			Register<SliderBar, GUIView>("SliderBar");
			Register<ProgressBar, GUIView>("ProgressBar");
			Register<Button, GUIView>("Button");
            Register<HorizontalList, GUIView>("HorizontalList");
            Register<VerticalList, GUIView>("VerticalList");
            
			//Defaults
			Register<HighlightButton, Button>("HighlightButton");
            Register<ToggleButton, Button>("ToggleButton");
			Register<HorizontalProgressBar, ProgressBar>("HorizontalProgressBar");
            Register<HorizontalSliderBar, SliderBar>("HorizontalSliderBar");
			Register<VerticalSliderBar, SliderBar>("VerticalSliderBar");
			Register<HorizontalEndcappedProgressBar, ProgressBar>("HorizontalEndcappedProgressBar");
            Register<VerticalEndcappedProgressBar, ProgressBar>("VerticalEndcappedProgressBar");
			Register<HorizontalClippingProgressBar, ProgressBar>("HorizontalClippingProgressBar");
            Register<StretchableHighlightButton, Button>("StretchableHighlightButton");
		}
		//--------------------------------------------------------
		/// Create GUI View 
		///
		/// Creates a widget view of the given type
		///
		/// @return GUI View
		//--------------------------------------------------------
		GUIViewUPtr GUIViewFactory::CreateGUIView(const std::string& instrTypeName, const Core::ParamDictionary& insParams)
		{
			//Create the UI of the given type
			MapDelegateToString::iterator it = mmapDelegateToType.find(instrTypeName);
			if(it != mmapDelegateToType.end())
			{
				return (it->second)(insParams);
			}

			return nullptr;
		}
		//--------------------------------------------------------
		/// Create GUI View From Script
		///
		/// Creates a widget view from the given script file
		///
        /// @param The storage location to load from
        /// @param the file path
		/// @param Optional dynamic array to which views will be pushed
		/// @return GUI View
		//--------------------------------------------------------
		GUIViewUPtr GUIViewFactory::CreateGUIViewFromScript(Core::StorageLocation ineStorageLocation, const std::string& instrScriptFile)
		{
			GUIViewUPtr pRootView;

            Core::XMLUPtr xml = Core::XMLUtils::ReadDocument(ineStorageLocation, instrScriptFile);
             CS_ASSERT(xml != nullptr, "Cannot open GUI file: " + instrScriptFile);
            
            Core::XML::Node* pDocRoot = Core::XMLUtils::GetFirstChildElement(xml->GetDocument());
			if(pDocRoot && Core::XMLUtils::GetName(pDocRoot) == "Layout")
			{
				Core::XML::Node* pViewElement = Core::XMLUtils::GetFirstChildElement(pDocRoot);
				if(pViewElement)
				{
					pRootView = CreateView(pViewElement);
				}
			}

			return pRootView;
		}
		//--------------------------------------------------------
		/// Create View
		///
		/// Recursive function used to create subviews
		///
		/// @param View XML element
		/// @return Created view
		//--------------------------------------------------------
		GUIViewUPtr GUIViewFactory::CreateView(Core::XML::Node* inpViewElement)
		{
			//Get the view type
			//Get the param dictionary config values
			GUIViewUPtr pView;
            std::string strType;
            std::string strSource;
            bool bExtern = false;
            Core::StorageLocation eStorageLoc = Core::StorageLocation::k_none;
            
            for(Core::XML::Attribute* pAttr = Core::XMLUtils::GetFirstAttribute(inpViewElement); pAttr != nullptr; pAttr = Core::XMLUtils::GetNextAttribute(pAttr))
            {
                
                if(Core::XMLUtils::GetName(pAttr) == "Type")
                {
                    strType = Core::XMLUtils::GetValue(pAttr);
                }
                else if(Core::XMLUtils::GetName(pAttr) == "Source")
                {
                    bExtern = true;
                    strSource = Core::XMLUtils::GetValue(pAttr);
                }
                else if(Core::XMLUtils::GetName(pAttr) == "StorageLocation")
                {
                    bExtern = true;
                    eStorageLoc = Core::ParseStorageLocation(Core::XMLUtils::GetValue(pAttr));
                }
            }
            
			//Create the UI of the given type
			MapDelegateToString::iterator it = mmapDelegateToType.find(strType);
			if(it != mmapDelegateToType.end())
			{
				Core::ParamDictionary sParams = Core::ParamDictionarySerialiser::FromString(inpViewElement->value());
	
				//Lets load the given type!
				pView = (it->second)(sParams);
                
                if(bExtern)
                {
                    pView->AddSubview(CreateGUIViewFromScript(eStorageLoc, strSource));
                }
                
				//Now we need to do some recursion and load any subviews
                for(Core::XML::Node* pNode = Core::XMLUtils::GetFirstChildElement(inpViewElement); pNode != nullptr; pNode = Core::XMLUtils::GetNextSiblingElement(pNode))
				{
                    pView->AddSubview(CreateView(pNode));
				}

				return pView;
			}
			else
			{
				CS_LOG_ERROR("Cannot create GUI view of type: " + strType);
				return pView;
			}
		}
	}
}
