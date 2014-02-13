/*
 *  SceneDescription.h
 *  SceneLoader
 *
 *  Created by Stuart McGaw on 16/02/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_CORE_SCENEDESCRIPTION_H_
#define _MOFLO_CORE_SCENEDESCRIPTION_H_

#include <moFlo/Core/Resource.h>
#include <moFlo/Core/XML/rapidxml_utils.hpp>
#include <moFlo/Core/ParamDictionary.h>
#include <moFlo/Core/FastDelegate.h>
#include <moFlo/Core/SceneAnimation/ForwardDeclarations.h>

namespace moFlo
{
	namespace Core
	{
		struct EntityTransform
		{
			CVector3	vTranslation;
			CQuaternion qOrientation;
			CVector3	vScale;
			f32         fOpacity;
            
			EntityTransform();
			void ToXml(rapidxml::xml_node<> * inpEl) const;
			void FromXml(rapidxml::xml_node<> * inpEl);
		};
		
        typedef SHARED_ARRAY_PTR<EntityTransform>       EntityTransformArrayPtr;
        typedef SHARED_ARRAY_PTR<const EntityTransform> EntityTransformArrayCPtr;
        
		class CSceneDesc : public IResource
		{
		public:
            struct ComponentDesc
			{
				std::string		strType;
				std::string		strName;
				ParamDictionary sParams;
                bool			bVisible;
				
				ComponentDesc()
				{
					bVisible = true;
				}
				
				void ToXml(rapidxml::xml_node<> * inpParentEl) const;
				void FromXml(rapidxml::xml_node<> * inpEl);
			};
            
			struct EntityDesc
			{
				std::string     strName;
                EntityTransform sTransform;
				DYNAMIC_ARRAY<EntityDesc> aChildEntities;
				DYNAMIC_ARRAY<ComponentDesc> aComponents;		
                ParamDictionary sParams;
                bool            bVisible;
				bool            bCustomEntityDefinition;
                
				EntityDesc()
				{
					bVisible = true;
					bCustomEntityDefinition = false;
				}
				void ToXml(rapidxml::xml_node<> * inpParentEl) const;
				void FromXml(rapidxml::xml_node<> * inpEl);
			};
            
            
            DECLARE_NAMED_INTERFACE(CSceneDesc);
            
            bool IsA(InterfaceIDType inInterfaceID) const;
            
            typedef fastdelegate::FastDelegate1<const EntityDesc&, EntityPtr> CustomEntityDelegate;
            EntityPtr BuildScene(CustomEntityDelegate inCustomEntityDelegate = CustomEntityDelegate());
            
			bool LoadFromFile(STORAGE_LOCATION ineStorageLocation, const std::string & incName);
            void WriteToFile(STORAGE_LOCATION ineStorageLocation, const std::string & incName);
            CSceneDesc::EntityDesc*  GetDescriptionForEntityWithName(const std::string & instrName);
			
        protected:
            EntityPtr    CreateEntity(EntityDesc & insEntityDesc, CustomEntityDelegate &inCustomEntityDelegate);
            void         CreateContentsForEntity(CEntity & insEntity, CSceneDesc::EntityDesc & insEntDesc, CustomEntityDelegate &inCustomEntityDelegate);
            ComponentPtr CreateComponent(ComponentDesc& insComponentDesc);

            CSceneDesc::EntityDesc*  RecursiveNameSearch(CSceneDesc::EntityDesc* inpDesc, const std::string & instrName);
            
			EntityDesc							sRootEntity;
		};
	}
}

#endif
