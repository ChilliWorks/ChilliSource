//
//  Component.h
//  Chilli Source
//  Created by Stuart McGaw on 28/09/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
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
			/// Triggered when the component is attached to
			/// an entity. Custom components should override this
            /// as they see fit
            ///
            /// @author S McGaw
			//----------------------------------------------------
			virtual void OnAddedToEntity(){}
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
			/// Called when the application is resumed while the
            /// component's owning entity is in a scene. This will
            /// also be called when the owning entity is added to
            /// the scene if the application is currently active,
            /// or when the component is added to the entity if the
            /// entity is already in the scene.
            ///
            /// @author Ian Copland
			//----------------------------------------------------
			virtual void OnResume(){}
            //----------------------------------------------------
			/// Called when the application is foregrounded while
            /// the entity is in the scene. This will also be called
            /// when the entity is added to the scene if the application
            /// is currently foregrounded, or when the component is
            /// added to the entity if the entity is already in
            /// the scene.
            ///
            /// @author Ian Copland
			//----------------------------------------------------
			virtual void OnForeground(){}
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
			/// Called when the application is backgrounded while
            /// the owning entity is in the scene. This will also
            /// be called when the owning entity is removed from
            /// the scene if the application is currently
            /// foregrounded or when the component is removed while
            /// the entity is in the scene.
            ///
            /// @author Ian Copland
			//----------------------------------------------------
			virtual void OnBackground(){}
            //----------------------------------------------------
			/// Called when the application is backgrounded while
            /// the owning entity is in the scene. This will also be
            /// called when the owning entity is removed from the
            /// scene if the application is currently active or
            /// when the component is removed while the entity is
            /// in the scene.
            ///
            /// @author Ian Copland
			//----------------------------------------------------
			virtual void OnSuspend(){}
			//----------------------------------------------------
			/// Triggered when the component is removed from
			/// an entity or when the entity is removed from the
            /// scene. Custom components should override this as
            /// they see fit
            ///
            /// @author S Downie
			//----------------------------------------------------
			virtual void OnRemovedFromScene(){}
            //----------------------------------------------------
			/// Triggered when the component is removed from
			/// an entity. Custom components should override this
            /// as they see fit
            ///
            /// @author S McGaw
			//----------------------------------------------------
			virtual void OnRemovedFromEntity(){}
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