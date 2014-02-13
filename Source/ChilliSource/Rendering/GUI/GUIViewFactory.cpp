//
//  GUIViewFactory.cpp
//  moFlo
//
//  Created by Scott Downie on 21/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/GUIViewFactory.h>

#include <ChilliSource/Core/Application.h>
#include <ChilliSource/GUI/GUI.h>

#include <ChilliSource/GUI/Defaults/HighlightButton.h>
#include <ChilliSource/GUI/Defaults/ToggleButton.h>
#include <ChilliSource/GUI/Defaults/HorizontalProgressBar.h>
#include <ChilliSource/GUI/Defaults/HorizontalSliderBar.h>
#include <ChilliSource/GUI/Defaults/VerticalSliderBar.h>
#include <ChilliSource/GUI/Defaults/HorizontalEndcappedProgressBar.h>
#include <ChilliSource/GUI/Defaults/VerticalEndcappedProgressBar.h>

#include <ChilliSource/GUI/Defaults/HorizontalClippingProgressBar.h>
#include <ChilliSource/GUI/Defaults/StretchableHighlightButton.h>
#include <ChilliSource/GUI/Defaults/VerticalStretchableImage.h>
#include <ChilliSource/GUI/HorizontalList.h>
#include <ChilliSource/GUI/VerticalList.h>

#include <ChilliSource/Core/XML/rapidxml.hpp>

namespace moFlo
{
	namespace GUI
	{
		CGUIViewFactory::MapDelegateToString CGUIViewFactory::mmapDelegateToType;

		//--------------------------------------------------------
		/// Register Defaults
		///
		/// Register default views with the factory
		//--------------------------------------------------------
		void CGUIViewFactory::RegisterDefaults()
		{
			Register<CGUIView>("GUIView");
			Register<CScrollView, CGUIView>("ScrollView");
			Register<CImageView, CGUIView>("ImageView");
			Register<CLabel, CGUIView>("Label");
			Register<CStretchableImage, CGUIView>("StretchableImage");
            Register<CVerticalStretchableImage, CGUIView>("VerticalStretchableImage");
			Register<CHorizontalStretchableImage, CGUIView>("HorizontalStretchableImage");
			Register<CGridView, CGUIView>("GridView");
			Register<CEditableLabel, CLabel>("EditableLabel");
            Register<CFormattedLabel, CLabel>("FormattedLabel");
			Register<CSliderBar, CGUIView>("SliderBar");
			Register<CProgressBar, CGUIView>("ProgressBar");
			Register<IButton, CGUIView>("Button");
            Register<CHorizontalList, CGUIView>("HorizontalList");
            Register<CVerticalList, CGUIView>("VerticalList");
            
			//Defaults
			Register<CHighlightButton, IButton>("HighlightButton");
            Register<CToggleButton, IButton>("ToggleButton");
			Register<CHorizontalProgressBar, CProgressBar>("HorizontalProgressBar");
            Register<CHorizontalSliderBar, CSliderBar>("HorizontalSliderBar");
			Register<CVerticalSliderBar, CSliderBar>("VerticalSliderBar");
			Register<CHorizontalEndcappedProgressBar, CProgressBar>("HorizontalEndcappedProgressBar");
            Register<CVerticalEndcappedProgressBar, CProgressBar>("VerticalEndcappedProgressBar");
			Register<CHorizontalClippingProgressBar, CProgressBar>("HorizontalClippingProgressBar");
            Register<CStretchableHighlightButton, IButton>("StretchableHighlightButton");
		}
		//--------------------------------------------------------
		/// Create GUI View 
		///
		/// Creates a widget view of the given type
		///
		/// @return GUI View
		//--------------------------------------------------------
		GUIViewPtr CGUIViewFactory::CreateGUIView(const std::string& instrTypeName, const Core::ParamDictionary& insParams)
		{
			//Create the UI of the given type
			MapDelegateToString::iterator it = mmapDelegateToType.find(instrTypeName);
			if(it != mmapDelegateToType.end())
			{
				return (it->second)(insParams);
			}

			return GUIViewPtr();
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
		GUIViewPtr CGUIViewFactory::CreateGUIViewFromScript(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrScriptFile, DYNAMIC_ARRAY<GUIViewPtr>* outpViews)
		{
			GUIViewPtr pRootView;

            Core::FileStreamPtr pFile = Core::CApplication::GetFileSystemPtr()->CreateFileStream(ineStorageLocation, instrScriptFile, Core::FM_READ);
            assert(pFile);
            
            std::string strFile;
            pFile->GetAll(strFile);
			
            //Load the script
            rapidxml::xml_document<> xDoc;
            xDoc.parse<0>((char*)strFile.c_str());
            
			
            rapidxml::xml_node<> * pDocRoot = xDoc.first_node();

			if(pDocRoot && pDocRoot->isNamed("Layout"))
			{
				rapidxml::xml_node<> *  pViewElement = pDocRoot->first_node();
				if(pViewElement)
				{
					pRootView = CreateView(pViewElement, outpViews);
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
		GUIViewPtr CGUIViewFactory::CreateView(rapidxml::xml_node<>* inpViewElement, DYNAMIC_ARRAY<GUIViewPtr>* outpViews)
		{
			//Get the view type
			//Get the param dictionary config values
			GUIViewPtr pView;
            std::string strType;
            std::string strSource;
            bool bExtern = false;
            Core::STORAGE_LOCATION eStorageLoc;
            
            for(rapidxml::xml_attribute<> * pAttr = inpViewElement->first_attribute(); pAttr != NULL; pAttr = pAttr->next_attribute())
            {
                if(pAttr->isNamed("Type"))
                {
                    strType = pAttr->value();
                }
                else if(pAttr->isNamed("Source"))
                {
                    bExtern = true;
                    strSource = pAttr->value();
                }
                else if(pAttr->isNamed("StorageLocation"))
                {
                    bExtern = true;
                    eStorageLoc = Core::GetStorageLocationFromString(pAttr->value());
                }
            }
            
			//Create the UI of the given type
			MapDelegateToString::iterator it = mmapDelegateToType.find(strType);
			if(it != mmapDelegateToType.end())
			{
				Core::ParamDictionary sParams;
				sParams.FromString(inpViewElement->value());

				//Lets load the given type!
				pView = (it->second)(sParams);
                
                if(bExtern)
                {
                    pView->AddSubview(CreateGUIViewFromScript(eStorageLoc, strSource));
                }

				if(outpViews)
				{
					outpViews->push_back(pView);
				}
                
				//Now we need to do some recursion and load any subviews
                for(rapidxml::xml_node<> * pNode = inpViewElement->first_node(); pNode != NULL; pNode = pNode->next_sibling())
				{
                    if(pNode->type() == rapidxml::node_element)
                        pView->AddSubview(CreateView(pNode, outpViews));
				}

				return pView;
			}
			else
			{
				ERROR_LOG("Cannot create GUI view of type: " + strType);
				return pView;
			}
		}
	}
}