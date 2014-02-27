//
//  ResourceManagerDispenser.h
//  moFloTest
//
//  Created by Scott Downie on 02/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_CORE_FACTORY_DISPENSER_H_
#define _MO_FLO_CORE_FACTORY_DISPENSER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

#define GET_COMPONENT_FACTORY(FACTORY_TYPE) ChilliSource::Core::ComponentFactoryDispenser::GetSingletonPtr()->GetFactoryWithInterface<FACTORY_TYPE>() 

namespace ChilliSource
{
    namespace Core
    {
        
        class ComponentFactoryDispenser
        {
        public:
            ComponentFactoryDispenser(Application* inpApp);
            ~ComponentFactoryDispenser();
            //--------------------------------------------------------------------------------------------------
            /// Get Singleton Ptr
            ///
            /// @return Singleton instance
            //--------------------------------------------------------------------------------------------------
            static ComponentFactoryDispenser* GetSingletonPtr();
			//--------------------------------------------------------------------------------------------------
            /// @return Singleton instance
            //--------------------------------------------------------------------------------------------------
            static ComponentFactoryDispenser& GetSingleton();
            //--------------------------------------------------------------------------------------------------
            /// Register Component Factory
            ///
            /// Adds a component factory to the application pool. 
            /// @param Component factory
            //--------------------------------------------------------------------------------------------------
            void RegisterComponentFactory(ComponentFactory * inpComponentFactory);
            //--------------------------------------------------------------------------------------------------
            /// Get Factory Producing
            ///
            /// Looks for a factory that can create the given type
            /// @param The type ID of the object you wish to create (i.e. Mesh, Texture)
            /// @return Factory that can produce the given interface or nullptr if none available
            //--------------------------------------------------------------------------------------------------
            ComponentFactory* GetFactoryProducing(const std::string & insName);
            //--------------------------------------------------------------------------------------------------
            /// Get Factory Producing
            ///
            /// Looks for a factory that can create the given component type
            /// @param The type ID of the object you wish to create (i.e. Sprite, Static Mesh)
            /// @return Factory that creates the given component
            //--------------------------------------------------------------------------------------------------
            ComponentFactory* GetFactoryProducing(InterfaceIDType inInterfaceID);
            //--------------------------------------------------------------------------------------------------
            /// Get Factory With Interface
            ///
            /// Looks for a factory with the given interface
            /// @param The type ID of the factory interface you are seeking
            /// @return Factory that implements the given interface or nullptr if none available
            //--------------------------------------------------------------------------------------------------
            ComponentFactory* GetFactoryWithInterface(InterfaceIDType inInterfaceID);
            //--------------------------------------------------------------------------------------------------
			/// Get Factory With Interface
			///
			/// Convenience template function for the above
			/// @return Factory that implements the templated interface or nullptr if none available
			//--------------------------------------------------------------------------------------------------
			template <typename T> T* GetFactoryWithInterface()
            {
				return static_cast<T*>(GetFactoryWithInterface(T::InterfaceID));
			}
            
        private:
            
            std::vector<ComponentFactory*> mComponentFactories;
            
            
            static ComponentFactoryDispenser* pInstance;
            Application* mpApp;
        };
    }
}

#endif
