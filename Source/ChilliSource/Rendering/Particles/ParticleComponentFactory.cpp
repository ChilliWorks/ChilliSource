/*
 *  ParticleComponentFactory.cpp
 *  moFloTest
 *
 *  Created by Scott Downie on 17/01/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Material/MaterialManager.h>
#include <ChilliSource/Rendering/Material/Material.h>

#include <ChilliSource/Rendering/Particles/ParticleComponent.h>
#include <ChilliSource/Rendering/Particles/ParticleComponentFactory.h>
#include <ChilliSource/Rendering/Particles/ParticleSystem.h>

#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Core/XML/XMLUtils.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>

namespace ChilliSource
{
	namespace Rendering
	{
		DEFINE_NAMED_INTERFACE(CParticleComponentFactory);
		
		//--------------------------------------------------------
		/// Constructor
		///
		/// Default
		//--------------------------------------------------------
		CParticleComponentFactory::CParticleComponentFactory(CParticleSystem* inpParticleSystem, CParticleEmitterFactory* inpEmitterFactory, CParticleEffectorFactory* inpEffectorFactory) :
        mpParticleSystem(inpParticleSystem), mpMaterialMgr(nullptr), mpEmitterFactory(inpEmitterFactory), mpEffectorFactory(inpEffectorFactory)
		{

		}
        //-------------------------------------------------------
        /// Is A
        ///
        /// @param Interface ID
        /// @return Whether the object is of given type
        //-------------------------------------------------------
		bool CParticleComponentFactory::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == CParticleComponentFactory::InterfaceID;
		}
		//--------------------------------------------------------
		/// Can Produce Component With Interface
		///
		/// Used to determine if this factory can produce 
		/// component of given type.
		///
		/// @param The ID of the component to create
		/// @return Whether the object can create component of ID
		//--------------------------------------------------------
		bool CParticleComponentFactory::CanProduceComponentWithInterface(Core::InterfaceIDType inTypeID) const
		{
			return (CParticleComponent::InterfaceID == inTypeID);
		}
        //----------------------------------------------------------------------------
        /// Can Produce Component With Type Name
        ///
        /// @param Type name
        /// @return Whether the factory can produce components with the given name
        //----------------------------------------------------------------------------
		bool CParticleComponentFactory::CanProduceComponentWithTypeName(const std::string & incName) const
		{
			return (CParticleComponent::TypeName == incName);
		}
        //--------------------------------------------------------
        /// Register Emitter Type
        ///
        /// Register a custom emitter type
        ///
        /// @param Delegate
        /// @param Emitter type name
        //--------------------------------------------------------
        void CParticleComponentFactory::RegisterEmitterType(const CParticleEmitterFactory::EmitterCreateDelegate& inDelegate, const std::string& instrName)
        {
            mpEmitterFactory->Register(inDelegate, instrName);
        }
        //--------------------------------------------------------
        /// Register Effector Type
        ///
        /// Register a custom effector type
        ///
        /// @param Delegate
        /// @param Effector type name
        //--------------------------------------------------------
        void CParticleComponentFactory::RegisterEffectorType(const CParticleEffectorFactory::EffectorCreateDelegate& inDelegate, const std::string& instrName)
        {
            mpEffectorFactory->Register(inDelegate, instrName);
        }
        //--------------------------------------------------------
        /// Create Particle Component
        ///
        /// Creates a default particle effect for customisation
        ///
        /// @return Particle Component
        //--------------------------------------------------------
        ParticleComponentPtr CParticleComponentFactory::CreateParticleComponent()
		{
			ParticleComponentPtr pParticleComp(new CParticleComponent());
			mpParticleSystem->AddParticleComponent(pParticleComp.get());			
			return pParticleComp;
		}
        //--------------------------------------------------------
        /// Create Particle Component From Script
        ///
        /// Creates a particle component using an external script
        ///
        /// @param The storage location to load from
        /// @param The filepath
        /// @return Particle Component
        //--------------------------------------------------------
        ParticleComponentPtr CParticleComponentFactory::CreateParticleComponentFromScript(Core::StorageLocation ineStorageLocation, const std::string& instrScriptFile)
		{
			ParticleComponentPtr pParticleComp(new CParticleComponent());
            
            //Load script
            TiXmlDocument Doc(instrScriptFile);
			Doc.LoadFile(ineStorageLocation);
			
			TiXmlElement* pDocRoot = Doc.RootElement();
			
			if(pDocRoot && pDocRoot->ValueStr() == "system") 
            {
                //---Load the material
                if(!mpMaterialMgr)
                {
                    mpMaterialMgr = static_cast<IMaterialManager*>(Core::CResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(CMaterial::InterfaceID));
                }
                
                MaterialPtr pMaterial;
                TiXmlElement* pMaterialEl = Core::XMLUtils::FirstChildElementWithName(pDocRoot, "material");
                if(pMaterialEl)
                {
                    pMaterial = mpMaterialMgr->GetMaterialFromFile(Core::StorageLocation::k_package, Core::XMLUtils::GetAttributeValueOrDefault<std::string>(pMaterialEl, "filename", ""));
                    if(pMaterial)
                    {
                        pParticleComp->SetMaterial(pMaterial);
                    }
                    else
                    {
                        CS_ERROR_LOG("Particle file: " + instrScriptFile + " no material found for file");
                    }
                }
                else
                {
                    CS_ERROR_LOG("Particle file: " + instrScriptFile + " no material found"); 
                }
                
                //---Emitters
				TiXmlElement* pEmittersEl = Core::XMLUtils::FirstChildElementWithName(pDocRoot, "emitters");
                if(pEmittersEl)
                {
                    TiXmlElement* pEmitterEl = Core::XMLUtils::FirstChildElementWithName(pEmittersEl, "emitter");
                    while(pEmitterEl)
                    {
                        //Get the param dictionary config values
                        Core::ParamDictionary sParams;
                        sParams.FromString(pEmitterEl->GetText());
                        
                        std::string strShape;
                        if(sParams.TryGetValue("Shape", strShape))
                        {
                            pParticleComp->AddEmitter(mpEmitterFactory->CreateParticleEmitter(strShape, sParams, pMaterial, pParticleComp.get()));
                        }
                        else
                        {
                            CS_ERROR_LOG("Particle file: " + instrScriptFile + " no emitters shape found");
                        }
                        
                        //Jump to the next emitter
                        pEmitterEl = Core::XMLUtils::NextSiblingElementWithName(pEmitterEl);
                    }
                }
                else
                {
                    CS_ERROR_LOG("Particle file: " + instrScriptFile + " no emitters found");
                }
                
                //---Effectors
                TiXmlElement* pEffectorsEl = Core::XMLUtils::FirstChildElementWithName(pDocRoot, "effectors");
                if(pEffectorsEl)
                {
                    TiXmlElement* pEffectorEl = Core::XMLUtils::FirstChildElementWithName(pEffectorsEl, "effector");
                    while(pEffectorEl)
                    {
                        //Get the param dictionary config values
                        Core::ParamDictionary sParams;
                        sParams.FromString(pEffectorEl->GetText());
                        
                        std::string strType;
                        if(sParams.TryGetValue("Type", strType))
                        {
                            //Add this effector to all the emitters
                            pParticleComp->AddEffector(mpEffectorFactory->CreateParticleEffector(strType, sParams));
                        }
                        
                        //Jump to the next effector
                        pEffectorEl = Core::XMLUtils::NextSiblingElementWithName(pEffectorEl);
                    }
                }
			} 
            else 
            {
				CS_ERROR_LOG("Particle component factory could not load file: " + instrScriptFile);
			}
            
			mpParticleSystem->AddParticleComponent(pParticleComp.get());			
			return pParticleComp;
		}
	}
}