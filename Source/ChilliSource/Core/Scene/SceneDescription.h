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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/XML/rapidxml_utils.hpp>

#include <functional>

namespace ChilliSource
{
	namespace Core
	{
		struct EntityTransform
		{
			Vector3	vTranslation;
			Quaternion qOrientation;
			Vector3	vScale;
			f32         fOpacity;
            
			EntityTransform();
			void ToXml(rapidxml::xml_node<> * inpEl) const;
			void FromXml(rapidxml::xml_node<> * inpEl);
		};
		
		class SceneDesc : public Resource
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
				std::vector<EntityDesc> aChildEntities;
				std::vector<ComponentDesc> aComponents;		
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
            
            
            CS_DECLARE_NAMEDTYPE(SceneDesc);
            
            bool IsA(InterfaceIDType inInterfaceID) const override;
            
            typedef std::function<EntitySPtr(const EntityDesc&)> CustomEntityDelegate;
            EntitySPtr BuildScene(CustomEntityDelegate inCustomEntityDelegate = CustomEntityDelegate());
            
			bool LoadFromFile(StorageLocation ineStorageLocation, const std::string & incName);
            void WriteToFile(StorageLocation ineStorageLocation, const std::string & incName);
            SceneDesc::EntityDesc*  GetDescriptionForEntityWithName(const std::string & instrName);
			
        protected:
            EntitySPtr    CreateEntity(EntityDesc & insEntityDesc, CustomEntityDelegate &inCustomEntityDelegate);
            void         CreateContentsForEntity(Entity & insEntity, SceneDesc::EntityDesc & insEntDesc, CustomEntityDelegate &inCustomEntityDelegate);
            ComponentSPtr CreateComponent(ComponentDesc& insComponentDesc);

            SceneDesc::EntityDesc*  RecursiveNameSearch(SceneDesc::EntityDesc* inpDesc, const std::string & instrName);
            
			EntityDesc							sRootEntity;
		};
	}
}

#endif
