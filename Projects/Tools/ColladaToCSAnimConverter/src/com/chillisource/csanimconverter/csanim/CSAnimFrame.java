package com.chillisource.csanimconverter.csanim;

import java.util.LinkedList;

import com.taggames.toolutils.CQuaternion;
import com.taggames.toolutils.CVector3;

public class CSAnimFrame 
{
	public LinkedList<CVector3> mNodeTranslations;
	public LinkedList<CQuaternion> mNodeOrienations;
	public LinkedList<CVector3> mNodeScalings;
	
	public CSAnimFrame()
	{
		mNodeOrienations = new LinkedList<CQuaternion>();
		mNodeTranslations = new LinkedList<CVector3>();
		mNodeScalings = new LinkedList<CVector3>();
	}
}
