//
//  CSParticleProvider.h
//  Chilli Source
//  Created by Ian Copland on 04/11/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_CSPARTICLEPROVIDER_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_CSPARTICLEPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//-------------------------------------------------------------------------
		/// A resource provider which creates Particle Effect resources from
		/// CSParticle files.
		///
		/// @author Ian Copland
		//-------------------------------------------------------------------------
		class CSParticleProvider final : public Core::ResourceProvider
		{
		public:
			CS_DECLARE_NAMEDTYPE(CSParticleProvider);
			//-----------------------------------------------------------------
			/// Allows querying of whether or not this implements the interface
			/// described by the given interface Id.
			/// 
			/// @author Ian Copland
			///
			/// @param The interface Id.
			///
			/// @return Whether this implements the interface.
			//-----------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//-----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The type of resource loaded by this provider.
			//-----------------------------------------------------------------
			Core::InterfaceIDType GetResourceType() const override;
			//-----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param The extension to check.
			///
			/// @return Whether or not this can load from files with the
			/// given extension.
			//-----------------------------------------------------------------
			bool CanCreateResourceWithFileExtension(const std::string& in_extension) const override;
		private:
			friend class Core::Application;
			//-----------------------------------------------------------------
			/// Factory method for creating new instances of this system.
			///
			/// @author S Downie
			///
			/// @return The instance of this system.
			//-----------------------------------------------------------------
			static CSParticleProviderUPtr Create();
			//-----------------------------------------------------------------
			/// Default constructor. Declared private to force the use of the
			/// CreateSystem() method in Application.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			CSParticleProvider() = default;
			//-----------------------------------------------------------------
			/// Populates the given Particle Effect resource with data from the
			/// given file. The resource should be checked to ensure it has
			/// loaded after this method is called.
			///
			/// @author Ian Copland
			///
			/// @param The storage location of the file.
			/// @param The file path.
			/// @param The resource options. This is always empty for a particle 
			/// effect.
			/// @param [Out] The output particle effect resource.
			//-----------------------------------------------------------------
			void CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, 
				const Core::ResourceSPtr& out_resource) override;
			//-----------------------------------------------------------------
			/// Populates the given Particle Effect resource with data from the
			/// given file. The loading is performed as a background task, the 
			/// resource should not be used until the completion delegate has
			/// been called. When the completion delegate is called the resource
			/// should be checked to confirm it has successfully loaded.
			///
			/// @author Ian Copland
			///
			/// @param The storage location of the file.
			/// @param The file path.
			/// @param The resource options. This is always empty for a particle 
			/// effect.
			/// @param The completion delegate. This should never be null.
			/// @param [Out] Resource object
			//------------------------------------------------------------------
			void CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, 
				const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource) override;
			//------------------------------------------------------------------
			/// Tries to acquire references to the drawable, emitter and affector
			/// definition factories. If they don't exist this will try to assert.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------
			void OnInit() override;
			//------------------------------------------------------------------
			/// Cleans up the handles to the drawable, emitter and affector
			/// definition factories.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------
			void OnDestroy() override;

			const ParticleDrawableDefFactory* m_drawableDefFactory = nullptr;
			const ParticleEmitterDefFactory* m_emitterDefFactory = nullptr;
			const ParticleAffectorDefFactory* m_affectorDefFactory = nullptr;
		};
	}
}

#endif
