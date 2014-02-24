/*
 *  SceneDescription.cpp
 *  SceneLoader
 *
 *  Created by Stuart McGaw on 16/02/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Scene/SceneDescription.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Entity/ComponentFactoryDispenser.h>
#include <ChilliSource/Core/Entity/ComponentFactory.h>

namespace ChilliSource
{
	namespace Core 
	{
		EntityTransform::EntityTransform() 
		:vScale(1.0f,1.0f,1.0f)
		,vTranslation(CVector3::ZERO)
		,qOrientation(CQuaternion::IDENTITY)
		,fOpacity(1.0f)
		{
		}
		
		void EntityTransform::ToXml(rapidxml::xml_node<> * inpEl) const
		{
            inpEl->name("Transform");
            			
			if (vTranslation != CVector3::ZERO)
                rapidxml::add_new_attribute(inpEl, "translation", STRING_CAST(vTranslation).c_str());
			if (qOrientation != CQuaternion::IDENTITY)
				rapidxml::add_new_attribute(inpEl, "orientation", STRING_CAST(qOrientation).c_str());
			if (vScale != CVector3(1,1,1))
				rapidxml::add_new_attribute(inpEl, "scale", STRING_CAST(vScale).c_str());
            if(fOpacity != 1.0f)
                rapidxml::add_new_attribute(inpEl, "opacity", STRING_CAST(fOpacity).c_str());
		}
		
		void EntityTransform::FromXml(rapidxml::xml_node<> * inpEl)
		{
            for(rapidxml::xml_attribute<> * pAttr = inpEl->first_attribute(); pAttr != NULL; pAttr = pAttr->next_attribute())
            {
                if(pAttr->isNamed("translation") )
                {
                    vTranslation = CStringConverter::ParseVector3(pAttr->value());
                }
                else if(pAttr->isNamed("orientation") )
                {
                    qOrientation = CStringConverter::ParseQuaternion(pAttr->value());
                }
                else if(pAttr->isNamed("scale") )
                {
                    vScale = CStringConverter::ParseVector3(pAttr->value());
                }
                else if(pAttr->isNamed("opacity") )
                {
                    fOpacity = CStringConverter::ParseFloat(pAttr->value());
                }
            }
        }
		
        
        DEFINE_NAMED_INTERFACE(CSceneDesc);
        
        bool CSceneDesc::IsA(InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == CSceneDesc::InterfaceID;
        }
        
        
        EntityPtr CSceneDesc::BuildScene(CustomEntityDelegate inCustomEntityDelegate)
        {
            EntityPtr pRoot = CreateEntity(sRootEntity, inCustomEntityDelegate);
            return pRoot;
        }
        
        EntityPtr CSceneDesc::CreateEntity(CSceneDesc::EntityDesc & insEntDesc, CustomEntityDelegate &inCustomEntityDelegate)
        {
            EntityPtr pResult;
            
            if(insEntDesc.bCustomEntityDefinition && inCustomEntityDelegate)
            {
                pResult = inCustomEntityDelegate(insEntDesc);
            }
            else
            {
                pResult = moFlo::Core::CreateEntity();
                
                for(u32 nComponent = 0; nComponent <insEntDesc.aComponents.size(); nComponent++)
                {
                    ComponentPtr pComponent = CreateComponent(insEntDesc.aComponents[nComponent]);
                    if(pComponent)
                        pResult->Attach(pComponent);
                }
            }
            
            pResult->SetName(insEntDesc.strName);
            pResult->SetVisible(insEntDesc.bVisible);
            CreateContentsForEntity(*pResult, insEntDesc, inCustomEntityDelegate);
            
            return pResult;
        }
        
        void CSceneDesc::CreateContentsForEntity(moFlo::Core::CEntity &insEntity, CSceneDesc::EntityDesc & insEntDesc, CustomEntityDelegate &inCustomEntityDelegate)
        {
            for (u32 nEntity = 0; nEntity < insEntDesc.aChildEntities.size(); nEntity++) 
			{
				EntityPtr pEnt = CreateEntity(insEntDesc.aChildEntities[nEntity], inCustomEntityDelegate);
				if (pEnt)
					insEntity.AddChild(pEnt);
			}
            
            insEntity.Transform().SetPositionScaleOrientation(insEntDesc.sTransform.vTranslation, insEntDesc.sTransform.vScale, insEntDesc.sTransform.qOrientation);
            insEntity.Transform().FadeTo(insEntDesc.sTransform.fOpacity);
        }
        
        
        ComponentPtr CSceneDesc::CreateComponent(moFlo::Core::CSceneDesc::ComponentDesc &insComponentDesc)
        {
            ComponentPtr pResult;
            
            IComponentFactory* pCompFac = CComponentFactoryDispenser::GetSingleton().GetFactoryProducing(insComponentDesc.strType);
            if(pCompFac)
            {
                pResult = pCompFac->CreateComponent(insComponentDesc.strType, insComponentDesc.sParams);
                if(!pResult)
                {
                    WARNING_LOG("SceneLoader: Factory failed to produce component of type: " + insComponentDesc.strType);

                }
            }
            else
            {
				WARNING_LOG("SceneLoader: No registered factory can produce type: " + insComponentDesc.strType);
            }
            
            
            return pResult;
        }
        
		bool CSceneDesc::LoadFromFile(StorageLocation ineStorageLocation, const std::string & incName)
		{
            std::string strFile;
            bool bReadFile = CUtils::FileToString(ineStorageLocation, incName, strFile);
            
            if(!bReadFile)
                return false;

            
            rapidxml::xml_document<> doc;
            doc.parse<0>((char*)strFile.c_str());
			
            rapidxml::xml_node<> * pDocRoot = doc.first_node();
			
			if (pDocRoot && pDocRoot->isNamed("Scene") )
			{
				rapidxml::xml_node<> * pRootEntityEl = pDocRoot->first_node();
				sRootEntity.FromXml(pRootEntityEl);
			} 
			else 
			{
				ERROR_LOG("SceneDescription could not load the file:" + incName);
                return false;
			}
            
            return true;		
		}
		
		void CSceneDesc::WriteToFile(StorageLocation ineStorageLocation, const std::string & incName)
		{
            rapidxml::xml_document<> doc;
            
            rapidxml::xml_node<> *pSceneRoot = rapidxml::add_new_child(&doc, "Scene");
            
            sRootEntity.ToXml(pSceneRoot);
            
            rapidxml::xml_node<> * pFirstChild =pSceneRoot->first_node();
            if(pFirstChild)
                pFirstChild->name("Root");            
            
            std::string strFile = rapidxml::doc_to_string<char>(&doc);
            CUtils::StringToFile(ineStorageLocation, incName, strFile);
		}
        
        CSceneDesc::EntityDesc*  CSceneDesc::GetDescriptionForEntityWithName(const std::string & instrName)
        {
            return RecursiveNameSearch(&sRootEntity, instrName);
        }
        
        CSceneDesc::EntityDesc*  CSceneDesc::RecursiveNameSearch(CSceneDesc::EntityDesc* inpDesc, const std::string & instrName)
        {
            if(inpDesc->strName == instrName)
                return inpDesc;
            
            CSceneDesc::EntityDesc* pEntityDesc = NULL;
            
            for(u32 udwChild = 0; udwChild < inpDesc->aChildEntities.size() && !pEntityDesc; ++udwChild)
            {
                pEntityDesc = RecursiveNameSearch(&(inpDesc->aChildEntities[udwChild]), instrName);
            }
            
            return pEntityDesc;

        }

		void CSceneDesc::EntityDesc::ToXml(rapidxml::xml_node<> * inpParentEl) const
		{
            rapidxml::xml_node<> * pEntityEl = rapidxml::add_new_child(inpParentEl, bCustomEntityDefinition ? "CustomEntity" : "Entity");
            
            rapidxml::add_new_attribute(pEntityEl, "name", strName.c_str());
			rapidxml::add_new_attribute(pEntityEl,"visible", STRING_CAST(bVisible).c_str());

			// Write out Transform
            rapidxml::xml_node<> * pTransform = rapidxml::add_new_child(pEntityEl, "Transform");
            sTransform.ToXml(pTransform);

			// Write out all parameters
			if (sParams.size() > 0)
			{
                rapidxml::xml_node<> * pParam = rapidxml::add_new_child(pEntityEl, "Params");
				sParams.ToXml(pParam);
			}

			// Write out all components
			if (aComponents.size() > 0) 
			{
                rapidxml::xml_node<> * pComponents = rapidxml::add_new_child(pEntityEl, "Components");				
				for (u32 nComponent = 0; nComponent < aComponents.size(); nComponent++)
					aComponents[nComponent].ToXml(pComponents);
			}

			// Write all children
			if (aChildEntities.size() > 0)
			{
				rapidxml::xml_node<> * pChildren = rapidxml::add_new_child(pEntityEl, "Children");
				for (u32 nChildEntity = 0; nChildEntity < aChildEntities.size(); nChildEntity++) 
					aChildEntities[nChildEntity].ToXml(pChildren);
			}
		}

		void CSceneDesc::EntityDesc::FromXml(rapidxml::xml_node<> * inpEntityEl)
		{
			// Read Entity/CustomEntity
			bCustomEntityDefinition = !inpEntityEl->isNamed("Entity");

			
            for(rapidxml::xml_attribute<> * pAttr = inpEntityEl->first_attribute(); pAttr != NULL; pAttr = pAttr->next_attribute())
            {
                // Read name
                if(pAttr->isNamed("name"))
                {
                    strName = pAttr->value();
                }
                // Read visibility
                else if(pAttr->isNamed("visible"))
                {
                    bVisible = CStringConverter::ParseBool(pAttr->value());
                }
			}

            for(rapidxml::xml_node<> * pNode = inpEntityEl->first_node(); pNode != NULL; pNode = pNode->next_sibling())
            {
                // Read transform
                if(pNode->isNamed("Transform"))
                {
                    sTransform.FromXml(pNode);
                }
                // Read components
                else if (pNode->isNamed("Components"))
                {
                    for(rapidxml::xml_node<> * pComp = pNode->first_node(); pComp != NULL; pComp = pComp->next_sibling())
                    {
                        aComponents.push_back(ComponentDesc());
                        aComponents.back().FromXml(pComp);
                    }
                }
                // Cycle through child elements processing Entity, CustomEntity and Node
                else if (pNode->isNamed("Children"))
                {
                    for(rapidxml::xml_node<> * pChild = pNode->first_node(); pChild != NULL; pChild = pChild->next_sibling())
                    {
                        if(pChild->isNamed("Entity") || pChild->isNamed("CustomEntity") )
                        {
                            aChildEntities.push_back(EntityDesc());
                            aChildEntities.back().FromXml(pChild);
                        }
                    }
                }
                else if(pNode->isNamed("Params"))
                {
                    sParams.FromXml(pNode);
                }
            }
						
		}		
				
		void CSceneDesc::ComponentDesc::ToXml(rapidxml::xml_node<> * inpParentEl) const{
            
            rapidxml::xml_node<> *pComponentEl = rapidxml::add_new_child(inpParentEl, strType.c_str());
                    
            rapidxml::xml_node<> *pParams = rapidxml::add_new_child(pComponentEl, "Params");
            sParams.ToXml(pParams);
		}
		void CSceneDesc::ComponentDesc::FromXml(rapidxml::xml_node<> * inpComponentEl){
            
            strType= inpComponentEl->name();
            for(rapidxml::xml_attribute<> * pAttr = inpComponentEl->first_attribute(); pAttr != NULL; pAttr = pAttr->next_attribute())
            {
                if(pAttr->isNamed("name") )
                {
                    strName = pAttr->value();
                }
                if(pAttr->isNamed("visible") )
                {
                    bVisible = CStringConverter::ParseBool(pAttr->value());
                }
            }
            
            for(rapidxml::xml_node<> * pNode = inpComponentEl->first_node(); pNode != NULL; pNode = pNode->next_sibling())
            {
                if(pNode->isNamed("Params"))
                {
                    sParams.FromXml(pNode);
                }
            }
        }
	}
}
