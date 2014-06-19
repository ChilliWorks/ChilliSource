//
//  GUIViewFactory.h
//  moFlo
//
//  Created by Scott Downie on 21/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_GUI_GUIVIEWFACTORY_H_
#define _CHILLISOURCE_GUI_GUIVIEWFACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/XML/XMLUtils.h>

#include <functional>

namespace ChilliSource
{
    namespace GUI
    {
        //--------------------------------------------------------
        /// Create GUI
        ///
        /// Templated call to the GUI view constructor
        /// with the given params
        ///
        /// @param Param dictionary
        //--------------------------------------------------------
        template<typename T> GUIViewUPtr CreateGUI(const Core::ParamDictionary & insParams)
        {
            return GUIViewUPtr(new T(insParams));
        }
        
        class GUIViewFactory
        {
        public:  
            typedef std::function<GUIViewUPtr(const Core::ParamDictionary&)> GUIViewCreateDelegate;
            //--------------------------------------------------------
            /// Register Defaults
            ///
            /// Register default views with the factory
            //--------------------------------------------------------
            static void RegisterDefaults();
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
            static GUIViewUPtr CreateGUIViewFromScript(Core::StorageLocation ineStorageLocation, const std::string& instrScriptFile);
			//--------------------------------------------------------
            /// Create GUI View 
            ///
            /// Creates a widget view of the given type
            ///
            /// @return GUI View
            //--------------------------------------------------------
            static GUIViewUPtr CreateGUIView(const std::string& instrTypeName, const Core::ParamDictionary& insParams);
			//--------------------------------------------------------
			/// Register 
			///
			/// Register a custom view type
			///
			/// @param View type name
			//--------------------------------------------------------
			template <typename T> static void Register(const std::string& instrName)
			{
				mmapDelegateToType.insert(std::make_pair(instrName, GUIViewCreateDelegate(&CreateGUI<T>)));
			}
			//--------------------------------------------------------
			/// Register 
			///
			/// Register a custom view type
			///
			/// @param View type name
			//--------------------------------------------------------
			template <typename T, typename BaseType> static void Register(const std::string& instrName)
			{
				mmapDelegateToType.insert(std::make_pair(instrName, GUIViewCreateDelegate(&CreateGUI<T>)));

#ifdef CS_ENABLE_REFLECTION
				T::StaticMetaClass->SetBaseClass(BaseType::StaticMetaClass);
#endif
			}
        private:
            //--------------------------------------------------------
            /// Create View
            ///
            /// Recursive function used to create subviews
            ///
            /// @param View XML element
            /// @return Created view
            //--------------------------------------------------------
            static GUIViewUPtr CreateView(Core::XML::Node* ipViewElement);
            
        private:
            
            typedef std::unordered_map<std::string, GUIViewCreateDelegate> MapDelegateToString;
            static MapDelegateToString mmapDelegateToType;
        };
    }
}

#endif