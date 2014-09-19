--
--  HighlightButton.lua
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
local setColour = Widget.setColour
local getDrawable = Widget.getDrawable
local getStorageLocationProperty = Widget.getStorageLocationProperty
local getStringProperty = Widget.getStringProperty
local getColourProperty = Widget.getColourProperty
local getPointerId = Pointer.getId

local activePointerIds = {}
local highlightingPointerIds = {}

-----------------------------------------------------
-- Called when the widget is added to the canvas
--
-- @author S Downie
-----------------------------------------------------
function onAddedToCanvas()
    unhighlight()
end
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
    if in_inputType == InputType.Default then
        pointerId = getPointerId(in_pointer)
        activePointerIds[pointerId] = true
        highlightingPointerIds[pointerId] = true
        highlight()
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
    pointerId = getPointerId(in_pointer)
    if activePointerIds[pointerId] then
        highlightingPointerIds[pointerId] = true
        highlight()
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
    pointerId = getPointerId(in_pointer)
    if activePointerIds[pointerId] then
        highlightingPointerIds[pointerId] = nil
        if next(highlightingPointerIds) == nil then
            unhighlight()
        end
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
        highlightingPointerIds[pointerId] = nil
        if next(highlightingPointerIds) == nil then
            unhighlight()
        end
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
-----------------------------------------------------
-- Set the view of the button to highlighted
--
-- @author S Downie
-----------------------------------------------------
function highlight()
    path = getStringProperty(this, "HighlightTexturePath")
    drawable = getDrawable(this)
    if drawable and path ~= "" then
        location = getStorageLocationProperty(this, "HighlightTextureLocation")
        setTexture(drawable, location, path)
    end

    r, g, b, a = getColourProperty(this, "HighlightColour")
    setColour(this, r, g, b, a)
end
-----------------------------------------------------
-- Set the view of the button to not be highlighted
--
-- @author S Downie
-----------------------------------------------------
function unhighlight()
    path = getStringProperty(this, "NormalTexturePath")
    drawable = getDrawable(this)
    if drawable and path ~= "" then
        location = getStorageLocationProperty(this, "NormalTextureLocation")
        setTexture(drawable, location, path)
    end

    r, g, b, a = getColourProperty(this, "NormalColour")
    setColour(this, r, g, b, a)
end
