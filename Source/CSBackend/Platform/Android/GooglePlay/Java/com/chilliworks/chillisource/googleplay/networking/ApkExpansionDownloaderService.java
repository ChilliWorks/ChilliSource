/**
 * ApkExpansionDownloaderService.java
 * ChilliSource
 * Created by Ian Copland on 22/04/2015.
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

package com.chilliworks.chillisource.googleplay.networking;

import com.chilliworks.chillisource.core.Logging;
import com.google.android.vending.expansion.downloader.impl.DownloaderService;

/**
 * The APK expansion downloader service.
 *
 * @author Ian Copland
 */
public final class ApkExpansionDownloaderService extends DownloaderService
{
    private static final byte[] SALT = new byte[]
    {
            1, 43, -12, -1, -34, 68, -16, -12, 43, 2, -8, -4, 9, 5, -106, 107, -36, 45, -10, 32
    };

    private static Object s_mutex = new Object();
    private static String s_publicKey = "";

    /**
     * Sets the Google Play LVL public key. This can only be called once in an app, once set it
     * can't be changed.
     *
     * This can be called on any thread.
     *
     * @author Ian Copland
     *
     * @param in_publicKey - The google play LVL public key.
     */
    protected static void setPublicKey(String in_publicKey)
    {
        synchronized (s_mutex)
        {
            assert (s_publicKey == "") : "The google play public key cannot be changed!";

            s_publicKey = in_publicKey;

            Logging.logError(">> PUBLIC KEY: " + in_publicKey);
        }
    }
    /**
     * @author Ian Copland
     *
     * @return The public key
     */
    @Override public String getPublicKey()
    {
        synchronized (s_mutex)
        {
            assert (s_publicKey != "") : "The google play public key has not been set!";

            return s_publicKey;
        }
    }
    /**
     * @author Ian Copland
     *
     * @return The salt
     */
    @Override public byte[] getSALT()
    {
        return SALT;
    }
    /**
     * @author Ian Copland
     *
     * @return The Alarm Receiver name.
     */
    @Override public String getAlarmReceiverClassName()
    {
        return ApkExpansionAlarmReceiver.class.getName();
    }
}
