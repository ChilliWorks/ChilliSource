//
//  ParticleAffectorFactory.h
//  Chilli Source
//  Created by S Downie on 07/04/2011.
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


#ifndef _CHILLISOURCE_RENDERING_PARTICLES_AFFECTORS_PARTICLEAFFECTORFACTORY_H_
#define _CHILLISOURCE_RENDERING_PARTICLES_AFFECTORS_PARTICLEAFFECTORFACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <functional>
#include <unordered_map>

namespace ChilliSource
{
    namespace Rendering
    {
        class ParticleAffectorFactory final : public Core::AppSystem
        {
        public:
            
            CS_DECLARE_NAMEDTYPE(ParticleAffectorFactory);
            
            //------------------------------------------------------------------
            /// Delegate method to handle the creation of affectors
            ///
            /// @author S Downie
            ///
            /// @param Properties as key value dictionary
            ///
            /// @return New affector
            //------------------------------------------------------------------
            typedef std::function<ParticleAffectorUPtr(const Core::ParamDictionary&)> AffectorCreateDelegate;
            //------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface Id
            ///
            /// @return Whether the interface Id matches this object
            //------------------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------------
            /// Registers a new affector factory method with the given key
            ///
            /// @author S Downie
            ///
            /// @param Type key string
            /// @param Creation delegate
            //------------------------------------------------------------------
            void AddCreator(const std::string& in_type, const AffectorCreateDelegate& in_delegate);
            //------------------------------------------------------------------
            /// Remove all registered creator methods
            ///
            /// @author S Downie
            //------------------------------------------------------------------
            void RemoveAllCreators();
            //------------------------------------------------------------------
            /// Construct a particle affector from the given type identifier
            /// and properties.
            ///
            /// @param Type i.e. "colour fader", "linear force"
            /// @param Param dictionary properties
            ///
            /// @return Ownership of particle affector of that type
            //-------------------------------------------------------------------
            ParticleAffectorUPtr CreateParticleAffector(const std::string& in_type, const Core::ParamDictionary& in_properties) const;
            
        private:
            
            friend class Core::Application;
            //-------------------------------------------------------------------
            /// Factory creation method
            ///
            /// @author S Downie
            ///
            /// @return Ownership of new instance
            //-------------------------------------------------------------------
            static ParticleAffectorFactoryUPtr Create();
            //-------------------------------------------------------------------
            /// Private constructor to enforce use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------------------
            ParticleAffectorFactory() = default;
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
            
            std::unordered_map<std::string, AffectorCreateDelegate> m_creators;
        };
    }
}

#endif
