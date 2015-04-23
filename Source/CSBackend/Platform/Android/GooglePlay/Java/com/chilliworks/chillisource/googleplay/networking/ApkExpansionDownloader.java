/**
 * ApkExpansionDownloader.java
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

import com.chilliworks.chillisource.core.*;
import com.chilliworks.chillisource.core.System;

/**
 * TODO
 *
 * @author Ian Copland
 */
public class ApkExpansionDownloader extends System
{
    public static InterfaceID INTERFACE_ID = new InterfaceID();

    /**
     * Allows querying of whether or not the system implements the interface described by the
     * given interface id.
     *
     * @author Ian Copland
     *
     * @param in_interfaceId - The interface id to check
     *
     * @return Whether the system implements the given interface
     */
    @Override public boolean IsA(InterfaceID in_interfaceId)
    {
        return (in_interfaceId == INTERFACE_ID);
    }
}
