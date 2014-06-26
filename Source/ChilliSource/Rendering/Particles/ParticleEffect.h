//
//  ParticleEffect.h
//  Chilli Source
//  Created by S Downie on 30/04/2014.
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLES_PARTICLEEFFECT_H_
#define _CHILLISOURCE_RENDERING_PARTICLES_PARTICLEEFFECT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/Resource.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //---------------------------------------------------------------------
        /// A resource that holds the information about a particle effect
        /// including the affectors and emitters descriptions
        ///
        /// @author S Downie
        //---------------------------------------------------------------------
		class ParticleEffect final : public Core::Resource
		{
		public:
			
			CS_DECLARE_NAMEDTYPE(ParticleEffect);
			
			//---------------------------------------------------------------------
			/// @author S Downie
			///
            /// @param Interface type
            ///
			/// @return Whether this object is of given type
			//---------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //---------------------------------------------------------------------
            /// Populate the resource with all the data it needs in order to
            /// describe a particle effect
            ///
            /// @author S Downie
            ///
            /// @param List of emitter descriptions
            /// @param List of affector descriptions
            /// @param Material
            /// @param Texture atlas (can be null)
            /// @param Atlas Id (can be empty)
            //---------------------------------------------------------------------
            void Build(std::vector<Core::ParamDictionary>&& in_emitterDescs, std::vector<Core::ParamDictionary>&& in_affectorDescs, const MaterialCSPtr& in_material,
                       const TextureAtlasCSPtr& in_atlas, std::string&& in_atlasId);
            //---------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Material used to render effect
            //---------------------------------------------------------------------
            const MaterialCSPtr& GetMaterial() const;
            //---------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Texture atlas used to render effect
            //---------------------------------------------------------------------
            const TextureAtlasCSPtr& GetAtlas() const;
            //---------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Texture atlas Id used in conjunction with atlas
            //---------------------------------------------------------------------
            const std::string& GetAtlasId() const;
            //---------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Number of emitter descriptors
            //---------------------------------------------------------------------
            u32 GetNumEmitters() const;
            //---------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Index of emitter
            ///
            /// @return Descriptor/Properties as key value dictionary
            //---------------------------------------------------------------------
            const Core::ParamDictionary& GetEmitterDesc(u32 in_index) const;
            //---------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Number of affector descriptors
            //---------------------------------------------------------------------
            u32 GetNumAffectors() const;
            //---------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Index of affector
            ///
            /// @return Descriptor/Properties as key value dictionary
            //---------------------------------------------------------------------
            const Core::ParamDictionary& GetAffectorDesc(u32 in_index) const;
            
        private:
            
            friend class Core::ResourcePool;
            //---------------------------------------------------------------------
            /// Factory method to create empty resource. Only called
            /// by the resource pool
            ///
            /// @author S Downie
            ///
            /// @return Ownership of resource
            //---------------------------------------------------------------------
            static ParticleEffectUPtr Create();
            //---------------------------------------------------------------------
            /// Private constructor to enforce use of factory create method
            ///
            /// @author S Downie
            //---------------------------------------------------------------------
            ParticleEffect() = default;
            
        private:
            
            std::vector<Core::ParamDictionary> m_emitterDescs;
            std::vector<Core::ParamDictionary> m_affectorDescs;
            
            MaterialCSPtr m_material;
            TextureAtlasCSPtr m_atlas;
            std::string m_atlasId;
		};
	}
}

#endif