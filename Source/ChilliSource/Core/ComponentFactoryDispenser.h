//
//  ResourceManagerDispenser.h
//  moFloTest
//
//  Created by Scott Downie on 02/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_CORE_FACTORY_DISPENSER_H_
#define _MO_FLO_CORE_FACTORY_DISPENSER_H_

#include <moFlo/Core/ForwardDeclarations.h>
#include <moFlo/Core/QueryableInterface.h>

#include <moFlo/Rendering/ForwardDeclarations.h>

#include <moFlo/Audio/ForwardDeclarations.h>

#include <moFlo/GUI/ForwardDeclarations.h>

#define GET_COMPONENT_FACTORY(FACTORY_TYPE) moFlo::Core::CComponentFactoryDispenser::GetSingletonPtr()->GetFactoryWithInterface<FACTORY_TYPE>() 

namespace moFlo
{
    namespace Core
    {
        
        class CComponentFactoryDispenser
        {
        public:
            CComponentFactoryDispenser(CApplication* inpApp);
            ~CComponentFactoryDispenser();
            //--------------------------------------------------------------------------------------------------
            /// Get Singleton Ptr
            ///
            /// @return Singleton instance
            //--------------------------------------------------------------------------------------------------
            static CComponentFactoryDispenser* GetSingletonPtr();
			//--------------------------------------------------------------------------------------------------
            /// @return Singleton instance
            //--------------------------------------------------------------------------------------------------
            static CComponentFactoryDispenser& GetSingleton();
            //--------------------------------------------------------------------------------------------------
            /// Register Component Factory
            ///
            /// Adds a component factory to the application pool. 
            /// @param Component factory
            //--------------------------------------------------------------------------------------------------
            void RegisterComponentFactory(IComponentFactory * inpComponentFactory);
            //--------------------------------------------------------------------------------------------------
            /// Get Factory Producing
            ///
            /// Looks for a factory that can create the given type
            /// @param The type ID of the object you wish to create (i.e. CMesh, ITexture)
            /// @return Factory that can produce the given interface or NULL if none available
            //--------------------------------------------------------------------------------------------------
            IComponentFactory* GetFactoryProducing(const std::string & insName);
            //--------------------------------------------------------------------------------------------------
            /// Get Factory Producing
            ///
            /// Looks for a factory that can create the given component type
            /// @param The type ID of the object you wish to create (i.e. Sprite, Static Mesh)
            /// @return Factory that creates the given component
            //--------------------------------------------------------------------------------------------------
            IComponentFactory* GetFactoryProducing(InterfaceIDType inInterfaceID);
            //--------------------------------------------------------------------------------------------------
            /// Get Factory With Interface
            ///
            /// Looks for a factory with the given interface
            /// @param The type ID of the factory interface you are seeking
            /// @return Factory that implements the given interface or NULL if none available
            //--------------------------------------------------------------------------------------------------
            IComponentFactory* GetFactoryWithInterface(InterfaceIDType inInterfaceID);
            //--------------------------------------------------------------------------------------------------
			/// Get Factory With Interface
			///
			/// Convenience template function for the above
			/// @return Factory that implements the templated interface or NULL if none available
			//--------------------------------------------------------------------------------------------------
			template <typename T> T* GetFactoryWithInterface()
            {
				return static_cast<T*>(GetFactoryWithInterface(T::InterfaceID));
			}
            
        private:
            
            DYNAMIC_ARRAY<IComponentFactory*> mComponentFactories;
            
            
            static CComponentFactoryDispenser* pInstance;
            CApplication* mpApp;
        };
    }
}

#endif
