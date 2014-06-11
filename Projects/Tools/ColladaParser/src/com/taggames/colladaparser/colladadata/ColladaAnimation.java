//
// ColladaAnimation.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparser.colladadata;

import java.util.Hashtable;
import java.util.LinkedList;

public class ColladaAnimation 
{
	public Hashtable<String, ColladaSource> mSourceTable;
	public Hashtable<String, ColladaSampler> mSamplerTable;
	public LinkedList<ColladaChannel> mChannelList;
	
	public ColladaAnimation()
	{
		mSourceTable = new Hashtable<String, ColladaSource>();
		mSamplerTable = new Hashtable<String, ColladaSampler>();
		mChannelList = new LinkedList<ColladaChannel>();
	}
}
