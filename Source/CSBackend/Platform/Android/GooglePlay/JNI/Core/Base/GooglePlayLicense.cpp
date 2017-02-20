//
//  GooglePlayLicense.cpp
//  ChilliSource
//  Created by Ian Copland on 27/05/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifdef CS_TARGETPLATFORM_ANDROID

#ifdef CS_ANDROIDFLAVOUR_GOOGLEPLAY

#include <ChilliSource/ChilliSource.h>

#include <jni.h>

//------------------------------------------------------------------------------
/// Returns the Google Play Lvl Public Key. This must return an identical value
/// every time it is called.
///
/// This may be called on any thread.
///
/// @author Ian Copland
///
/// @return The LVL public Key.
//------------------------------------------------------------------------------
extern std::string GetGooglePlayLvlPublicKey();

extern "C"
{
    //------------------------------------------------------------------------------
    /// A method called from java which will return the user supplied Google Play
    /// LVL Public Key. This key is required by the various google play systems
    /// which need to verify the the application was downloaded from the google
    /// play store.
    ///
    /// @author Ian Copland
    ///
    /// @param in_environment - The JNI Environment.
    /// @param in_this - The java object that called the method.
    //------------------------------------------------------------------------------
	jstring Java_com_chilliworks_chillisource_core_GooglePlayLicensing_calcLvlPublicKey(JNIEnv* in_environment, jobject in_this);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
jstring Java_com_chilliworks_chillisource_core_GooglePlayLicensing_calcLvlPublicKey(JNIEnv* in_environment, jobject in_this)
{
    auto publicKey = GetGooglePlayLvlPublicKey();

    //Convert to jstring directly using the environment rather than JavaUtils as JavaVirtualMachine, which JavaUtils relies on, may
    //not be initialised at the point when this is called.
    return in_environment->NewStringUTF(publicKey.c_str());
}

#endif

#endif
