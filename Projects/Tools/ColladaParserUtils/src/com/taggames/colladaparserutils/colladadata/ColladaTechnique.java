//
// ColladaTechnique.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparserutils.colladadata;

public class ColladaTechnique 
{
	public String mstrSId;
	public String mstrProfile;
	public ColladaBlinnAndPhong mBlinnAndPhong;
	public ColladaUserProperties mUserProperties; 
	
	public ColladaTechnique()
	{
		mstrSId = new String();
		mstrProfile = new String();
		mBlinnAndPhong = new ColladaBlinnAndPhong();
		mUserProperties = new ColladaUserProperties();
	}
}
