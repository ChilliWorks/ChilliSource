//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_RPI

#ifndef _CSBACKEND_PLATFORM_RPI_CORE_BASE_PLATFORMSYSTEM_H_
#define _CSBACKEND_PLATFORM_RPI_CORE_BASE_PLATFORMSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/RPi/ForwardDeclarations.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>

namespace CSBackend
{
	namespace RPi
	{
		/// Raspberry Pi backend for the platform system.
		/// Mostly used for interfacing with the broadcom display system.
		///
		class PlatformSystem final : public ChilliSource::PlatformSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(PlatformSystem);

			/// Queries whether or not this system implements the
			/// interface with the given Id.
			///
			/// @param interfaceId
			///		The id of the interface that we need to check if this system implements
			///
			/// @retrun TRUE if the system 'IsA' interface
			///
			bool IsA(ChilliSource::InterfaceIDType interfaceId) const override;

			/// Adds default systems for this platform to the default systems
			/// of the application
			///
			/// @param application
			///		Application to add systems to
			///
			void CreateDefaultSystems(ChilliSource::Application* application) override;

            /// NOTE: The Raspberry Pi enforces VSync (like iOS) and therefore this just
			/// sets the vsync interval (e.g 60 = every vblank, 30 = every second vblank)
			///
			///	@param fps
			///		The maximum frames per second to clamp to.
			///		This should be in multiples of 15 to match the behaviour of other platforms (e.g. 15, 30, 60)
            ///
			void SetPreferredFPS(u32 fps) override;

			/// NOTE: Vsync cannot be disabled on RPi so this does nothing
			///
			/// @param enabled
			///		TRUE to enable, FALSE to disable
			///
			void SetVSyncEnabled(bool enabled) override {}

			/// Stops the update loop causing the application
			/// to terminate.
			///
			void Quit() override;

		private:
			friend ChilliSource::PlatformSystemUPtr ChilliSource::PlatformSystem::Create();

			///
			PlatformSystem() = default;
		};
	}
}

#endif

#endif
