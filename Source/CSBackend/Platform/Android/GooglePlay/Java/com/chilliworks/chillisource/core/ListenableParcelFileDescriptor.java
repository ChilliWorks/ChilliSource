/**
 * ApkExpansionFileDescriptor.java
 * ChilliSource
 * Created by Ian Copland on 23/06/2015.
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

import android.os.ParcelFileDescriptor;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

/**
 * A parcel file descriptor which provides a means to by notified when the descriptor is closed.
 * This is the equivalent of using the ParcelFileDescriptor.OnCloseListener but is available prior
 * to API Level 19.
 *
 * @author Ian Copland
 */
public class ListenableParcelFileDescriptor extends ParcelFileDescriptor
{
    private OnClose m_listener;

    /**
     * An interface which can be implemented to be notified when the file descriptor is closed.
     *
     * @author Ian Copland
     */
    public interface OnClose
    {
        /**
         * Called when the file descriptor is closed.
         *
         * @author Ian Copland
         */
        void onClosed();
    }
    /**
     * Constructor.
     *
     * @author Ian Copland
     *
     * @param in_file - The file which this descriptor should point to.
     * @param in_mode - The file mode.
     * @param in_listener - The listener which should be notified of the closed event.
     *
     * @throws FileNotFoundException - Thrown if the file cannot be found. Note that this is the
     * only case in which the closed event will not be fired.
     */
    public ListenableParcelFileDescriptor(File in_file, int in_mode, OnClose in_listener) throws FileNotFoundException
    {
        super(ParcelFileDescriptor.open(in_file, in_mode));

        assert (in_listener != null) : "Cannot create a ListenableParcelFileDescriptor without a listener.";

        m_listener = in_listener;
    }
    /**
     * Calls the base classes close() then calls the closed listener. The listener will be called
     * regardless of whether an IO exception occurs.
     *
     * @author Ian Copland
     *
     * @throws IOException - Will be thrown if the base class throws during close().
     */
    @Override public void close() throws IOException
    {
        try
        {
            super.close();
        }
        catch (Exception e)
        {
            m_listener.onClosed();
            throw new IOException(e);
        }

        m_listener.onClosed();
    }
}
