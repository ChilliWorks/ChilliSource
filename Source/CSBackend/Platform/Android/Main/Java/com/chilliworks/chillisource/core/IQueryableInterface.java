/**
 * QueryableInterface.java
 * ChilliSource
 * Created by Ian Copland on 09/08/2012.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2012 Tag Games Limited
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
 * An interface for types that can have their type queried at runtime. This is an alternate
 * to using the instanceof keyword.
 *
 * Classes which implement this should provide a static, public instance of InterfaceID called
 * INTERFACE_ID which can be used to query the type.
 *
 * @author Ian Copland
 */
interface IQueryableInterface
{
    /**
     * Allows querying of whether the class implements the interface associated with the
     * given interface Id.
     *
     * @param in_interfaceId - The interface Id
     *
     * @return Whether or not the interface is implemented.
     */
	boolean isA(InterfaceId in_interfaceId);
}
