//
//  ResourceManagerDispenser.cpp
//  moFloTest
//
//  Created by Scott Downie on 02/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Entity/ComponentFactoryDispenser.h>
#include <ChilliSource/Core/Entity/ComponentFactory.h>

#include <ChilliSource/Audio/3D/AudioComponentFactory.h>
#include <ChilliSource/Audio/3D/AudioComponent.h>

#include <ChilliSource/Rendering/Base/RenderComponentFactory.h>
#include <ChilliSource/Rendering/Sprite/SpriteComponent.h>

namespace ChilliSource
{
    namespace Core
    {
        CComponentFactoryDispenser* CComponentFactoryDispenser::pInstance = nullptr;
        //----------------------------------------------------
        /// Constructor
        ///
        /// Singleton instance
        //----------------------------------------------------
        CComponentFactoryDispenser::CComponentFactoryDispenser(CApplication* inpApp) : mpApp(inpApp)
        {
            pInstance = this;
        }
        //--------------------------------------------------------------------------------------------------
        /// Get Singleton Ptr
        ///
        /// @return Singleton instance
        //--------------------------------------------------------------------------------------------------
        CComponentFactoryDispenser* CComponentFactoryDispenser::GetSingletonPtr()
        {
            return pInstance;
        }
		//--------------------------------------------------------------------------------------------------
		/// @return Singleton instance
		//--------------------------------------------------------------------------------------------------
		CComponentFactoryDispenser& CComponentFactoryDispenser::GetSingleton()
        {
			return *pInstance;
		}
        //--------------------------------------------------------------------------------------------------
		/// Register Component Factory
		///
		/// Adds a component factory to the application pool. 
		/// @param Component factory
		//--------------------------------------------------------------------------------------------------
		void CComponentFactoryDispenser::RegisterComponentFactory(IComponentFactory * inpComponentFactory)
		{
			mComponentFactories.push_back(inpComponentFactory);
		}
        //--------------------------------------------------------------------------------------------------
        /// Get Factory Producing
        ///
        /// Looks for a factory that can create the given type
        /// @param The type ID of the object you wish to create (i.e. Mesh, ITexture)
        /// @return Factory that can produce the given interface or NULL if none available
        //--------------------------------------------------------------------------------------------------
        IComponentFactory* CComponentFactoryDispenser::GetFactoryProducing(const std::string & insName)
        {
            for (std::vector<IComponentFactory*>::iterator it = mComponentFactories.begin(); it != mComponentFactories.end(); ++it) 
			{
                if((*it)->CanProduceComponentWithTypeName(insName)) 
                {
                    return (*it);
                }
			}
			
			CS_WARNING_LOG("Application cannot find factory producing: " + insName);
			return nullptr;
        }
        //--------------------------------------------------------------------------------------------------
        /// Get Factory Producing
        ///
        /// Looks for a factory that can create the given component type
        /// @param The type ID of the object you wish to create (i.e. Sprite, Static Mesh)
        /// @return Factory that creates the given component
        //--------------------------------------------------------------------------------------------------
        IComponentFactory* CComponentFactoryDispenser::GetFactoryProducing(InterfaceIDType inInterfaceID)
        {
            for (std::vector<IComponentFactory*>::iterator it = mComponentFactories.begin(); it != mComponentFactories.end(); ++it) 
			{
                if((*it)->CanProduceComponentWithInterface(inInterfaceID)) 
                {
                    return (*it);
                }
			}
			
			CS_WARNING_LOG("Application cannot find factory for interface ID: " + ToString(inInterfaceID));
			return nullptr;
        }
        //--------------------------------------------------------------------------------------------------
        /// Get Factory With Interface
        ///
        /// Looks for a factory with the given interface
        /// @param The type ID of the factory interface you are seeking
        /// @return Factory that implements the given interface or NULL if none available
        //--------------------------------------------------------------------------------------------------
        IComponentFactory* CComponentFactoryDispenser::GetFactoryWithInterface(InterfaceIDType inInterfaceID)
        {
            for(std::vector<IComponentFactory*>::iterator it = mComponentFactories.begin(); it != mComponentFactories.end(); ++it) 
			{
                if((*it)->IsA(inInterfaceID)) 
                {
                    return (*it);
                }
			}
            
            CS_WARNING_LOG("Cannot find component producer for interface ID: " + ToString(inInterfaceID));
            return nullptr;
        }
        //--------------------------------------------------------------------
        /// Destructor
        //--------------------------------------------------------------------
        CComponentFactoryDispenser::~CComponentFactoryDispenser()
        {

        }
    }
}
