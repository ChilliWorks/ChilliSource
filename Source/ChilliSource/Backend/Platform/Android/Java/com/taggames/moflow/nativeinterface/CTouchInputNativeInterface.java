package com.taggames.moflow.nativeinterface;

public class CTouchInputNativeInterface 
{
	public static native void TouchDown(int indwID, float infX, float infY);
	public static native void TouchUp(int indwID, float infX, float infY);
	public static native void TouchMoved(int indwID, float infX, float infY);
}
