package com.chillisource.csanimconverter.csanim;

import java.util.LinkedList;

import com.chillisource.toolutils.Quaternion;
import com.chillisource.toolutils.Vector3;

public class CSAnimFrame 
{
	public LinkedList<Vector3> mNodeTranslations;
	public LinkedList<Quaternion> mNodeOrienations;
	public LinkedList<Vector3> mNodeScalings;
	
	public CSAnimFrame()
	{
		mNodeOrienations = new LinkedList<Quaternion>();
		mNodeTranslations = new LinkedList<Vector3>();
		mNodeScalings = new LinkedList<Vector3>();
	}
}
