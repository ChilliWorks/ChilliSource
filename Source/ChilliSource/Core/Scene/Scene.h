//
// Scene.h
// ChilliSource
//
// Created by Scott Downie on 21/09/2010.
// Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_SCENE_SCENE_H_
#define _CHILLISOURCE_CORE_SCENE_SCENE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
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
		class Scene final
		{
		public:
            //-------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Input system used by the window to listen
            /// for input events
            //-------------------------------------------------------
			Scene(Input::InputSystem* in_inputSystem = nullptr);
            //-------------------------------------------------------
            /// Destructor
            ///
            /// @author S Downie
            //-------------------------------------------------------
			~Scene();

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
			/// Remove the entity from the scene
            ///
            /// @author S Downie
			///
			/// @param Entity
			//-------------------------------------------------------
			void Remove(Entity* inpEntity);
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
			//-------------------------------------------------------
			/// Force the scene to become the active one and to
			/// receive user interaction
            ///
            /// @author J Dixon
			//-------------------------------------------------------
			void BecomeActive();
			//-------------------------------------------------------
			/// Set this scene to be inactive. It will no longer
			/// be interactable for the user
            ///
            /// @author J Dixon
			//-------------------------------------------------------
			void BecomeInactive();
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
            //--------------------------------------------------------------------------------------------------
            /// @author S Downie
            ///
			/// @return The main window that all the scene's UI is attached to.
			//--------------------------------------------------------------------------------------------------
			GUI::Window* GetWindow();
			//--------------------------------------------------------------------------------------------------
			/// Updates the root window and the entities
            ///
            /// @author S Downie
			///
			/// @param Time since last update in seconds
			//--------------------------------------------------------------------------------------------------
			void OnUpdate(f32 in_timeSinceLastUpdate);
            //--------------------------------------------------------------------------------------------------
			/// Updates the root window and the entities at a fixed timestep
            ///
            /// @author S Downie
			///
			/// @param Time since last update in seconds
			//--------------------------------------------------------------------------------------------------
			void OnFixedUpdate(f32 in_fixedTimeSinceLastUpdate);
            
		private:
			
			GUI::WindowUPtr m_rootWindow;
			
			SharedEntityList m_entities;
		};		
	}
}

#endif
