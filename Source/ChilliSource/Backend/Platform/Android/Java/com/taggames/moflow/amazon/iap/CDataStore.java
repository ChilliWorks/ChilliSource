//  
//  CDataStore.java
//  moFlow
//  
//  Created by Ian Copland on 16/12/2013
//  Copyright (c)2013 Tag Games Limited - All rights reserved
// 

package com.taggames.moflow.amazon.iap;

import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Set;

import org.json.JSONArray;
import org.json.JSONObject;

import com.amazon.inapp.purchasing.Offset;
import com.taggames.moflow.amazon.iap.CPurchaseTransaction.ProductType;
import com.taggames.moflow.core.CAESEncryption;
import com.taggames.moflow.core.CExceptionUtils;
import com.taggames.moflow.core.CFileUtils;
import com.taggames.moflow.core.CHashSHA256;
import com.taggames.moflow.core.CStringUtils;

//====================================================
/// Data Store
///
/// Contains persistent data for the amazon IAP system.
/// This includes information on pending purchases,
/// current entitlements and the current position update
/// offset. This data is stored as encrypted JSON in
/// application specfic internal storage.
//====================================================
public class CDataStore 
{
	//---------------------------------------------
	/// Constants
	//---------------------------------------------
	private final String kstrPurchaseUpdateOffsetKey = "PurchaseUpdateOffset";
	private final String kstrEntitledSKUsArrayKey = "EntitledSKUs";
	private final String kstrPendingPurchaseTransactionsArrayKey = "PendingPuchaseTransactions";
	private final String kstrResultKey = "Result";
	private final String kstrSKUKey = "SKU";
	private final String kstrProductTypeID = "ProductType";
	private final String kstrTransactionIDKey = "TransactionID";
	private final String kstrPurchaseTokenID = "PurchaseToken";
	//---------------------------------------------
	/// Member Data
	//---------------------------------------------
	private final String mstrCacheFilename;
	private String mstrPrivateKey = "";
	private String mstrUDID = "";
	private String mstrUserID = "";
	private Offset mPurchaseUpdateOffset = Offset.BEGINNING;
	private Set<String> mastrEntitledSKUs = new HashSet<String>();
	private LinkedList<CPurchaseTransaction> maPendingPurchaseTransactions = new LinkedList<CPurchaseTransaction>();
	//---------------------------------------------
	/// Constructor
	///
	/// @param The filename for the data store.
	/// @param The private key used for encrypting
	/// the data store.
	/// @param The device UDID. This is used as a
	/// salt in encryption.
	/// @param The amazon user ID. This is used as
	/// a salt in the encryption.
	//---------------------------------------------
	public CDataStore(final String instrFilename, final String instrPrivateKey, final String instrUDID, final String instrUserID)
	{
		mstrCacheFilename = instrFilename;
		mstrPrivateKey = instrPrivateKey;
		mstrUDID = instrUDID;
		mstrUserID = instrUserID;
		ReadCache();
	}
	//---------------------------------------------
	/// Get Purchase Update Offset
	///
	/// @return The current purchase update offset.
	//---------------------------------------------
	public final Offset GetPurchaseUpdateOffset()
	{
		return mPurchaseUpdateOffset;
	}
	//---------------------------------------------
	/// Set Purchase Update Offset
	///
	/// @return The current purchase update offset.
	//---------------------------------------------
	public final void SetPurchaseUpdateOffset(final Offset inOffset)
	{
		mPurchaseUpdateOffset = inOffset;
		WriteCache();
	}
	//---------------------------------------------
	/// Add Entitled SKU
	///
	/// Adds a new SKU to the entitled list if it
	/// is not already there.
	///
	/// @param The SKU to add.
	//---------------------------------------------
	public void AddEntitledSKU(final String instrSKU)
	{
		if (mastrEntitledSKUs.contains(instrSKU) == false)
		{
			mastrEntitledSKUs.add(instrSKU);
			WriteCache();
		}
	}
	//---------------------------------------------
	/// Is Entitled
	///
	/// @return whether or not the given SKU is
	/// in the entitlement cache.
	//---------------------------------------------
	public boolean IsEntitled(final String instrSKU)
	{
		return mastrEntitledSKUs.contains(instrSKU);
	}
	//---------------------------------------------
	/// Remove Entitled SKU
	///
	/// Removes the given SKU from the entitled list.
	///
	/// @param This returns true if the SKU was stored
	/// and has now been removed.
	//---------------------------------------------
	public boolean RemoveEntitledSKU(final String instrSKU)
	{
		if (mastrEntitledSKUs.contains(instrSKU) == false)
		{
			mastrEntitledSKUs.remove(instrSKU);
			WriteCache();
			return true;
		}
		return false;
	}
	//---------------------------------------------
	/// Clear Entitled SKUs
	///
	/// Removes all entitled SKUs from the data
	/// store.
	//---------------------------------------------
	public void ClearEntitledSKUs()
	{
		mastrEntitledSKUs.clear();
		WriteCache();
	}
	//---------------------------------------------
	/// Add Pending Purchase Transaction
	///
	/// Adds a new pending transaction to the data
	/// store.
	///
	/// @param The purchase transaction.
	//---------------------------------------------
	public void AddPendingPurchaseTransaction(CPurchaseTransaction inPurchaseTransaction)
	{
		maPendingPurchaseTransactions.add(inPurchaseTransaction);
		WriteCache();
	}
	//---------------------------------------------
	/// Get Pending Purchase Transactions
	///
	/// @return The pending purchase transaction 
	/// list.
	//---------------------------------------------
	public final LinkedList<CPurchaseTransaction> GetPendingPurchaseTransactions()
	{
		return maPendingPurchaseTransactions;
	}
	//---------------------------------------------
	/// Remove Pending Purchase Transaction
	///
	/// Removes the purchase transaction with the
	/// given transaction ID and SKU from the cache.
	///
	/// @param The SKU.
	/// @param The transaction ID.
	/// @return Whether or not the transaction was
	/// successfully removed from the cache.
	//---------------------------------------------
	public boolean RemovePendingPurchaseTransaction(final String instrSKU, final String instrTransactionID)
	{
		boolean bSuccess = false;
		Iterator<CPurchaseTransaction> it = maPendingPurchaseTransactions.iterator();
		while (it.hasNext())
		{
			CPurchaseTransaction transaction = it.next();
			if (transaction.GetSKU().equals(instrSKU) == true && transaction.GetTransactionID().equals(instrTransactionID) == true)
			{
				it.remove();
				bSuccess = true;
				break;
			}
		}
		
		if (bSuccess == true)
		{
			WriteCache();
		}
		
		return bSuccess;
	}
	//---------------------------------------------
	/// Read Cache
	///
	/// Parses the contents of the cache file into
	/// a list of purchase transactions. The cache
	/// file is stored as encrypted JSON. 
	//---------------------------------------------
	private void ReadCache()
	{
		maPendingPurchaseTransactions.clear();
		mastrEntitledSKUs.clear();
		
		try
		{
			byte[] abyEncryptedJson = CFileUtils.ReadFile(CFileUtils.StorageLocation.INTERNAL_STORAGE, mstrCacheFilename);
			if (abyEncryptedJson != null && abyEncryptedJson.length > 0)
			{
				byte[] abyJson = CAESEncryption.Decrypt(abyEncryptedJson, CalculateSaltedPrivateKey());
				if (abyJson != null)
				{
					String strJson = CStringUtils.UTF8ByteArrayToString(abyJson);
					JSONObject root = new JSONObject(strJson);
					
					//read the purchase update offset
					String strOffset = root.getString(kstrPurchaseUpdateOffsetKey);
					mPurchaseUpdateOffset = Offset.fromString(strOffset);
					
					//read the entitled SKUs
					JSONArray entitlementArray = root.getJSONArray(kstrEntitledSKUsArrayKey);
					for (int i = 0; i < entitlementArray.length(); i++)
					{
						JSONObject entitlementObject = entitlementArray.getJSONObject(i);
						String strSKU = entitlementObject.getString(kstrSKUKey);
						mastrEntitledSKUs.add(strSKU);
					}
					
					//read the pending purchases
					JSONArray transactionArray = root.getJSONArray(kstrPendingPurchaseTransactionsArrayKey);
					for(int i = 0; i < transactionArray.length(); i++)
					{
						JSONObject transactionObject = transactionArray.getJSONObject(i);
						int dwResult = transactionObject.getInt(kstrResultKey);
						String strSKU = transactionObject.getString(kstrSKUKey);
						ProductType eProductType = ProductType.valueOf(transactionObject.getString(kstrProductTypeID));
						String strTransactionID = transactionObject.getString(kstrTransactionIDKey);
						String strPurchaseToken = transactionObject.getString(kstrPurchaseTokenID);
						CPurchaseTransaction transaction = new CPurchaseTransaction(dwResult, strSKU, eProductType, strTransactionID, strPurchaseToken);
						maPendingPurchaseTransactions.add(transaction);
					}
				}
			}
		}
		catch (Exception e)
		{
			CExceptionUtils.ConvertToString(e);
		}
	}
	//---------------------------------------------
	/// Write Cache
	///
	/// Writes the list of purchase transactions to
	/// the cache file. This file is stored as 
	/// encrypted JSON.
	//---------------------------------------------
	private void WriteCache()
	{
		try
		{
			//add the purchase update offset
			JSONObject root = new JSONObject();
			root.put(kstrPurchaseUpdateOffsetKey, mPurchaseUpdateOffset.toString());
			
			//add entitlement array
			JSONArray entitlementArray = new JSONArray();
			for (String strSKU : mastrEntitledSKUs)
			{
				JSONObject entitlementObject = new JSONObject();
				entitlementObject.put(kstrSKUKey, strSKU);
				entitlementArray.put(entitlementObject);
			}
			root.put(kstrEntitledSKUsArrayKey, entitlementArray);
			
			//add transaction array
			JSONArray transactionArray = new JSONArray();
			for (CPurchaseTransaction transaction : maPendingPurchaseTransactions)
			{
				JSONObject transactionObject = new JSONObject();
				transactionObject.put(kstrResultKey, transaction.GetResult());
				transactionObject.put(kstrSKUKey, transaction.GetSKU());
				transactionObject.put(kstrProductTypeID, transaction.GetProductType().name());
				transactionObject.put(kstrTransactionIDKey, transaction.GetTransactionID());
				transactionObject.put(kstrPurchaseTokenID, transaction.GetPurchaseReceipt());
				transactionArray.put(transactionObject);
			}
			root.put(kstrPendingPurchaseTransactionsArrayKey, transactionArray);
			
			String strJson = root.toString();
			byte[] abyEncryptedJson = CAESEncryption.Encrypt(CStringUtils.StringToUTF8ByteArray(strJson), CalculateSaltedPrivateKey());
			if (abyEncryptedJson != null)
			{
				CFileUtils.WriteFile(CFileUtils.StorageLocation.INTERNAL_STORAGE, mstrCacheFilename, abyEncryptedJson);
			}
			else
			{
				android.util.Log.e("MoFlow", "Purchase Transaction Cache: Failed to write encrpyted cache.");
			}
		}
		catch (Exception e)
		{
			CExceptionUtils.ConvertToString(e);
		}
	}
	//---------------------------------------------
	/// Calculate Salted Private Key
	///
	/// Uses the private key,
	///
	/// @param The cache filename.
	/// @param The list of purchase transactions.
	//---------------------------------------------
	private byte[] CalculateSaltedPrivateKey()
	{
		byte[] abySaltedPrivateKey = CHashSHA256.Generate(CStringUtils.StringToUTF8ByteArray(mstrPrivateKey + mstrUDID + mstrUserID));
		return abySaltedPrivateKey;
	}
}
