//Copyright (c) 2013 Tag Games Limited.

package com.chillisource.networking;

public class IAPTransactionDescription
{
	public String ProductID;
	public String TransactionID;
	public String Receipt;

	public final static int SUCCEEDED = 0;
	public final static int FAILED = 1;
	public final static int CANCELLED = 2;
	public final static int RESTORED = 3;
	public final static int RESUMED = 4;
	public final static int REFUNDED = 5;
}