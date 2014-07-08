/**
 * NativeKeyboardInputView.java
 * Chilli Source
 * Created by Ian Copland on 04/02/2014.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Tag Games Limited
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

package com.chillisource.input;

import com.chillisource.core.CSApplication;

import android.content.Context;
import android.text.InputType;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;

//============================================================
/// Native Keyboard Input View
/// 
/// An invisible view for receiving keyboard and passing it
/// on to the native side of the engine. This will not consume
/// touches and therefore can be placed over other views that
/// require touches. This class is not thread-safe, it should
/// only be used on the UI thread.
//============================================================
public class NativeKeyboardInputView extends View
{
	//------------------------------------------------------
	/// Keyboard Type
	///
	/// Describes the different keyboard types that can be
	/// created.
	//------------------------------------------------------
    enum KeyboardType
	{
		TEXT,
		NUMERIC
	};
	//------------------------------------------------------
	/// KeyboardCapitalisation
	///
	/// Describes the various keyboard capitalisation methods
	/// that can be used.
	//------------------------------------------------------
    enum KeyboardCapitalisation
    {
        NONE,
        WORDS,
        SENTENCES,
        ALL
    };
	//------------------------------------------------------
	/// Member data
	//------------------------------------------------------
	boolean mbKeyboardActive = false;
	boolean mbHardwareKeyboardShown = false;
	TextEntryNativeInterface mTextEntryNativeInterface = null;
	KeyboardType meKeyboardType;
	KeyboardCapitalisation meKeyboardCapitalisation;
	//------------------------------------------------------
	/// Constructor
	///
	/// @param The chilli source activity.
	/// @param The chilli source surface.
	//------------------------------------------------------
	public NativeKeyboardInputView(TextEntryNativeInterface inNativeInterface, KeyboardType ineKeyboardType, KeyboardCapitalisation ineKeyboardCapitalisation) 
	{
		super(CSApplication.get().getActivityContext());
		
		mTextEntryNativeInterface = inNativeInterface;
		meKeyboardType = ineKeyboardType;
		meKeyboardCapitalisation = ineKeyboardCapitalisation;
		
		//set as focusable
		setFocusable(true);
        setFocusableInTouchMode(true);
	}
	//------------------------------------------------------
	/// On Check Is Text Editor
	///
	/// @return Whether or not this view is a text editor - 
	/// i.e needs to be able to handle a keyboard. We always 
	/// return true for this.
	//------------------------------------------------------
	@Override public boolean onCheckIsTextEditor() 
	{
		return true;
	}
	//------------------------------------------------------
	/// On Create Input Connection
	///
	/// Registers an Input Connection and sets up the editor 
	/// attributes for the keyboard.
	///
	/// @param Output editor attributes.
	/// @return The input connection for the IME to use.
	//------------------------------------------------------
	@Override public InputConnection onCreateInputConnection(EditorInfo outAttrs) 
	{
		int dwInputTypeFlags = 0;
		
		//set the keyboard type
		switch (meKeyboardType)
		{
		case NUMERIC:
			dwInputTypeFlags = InputType.TYPE_CLASS_NUMBER;
			break;
		case TEXT:
		default:
			dwInputTypeFlags = InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_NORMAL | InputType.TYPE_TEXT_FLAG_NO_SUGGESTIONS;
			break;
		}
		
		//set the keyboard capitalisation
		if ((dwInputTypeFlags | InputType.TYPE_CLASS_TEXT) != 0)
		{
			switch (meKeyboardCapitalisation)
			{
			case ALL:
				dwInputTypeFlags |= InputType.TYPE_TEXT_FLAG_CAP_CHARACTERS;
				break;
			case SENTENCES:
				dwInputTypeFlags |= InputType.TYPE_TEXT_FLAG_CAP_SENTENCES;
				break;
			case WORDS:
				dwInputTypeFlags |= InputType.TYPE_TEXT_FLAG_CAP_WORDS;
				break;
			default:
				break;
			
			}
		}
        
		//set the output attributes to determine the type
		outAttrs.actionLabel = "Done";
		outAttrs.inputType = dwInputTypeFlags;
        outAttrs.imeOptions |= EditorInfo.IME_FLAG_NO_EXTRACT_UI | EditorInfo.IME_ACTION_NONE;
		
        //init the input connection
        BaseInputConnection inputConnection = new BaseInputConnection(this, false);
        return inputConnection;
    }
	//-----------------------------------------------------
	/// On Key Multiple
	///
	/// This is for keyboards that suggest words and feed 
	/// them in bulk.
	///
	/// @param key code
	/// @param Count
	/// @param Key event that contains the character string
	//-----------------------------------------------------
	@Override public boolean onKeyMultiple (int indwKeyCode, int inCount, KeyEvent inEvent)
	{
		//check for keyboard events if the keyboard is active
		if (mbKeyboardActive == true && indwKeyCode != KeyEvent.KEYCODE_DEL)
		{
			String strCharacters = inEvent.getCharacters();
			mTextEntryNativeInterface.OnTextAdded(strCharacters);	
			return true;
		}
		
		return false;
	}
	//----------------------------------------------------
	/// On Key Down
	///
	/// Whenever a key is pressed down this is called 
	/// reporting the key code and the event.
	///
	/// @param The key code.
	/// @param The event.
	/// @return Whether or not this view wants to pass on 
	/// the key event to the next applicable view.
	//----------------------------------------------------
	@Override public boolean onKeyDown(int indwKeyCode, KeyEvent inEvent)
	{
		//check for keyboard events if the keyboard is active
		if (mbKeyboardActive == true)
		{
			//get the unicode character
			int dwUnicode = inEvent.getUnicodeChar();
				
			//if the unicode character is non-zero, then the character should be added to the keyboard text string
			//other wise check for other keycodes of interest
			if (dwUnicode != 0)
			{
				//check for enter
				if (indwKeyCode == KeyEvent.KEYCODE_ENTER)
				{
					DeactivateKeyboard();
					mTextEntryNativeInterface.OnKeyboardDismissed();
				}
				else
				{
					String strText = "";
					strText += (char)dwUnicode;
					mTextEntryNativeInterface.OnTextAdded(strText);
				}
			}
			else if (indwKeyCode == KeyEvent.KEYCODE_DEL)
			{
				mTextEntryNativeInterface.OnTextDeleted();
			}
		
			return true;
		}
		
		//check for keys we want to disable being pressed and disable it.
		if (indwKeyCode == KeyEvent.KEYCODE_SEARCH || indwKeyCode == KeyEvent.KEYCODE_MENU)
			return true;
		
		return false;
	}
	//----------------------------------------------------
	/// On Key Pre Ime
	///
	/// View hardware key presses before the IME gets to 
	/// view them. This allows us to check for hardware 
	/// button presses that the IME will consume, the IME 
	/// will close the keyboard without informing us if Back 
	/// is pressed. Using this, we can know if the keyboard 
	/// is about to close.
	///
	/// @param The key code.
	/// @param The event.
	/// @return Whether or not this view wants to pass on 
	/// the key event to the next applicable view. In our 
	/// case this will always be false, as we want to pass 
	/// it on to the IME or onKeyDown.
	//----------------------------------------------------
	@Override public boolean onKeyPreIme(int indwKeyCode, KeyEvent inEvent) 
	{
		if (inEvent.getAction() == KeyEvent.ACTION_DOWN)
		{
			//check for back being pressed
			if (indwKeyCode == KeyEvent.KEYCODE_BACK && mbKeyboardActive == true && mbHardwareKeyboardShown == false)
			{
				mbKeyboardActive = false;
				mTextEntryNativeInterface.OnKeyboardDismissed();
			}
		}
		
		return false;
	};
	//-----------------------------------------------------
	/// On Key Up
	///
	/// Handles the On Key Up event by doing nothing.
	///
	/// @param The keycode.
	/// @param The event.
	//-----------------------------------------------------
	@Override public boolean onKeyUp(int indwKeyCode, KeyEvent inEvent) 
	{
		return false;
	};
	//-----------------------------------------------------
	/// On Key Long Press
	///
	/// Handles the On Key Long Press event by doing nothing.
	///
	/// @param The keycode.
	/// @param The event.
	//-----------------------------------------------------
	@Override public boolean onKeyLongPress(int indwKeyCode, KeyEvent inEvent)
	{
		return false;
	};
	//-----------------------------------------------------
	/// On Key Up
	///
	/// Handles the On Key Shortcut event by doing nothing.
	///
	/// @param The keycode.
	/// @param The event.
	//-----------------------------------------------------
	@Override public boolean onKeyShortcut(int indwKeyCode, KeyEvent inEvent)
	{
		return false;
	};
	//------------------------------------------------------
	/// Activate Keyboard
	///
	/// Displays the software keyboard on screen and starts
	/// reading from the hardware keyboard if possible.
	//------------------------------------------------------
	public void ActivateKeyboard()
	{
		if (mbKeyboardActive == false)
		{
			requestFocus();
			
			InputMethodManager imm = (InputMethodManager)getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
			imm.showSoftInput(this, InputMethodManager.SHOW_FORCED);
			mbKeyboardActive = true;
		}
	}
	//------------------------------------------------------
	/// Deactivate Keyboard
	///
	/// Hides the software keyboard and stops reading from 
	/// the hardware keyboard if there is one. Accessible 
	/// from native.
	//------------------------------------------------------
	public void DeactivateKeyboard()
	{
		if (mbKeyboardActive == true)
		{
			InputMethodManager imm = (InputMethodManager)getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
			imm.toggleSoftInput(0, 0);
			
			mbKeyboardActive = false;
		}
	}
	//-----------------------------------------------------
	/// Set Hardware Keyboard Open
	///
	/// Informs the view that the hardware keyboard has
	/// opened.
	//-----------------------------------------------------
	public void SetHardwareKeyboardOpen()
	{
		if (mbHardwareKeyboardShown == false)
		{
			mbHardwareKeyboardShown = true;
		}
	}
	//----------------------------------------------------
	/// Set Hardware Keyboard Closed
	///
	/// Informs the view that the hardware keyboard has
	/// closed.
	//----------------------------------------------------
	public void SetHardwareKeyboardClosed()
	{
		if (mbHardwareKeyboardShown == true)
		{
			mbHardwareKeyboardShown = false;
			
			if (mbKeyboardActive == true)
			{
				mbKeyboardActive = false;
				mTextEntryNativeInterface.OnKeyboardDismissed();
			}
		}
	}
}
