----------------------------------------------------------------
-- This is the logic that describes a simple highlight button.
-- A highlight button will highlight when pressed and unhighlight
-- when released or input is moved outwith the widget. The button
-- only responds to the primary input type but multiple pointers
-- can interact with the button at the same time.
--
-- THe highlight can be created either with an additional texture
-- or simply a colour change.
--
-- @author S Downie
----------------------------------------------------------------

--Caching the functions locally means we don't have to peform a lookup for each call
local setTexture = Drawable.setTexture
local getDrawable = Widget.getDrawable
local getStorageLocationProperty = Widget.getStorageLocationProperty
local getStringProperty = Widget.getStringProperty
local getPointerId = Pointer.getId

local activePointerIds = {}

-----------------------------------------------------
-- Called when the widget recieves an input press
-- event within its bounds. This will toggle the
-- highlight.
--
-- @author S Downie
--
-- @param Pointer that triggered the event
-- @param Timestamp of the input event
-- @param Input type i.e. touch, left mouse, right mouse, etc.
-----------------------------------------------------
function onPressedInside(in_pointer, in_timeStamp, in_inputType)
    if(in_inputType == InputType.Default) then
        activePointerIds[getPointerId(in_pointer)] = true
        drawable = getDrawable(this)
        highlightLocation = getStorageLocationProperty(this, "HighlightTextureLocation")
        highlightPath = getStringProperty(this, "HighlightTexturePath")
        setTexture(drawable, highlightLocation, highlightPath)
    end
end
-----------------------------------------------------
-- Called when the widget recieves an input move
-- event within its bounds having previously receieved
-- one outside the bounds. This will toggle back to highlight
-- state.
--
-- @author S Downie
--
-- @param Pointer that triggered the event
-- @param Timestamp of the input event
-----------------------------------------------------
function onMoveEntered(in_pointer, in_timeStamp)
    if activePointerIds[getPointerId(in_pointer)] then
        drawable = getDrawable(this)
        highlightLocation = getStorageLocationProperty(this, "HighlightTextureLocation")
        highlightPath = getStringProperty(this, "HighlightTexturePath")
        setTexture(drawable, highlightLocation, highlightPath)
    end
end
-----------------------------------------------------
-- Called when the widget recieves an input move
-- event outside its bounds having previously receieved
-- one inside the bounds. This will toggle back to normal
-- state.
--
-- @author S Downie
--
-- @param Pointer that triggered the event
-- @param Timestamp of the input event
-----------------------------------------------------
function onMoveExited(in_pointer, in_timeStamp)
    if activePointerIds[getPointerId(in_pointer)] then
        drawable = getDrawable(this)
        normalLocation = getStorageLocationProperty(this, "NormalTextureLocation")
        normalPath = getStringProperty(this, "NormalTexturePath")
        setTexture(drawable, normalLocation, normalPath)
    end
end
-----------------------------------------------------
-- Called when the widget recieves an input released
-- event within its bounds having previously receieved
-- a pressed event. This will toggle back to normal.
--
-- @author S Downie
--
-- @param Pointer that triggered the event
-- @param Timestamp of the input event
-- @param Input type i.e. touch, left mouse, right mouse, etc.
-----------------------------------------------------
function onReleasedInside(in_pointer, in_timeStamp, in_inputType)
    pointerId = getPointerId(in_pointer)
    if activePointerIds[pointerId] then
        activePointerIds[pointerId] = nil
        drawable = getDrawable(this)
        normalLocation = getStorageLocationProperty(this, "NormalTextureLocation")
        normalPath = getStringProperty(this, "NormalTexturePath")
        setTexture(drawable, normalLocation, normalPath)
    end
end
-----------------------------------------------------
-- Called when the widget recieves an input released
-- event outside its bounds having previously receieved
-- a pressed event.
--
-- @author S Downie
--
-- @param Pointer that triggered the event
-- @param Timestamp of the input event
-- @param Input type i.e. touch, left mouse, right mouse, etc.
-----------------------------------------------------
function onReleasedOutside(in_pointer, in_timeStamp, in_inputType)
    pointerId = getPointerId(in_pointer)
    if activePointerIds[pointerId] then
        activePointerIds[pointerId] = nil
    end
end
