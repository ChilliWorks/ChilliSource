//
//  Scene.h
//  Chilli Source
//  Created by Scott Downie on 21/09/2010.
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

#ifndef _CHILLISOURCE_CORE_SCENE_SCENE_H_
#define _CHILLISOURCE_CORE_SCENE_SCENE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/System/StateSystem.h>
#include <ChilliSource/Core/Volume/VolumeComponent.h>

namespace ChilliSource
{
	namespace Core
	{
		//--------------------------------------------------------------------------------------------------
        /// Holds the entities that form the scene. Entities will be updated from the scene and the
        /// scene is queried by other systems for render components etc.
        ///
        /// @author S Downie
		//--------------------------------------------------------------------------------------------------
		class Scene final : public StateSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(Scene);
            //-------------------------------------------------------
            /// Factory method
            ///
            /// @author S Downie
            ///
            /// @param Input system used by the window to listen
            /// for input events
            //-------------------------------------------------------
			static SceneUPtr Create();
            
            //-------------------------------------------------------
            /// Destructor
            ///
            /// @author S Downie
            //-------------------------------------------------------
			~Scene();
            //-------------------------------------------------------
            /// Query interface to determine if the object implements
            /// the interface with the given ID
            ///
            /// @author S Downie
            ///
            /// @param Interface ID
            ///
            /// @return Whether the system has the given interface
            //-------------------------------------------------------
            bool IsA(InterfaceIDType in_interfaceId) const;
			//-------------------------------------------------------
			/// Add an entity to the scene. This entity cannot
            /// exist on another scene prior to adding. The entity
            /// will now be updated from the scene
            ///
            /// @author S Downie
			///
			/// @param Entity
			//-------------------------------------------------------
			void Add(const EntitySPtr& in_entity);
            //-------------------------------------------------------
			/// Remove all the entities from the scene
            ///
            /// @author S Downie
			//-------------------------------------------------------
			void RemoveAllEntities();
			//-------------------------------------------------------
            /// @author A Mackie
            ///
			/// @return the list of all the entities in the scene.
			//-------------------------------------------------------
			const SharedEntityList& GetEntities() const;
            //------------------------------------------------------
			/// @author S Downie
			///
			/// @param Screen clear colour
			//------------------------------------------------------
			void SetClearColour(const Core::Colour& in_colour);
			//------------------------------------------------------
			/// @author S Downie
			///
			/// @return Screen clear colour
			//------------------------------------------------------
			const Core::Colour& GetClearColour() const;
            //-------------------------------------------------------
            /// Sends the resume event on to the entities.
            ///
            /// @author S Downie
			//-------------------------------------------------------
			void ResumeEntities();
            //-------------------------------------------------------
            /// Sends the foreground event on to the entities.
            ///
            /// @author Ian Copland
			//-------------------------------------------------------
			void ForegroundEntities();
            //-------------------------------------------------------
			/// Updates all entities.
            ///
            /// @author Ian Copland
			///
			/// @param Time since last update in seconds.
			//-------------------------------------------------------
			void UpdateEntities(f32 in_timeSinceLastUpdate);
            //-------------------------------------------------------
			/// Fixed updates all entities.
            ///
            /// @author Ian Copland
			///
			/// @param Time since last update in seconds.
			//-------------------------------------------------------
			void FixedUpdateEntities(f32 in_timeSinceLastUpdate);
            //-------------------------------------------------------
            /// Sends the background event on to the entities.
            ///
            /// @author Ian Copland
			//-------------------------------------------------------
			void BackgroundEntities();
            //-------------------------------------------------------
            /// Sends the suspend event on to the entities.
            ///
            /// @author Ian Copland
			//-------------------------------------------------------
			void SuspendEntities();
			//--------------------------------------------------------------------------------------------------
			/// Traverses the contents of the scene and adds any objects that intersect with the ray to the
			/// list. The list order is undefined. Use the query intersection value on the volume component
            /// to sort by depth
            ///
            /// @author S Downie
            ///
			/// @param Ray to check intersection
			/// @param [Out] Container to fill with intersecting components
			//--------------------------------------------------------------------------------------------------
			void QuerySceneForIntersection(const Ray &in_ray, std::vector<VolumeComponent*>& out_volumeComponents);
            //--------------------------------------------------------------------------------------------------
			/// Traverse the scene for the given component type and fill the list with those components
            ///
            /// @author S Downie
            ///
			/// @param [Out] Empty container to be filled
			//--------------------------------------------------------------------------------------------------
			template <typename TComponentType>
            void QuerySceneForComponents(std::vector<TComponentType*>& out_components)
			{
				for(SharedEntityList::const_iterator it = m_entities.begin(); it != m_entities.end(); ++it)
				{
					(*it)->GetComponents<TComponentType>(out_components);
				}
			}
            //--------------------------------------------------------------------------------------------------
			/// Traverse the scene for the given component types and fill the lists with those components
            //
            /// @author S Downie
            ///
            /// @param [Out] Empty container to be filled with T1
            /// @param [Out] Empty container to be filled with T2
			//--------------------------------------------------------------------------------------------------
			template <typename TComponentType1, typename TComponentType2>
            void QuerySceneForComponents(std::vector<TComponentType1*>& out_components1, std::vector<TComponentType2*>& out_components2)
			{
				for(SharedEntityList::const_iterator it = m_entities.begin(); it != m_entities.end(); ++it)
				{
					(*it)->GetComponents<TComponentType1, TComponentType2>(out_components1, out_components2);
				}
			}
            //--------------------------------------------------------------------------------------------------
			/// Traverse the scene for the given component types and fill the lists with those components
            ///
            /// @author S Downie
            ///
            /// @param [Out] Empty container to be filled with T1
            /// @param [Out] Empty container to be filled with T2
            /// @param [Out] Empty container to be filled with T3
			//--------------------------------------------------------------------------------------------------
			template <typename TComponentType1, typename TComponentType2, typename TComponentType3>
            void QuerySceneForComponents(std::vector<TComponentType1*>& out_components1, std::vector<TComponentType2*>& out_components2, std::vector<TComponentType3*>& out_components3)
			{
				for(SharedEntityList::const_iterator it = m_entities.begin(); it != m_entities.end(); ++it)
				{
					(*it)->GetComponents<TComponentType1, TComponentType2, TComponentType3>(out_components1, out_components2, out_components3);
				}
			}
            
        private:
            friend class Entity;
            
            //-------------------------------------------------------
            /// Private to enforce use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            Scene();
            //-------------------------------------------------------
            /// Remove the entity from the scene
            ///
            /// @author S Downie
            ///
            /// @param Entity
            //-------------------------------------------------------
            void Remove(Entity* inpEntity);
            
		private:
			
			SharedEntityList m_entities;
            Colour m_clearColour;
            bool m_entitiesActive = false;
            bool m_entitiesForegrounded = false;
		};		
	}
}

#endif
