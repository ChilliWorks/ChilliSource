package com.taggames.moanimconverter.moanim;

import java.util.LinkedList;

import com.taggames.toolutils.CQuaternion;
import com.taggames.toolutils.CVector3;

public class MoAnimFrame 
{
	public LinkedList<CVector3> mNodeTranslations;
	public LinkedList<CQuaternion> mNodeOrienations;
	public LinkedList<CVector3> mNodeScalings;
	
	public MoAnimFrame()
	{
		mNodeOrienations = new LinkedList<CQuaternion>();
		mNodeTranslations = new LinkedList<CVector3>();
		mNodeScalings = new LinkedList<CVector3>();
	}
}
