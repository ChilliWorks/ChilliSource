/*
 *  Component.cpp
 *  MoFlow
 *
 *  Created by Tag Games on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Entity/Component.h>

namespace moFlo
{

	namespace Core
	{
		IComponent::IComponent()
		:mpEntityOwner(NULL), mudwQueryMask(0)
		{
			
		}
		IComponent::~IComponent()
		{

		}
		//----------------------------------------------------
		/// Get Entity Owner
		///
		/// @return Entity to which component is attached
		//----------------------------------------------------
		CEntity * IComponent::GetEntityOwner()
		{
			return mpEntityOwner;
		}
		//----------------------------------------------------
		/// Get Entity Owner (Const)
		///
		/// @return Entity to which component is attached
		//----------------------------------------------------
		const CEntity * IComponent::GetEntityOwner() const
		{
			return mpEntityOwner;
		}
		//----------------------------------------------------
		/// Set Entity Owner (Const)
		///
		/// @param Entity to which component is attached
		//----------------------------------------------------
		void IComponent::SetEntityOwner(CEntity* inpOwner)
		{
			if(mpEntityOwner != NULL)
			{
				OnDetachedFromEntity();
			}
			
			mpEntityOwner = inpOwner;
			
			if (inpOwner != NULL) 
			{
				OnAttachedToEntity();
			}
		}
		//----------------------------------------------------
		/// Get Query Mask
		///
		/// @return Query mask for filtering
		//----------------------------------------------------
		u32 IComponent::GetQueryMask() const
		{
			return mudwQueryMask;
		}
		//----------------------------------------------------
		/// Set Query Mask
		///
		/// @param Filter mask
		//----------------------------------------------------
		void IComponent::SetQueryMask(u32 inudwQueryMask)
		{
			mudwQueryMask = inudwQueryMask;
		}
	}
	
}