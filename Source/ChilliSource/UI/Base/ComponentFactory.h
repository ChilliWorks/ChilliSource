//
//  ComponentFactory.h
//  Chilli Source
//  Created by Ian Copland on 14/11/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#ifndef _CHILLISOURCE_UI_BASE_COMPONENTFACTORY_H_
#define _CHILLISOURCE_UI_BASE_COMPONENTFACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/Property/PropertyMap.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/UI/Base/Component.h>

#include <functional>
#include <unordered_map>

namespace ChilliSource
{
    namespace UI
    {
        //--------------------------------------------------------------------------
        /// A factory for creating new instances of Components from a string
        /// identifier and a property factory. Component types must be registered
        /// with the factory using the Register<>() method before they can be
        /// used in a WidgetDef. Component implementations should provide a
        /// constructor and a static method with the following signatures:
        ///
        /// TComponent(const std::string& in_name, const PropertyMap& in_propMap);
        /// static const std::vector<PropertyMap::PropertyDesc>& GetPropertyDescs();
        ///
        /// @author Ian Copland
        //--------------------------------------------------------------------------
        class ComponentFactory final : public Core::AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(ComponentFactory);
            //-----------------------------------------------------------------
            /// Allows querying of whether or not the system implements the
            /// interface associated with the given interface Id.
            ///
            /// @author Ian Copland
            ///
            /// @param The interface Id.
            ///
            /// @return Whether or not the interface is implemented.
            //-----------------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //-----------------------------------------------------------------
            /// Registers a new component type with the factory. Future calls
            /// to CreateComponent() with the given name will instantiate a
            /// component of this type. The given name must be unique; if a
            /// duplicate component name is registered the app is considered
            /// to be in an irrecoverable state and will terminate.
            ///
            /// This is not thread-safe and should only be called on the main
            /// thread.
            ///
            /// @author Ian Copland
            ///
            /// @param The unique name of the component.
            //-----------------------------------------------------------------
            template <typename TComponentType> void Register(const std::string& in_componentTypeName);
            //-----------------------------------------------------------------
            /// Returns the property descs for the given component type. This
            /// will log a fatal error if the component type has not been
            /// registered.
            ///
            /// @author Ian Copland
            ///
            /// @param The name of the component type to get the property descs
            /// for.
            ///
            /// @return The property descs.
            //-----------------------------------------------------------------
            std::vector<Core::PropertyMap::PropertyDesc> GetPropertyDescs(const std::string& in_componentTypeName) const;
            //-----------------------------------------------------------------
            /// Creates an instance of the component type registered under the
            /// given name with the given owner widget and property map.
            ///
            /// @author Ian Copland
            ///
            /// @param The name of the property type previously registered with
            /// the factory.
            /// @param The name of the component instance.
            /// @param The property map to create the component with.
            ///
            /// @return The new component instance.
            //-----------------------------------------------------------------
            ComponentUPtr CreateComponent(const std::string& in_componentTypeName, const std::string& in_name, const Core::PropertyMap& in_propertyMap) const;
        private:
            friend class Core::Application;
            //-----------------------------------------------------------------
            /// A factory method for creating new instances of the system.
            ///
            /// @author Ian Copland
            ///
            /// @return A new instance of the system.
            //-----------------------------------------------------------------
            static ComponentFactoryUPtr Create();
            //-----------------------------------------------------------------
            /// A delegate which is used to instantiate the registered component
            /// types.
            ///
            /// @author Ian Copland
            ///
            /// @param The name of the component instance.
            /// @param The property map to create the component with.
            ///
            /// @return The newly created component.
            //-----------------------------------------------------------------
            using CreatorDelegate = std::function<ComponentUPtr(const std::string& in_name, const Core::PropertyMap& in_propertyMap)>;
            //-----------------------------------------------------------------
            /// Creates a new instance of the given component type. This is the
            /// method referred to by the creator delegates.
            ///
            /// @author Ian Copland
            ///
            /// @param The name of the component instance.
            /// @param The property map to create the component with.
            ///
            /// @return The newly created component.
            //-----------------------------------------------------------------
            template <typename TComponentType> std::unique_ptr<TComponentType> CreateComponent(const std::string& in_name, const Core::PropertyMap& in_propertyMap) const;
            //-----------------------------------------------------------------
            /// Initialised the factory, registering all default component
            /// types.
            ///
            /// @author Ian Copland
            //-----------------------------------------------------------------
            void OnInit() override;
            //-----------------------------------------------------------------
            /// Deregisters all component types classes.
            ///
            /// @author Ian Copland
            //-----------------------------------------------------------------
            void OnDestroy() override;
            
            std::unordered_map<std::string, CreatorDelegate> m_creatorDelegateMap;
            std::unordered_map<std::string, std::vector<Core::PropertyMap::PropertyDesc>> m_descsMap;
        };
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        template <typename TComponentType> void ComponentFactory::Register(const std::string& in_componentTypeName)
        {
            m_creatorDelegateMap.insert(std::make_pair(in_componentTypeName, Core::MakeDelegate(this, &ComponentFactory::CreateComponent<TComponentType>)));
            m_descsMap.insert(std::make_pair(in_componentTypeName, TComponentType::GetPropertyDescs()));
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        template <typename TComponentType> std::unique_ptr<TComponentType> ComponentFactory::CreateComponent(const std::string& in_name, const Core::PropertyMap& in_propertyMap) const
        {
            return std::unique_ptr<TComponentType>(new TComponentType(in_name, in_propertyMap));
        }
    }
}

#endif