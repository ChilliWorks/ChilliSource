//
//  Component.h
//  ChilliSource
//
//  Created by Stuart McGaw on 28/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_ENTITY_COMPONENT_H_
#define _CHILLISOURCE_CORE_ENTITY_COMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

namespace ChilliSource
{
	namespace Core
	{
		class Component : public QueryableInterface
		{
		public:
            CS_DECLARE_NOCOPY(Component);
            //----------------------------------------------------
            /// Constructor
            ///
            /// @author S McGaw
            //----------------------------------------------------
			Component();
            //----------------------------------------------------
            /// Destructor
            ///
            /// @author S McGaw
            //----------------------------------------------------
			virtual ~Component(){}
			//----------------------------------------------------
			/// @author S Downie
			///
			/// @return Entity to which component is attached or
            /// null.
			//----------------------------------------------------
			Entity * GetEntity();
			//----------------------------------------------------
			/// @author S Downie
			///
			/// @return Const entity to which component is attached
            /// or null.
			//----------------------------------------------------
			const Entity * GetEntity() const;
            //----------------------------------------------------
            /// Remove the component from its owning entity
            ///
            /// @author S Downie
            //----------------------------------------------------
            void RemoveFromEntity();
			
		protected:
		
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @param Time since last update in seconds
            //----------------------------------------------------
            virtual void OnUpdate(f32 in_timeSinceLastUpdate){}
            //----------------------------------------------------
            /// Fixed update is triggered at fixed time periods
            ///
            /// @author S Downie
            ///
            /// @param Time since last fixed update in seconds
            //----------------------------------------------------
            virtual void OnFixedUpdate(f32 in_fixedTimeSinceLastUpdate){}
			//----------------------------------------------------
			/// Triggered when the component is attached to
			/// an entity. Custom components should override this
            /// as they see fit
            ///
            /// @author S McGaw
			//----------------------------------------------------
			virtual void OnAddedToEntity(){}
			//----------------------------------------------------
			/// Triggered when the component is removed from
			/// an entity. Custom components should override this
            /// as they see fit
            ///
            /// @author S McGaw
			//----------------------------------------------------
			virtual void OnRemovedFromEntity(){}
            //----------------------------------------------------
			/// Triggered when the component is attached to
			/// an entity and that entity is on a scene.
            /// Custom components should override this as they see
            /// fit
            ///
            /// @author S Downie
			//----------------------------------------------------
			virtual void OnAddedToScene(){}
			//----------------------------------------------------
			/// Triggered when the component is removed from
			/// an entity or when the entity is removed from the
            /// scene. Custom components should override this as
            /// they see fit
            ///
            /// @author S Downie
			//----------------------------------------------------
			virtual void OnRemovedFromScene(){}
            
        private:
            
            friend class Entity;
            //----------------------------------------------------
			/// @author S Downie
            ///
			/// @param Entity to which component is attached
			//----------------------------------------------------
            void SetEntity(Entity* in_newOwner);

		private:
            
			Entity * m_entity;
		};
	}
}

#endif