/*
 *  Component.cpp
 *  MoFlow
 *
 *  Created by Tag Games on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Entity/Component.h>

namespace ChilliSource
{

	namespace Core
	{
		Component::Component()
		:mpEntityOwner(nullptr), mudwQueryMask(0)
		{
			
		}
		Component::~Component()
		{

		}
		//----------------------------------------------------
		/// Get Entity Owner
		///
		/// @return Entity to which component is attached
		//----------------------------------------------------
		Entity * Component::GetEntityOwner()
		{
			return mpEntityOwner;
		}
		//----------------------------------------------------
		/// Get Entity Owner (Const)
		///
		/// @return Entity to which component is attached
		//----------------------------------------------------
		const Entity * Component::GetEntityOwner() const
		{
			return mpEntityOwner;
		}
		//----------------------------------------------------
		/// Set Entity Owner (Const)
		///
		/// @param Entity to which component is attached
		//----------------------------------------------------
		void Component::SetEntityOwner(Entity* inpOwner)
		{
			if(mpEntityOwner != nullptr)
			{
				OnDetachedFromEntity();
			}
			
			mpEntityOwner = inpOwner;
			
			if (inpOwner != nullptr) 
			{
				OnAttachedToEntity();
			}
		}
		//----------------------------------------------------
		/// Get Query Mask
		///
		/// @return Query mask for filtering
		//----------------------------------------------------
		u32 Component::GetQueryMask() const
		{
			return mudwQueryMask;
		}
		//----------------------------------------------------
		/// Set Query Mask
		///
		/// @param Filter mask
		//----------------------------------------------------
		void Component::SetQueryMask(u32 inudwQueryMask)
		{
			mudwQueryMask = inudwQueryMask;
		}
	}
	
}