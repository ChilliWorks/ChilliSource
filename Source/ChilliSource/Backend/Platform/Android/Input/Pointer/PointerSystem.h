//
//  PointerSystem.h
//  Chilli Source
//
//  Created by Ian Copland on 27/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_INPUT_POINTER_POINTERSYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_INPUT_POINTER_POINTERSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Input/Pointer/PointerSystem.h>

namespace ChilliSource
{
    namespace Android
    {
        //------------------------------------------------------------
        /// The Android backend for the pointer system. This provides
        /// access to touch events.
        ///
        /// @author I Copland
        //------------------------------------------------------------
        class PointerSystem final : public Input::PointerSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(PointerSystem);
            //----------------------------------------------------
			/// Queries whether or not this system implements the
            /// interface with the given ID.
            ///
            /// @author I Copland
            ///
            /// @param The interface Id.
            ///
			/// @return Whether or not the interface is implemented.
			//----------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //----------------------------------------------------
			/// Called when a touch down event is received.
            ///
            /// @author I Copland
            ///
            /// @param The system Id of the touch.
            /// @param The touch location.
			//----------------------------------------------------
            void OnTouchDown(s32 in_systemId, const Core::Vector2& in_location);
            //----------------------------------------------------
			/// Called when a touch moved event is received.
            ///
            /// @author I Copland
            ///
            /// @param The system Id of the touch.
            /// @param The touch location.
			//----------------------------------------------------
			void OnTouchMoved(s32 in_systemId, const Core::Vector2& in_location);
            //----------------------------------------------------
			/// Called when a touch up event is received.
            ///
            /// @author I Copland
            ///
            /// @param The system Id of the touch.
			//----------------------------------------------------
			void OnTouchUp(s32 in_systemId);
        private:
            friend Input::PointerSystemUPtr Input::PointerSystem::Create();

            //------------------------------------------------
            /// Default constructor. Declared private to force
            /// the use of the factory method.
            ///
            /// @author I Copland
            //------------------------------------------------
            PointerSystem() = default;
            //------------------------------------------------
            /// Initialisation method called at a time when
            /// all App Systems have been created. System
            /// initialisation occurs in the order they were
            /// created.
            ///
            /// @author I Copland
            //------------------------------------------------
            void OnInit() override;
            //------------------------------------------------
            /// Called when the application is being destroyed.
            /// This should be used to cleanup memory and
            /// references to other systems. System destruction
            /// occurs in the reverse order to which they
            /// were created
            ///
            /// @author I Copland
            //------------------------------------------------
            void OnDestroy() override;

            std::unordered_map<s32, PointerId> m_systemIdToPointerIdMap;
        };
    }
}

#endif
