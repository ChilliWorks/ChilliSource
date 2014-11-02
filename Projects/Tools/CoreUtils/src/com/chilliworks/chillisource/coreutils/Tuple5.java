/**
 * Tuple5.java
 * Chilli Source
 * Created by Ian Copland on 23/10/2014.
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
 * A simple 5-tuple class similar to std::tuple<x, y, z, w, v> in C++. Typical use
 * case for this is private methods that return multiple values.
 * 
 * @author Ian Copland
 *
 * @param <TFirst> The type of the first value.
 * @param <TSecond> The type of the second value.
 * @param <TThird> The type of the third value.
 * @param <TFourth> The type of the fourth value.
 * @param <TFifth> The type of the fifth value.
 */
public final class Tuple5<TFirst, TSecond, TThird, TFourth, TFifth>
{
	private final TFirst m_first;
	private final TSecond m_second;
	private final TThird m_third;
	private final TFourth m_fourth;
	private final TFifth m_fifth;
	
	/**
	 * Constructor. Creates the tuple from the given values.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_first - The first value.
	 * @param in_second - The second value.
	 * @param in_third - The third value.
	 * @param in_fouth - The fourth value.
	 * @param in_fifth - The fifth value.
	 */
	public Tuple5(TFirst in_first, TSecond in_second, TThird in_third, TFourth in_fourth, TFifth in_fifth)
	{
		m_first = in_first;
		m_second = in_second;
		m_third = in_third;
		m_fourth = in_fourth;
		m_fifth = in_fifth;
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
	 * @return The third value.
	 */
	public TThird getThird()
	{
		return m_third;
	}
	/**
	 * @author Ian Copland
	 *
	 * @return The fourth value.
	 */
	public TFourth getFourth()
	{
		return m_fourth;
	}
	/**
	 * @author Ian Copland
	 *
	 * @return The fourth value.
	 */
	public TFifth getFifth()
	{
		return m_fifth;
	}
	/**
	 * @author Ian Copland
	 *
	 * @return The tuple as a string.
	 */
	@Override public String toString() 
	{
        return m_first + ", " + m_second + ", " + m_third + ", " + m_fourth + ", " + m_fifth;
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
        
        if ((in_object instanceof Tuple5<?, ?, ?, ?, ?>) == false)
        {
            return false;
        }
        
        Tuple5<?, ?, ?, ?, ?> tuple = (Tuple5<?, ?, ?, ?, ?>)in_object;
        boolean firstEquals = ((m_first == null && tuple.m_first == null) || (m_first != null && m_first.equals(tuple.m_first) == true));
        boolean secondEquals = ((m_second == null && tuple.m_second == null) || (m_second != null && m_second.equals(tuple.m_second) == true));
        boolean thirdEquals = ((m_third == null && tuple.m_third == null) || (m_third != null && m_third.equals(tuple.m_third) == true));
        boolean fourthEquals = ((m_fourth == null && tuple.m_fourth == null) || (m_fourth != null && m_fourth.equals(tuple.m_fourth) == true));
        boolean fifthEquals = ((m_fifth == null && tuple.m_fifth == null) || (m_fifth != null && m_fifth.equals(tuple.m_fifth) == true));
        return (firstEquals == true && secondEquals == true && thirdEquals == true && fourthEquals == true && fifthEquals == true);
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
		hash = HashCodeUtils.add(hash, m_third);
		hash = HashCodeUtils.add(hash, m_fourth);
		hash = HashCodeUtils.add(hash, m_fifth);
		return hash;
	}
}
