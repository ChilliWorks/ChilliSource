/**
 * Pair.java
 * Chilli Source
 * Created by Ian Copland on 08/10/2014.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Tag Games Limited
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

package com.chilliworks.chillisource.coreutils;

/**
 * A simple pair, or 2-tuple class similar to std::pair<x,y> in C++. Typical use
 * case for this is private methods that return multiple values.
 * 
 * @author Ian Copland
 *
 * @param <TFirst> The type of the first value.
 * @param <TSecond> The type of the second value.
 */
public final class Pair<TFirst, TSecond>
{
	private final TFirst m_first;
	private final TSecond m_second;
	
	/**
	 * Constructor. Creates the pair from the two given values.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_first - The first value.
	 * @param in_second - The second value.
	 */
	public Pair(TFirst in_first, TSecond in_second)
	{
		m_first = in_first;
		m_second = in_second;
	}
	/**
	 * @author Ian Copland
	 *
	 * @return The first value.
	 */
	public TFirst getFirst()
	{
		return m_first;
	}
	/**
	 * @author Ian Copland
	 *
	 * @return The second value.
	 */
	public TSecond getSecond()
	{
		return m_second;
	}
	/**
	 * @author Ian Copland
	 *
	 * @return The pair as a string.
	 */
	@Override public String toString() 
	{
        return m_first + ", " + m_second;
    }
	/**
	 * Compares equality with the given object.
	 * 
	 * @author Ian Copland
	 *
	 * @return Whether or not the two objects are equal.
	 */
    @Override public boolean equals(Object in_object) 
    {
        if (in_object == this) 
        {
            return true;
        }
    	
        if (in_object == null) 
        {
            return false;
        }
        
        if ((in_object instanceof Pair<?, ?>) == false)
        {
            return false;
        }
        
        Pair<?, ?> pair = (Pair<?, ?>)in_object;
        boolean firstEquals = ((m_first == null && pair.m_first == null) || (m_first != null && m_first.equals(pair.m_first) == true));
        boolean secondEquals = ((m_second == null && pair.m_second == null) || (m_second != null && m_second.equals(pair.m_second) == true));
        return (firstEquals == true && secondEquals == true);
    }
	/**
	 * @author Ian Copland
	 *
	 * @return The hash code for this object.
	 */
	@Override public int hashCode()
	{
		int hash = HashCodeUtils.INITIAL_VALUE;
		hash = HashCodeUtils.add(hash, m_first);
		hash = HashCodeUtils.add(hash, m_second);
		return hash;
	}
}
