/**
 * BoxedPointer.java
 * ChilliSource
 * Created by HMcLaughlin on 31/07/2015.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2011 Tag Games Limited
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
 * This can be used to wrap a c-pointer and type up in a java object.
 *
 * The BoxedPointer, native side, can be used to box (Wrap a c-pointer and type to java) and
 * Unbox (Unwrap the java object back to a c-pointer)
 *
 * @author HMcLaughlin
 */
public final class BoxedPointer
{
    final private long m_pointerAddress;
    final private long m_typeHash;

    /**
     * Constructor
     *
     * @author HMcLaughlin
     *
     * @param in_pointerAddress
     * @param in_typeHash
     */
    public BoxedPointer(long in_pointerAddress, long in_typeHash)
    {
        m_pointerAddress = in_pointerAddress;
        m_typeHash = in_typeHash;
    }
    /**
     * @author HMcLaughlin
     *
     * @return Pointer Address
     */
    public long getPointerAddress()
    {
        return m_pointerAddress;
    }
    /**
     * @author HMcLaughlin
     *
     * @return Pointer type hash
     */
    public long getTypeHash()
    {
        return m_typeHash;
    }
}
