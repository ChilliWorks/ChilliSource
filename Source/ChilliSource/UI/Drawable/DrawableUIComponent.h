//
//  DrawableUIComponent.h
//  Chilli Source
//  Created by Ian Copland on 28/11/2014.
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

#ifndef _CHILLISOURCE_UI_DRAWABLE_DRAWABLEUICOMPONENT_H_
#define _CHILLISOURCE_UI_DRAWABLE_DRAWABLEUICOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/Property/PropertyMap.h>
#include <ChilliSource/Core/Event/EventConnection.h>
#include <ChilliSource/Input/Pointer/Pointer.h>
#include <ChilliSource/UI/Base/UIComponent.h>
#include <ChilliSource/UI/Drawable/UIDrawable.h>

namespace ChilliSource
{
    //--------------------------------------------------------------------------
    /// A component for rendering image in a widget. This is handled through
    /// setting a UIDrawable. Drawables come in 3 different types: Standard,
    /// Three-Patch and Nine-Patch. A Standard drawable renders a basic image,
    /// A Three-Patch drawable will stretch without distorting the image in
    /// 1 direction and a Nine-Patch can stretch in any direction without
    /// distorting the image. Any drawable type can be created using an entire
    /// texture or using a texture atlas, which describes the portion of a
    /// texture which should be rendered.
    ///
    /// A DrawableUIComponent contains the following properties:
    ///
    /// "Drawable": The description of the drawable that will be rendered. The
    /// contents of a drawable are described in each of the drawable def types
    /// documentation.
    ///
    /// @author Ian Copland
    //--------------------------------------------------------------------------
    class DrawableUIComponent final : public UIComponent
    {
    public:
        CS_DECLARE_NAMEDTYPE(DrawableUIComponent);
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The list of properties supported by a drawable component.
        //-------------------------------------------------------------------
        static const std::vector<PropertyMap::PropertyDesc>& GetPropertyDescs();
        //-------------------------------------------------------------------
        /// Allows querying of whether or not the component implements the
        /// interface associated with the given interface Id. Typically
        /// this won't be called directly, instead the templated version
        /// IsA<Interface>() should be used.
        ///
        /// @author Ian Copland
        ///
        /// @param The interface Id.
        ///
        /// @return Whether the object implements the given interface.
        //-------------------------------------------------------------------
        bool IsA(InterfaceIDType in_interfaceId) const override;
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The drawable object that performs the rendering. This
        /// can be used to directly change properties such as the UVs and
        /// colour of the rendered image.
        //-------------------------------------------------------------------
        UIDrawable* GetDrawable();
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return A const pointer to the drawable object that performs the
        /// rendering. 
        //-------------------------------------------------------------------
        const UIDrawable* GetDrawable() const;
        //-------------------------------------------------------------------
        /// Sets the drawable definition that is used to create the underlying
        /// drawable object.
        ///
        /// @author Ian Copland
        ///
        /// @param The drawable defintion.
        //-------------------------------------------------------------------
        void ApplyDrawableDef(const UIDrawableDefCSPtr& in_drawableDef);
        
    private:
        friend class UIComponentFactory;
        //-------------------------------------------------------------------
        /// Constructor that builds the component from key-value properties.
        /// The properties used to create a drawable component are described
        /// in the class documentation.
        ///
        /// @author Ian Copland
        ///
        /// @param The component name.
        /// @param The property map.
        //-------------------------------------------------------------------
        DrawableUIComponent(const std::string& in_componentName, const PropertyMap& in_properties);
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The drawable definition that was used to create the
        /// underlying drawable object.
        //-------------------------------------------------------------------
        const UIDrawableDefCSPtr& GetDrawableDef() const;
        //----------------------------------------------------------------
        /// This is called during the draw event whenever the application
        /// is active and the owning widget in on the canvas. This will
        /// render the contained drawable to the screen.
        ///
        /// @author Ian Copland
        ///
        /// @param The canvas renderer.
        /// @param The final screen space transform.
        /// @param The final screen space size.
        /// @param The final colour.
        //----------------------------------------------------------------
        void OnDraw(CanvasRenderer* in_renderer, const Matrix3& in_transform, const Vector2& in_absSize, const Colour& in_absColour) override;
        
        UIDrawableDefCSPtr m_drawableDef;
        UIDrawableUPtr m_drawable;
    };
}

#endif