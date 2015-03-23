/**
 * PurchaseTransaction.java
 * Chilli Source
 * Created by Ian Copland on 07/01/2014.
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

package com.chilliworks.chillisource.amazon;

//============================================================
/// Purchase Transaction
///
/// A container for data about a single purchase transaction.
//============================================================
public class PurchaseTransaction 
{
	//---------------------------------------------------
	/// Product Type
	//---------------------------------------------------
	public enum ProductType
	{
		NONE,
		CONSUMABLE,
		ENTITLEMENT
	}
	//---------------------------------------------------
	/// Constants
	//---------------------------------------------------
	public final static int kdwPurchaseSuccessful = 0;
	public final static int kdwPurchaseResumed = 1;
	public final static int kdwPurchaseRestored = 2;
	public final static int kdwPurchaseFailed = 3;
	//---------------------------------------------------
	/// Member Data
	//---------------------------------------------------
	private final int mdwResult;
	private final ProductType meProductType;
	private final String mstrSKU;
	private final String mstrTransactionID;
	private final String mstrPurchaseReceipt;
	//---------------------------------------------------
	/// Constructor
	///
	/// @param The result.
	/// @param The request ID.
	/// @param The purchase token.
	//---------------------------------------------------
	public PurchaseTransaction(int indwResult, final String instrSKU, final ProductType ineProductType, final String instrTransactionID, final String instrPurchaseReceipt)
	{
		mdwResult = indwResult;
		mstrSKU = instrSKU;
		meProductType = ineProductType;
		mstrTransactionID = instrTransactionID;
		mstrPurchaseReceipt = instrPurchaseReceipt;
	}
	//---------------------------------------------------
	/// Get Result
	///
	/// @return An integer value representing the result
	/// of the purchase transaction. The possible values
	/// are defined as constants in this class.
	//---------------------------------------------------
	public int GetResult()
	{
		return mdwResult;
	}
	//---------------------------------------------------
	/// Get SKU
	///
	/// @return The ID of the SKU this transaction was
	/// for.
	//---------------------------------------------------
	public final String GetSKU()
	{
		return mstrSKU;
	}
	//---------------------------------------------------
	/// Get Product Type
	///
	/// @return The product type.
	//---------------------------------------------------
	public final ProductType GetProductType()
	{
		return meProductType;
	}
	//---------------------------------------------------
	/// Get Transaction ID
	///
	/// @return The ID of the request.
	//---------------------------------------------------
	public final String GetTransactionID()
	{
		return mstrTransactionID;
	}
	//---------------------------------------------------
	/// Get Purchase Receipt
	///
	/// @return The purchase reciept. This contains the
	/// user ID and the purchase token.
	//---------------------------------------------------
	public final String GetPurchaseReceipt()
	{
		return mstrPurchaseReceipt;
	}
}
