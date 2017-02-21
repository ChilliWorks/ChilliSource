//
//  ProgressBarUIComponent.h
//  ChilliSource
//  Created by Ian Copland on 01/12/2014.
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

#ifndef _CHILLISOURCE_UI_PROGRESSBAR_PROGRESSBARUICOMPONENT_H_
#define _CHILLISOURCE_UI_PROGRESSBAR_PROGRESSBARUICOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/Property/PropertyMap.h>
#include <ChilliSource/Core/Event/EventConnection.h>
#include <ChilliSource/Input/Pointer/Pointer.h>
#include <ChilliSource/UI/Base/UIComponent.h>
#include <ChilliSource/UI/ProgressBar/ProgressBarDirection.h>
#include <ChilliSource/UI/ProgressBar/ProgressBarType.h>

namespace ChilliSource
{
    //--------------------------------------------------------------------------
    /// A logic component for controlling a progress bar. This handles both
    /// vertical and horizontal progress bar types.
    ///
    /// "RelBarSize": A Vector2 describing the relative size of the bar when
    /// progress is at 100%. This defaults to (1.0, 1.0).
    ///
    /// "Progress": A float describing the current progress of the bar. This
    /// must be in the range 0.0-1.0. If a value outside this range is provided
    /// the app is considered to be in an irrecoverable state and will terminate.
    /// This defaults to 0.0.
    ///
    /// "BarType": A string describing the type of progress bar. The possible
    /// values for this are "Fill" or "Stretch". A stretch progress bar
    /// simply changes the size of the bar widget as progress changes. A
    /// fill progress bar will also alter the UVs of the widgets drawable
    /// such that it appears to reveal more of the bar as it progresses. This
    /// defaults to "Fill". If using Fill the underlying bar widget must
    /// have a UIDrawable UIComponent, otherwise the app is considered to be in an
    /// irrecoverable state and will terminate.
    ///
    /// "BarDirection": A string describing the direction of the progress bar.
    /// The possible values are 'Horizontal' or 'Vertical' and defaults to
    /// 'Horizontal'.
    ///
    /// "BarWidgetName": The name of the internal widget that should be used
    /// as the progress bar. This cannot be changed after the initial creation
    /// of the component. Attempting to set it using SetProperty will cause the
    /// app to be in an irrecoverable state and terminate. Typically this is not
    /// exposed to the user of the widget.
    ///
    /// @author Ian Copland
    //--------------------------------------------------------------------------
    class ProgressBarUIComponent final : public UIComponent
    {
    public:
        CS_DECLARE_NAMEDTYPE(ProgressBarUIComponent);
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The list of properties supported by a progress bar component.
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
        /// @param The relative size of the bar when it is at 100% progress.
        //-------------------------------------------------------------------
        const Vector2& GetRelativeBarSize() const;
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param The current progress of the bar.
        //-------------------------------------------------------------------
        f32 GetProgress() const;
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param The type of the progress bar: Fill or Stretch.
        //-------------------------------------------------------------------
        ProgressBarType GetBarType() const;
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param The direction of the progress bar: horizontal or vertical.
        //-------------------------------------------------------------------
        ProgressBarDirection GetBarDirection() const;
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The name of the bar widget.
        //-------------------------------------------------------------------
        const std::string& GetBarWidgetName() const;
        //-------------------------------------------------------------------
        /// Sets the relative size of the bar when it is at 100% progress.
        ///
        /// @author Ian Copland
        ///
        /// @param The new relative size.
        //-------------------------------------------------------------------
        void SetRelativeBarSize(const Vector2& in_relSize);
        //-------------------------------------------------------------------
        /// Sets the current progress of the bar. This must be in the range
        /// 0.0 to 1.0. If a value outside this range is provided the app
        /// is considered to be in an irrecoverable state and will terminate.
        ///
        /// @author Ian Copland
        ///
        /// @param The new progress.
        //-------------------------------------------------------------------
        void SetProgress(f32 in_progress);
        //-------------------------------------------------------------------
        /// Sets the type of the progress bar: Fill or Stretch.
        ///
        /// @author Ian Copland
        ///
        /// @param The bar type.
        //-------------------------------------------------------------------
        void SetBarType(ProgressBarType in_barType);
        //-------------------------------------------------------------------
        /// Sets the direction of the progress bar: horizontal or vertical.
        ///
        /// @author Ian Copland
        ///
        /// @param The direction.
        //-------------------------------------------------------------------
        void SetBarDirection(ProgressBarDirection in_direction);

    private:
        friend class UIComponentFactory;
        //-------------------------------------------------------------------
        /// Constructor that builds the component from key-value properties.
        /// The properties used to create a progress bar component are described
        /// in the class documentation.
        ///
        /// @author Ian Copland
        ///
        /// @param The component name.
        /// @param The property map.
        //-------------------------------------------------------------------
        ProgressBarUIComponent(const std::string& in_componentName, const PropertyMap& in_properties);
        //-------------------------------------------------------------------
        /// Sets the name of the bar widget. This can only be called once,
        /// on construction. If this is called a second time the app is
        /// considered to be in an irrecoverable state and will terminate.
        ///
        /// @author Ian Copland
        ///
        /// @return The name of the bar widget.
        //-------------------------------------------------------------------
        void SetBarWidgetName(const std::string& in_name);
        //-------------------------------------------------------------------
        /// Updates the size of the bar and applies fill if the bar is of
        /// the "fill" type.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------
        void UpdateBar();
        //-------------------------------------------------------------------
        /// Updates the size of the bar and applies fill if the bar is of
        /// the "fill" type for horizontal bar types.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------
        void UpdateHorizontalBar();
        //-------------------------------------------------------------------
        /// Updates the size of the bar and applies fill if the bar is of
        /// the "fill" type for vertical bar types.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------
        void UpdateVerticalBar();
        //-------------------------------------------------------------------
        /// Called when the component is first added to the owning widget.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------
        void OnInit() override;
        //-------------------------------------------------------------------
        /// Called when the owning widget is being destructed.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------
        void OnDestroy() override;
        
        Vector2 m_relBarSize;
        f32 m_progress = 0.0f;
        ProgressBarType m_barType = ProgressBarType::k_fill;
        ProgressBarDirection m_direction = ProgressBarDirection::k_horizontal;
        std::string m_barWidgetName;
        
        Widget* m_barWidget = nullptr;
        DrawableUIComponent* m_barDrawableComponent = nullptr;
    };
}

#endif
