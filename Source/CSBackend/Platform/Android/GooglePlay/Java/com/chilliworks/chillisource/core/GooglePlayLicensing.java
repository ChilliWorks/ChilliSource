/**
 * GooglePlayLicensing.java
 * ChilliSource
 * Created by Ian Copland on 27/05/2015.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Tag Games Limited
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

package com.chilliworks.chillisource.core;

/**
 * Provides a unified means for systems to access the Google Play LVL Public Key supplied by the
 * user of the engine.
 *
 * @author Ian Copland
 */
public final class GooglePlayLicensing
{
    static private volatile String s_publicKey = "";

    /**
     * This can be called on any thread.
     *
     * @author Ian Copland
     *
     * @return The Public Key used by each of the google play systems to verify the app license.
     */
    public static String getLvlPublicKey()
    {
        synchronized (s_publicKey)
        {
            if (s_publicKey.isEmpty() == true)
            {
                s_publicKey = calcLvlPublicKey();
                assert (s_publicKey != "") : "The google play public key has not been set!";
            }

            return s_publicKey;
        }
    }


    /**
     * Gets the lvl public key from native.
     *
     * @author Ian Copland
     *
     * @return The Public Key used by each of the google play systems to verify the app license.
     */
    public static native String calcLvlPublicKey();
}
