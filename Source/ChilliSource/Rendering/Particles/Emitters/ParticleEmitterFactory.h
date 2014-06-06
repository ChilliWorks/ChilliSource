//
//  ParticleEmitterFactory.h
//  Chilli Source
//  Created by S Downie on 11/04/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLES_EMITTERS_PARTICLEEMITTERFACTORY_H_
#define _CHILLISOURCE_RENDERING_PARTICLES_EMITTERS_PARTICLEEMITTERFACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <functional>
#include <unordered_map>

namespace ChilliSource
{
    namespace Rendering
    {
        class ParticleEmitterFactory final : public Core::AppSystem
        {
        public:

            CS_DECLARE_NAMEDTYPE(ParticleEmitterFactory);
            
            //------------------------------------------------------------------
            /// Delegate method to handle the creation of emitters
            ///
            /// @author S Downie
            ///
            /// @param Properties as key value dictionary
            /// @param Material
            /// @param Owning component
            ///
            /// @return New emitter
            //------------------------------------------------------------------
            typedef std::function<ParticleEmitterUPtr(const Core::ParamDictionary&, const MaterialCSPtr&, ParticleComponent*)> EmitterCreateDelegate;
            //------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface Id
            ///
            /// @return Whether the interface Id matches this object
            //------------------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------------
            /// Registers a new emitter factory method with the given key
            ///
            /// @author S Downie
            ///
            /// @param Type key string
            /// @param Creation delegate
            //------------------------------------------------------------------
            void AddCreator(const std::string& in_type, const EmitterCreateDelegate& in_delegate);
            //------------------------------------------------------------------
            /// Unsubscribe all emitter factory methods
            ///
            /// @author S Downie
            //------------------------------------------------------------------
            void RemoveAllCreators();
            //------------------------------------------------------------------
            /// Construct a particle emitter from the given type and properties
            ///
            /// @param Type i.e. "Point", "Ring"
            /// @param Param dictionary properties
            /// @param Material
            /// @param Owning particle system
            ///
            /// @return Ownership of particle emitter of that type
            //-------------------------------------------------------------------
            ParticleEmitterUPtr CreateParticleEmitter(const std::string& in_type, const Core::ParamDictionary& in_properties, const MaterialCSPtr& in_material, ParticleComponent* in_owner) const;
            
        private:
            
            friend class Core::Application;
            //-------------------------------------------------------------------
            /// Factory creation method
            ///
            /// @author S Downie
            ///
            /// @return Ownership of new instance
            //-------------------------------------------------------------------
            static ParticleEmitterFactoryUPtr Create();
            //-------------------------------------------------------------------
            /// Private constructor to enforce use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------------------
            ParticleEmitterFactory() = default;
            //-------------------------------------------------------------------
            /// Called when the system is created. Registers all the common/default
            /// creation methods
            ///
            /// @author S Downie
            //-------------------------------------------------------------------
            void OnInit() override;
            //-------------------------------------------------------------------
            /// Called when the system is Destroyed.
            ///
            /// @author S Downie
            //-------------------------------------------------------------------
            void OnDestroy() override;
            
        private:
            
            std::unordered_map<std::string, EmitterCreateDelegate> m_creators;
        };
    }
}

#endif
