--
--  VerticalSlider.lua
--  Chilli Source
--  Created by Scott Downie on 30/08/2014.
--
--  The MIT License (MIT)
--
--  Copyright (c) 2014 Tag Games Limited
--
--  Permission is hereby granted, free of charge, to any person obtaining a copy
--  of this software and associated documentation files (the "Software"), to deal
--  in the Software without restriction, including without limitation the rights
--  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
--  copies of the Software, and to permit persons to whom the Software is
--  furnished to do so, subject to the following conditions:
--
--  The above copyright notice and this permission notice shall be included in
--  all copies or substantial portions of the Software.
--
--  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
--  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
--  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
--  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
--  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
--  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
--  THE SOFTWARE.
--

-------------------------------------------------------------
-- This is the logic that describes a vertical slider bar.
-- The slider bar consists of 2 widgets the bar and the slider.
-- The slider runs on a rail that goes from from the start
-- of the bar to the end of the bar and moves only vertically.
-- The slider responds to user dragging input and will respond to multiple
-- pointers but only to the primary input type
--
-- @author S Downie
-------------------------------------------------------------

--Caching the functions locally means we don't have to peform a lookup for each call
local getPointerPosition = Pointer.getPosition
local getFinalPositionOfCentre = Widget.getFinalPositionOfCentre
local getFinalSize = Widget.getFinalSize
local setRelativePosition = Widget.setRelativePosition
local setFloatCustomProperty = Widget.setFloatCustomProperty

local sliderWidget = nil
local isDragging = false

-----------------------------------------------------
-- Called when the widget is added to the canvas
--
-- @author S Downie
-----------------------------------------------------
function onAddedToCanvas()
    sliderWidget = Widget.getInternalWidget(this, "Slider")
end
-----------------------------------------------------
-- Called when the widget recieves an input press
-- event within its bounds. This will force the slider
-- to jump to the input position
--
-- @author S Downie
--
-- @param Pointer that triggered the event
-- @param Timestamp of the input event
-- @param Input type i.e. touch, left mouse, right mouse, etc.
-----------------------------------------------------
function onPressedInside(in_pointer, in_timeStamp, in_inputType)
    if in_inputType == InputType.Default then
        isDragging = true
        pointerPosX, pointerPosY = getPointerPosition(in_pointer)
        updateSlider(pointerPosY)
    end
end
-----------------------------------------------------
-- Called when the widget receives an input move event
-- within its bounds. This causes the slider to move
-- if the slider is in the moving state.
--
-- @author S Downie
--
-- @param Pointer that triggered the event
-- @param Timestamp of the input event
-----------------------------------------------------
function onDraggedInside(in_pointer, in_timeStamp)
    if isDragging == true then
        pointerPosX, pointerPosY = getPointerPosition(in_pointer)
        updateSlider(pointerPosY)
    end
end
-----------------------------------------------------
-- Called when the widget receives an input move event
-- outside its bounds. This causes the slider to move
-- if the slider is in the moving state.
--
-- @author Ian Copland
--
-- @param Pointer that triggered the event
-- @param Timestamp of the input event
-----------------------------------------------------
function onDraggedOutside(in_pointer, in_timeStamp)
    if isDragging == true then
        pointerPosX, pointerPosY = getPointerPosition(in_pointer)
        updateSlider(pointerPosY)
    end
end
-----------------------------------------------------
-- Called when a pointer up event occurs inside the
-- the widget. This will listen for default input
-- type releases and stop the slider moving.
--
-- @author Ian Copland
--
-- @param Pointer that triggered the event
-- @param Timestamp of the input event
-- @param Input type i.e. touch, left mouse, right 
-- mouse, etc.
-----------------------------------------------------
function onReleasedInside(in_pointer, in_timeStamp, in_inputType)
    if in_inputType == InputType.Default then
        pointerPosX, pointerPosY = getPointerPosition(in_pointer)
        updateSlider(pointerPosY)
        isDragging = false
    end
end
-----------------------------------------------------
-- Called when a pointer up event occurs outside the
-- the widget. This will listen for default input
-- type releases and stop the slider moving.
--
-- @author Ian Copland
--
-- @param Pointer that triggered the event
-- @param Timestamp of the input event
-- @param Input type i.e. touch, left mouse, right 
-- mouse, etc.
-----------------------------------------------------
function onReleasedOutside(in_pointer, in_timeStamp, in_inputType)
    if in_inputType == InputType.Default then
        pointerPosX, pointerPosY = getPointerPosition(in_pointer)
        updateSlider(pointerPosY)
        isDragging = false
    end
end
-----------------------------------------------------
-- Move the slider to the correct position based
-- on the given input position
--
-- @author S Downie
--
-- @param Pointer input position Y
-----------------------------------------------------
function updateSlider(in_pointerPositionY)
    sliderWidth, sliderHeight = getFinalSize(sliderWidget)
    barWidth, barHeight = getFinalSize(this)
    barPosX, barPosY = getFinalPositionOfCentre(this)
    barBottomY = barPosY - 0.5 * barHeight

    --calculate the new slider fraction 
    slideRange =  math.max(barHeight - sliderHeight, 0.0)
    pointerPositionOnBar = math.max((in_pointerPositionY - barBottomY - 0.5 * sliderHeight), 0.0)
    sliderFraction = math.max(math.min((pointerPositionOnBar / slideRange), 1.0), 0.0)
    setFloatCustomProperty(this, "SliderFraction", sliderFraction)

    --convert the slider fraction to a relative position for the slider
    sliderPosition = sliderFraction * (slideRange / barHeight);
    setRelativePosition(sliderWidget, 0.0, sliderPosition)
end
-----------------------------------------------------
-- Called when a widget custom property is changed
--
-- @author HMcLaughlin
--
-- @param Custom property name
-- @param Custom property value
-----------------------------------------------------
function onCustomPropertyChanged(in_propName, in_propValue)
    if in_propName == "SliderFraction" then
        sliderWidth, sliderHeight = getFinalSize(sliderWidget)
        barWidth, barHeight = getFinalSize(this)
        slideRange =  math.max(barHeight - sliderHeight, 0.0)

        --convert the slider fraction to a relative position for the slider
        sliderPosition = in_propValue * (slideRange / barHeight);
        setRelativePosition(sliderWidget, 0.0, sliderPosition)
    end
end



