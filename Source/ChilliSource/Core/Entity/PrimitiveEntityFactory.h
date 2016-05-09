//
//  PrimitiveEntityFactory.h
//  ChilliSource
//  Created by Ian Copland on 04/08/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_ENTITY_PRIMITIVEENTITYFACTORY_H_
#define _CHILLISOURCE_CORE_ENTITY_PRIMITIVEENTITYFACTORY_H_

#include <ChilliSource/ChilliSource.h>

#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/System/AppSystem.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    /// A PrimitiveEntityFactory is a factory for creating primitive shape entities
    /// such as planes and boxes. Entities will not be added to the Scene on
    /// creation so this will have to be handled manually.
    //
    ///
    /// PrimitiveEntityFactory is not a default system so it will need to be added
    /// using Application::CreateSystem() prior to use.
    ///
    /// An entity can only be created on the main thread as PrimitiveEntityFactory
    /// is not thread-safe.
    ///
    /// @author Ian Copland
    //------------------------------------------------------------------------------
    class PrimitiveEntityFactory final : public AppSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(PrimitiveEntityFactory);
        //------------------------------------------------------------------------------
        /// Allows querying of whether or not this system implements the interface
        /// described by the given interface Id. Typically this is not called directly
        /// as the templated equivalent IsA<Interface>() is preferred.
        ///
        /// @author Ian Copland
        ///
        /// @param in_interfaceId - The interface Id.
        ///
        /// @return Whether or not the interface is implemented.
        //------------------------------------------------------------------------------
        bool IsA(InterfaceIDType in_interfaceId) const override;
        //------------------------------------------------------------------------------
        /// Creates a coloured plane of the given size. The plane will use a blinn
        /// shading material so there must be lights in the scene to see it.
        ///
        /// @author Ian Copland
        ///
        /// @param in_colour - The colour of the plane.
        /// @param in_size - [Optional] The size of the plane. Defaults to [10, 10].
        ///
        /// @return The new entity.
        //------------------------------------------------------------------------------
        EntityUPtr CreatePlane(const Colour& in_colour, const Vector2& in_size = Vector2(10.0f, 10.0f));
        //------------------------------------------------------------------------------
        /// Creates a coloured box of the given size. The box will use a blinn shading
        /// material so there must be lights in the scene to see it.
        ///
        /// @author Ian Copland
        ///
        /// @param in_colour - The colour of the box.
        /// @param in_size - [Optional] The size of the box. Defaults to [1, 1, 1].
        ///
        /// @return The new entity.
        //------------------------------------------------------------------------------
        EntityUPtr CreateBox(const Colour& in_colour, const Vector3& in_size = Vector3(1.0f, 1.0f, 1.0f));
        
    private:
        friend class Application;
        //------------------------------------------------------------------------------
        /// A factory method for creating new instances of the system.
        ///
        /// @author Ian Copland
        ///
        /// @return The new instance.
        //------------------------------------------------------------------------------
        static PrimitiveEntityFactoryUPtr Create();
        //------------------------------------------------------------------------------
        /// Default constructor. Declared private to ensure the system is created
        /// through State::CreateSystem<>().
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        PrimitiveEntityFactory() = default;
        //------------------------------------------------------------------------------
        /// Creates a new material with the given colour. The material is for static
        /// models and will apply blinn lighting. If a material of the same colour
        /// already exists it will be used rather than creating a new instance.
        ///
        /// @author Ian Copland
        ///
        /// @param in_colour - The colour of the material.
        ///
        /// @return The material.
        //------------------------------------------------------------------------------
        MaterialCSPtr CreateStaticBlinnColourMaterial(const Colour& in_colour) const;
        //------------------------------------------------------------------------------
        /// Initialises the system.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void OnInit() override;
        //------------------------------------------------------------------------------
        /// Cleans up the system.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void OnDestroy() override;
        
        ResourcePool* m_resourcePool = nullptr;
        RenderComponentFactory* m_renderComponentFactory = nullptr;
        PrimitiveModelFactory* m_primitiveModelFactory = nullptr;
        MaterialFactory* m_materialFactory = nullptr;
        u64 m_entityCount = 0;
    };
}


#endif
