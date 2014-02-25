/** 
 * File: Component.h
 * Date: 28 Sept 2010
 * Description: Defines the base Component class for moFlo
 */

/** 
 *
 * Author Stuart McGaw
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_CORE_COMPONENT_H
#define _MOFLO_CORE_COMPONENT_H

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

namespace ChilliSource
{
	namespace Core
	{
		class IComponent : public IQueryableInterface
		{
		public:
			IComponent();
			virtual ~IComponent();
			//----------------------------------------------------
			/// Get Entity Owner
			///
			/// @return Entity to which component is attached
			//----------------------------------------------------
			CEntity * GetEntityOwner();
			//----------------------------------------------------
			/// Get Entity Owner (Const)
			///
			/// @return Entity to which component is attached
			//----------------------------------------------------
			const CEntity * GetEntityOwner() const;
			//----------------------------------------------------
			/// Set Entity Owner (Const)
			///
			/// @param Entity to which component is attached
			//----------------------------------------------------
			virtual void SetEntityOwner(CEntity* inpOwner);
			//----------------------------------------------------
			/// Get Query Mask
			///
			/// @return Query mask for filtering
			//----------------------------------------------------
			u32 GetQueryMask() const;
			//----------------------------------------------------
			/// Set Query Mask
			///
			/// @param Filter mask
			//----------------------------------------------------
			void SetQueryMask(u32 inudwQueryMask);
            //----------------------------------------------------
            /// Get Interface ID
            ///
            /// @return Interface ID of the class
            //----------------------------------------------------
            virtual InterfaceIDType GetInterfaceID() const = 0;
            //----------------------------------------------------
            /// Get Interface Type Name
            ///
            /// @return String representation of interface ID of the class
            //----------------------------------------------------
            virtual const std::string& GetInterfaceTypeName() const = 0;
			
		protected:
		
			//----------------------------------------------------
			/// On Attached To Entity
			///
			/// Triggered when the component is attached to
			/// an entity
			//----------------------------------------------------
			virtual void OnAttachedToEntity(){}
			//----------------------------------------------------
			/// On Detached From Entity
			///
			/// Triggered when the component is removed from
			/// an entity
			//----------------------------------------------------
			virtual void OnDetachedFromEntity(){}

		protected:
			CEntity * mpEntityOwner;
			
			u32 mudwQueryMask;
		};
		typedef SHARED_PTR<IComponent> ComponentPtr;
		typedef WEAK_PTR<IComponent> ComponentWeakPtr;
	}
}

#endif