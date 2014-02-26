//
//  AnimatedMeshComponentUpdater.h
//  moFlow
//
//  Created by Ian Copland on 22/04/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Core/System/SystemConcepts.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //======================================================
        /// Animated Mesh Component Updater
        ///
        /// A system for updating animation mesh components.
        //======================================================
        class AnimatedMeshComponentUpdater : public Core::ISystem, public Core::IUpdateable
        {
        public:
            DECLARE_NAMED_INTERFACE(AnimatedMeshComponentUpdater);
            //------------------------------------------------
			/// Constructor
			//------------------------------------------------
            AnimatedMeshComponentUpdater();
            //------------------------------------------------
			/// Is A
			///
			/// @param Interface to compare
			/// @return Whether the object implements the given interface
			//------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
            //------------------------------------------------
			/// Add Component
			///
			/// Adds a new component for updating to the
            /// updater.
            ///
            /// @param The component.
			//------------------------------------------------
			void AddComponent(AnimatedMeshComponent* inpAnimatedMesh);
            //------------------------------------------------
			/// Update
			///
			/// Updates all of the registered Animated Mesh
            /// Components.
            ///
            /// @param The delta time.
			//------------------------------------------------
			void Update(f32 infDeltaTime) override;
            //------------------------------------------------
			/// Remove Component
			///
			/// Removes a component from being updated.
            ///
            /// @param The component.
			//------------------------------------------------
			void RemoveComponent(AnimatedMeshComponent* inpAnimatedMesh);
        private:
            std::vector<AnimatedMeshComponent*> mapComponentsToUpdate;
        };
    }
}