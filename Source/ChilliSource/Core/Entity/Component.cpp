//
//  Component.cpp
//  ChilliSource
//
//  Created by Stuart McGaw on 28/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Entity/Component.h>

#include <ChilliSource/Core/Entity/Entity.h>

namespace ChilliSource
{
	namespace Core
	{
        //----------------------------------------------------
        //----------------------------------------------------
		Component::Component()
		: m_entity(nullptr)
		{
			
		}
		//----------------------------------------------------
		//----------------------------------------------------
		Entity * Component::GetEntity()
		{
			return m_entity;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		const Entity * Component::GetEntity() const
		{
			return m_entity;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void Component::SetEntity(Entity* in_newOwner)
		{
			m_entity = in_newOwner;
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void Component::RemoveFromEntity()
        {
            CS_ASSERT(m_entity != nullptr, "Must have an entity to remove from");
            m_entity->Remove(this);
        }
	}
}