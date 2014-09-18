-------------------------------------------------------------
-- This is the logic that describes a horizontal slider bar.
-- The slider bar consists of 2 widgets the bar and the slider.
-- The slider runs on a rail that goes from from the start
-- of the bar to the end of the bar and moves only horizontally.
-- The slider responds to user dragging input and will respond to multiple
-- pointers but only to the primary input type
--
-- @author S Downie
-------------------------------------------------------------

--Caching the functions locally means we don't have to peform a lookup for each call
local getPointerPosition = Pointer.getPosition
local isInputDown = Pointer.isInputDown
local getFinalPosition = Widget.getFinalPosition
local getFinalSize = Widget.getFinalSize
local setRelativePosition = Widget.setRelativePosition

local sliderWidget = nil
local sliderFraction = 0.0
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
        pointerPosX = getPointerPosition(in_pointer)
        updateSlider(pointerPosX)
    end
end
-----------------------------------------------------
-- Called when the widget receives an input move event
-- within its bounds. This causes the slider to move
-- provided the primary input is down.
--
-- @author S Downie
--
-- @param Pointer that triggered the event
-- @param Timestamp of the input event
-----------------------------------------------------
function onMovedInside(in_pointer, in_timeStamp)
    if isInputDown(in_pointer, InputType.Default) then
        pointerPosX = getPointerPosition(in_pointer)
        updateSlider(pointerPosX)
    end
end
-----------------------------------------------------
-- Move the slider to the correct position based
-- on the given input position
--
-- @author S Downie
--
-- @param Pointer input position X
-----------------------------------------------------
function updateSlider(in_pointerPositionX)
    barPosX = getFinalPosition(this)
    barWidth = getFinalSize(this)
    sliderFraction = ((in_pointerPositionX - barPosX)/barWidth) + 0.5;
    setRelativePosition(sliderWidget, sliderFraction, 0.0)
end



