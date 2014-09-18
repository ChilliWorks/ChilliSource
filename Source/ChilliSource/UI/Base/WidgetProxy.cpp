//
//  WidgetProxy.cpp
//  Chilli Source
//  Created by Scott Downie on 18/08/2014.
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

#include <ChilliSource/UI/Base/WidgetProxy.h>

#include <ChilliSource/Core/File/StorageLocation.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/Scripting/Lua/LuaSystem.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace WidgetProxy
        {
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void RegisterWithLua(Scripting::LuaSystem* in_system)
            {
                in_system->RegisterStaticClass("Widget",
                                               "setName", &WidgetProxy::SetName,
                                               "setRelativePosition", &WidgetProxy::SetRelativePosition,
                                               "setAbsolutePosition", &WidgetProxy::SetAbsolutePosition,
                                               "setRelativeSize", &WidgetProxy::SetRelativeSize,
                                               "setAbsoluteSize", &WidgetProxy::SetAbsoluteSize,
                                               "relativeMoveBy", &WidgetProxy::RelativeMoveBy,
                                               "absoluteMoveBy", &WidgetProxy::AbsoluteMoveBy,
                                               "getLocalAbsolutePosition", &WidgetProxy::GetLocalAbsolutePosition,
                                               "getLocalRelativePosition", &WidgetProxy::GetLocalRelativePosition,
                                               "getFinalPosition", &WidgetProxy::GetFinalPosition,
                                               "getLocalAbsoluteSize", &WidgetProxy::GetLocalAbsoluteSize,
                                               "getLocalRelativeSize", &WidgetProxy::GetLocalRelativeSize,
                                               "getFinalSize", &WidgetProxy::GetFinalSize,
                                               "scaleBy", &WidgetProxy::ScaleBy,
                                               "scaleTo", &WidgetProxy::ScaleTo,
                                               "getLocalScale", &WidgetProxy::GetLocalScale,
                                               "getFinalScale", &WidgetProxy::GetFinalScale,
                                               "rotateTo", &WidgetProxy::RotateTo,
                                               "rotateBy", &WidgetProxy::RotateBy,
                                               "getLocalRotation", &WidgetProxy::GetLocalRotation,
                                               "getFinalRotation", &WidgetProxy::GetFinalRotation,
                                               "setColour", &WidgetProxy::SetColour,
                                               "getLocalColour", &WidgetProxy::GetLocalColour,
                                               "getFinalColour", &WidgetProxy::GetFinalColour,
                                               "setVisible", &WidgetProxy::SetVisible,
                                               "isVisible", &WidgetProxy::IsVisible,
                                               "bringToFront", &WidgetProxy::BringToFront,
                                               "bringForward", &WidgetProxy::BringForward,
                                               "sendToBack", &WidgetProxy::SendToBack,
                                               "sendBackward", &WidgetProxy::SendBackward,
                                               "setSizePolicy", &WidgetProxy::SetSizePolicy,
                                               "getSizePolicy", &WidgetProxy::GetSizePolicy,
                                               "setOriginAnchor", &WidgetProxy::SetOriginAnchor,
                                               "getOriginAnchor", &WidgetProxy::GetOriginAnchor,
                                               "setParentalAnchor", &WidgetProxy::SetParentalAnchor,
                                               "getParentalAnchor", &WidgetProxy::GetParentalAnchor,
                                               "getInternalWidget", &WidgetProxy::GetInternalWidget,
                                               "getWidget", &WidgetProxy::GetWidget,
                                               "getDrawable", &WidgetProxy::GetDrawable,
                                               "getBoolProperty", &WidgetProxy::GetProperty<bool>,
                                               "getIntProperty", &WidgetProxy::GetProperty<s32>,
                                               "getFloatProperty", &WidgetProxy::GetProperty<f32>,
                                               "getStringProperty", &WidgetProxy::GetProperty<std::string>,
                                               "getVec2Property", &WidgetProxy::GetProperty<Core::Vector2>,
                                               "getVec3Property", &WidgetProxy::GetProperty<Core::Vector3>,
                                               "getVec4Property", &WidgetProxy::GetProperty<Core::Vector4>,
                                               "getColourProperty", &WidgetProxy::GetProperty<Core::Colour>,
                                               "getAlignmentAnchorProperty", &WidgetProxy::GetProperty<Rendering::AlignmentAnchor>,
                                               "getSizePolicyProperty", &WidgetProxy::GetProperty<SizePolicy>,
                                               "getStorageLocationProperty", &WidgetProxy::GetProperty<Core::StorageLocation>,
                                               "setBoolProperty", &WidgetProxy::SetProperty<bool>,
                                               "setIntProperty", &WidgetProxy::SetProperty<s32>,
                                               "setFloatProperty", &WidgetProxy::SetProperty<f32>,
                                               "setStringProperty", &WidgetProxy::SetProperty<std::string>,
                                               "setVec2Property", &WidgetProxy::SetProperty<Core::Vector2>,
                                               "setVec3Property", &WidgetProxy::SetProperty<Core::Vector3>,
                                               "setVec4Property", &WidgetProxy::SetProperty<Core::Vector4>,
                                               "setColourProperty", &WidgetProxy::SetProperty<Core::Colour>,
                                               "setAlignmentAnchorProperty", &WidgetProxy::SetProperty<Rendering::AlignmentAnchor>,
                                               "setSizePolicyProperty", &WidgetProxy::SetProperty<SizePolicy>,
                                               "setStorageLocationProperty", &WidgetProxy::SetProperty<Core::StorageLocation>
                                               );
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetName(Widget* in_widget, const std::string& in_name)
            {
                in_widget->SetName(in_name);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            const std::string& GetName(Widget* in_widget)
            {
                return in_widget->GetName();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            IDrawable* GetDrawable(Widget* in_widget)
            {
                return in_widget->GetDrawable();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetRelativeSize(Widget* in_widget, const Core::Vector2& in_size)
            {
                in_widget->SetRelativeSize(in_size);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetLocalRelativeSize(Widget* in_widget)
            {
                return in_widget->GetLocalRelativeSize();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetAbsoluteSize(Widget* in_widget, const Core::Vector2& in_size)
            {
                in_widget->SetAbsoluteSize(in_size);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetLocalAbsoluteSize(Widget* in_widget)
            {
                return in_widget->GetLocalAbsoluteSize();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetSizePolicy(Widget* in_widget, SizePolicy in_policy)
            {
                in_widget->SetSizePolicy(in_policy);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            SizePolicy GetSizePolicy(Widget* in_widget)
            {
                return in_widget->GetSizePolicy();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetRelativePosition(Widget* in_widget, const Core::Vector2& in_pos)
            {
                in_widget->SetRelativePosition(in_pos);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetLocalRelativePosition(Widget* in_widget)
            {
                return in_widget->GetLocalRelativePosition();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetAbsolutePosition(Widget* in_widget, const Core::Vector2& in_pos)
            {
                in_widget->SetAbsolutePosition(in_pos);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetLocalAbsolutePosition(Widget* in_widget)
            {
                return in_widget->GetLocalAbsolutePosition();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void RelativeMoveBy(Widget* in_widget, const Core::Vector2& in_translate)
            {
                in_widget->RelativeMoveBy(in_translate);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void AbsoluteMoveBy(Widget* in_widget, const Core::Vector2& in_translate)
            {
                in_widget->AbsoluteMoveBy(in_translate);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void RotateBy(Widget* in_widget, f32 in_angleRads)
            {
                in_widget->RotateBy(in_angleRads);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void RotateTo(Widget* in_widget, f32 in_angleRads)
            {
                in_widget->RotateTo(in_angleRads);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            f32 GetLocalRotation(Widget* in_widget)
            {
                return in_widget->GetLocalRotation();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void ScaleBy(Widget* in_widget, const Core::Vector2& in_scale)
            {
                in_widget->ScaleBy(in_scale);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void ScaleTo(Widget* in_widget, const Core::Vector2& in_scale)
            {
                in_widget->ScaleTo(in_scale);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetLocalScale(Widget* in_widget)
            {
                return in_widget->GetLocalScale();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetParentalAnchor(Widget* in_widget, Rendering::AlignmentAnchor in_anchor)
            {
                in_widget->SetParentalAnchor(in_anchor);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Rendering::AlignmentAnchor GetParentalAnchor(Widget* in_widget)
            {
                return in_widget->GetParentalAnchor();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetOriginAnchor(Widget* in_widget, Rendering::AlignmentAnchor in_anchor)
            {
                in_widget->SetOriginAnchor(in_anchor);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Rendering::AlignmentAnchor GetOriginAnchor(Widget* in_widget)
            {
                return in_widget->GetOriginAnchor();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetColour(Widget* in_widget, const Core::Colour& in_colour)
            {
                in_widget->SetColour(in_colour);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Core::Colour GetLocalColour(Widget* in_widget)
            {
                return in_widget->GetLocalColour();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetVisible(Widget* in_widget, bool in_visible)
            {
                in_widget->SetVisible(in_visible);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            bool IsVisible(Widget* in_widget)
            {
                return in_widget->IsVisible();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetClippingEnabled(Widget* in_widget, bool in_enabled)
            {
                in_widget->SetClippingEnabled(in_enabled);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            bool IsClippingEnabled(Widget* in_widget)
            {
                return in_widget->IsClippingEnabled();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Widget* GetWidget(Widget* in_widget, const std::string& in_name)
            {
                return in_widget->GetWidget(in_name);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Widget* GetInternalWidget(Widget* in_widget, const std::string& in_name)
            {
                return in_widget->GetInternalWidget(in_name);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void BringToFront(Widget* in_widget)
            {
                in_widget->BringToFront();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void BringForward(Widget* in_widget)
            {
                in_widget->BringForward();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SendBackward(Widget* in_widget)
            {
                in_widget->SendBackward();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SendToBack(Widget* in_widget)
            {
                in_widget->SendToBack();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetFinalPosition(Widget* in_widget)
            {
                return in_widget->GetFinalPosition();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetFinalSize(Widget* in_widget)
            {
                return in_widget->GetFinalSize();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetPreferredSize(Widget* in_widget)
            {
                return in_widget->GetPreferredSize();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            f32 GetFinalRotation(Widget* in_widget)
            {
                return in_widget->GetFinalRotation();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetFinalScale(Widget* in_widget)
            {
                return in_widget->GetFinalScale();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Core::Colour GetFinalColour(Widget* in_widget)
            {
                return in_widget->GetFinalColour();
            }
        }
    }
}
