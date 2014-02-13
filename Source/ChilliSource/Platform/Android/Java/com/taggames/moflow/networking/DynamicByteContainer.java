package com.taggames.moflow.networking;

public class DynamicByteContainer 
{
	byte[] mabyData;
	int mdwSize;
	int mdwCapacity;
	
	//------------------------------------------------------------
	/// Constructor
	//------------------------------------------------------------
	public DynamicByteContainer()
	{
		mdwCapacity = 1;
		mabyData = new byte[mdwCapacity];
		mdwSize = 0;
	}
	//------------------------------------------------------------
	/// Constructor
	///
	/// @param the default capacity for the array
	//------------------------------------------------------------
	public DynamicByteContainer(int indwDefaultCapacity)
	{
		mdwCapacity = indwDefaultCapacity;
		mabyData = new byte[mdwCapacity];
		mdwSize = 0;
	}
	//------------------------------------------------------------
	/// Get Bytes
	///
	/// @return the underlying byte data.
	//------------------------------------------------------------
	public byte[] GetBytes()
	{
		return mabyData;
	}
	//------------------------------------------------------------
	/// Get Size
	///
	/// @return the size of the container.
	//------------------------------------------------------------
	public int GetSize()
	{
		return mdwSize;
	}
	//------------------------------------------------------------
	/// Add Bytes
	///
	/// Adds an array of bytes to the dynamic byte container
	///
	/// @param the byte data.
	/// @param the offset.
	/// @param the count;
	//------------------------------------------------------------
	public void AddBytes(byte[] inabyData, int indwOffset, int indwCount)
	{
		if (inabyData != null)
		{
			CheckCapacity(indwCount);
		
			for (int i = 0; i < indwCount; i++)
			{
				mabyData[mdwSize + i] = inabyData[indwOffset + i];
			}
			
			mdwSize += indwCount;
		}
	}
	//------------------------------------------------------------
	/// Check Capacity
	///
	/// This makes sure the capacity is always big enough to fit
	/// any new objects being added. Any time the size will not
	/// be enough this will simply double the size.
	///
	/// @param the amount of extra items being added.
	//------------------------------------------------------------
	protected void CheckCapacity(int indwAdditionalCount)
	{
		//if its not big enough, then make sure it is.
		if (mdwSize + indwAdditionalCount > mdwCapacity)
		{
			//create the new buffer
			int dwNewCapacity = mdwCapacity * 2;
			byte[] abyNewData = new byte[dwNewCapacity];
			
			//put the old data in the new buffer
			for (int i = 0; i < mdwSize; i++)
				abyNewData[i] = mabyData[i];
			
			//scap the old buffer
			mabyData = abyNewData;
			
			//update the capacity
			int dwAmountIncreased = dwNewCapacity - mdwCapacity;
			mdwCapacity = dwNewCapacity;
			
			//recurse until it is indeed big enough! (hopefully this doesn't happen often...)
			CheckCapacity(indwAdditionalCount - dwAmountIncreased);
		}
	}
	
}
