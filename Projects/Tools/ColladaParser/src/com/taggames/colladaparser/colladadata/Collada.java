//
// Collada.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.taggames.colladaparser.colladadata;

import java.util.Hashtable;

public class Collada 
{
	public ColladaAsset mAsset;
	public Hashtable<String, ColladaGeometry> mLibraryGeometries;
	public Hashtable<String, ColladaImage> mLibraryImages;
	public Hashtable<String, ColladaMaterial> mLibraryMaterials;
	public Hashtable<String, ColladaEffect> mLibraryEffects;
	public Hashtable<String, ColladaVisualScene> mLibraryVisualScenes;
	public Hashtable<String, ColladaController> mLibraryControllers;
	public ColladaAnimation mAnimation;
	public ColladaScene mScene;
	
	public Collada()
	{
		mAsset = new ColladaAsset();
		mLibraryGeometries = new Hashtable<String, ColladaGeometry>();
		mLibraryImages = new Hashtable<String, ColladaImage>();
		mLibraryMaterials = new Hashtable<String, ColladaMaterial>();
		mLibraryEffects = new Hashtable<String, ColladaEffect>();
		mLibraryControllers = new Hashtable<String, ColladaController>();
		mLibraryVisualScenes = new Hashtable<String, ColladaVisualScene>();
		mAnimation = new ColladaAnimation();
		mScene = new ColladaScene();
	}
}
