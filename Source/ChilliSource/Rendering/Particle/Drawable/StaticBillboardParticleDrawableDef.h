//
//  StaticBillboardParticleDrawableDef.h
//  Chilli Source
//  Created by Ian Copland on 13/10/2014.
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_DRAWABLE_BILLBOARDPARTICLEDRAWABLEDEF_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_DRAWABLE_BILLBOARDPARTICLEDRAWABLEDEF_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Rendering/Particle/Drawable/ParticleDrawableDef.h>

#include <json/json.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//-----------------------------------------------------------------------
		/// The definition for a billboard particle drawable. This enables the 
		/// drawing of individual particles as camera facing sprites.
		///
		/// As a particle drawable def's contents can potentially be read from 
		/// multiple threads, it is immutable after construction. The exception 
		/// to this is if it was created from a param dictionary with a 
		/// asynchronous delegate, in which case it is immutable after the
		/// delegate returns.
		///
		/// The following are the parameters of a billboard particle drawable
		/// def:
		///
		/// "MaterialLocation": The storage location of the material that 
		/// will be used to render the particles.
		///
		/// "MaterialPath": The file path of the material that will be used
		/// to render the particles.
		///
		/// "AtlasLocation": The storage location of the texture atlas 
		/// that will be used to render the particles.
		///
		/// "AtlasPath": The file path of the texture atlas that will be 
		/// used to render the particles.
		///
		/// "AtlasIds": A comma separated list of texture atlas Ids that 
		/// will be used to render the particles.
		///
		/// "ImageSelectionType": A string describing the method used to 
		/// select the atlas Id. Possible values are "Random" or "Cycle".
		///
		/// "ParticleSize": The base size for a billboard.
		///
		/// "SizePolicy": The size policy describing how the particle is 
		/// rendered when the rendered image has a different aspect ratio
		/// to the given size.
		///
		/// @author Ian Copland
		//-----------------------------------------------------------------------
		class StaticBillboardParticleDrawableDef final : public ParticleDrawableDef
		{
		public:
			CS_DECLARE_NAMEDTYPE(StaticBillboardParticleDrawableDef);
			//----------------------------------------------------------------
			/// An enum describing the different types of image selection. 
			/// Random will pick an image from the list at random, while cycle
			/// will cycle through each image in the list in order.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			enum class ImageSelectionType
			{
				k_random,
				k_cycle
			};
			//----------------------------------------------------------------
			/// Identifiers for functions that maintain the aspect ratio of the 
			/// sprite based on current size and preferred image size
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			enum class SizePolicy
			{
				k_none,
				k_usePreferredSize,
				k_useWidthMaintainingAspect,
				k_useHeightMaintainingAspect,
				k_fitMaintainingAspect,
				k_fillMaintainingAspect
			};
			//----------------------------------------------------------------
			/// Constructor for creating a billboard particle drawable
			/// definition which uses just a material.
			///
			/// @author Ian Copland
			///
			/// @param The material that will be used to render the particles.
			/// @param The base size for a particles.
			/// @param The size policy describing how the particle is rendered
			/// when the rendered image has a different aspect ratio to the 
			/// given size.
			//----------------------------------------------------------------
			StaticBillboardParticleDrawableDef(const MaterialCSPtr& in_material, const Core::Vector2& in_particleSize, SizePolicy in_sizePolicy);
			//----------------------------------------------------------------
			/// Constructor for creating a billboard particle drawable definition
			/// which uses a texture atlas and multiple atlas Ids.
			///
			/// @author Ian Copland
			///
			/// @param The material that will be used to render the particles.
			/// @param The texture altas that will be used to render the 
			/// particles.
			/// @param The atlas Id.
			/// @param The base size for a particles.
			/// @param The size policy describing how the particle is rendered 
			/// when the rendered image has a different aspect ratio to the 
			/// given size.
			//----------------------------------------------------------------
			StaticBillboardParticleDrawableDef(const MaterialCSPtr& in_material, const TextureAtlasCSPtr& in_textureAtlas, const std::string& in_atlasId, const Core::Vector2& in_particleSize, SizePolicy in_sizePolicy);
			//----------------------------------------------------------------
			/// Constructor for creating a billboard particle drawable 
			/// definition which uses a texture atlas and multiple atlas Ids.
			///
			/// @author Ian Copland
			///
			/// @param The material that will be used to render the particles.
			/// @param The texture altas that will be used to render the 
			/// particles.
			/// @param A list of Image Ids describing the various images that
			/// will be used to render the particles.
			/// @param The method that will be used to select Image Ids.
			/// @param The base size for a particles.
			/// @param The size policy describing how the particle is rendered
			/// when the rendered image has a different aspect ratio to the 
			/// given size.
			//----------------------------------------------------------------
			StaticBillboardParticleDrawableDef(const MaterialCSPtr& in_material, const TextureAtlasCSPtr& in_textureAtlas, const std::vector<std::string>& in_atlasIds, ImageSelectionType in_imageSelectionType, const Core::Vector2& in_particleSize, SizePolicy in_sizePolicy);
			//----------------------------------------------------------------
			/// Constructor. Loads the params for the drawable def from the 
			/// given json params. If the async delegate is not null, then
			/// any resource loading will occur as a background task. Once 
			/// complete the delegate will be called. The values read from
            /// json are described in the class documentation.
			///
			/// @author Ian Copland
            ///
            /// @param The json params.
            /// @param The asynchronous load delegate.
			//----------------------------------------------------------------
			StaticBillboardParticleDrawableDef(const Json::Value& in_paramsJson, const LoadedDelegate& in_asyncDelegate = nullptr);
			//----------------------------------------------------------------
			/// Allows querying of whether or not this implements the interface 
			/// described by the given Id.
			///
			/// @author Ian Copland
			///
			/// @param The interface Id.
			///
			/// @return Whether or not the interface is implemented.
			//----------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------------
			/// Creates an instance of the particle drawable described by this.
			///
			/// @author Ian Copland.
			///
			/// @param The entity that owns the effect.
			/// @param The concurrent particle data.
			///
			/// @return the instance.
			//----------------------------------------------------------------
			ParticleDrawableUPtr CreateInstance(const Core::Entity* in_entity, ConcurrentParticleData* in_concurrentParticleData) const override;
			//----------------------------------------------------------------
			/// @author Ian Copland.
			///
			/// @return The material that will be used to render the particles.
			//----------------------------------------------------------------
			const MaterialCSPtr& GetMaterial() const override;
			//----------------------------------------------------------------
			/// @author Ian Copland.
			///
			/// @return The texture atlas that will be used to render the 
			/// particles. If this is null then the particles will be rendered
			/// using the whole texture.
			//----------------------------------------------------------------
			const TextureAtlasCSPtr& GetTextureAltas() const;
			//----------------------------------------------------------------
			/// @author Ian Copland.
			///
			/// @return The list of texture Ids that will be used to render 
			/// the particles. If this is empty then the particles will be 
			/// rendered using the whole texture.
			//----------------------------------------------------------------
			const std::vector<std::string>& GetAtlasIds() const;
			//----------------------------------------------------------------
			/// @author Ian Copland.
			///
			/// @return The method that will be used to select the next image 
			/// from the atlas id list. 
			//----------------------------------------------------------------
			ImageSelectionType GetImageSelectionType() const;
			//----------------------------------------------------------------
			/// @author Ian Copland.
			///
			/// @return The particle size.
			//----------------------------------------------------------------
			const Core::Vector2& GetParticleSize() const;
			//----------------------------------------------------------------
			/// @author Ian Copland.
			///
			/// @return The method that will be used to size a particle when 
			/// the image size and the particle size have a different aspect 
			/// ratio.
			//----------------------------------------------------------------
			SizePolicy GetSizePolicy() const;
		private:
			//----------------------------------------------------------------
			/// Loads the billboard resources on the main thread.
			///
			/// @author Ian Copland
			///
			/// @param A json object describing the resources.
			//----------------------------------------------------------------
			void LoadResources(const Json::Value& in_paramsJson);
			//----------------------------------------------------------------
			/// Loads the billboard resources on a background thread. Once
			/// complete the async delegate will be called.
			///
			/// @author Ian Copland
			///
			/// @param A json object describing the resources.
			/// @param The async delegate.
			//----------------------------------------------------------------
			void LoadResourcesAsync(const Json::Value& in_paramsJson, const LoadedDelegate& in_asyncDelegate);

			MaterialCSPtr m_material;
			TextureAtlasCSPtr m_textureAtlas;
			std::vector<std::string> m_atlasIds;
			ImageSelectionType m_imageSelectionType = ImageSelectionType::k_cycle;
			Core::Vector2 m_particleSize = Core::Vector2::k_one;
			SizePolicy m_sizePolicy = SizePolicy::k_none;
		};
	}
}

#endif