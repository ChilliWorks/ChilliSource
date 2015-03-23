/**
 * AmazonIAPNativeInterface.java
 * Chilli Source
 * Created by Ian Copland on 10/12/2013.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2013 Tag Games Limited
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

import java.util.HashSet;
import java.util.LinkedList;
import java.util.Map;
import java.util.Set;

import org.json.JSONObject;

import com.amazon.inapp.purchasing.GetUserIdResponse;
import com.amazon.inapp.purchasing.Item;
import com.amazon.inapp.purchasing.Item.ItemType;
import com.amazon.inapp.purchasing.ItemDataResponse;
import com.amazon.inapp.purchasing.Offset;
import com.amazon.inapp.purchasing.PurchaseResponse;
import com.amazon.inapp.purchasing.PurchaseResponse.PurchaseRequestStatus;
import com.amazon.inapp.purchasing.PurchaseUpdatesResponse;
import com.amazon.inapp.purchasing.PurchasingManager;
import com.amazon.inapp.purchasing.Receipt;
import com.chilliworks.chillisource.amazon.PurchaseTransaction.ProductType;
import com.chilliworks.chillisource.core.CSApplication;
import com.chilliworks.chillisource.core.ExceptionUtils;
import com.chilliworks.chillisource.core.HashCRC32;
import com.chilliworks.chillisource.core.INativeInterface;
import com.chilliworks.chillisource.core.InterfaceIDType;
import com.chilliworks.chillisource.core.Logging;
import com.chilliworks.chillisource.core.StringUtils;

//===========================================================
/// Amazon IAP Native Interface
///
/// A native interface for the Amazon IAP system. 
//===========================================================
public class AmazonIAPNativeInterface extends INativeInterface
{
	//-----------------------------------------------------
	/// State
	///
	/// Describes the current state of the IAP system.
	//-----------------------------------------------------
	enum State
	{
		UNINITIALISED,
		REGISTERING_OBSERVER,
		GETTING_USER_ID,
		READY,
		REQUESTING_PRODUCT_DESCRIPTIONS
	}
	//-----------------------------------------------------
	/// Constants
	//-----------------------------------------------------
	public static InterfaceIDType InterfaceID = new InterfaceIDType("CAmazonIAPNativeInterface");
	private static String kstrCacheNamePrefix = "AmazonIAP-";
	private static String kstrCacheExtension = ".iapcache";
	private static String kstrReceiptUserIDKey = "UserID";
	private static String kstrReceiptTokenKey = "Token";
	//-----------------------------------------------------
	/// Member Data
	//-----------------------------------------------------
	private volatile State meState = State.UNINITIALISED;
	private volatile DataStore mDataStore = null;
	private boolean mbSandboxMode = false;
	private boolean mbListeningForPurchaseTransactions = false;
	private Set<String> mastrStartupProductDescSkuSet = null;
	private boolean mbCancellingProductDescRequest = false;
	private String mstrUserID = "";
	private String mstrPrivateKey = "";
	private String mstrUDID = "";
	private String mstrCurrentSKU = "";
	//-----------------------------------------------------
	/// Constructor
	//-----------------------------------------------------
	public AmazonIAPNativeInterface()
	{
	}
	//-----------------------------------------------------
	/// Is A
	///
	/// @param Interface ID
	/// @return Whether the system implements the given 
	/// interface
	//-----------------------------------------------------
	@Override synchronized public boolean IsA(InterfaceIDType inInterfaceType) 
	{
		return (inInterfaceType == InterfaceID);
	}
	//-----------------------------------------------------
	/// Init
	///
	/// Prepares and starts the IAP system. This should be 
	/// the first method called and it should only be called 
	/// once. 
	/// 
	/// @param The private key used to for encrypting the
	/// IAP cache files.
	/// @param The UDID.
	//-----------------------------------------------------
	public void Init(final String instrPrivateKey, final String instrUDID)
	{
		mstrPrivateKey = instrPrivateKey;
		mstrUDID = instrUDID;
		
		final AmazonIAPNativeInterface thisClass = this;
		
		//Run as a UI thread task as this will be constructed on the Render thread.
		Runnable task = new Runnable()
		{
			@Override public void run()
			{
				if (meState == State.UNINITIALISED)
				{
					meState = State.REGISTERING_OBSERVER;
					PurchasingManager.registerObserver(new PurchasingObserver(CSApplication.get().getActivityContext(), thisClass));
				}
			}
		};
		
		CSApplication.get().scheduleUIThreadTask(task);
	}
	//-----------------------------------------------------
	/// On Resume
	///
	/// Called when the application is brought into the 
	/// foreground. This requests an update to the users
	/// ID.
	//-----------------------------------------------------
	@Override synchronized public void onActivityResume() 
	{
		//if a user ID has already been requested it may now be out of date, so update it.
		if (meState != State.UNINITIALISED && meState != State.REGISTERING_OBSERVER)
		{
			meState = State.GETTING_USER_ID;
			PurchasingManager.initiateGetUserIdRequest();
		}
	}
	//-----------------------------------------------------
	/// Is Initialised
	///
	/// @return whether or not the Amazon IAP system is
	/// finished initialising. The system is considered to
	/// be initialising if it is in any of the following
	/// states:
	///  - UNINITIALISED
	///  - REGISTERING_OBSERVER
	///  - GETTING_USER_ID
	//-----------------------------------------------------
	synchronized public boolean IsInitialised() 
	{
		if (meState == State.UNINITIALISED || meState == State.REGISTERING_OBSERVER || meState == State.GETTING_USER_ID)
		{
			return false;
		}
		return true;
	}
    //-----------------------------------------------------
	/// Request Product Descriptions
	///
	/// Request the descriptions of the products on the 
	/// store.
	///
	/// @param Array of product IDs
	//-----------------------------------------------------
	public synchronized void RequestProductDescriptions(final String[] inastrProductIDs)
	{
		if (meState == State.READY)
		{
			if (mbCancellingProductDescRequest == false)
			{
				meState = State.REQUESTING_PRODUCT_DESCRIPTIONS;
				
				Set<String> astrSkuSet = new HashSet<String>();
				for (String strProductID : inastrProductIDs)
				{
					astrSkuSet.add(strProductID);
				}
				
				PurchasingManager.initiateItemDataRequest(astrSkuSet);
			}
			else
			{
				//This this will cause the system to received the previously requested product descriptions. This
				//means it will only work if the new request is identical to the previous one. 
				mbCancellingProductDescRequest = false;
				meState = State.REQUESTING_PRODUCT_DESCRIPTIONS;
			}
		}
		else if (IsInitialised() == false && mastrStartupProductDescSkuSet == null)
		{
			mastrStartupProductDescSkuSet = new HashSet<String>();
			for (String strProductID : inastrProductIDs)
			{
				mastrStartupProductDescSkuSet.add(strProductID);
			}
		}
    }
    //-----------------------------------------------------
    /// Cancel Product Descriptions Request
    ///
    /// Prevent the delegate being called with descriptions 
	/// and attempt to cancel the pending request.
    //-----------------------------------------------------
    public synchronized void CancelProductDescriptionsRequest()
    {
    	if (meState == State.REQUESTING_PRODUCT_DESCRIPTIONS)
    	{
    		mbCancellingProductDescRequest = true;
    		meState = State.READY;
    	}
    	else if (IsInitialised() == false)
    	{
    		mastrStartupProductDescSkuSet = null;
    	}
    }
	//-----------------------------------------------------
	/// Is Purchasing Enabled
	///
	/// @return whether or not IAP is supported on this 
    /// device/OS
	//-----------------------------------------------------
	public  synchronized boolean IsPurchasingEnabled()
	{
		//The amazon IAP interface doesn't expose a way of finding out if purchasing is
		//available so we are just returning true.
		return true;
	}
	//-----------------------------------------------------
	/// Request Product Purchase
	///
	/// Start a transaction of an object with the given 
	/// product ID. The transaction listener will receive 
	/// updates on the transaction status.
	///
	/// @param Product ID
	//-----------------------------------------------------
	public synchronized void RequestProductPurchase(final String instrProductID)
	{
		if (meState == State.READY)
		{
			mstrCurrentSKU = instrProductID;
			PurchasingManager.initiatePurchaseRequest(instrProductID);
		}
		else
		{
			//if the system is not in a position to request purchases let the application know that it has failed.
			ProcessPurchase(PurchaseTransaction.kdwPurchaseFailed, instrProductID, ItemType.CONSUMABLE, "", "", false);
		}
	}
	//-----------------------------------------------------
	/// Close Transaction
	///
	/// Consumes the transaction from the purchase list so 
	/// that it may be purchased again.
	///
	/// @param Product ID
	/// @param Transaction ID
	//-----------------------------------------------------
	public synchronized void CloseTransaction(final String instrProductID, final String instrTransactionID)
	{
		mDataStore.RemovePendingPurchaseTransaction(instrProductID, instrTransactionID);
		NativeOnTransactionClosed(instrProductID, instrTransactionID);
	}
	//-----------------------------------------------------
	/// Restore Managed Purchases
	///
	/// Restores all previous managed purchase by injecting 
	/// them as new transactions
	//-----------------------------------------------------
	public synchronized void RestoreManagedPurchases()
	{
		mDataStore.ClearEntitledSKUs();
		PurchasingManager.initiatePurchaseUpdatesRequest(Offset.BEGINNING);
	}
	//-----------------------------------------------------
	/// Start Listening For TransactionUpdates
	///
	/// Starts listening for purchase transactions. This
	/// means that purchase transactions will be passed down 
	/// to the native side of the engine when received. This
	/// will also send any transactions currently in the
	/// purchase transaction cache.
	//-----------------------------------------------------
	public synchronized void StartListeningForTransactionUpdates()
	{
		if (mbListeningForPurchaseTransactions == false)
		{
			mbListeningForPurchaseTransactions = true;
			
			//there may be some transactions that have been cached while we were not listening, so these should now
			//be sent down to the application.
			ProcessPendingTransactions();
		}
	}
	//-----------------------------------------------------
	/// Stop Listening For TransactionUpdates
	///
	/// Stops listening for purchase transactions. All
	/// purchase transactions received will no longer be
	/// passed down to the native side of the engine and will
	/// instead simply be added to the purchase transactions
	/// cache.
	//-----------------------------------------------------
	public synchronized void StopListeningForTransactionUpdates()
	{
		mbListeningForPurchaseTransactions = false;
	}
	//-----------------------------------------------------
	/// On SDK Available
	///
	/// This is called when the observer is registered.
	/// It provides information on whether the 
	/// application is in sand box mode or not.
	///
	/// @param Whether or not this application is
	/// in sand box mode.
	//-----------------------------------------------------
	public synchronized void OnSDKAvailable(final boolean inbSandboxMode)
	{
		if (meState == State.REGISTERING_OBSERVER)
		{
			mbSandboxMode = inbSandboxMode;
			PurchasingManager.initiateGetUserIdRequest();
			meState = State.GETTING_USER_ID;
		}
	}	
	//-----------------------------------------------------
	/// On Get User ID Response
	///
	/// Called in response to a request for the users
	/// amazon store id. This is called on the UI Thread.
	///
	/// @param The response.
	//-----------------------------------------------------
	public synchronized void OnGetUserIdResponse(final GetUserIdResponse inResponse) 
	{
		if (meState == State.GETTING_USER_ID)
		{
			if (inResponse.getUserIdRequestStatus() == GetUserIdResponse.GetUserIdRequestStatus.SUCCESSFUL) 
			{
				mstrUserID = inResponse.getUserId();
				RefreshDataStore();
				meState = State.READY;
				
				//Request an update on transactions for the current user.
				PurchasingManager.initiatePurchaseUpdatesRequest(mDataStore.GetPurchaseUpdateOffset());
				
				//if product descriptions were requested prior to the system finishing initialising the request should now be fired.
				if (mastrStartupProductDescSkuSet != null)
				{
					String[] astrIds = new String[mastrStartupProductDescSkuSet.size()];
					mastrStartupProductDescSkuSet.toArray(astrIds);
					RequestProductDescriptions(astrIds);
					mastrStartupProductDescSkuSet = null;
				}
			}
			else 
			{ 
				Logging.logError("Amazon IAP System: failed to get the user ID.");
			}
		}
		else
		{
			Logging.logError("Amazon IAP System: User Id response received while not listening for one.");
		}
	}
	//-----------------------------------------------------
	/// On Item Data Response
	///
	/// Called in response to a request for product descriptions.
	/// This is called on the UI Thread.
	///
	/// @param The response.
	//-----------------------------------------------------
	public synchronized void OnItemDataResponse(final ItemDataResponse inResponse) 
	{
		if (meState == State.REQUESTING_PRODUCT_DESCRIPTIONS)
		{
			meState = State.READY;
			
			switch (inResponse.getItemDataRequestStatus()) 
			{
		        case SUCCESSFUL_WITH_UNAVAILABLE_SKUS:
		        {
		            for (final String strSKU : inResponse.getUnavailableSkus()) 
		            { 
		            	Logging.logError("Amazon IAP System: unavailable SKU '" + strSKU + "'");
		            }
		            
		            //Fall through to the "Successful" block to pass down the valid SKUs.
		        }
		        case SUCCESSFUL:
		        {
		            final Map<String, Item> items = inResponse.getItemData();
	            	String[] astrIDs = new String[items.size()];
	            	String[] astrNames = new String[items.size()];
	            	String[] astrDescriptions = new String[items.size()];
	            	String[] astrFormattedPrices = new String[items.size()];
	            	int dwCount = 0;
		            for (final String key : items.keySet()) 
		            {
		                Item item = items.get(key);
		                astrIDs[dwCount] = item.getSku();
		                astrNames[dwCount] = item.getTitle();
		                astrDescriptions[dwCount] = item.getDescription();
		                astrFormattedPrices[dwCount] = item.getPrice();
		                dwCount++;
		            }
		            
		    		//send event event down to the game code. It will be passed on to the render thread by the Java Interface, so no need to do it here.
	            	final String[] kastrIDs = astrIDs;
	            	final String[] kastrNames = astrNames;
	            	final String[] kastrDescriptions = astrDescriptions;
	            	final String[] kastrFormattedPrices = astrFormattedPrices;
	            	NativeOnProductsDescriptionsRequestComplete(kastrIDs, kastrNames, kastrDescriptions, kastrFormattedPrices);
		            break;
		        }
		        case FAILED:
		        {
		        	//The Amazon IAP documentation suggests that the application should disable IAPs if this fails. As the in app purchasing 
		        	//system requires this to send an event to proceed we can silently disable it by simply not firing it.
		        	Logging.logError("Amazon IAP System: Request for item data failed! IAPs are now disabled.");
		            break;
		        }
			}
		}
		
		mbCancellingProductDescRequest = false;
	}
	//-----------------------------------------------------
	/// On Purchase Response
	///
	/// Called in response to a purchase request. This is 
	/// called on the UI Thread.
	///
	/// @param The response
	//-----------------------------------------------------
	public synchronized void OnPurchaseResponse(final PurchaseResponse inResponse)
	{
		final PurchaseRequestStatus eStatus = inResponse.getPurchaseRequestStatus();
		switch (eStatus)
        {
	        case SUCCESSFUL:
	        {
	            Receipt receipt = inResponse.getReceipt();
	            if (inResponse.getUserId().equals(mstrUserID) == true)
	            {
	            	ProcessPurchase(PurchaseTransaction.kdwPurchaseSuccessful, receipt.getSku(), receipt.getItemType(), inResponse.getRequestId(), receipt.getPurchaseToken(), true);
	            }
	            else
	            {
	            	ProcessPurchaseForUser(inResponse.getUserId(), PurchaseTransaction.kdwPurchaseSuccessful, receipt.getSku(), receipt.getItemType(), inResponse.getRequestId(), receipt.getPurchaseToken());
	            }
	            break;
	        }
	        default:
	        {
	        	ProcessPurchase(PurchaseTransaction.kdwPurchaseFailed, mstrCurrentSKU, ItemType.CONSUMABLE, "", "", false);
	        	break;
	        }
        }
		
		mstrCurrentSKU = "";
	}
	//-----------------------------------------------------
	/// On Item Data Response
	///
	/// Called in response to a request for an update on
	/// purchase receipts. This can occur in two cases.
	/// When the user is registered (on start up and on 
	/// resume) transactions are updated from the point
	/// they were last updated. The user can also request
	/// a full update.
	/// 
	/// @param The response.
	//-----------------------------------------------------
	public synchronized void OnPurchaseUpdatesResponse(final PurchaseUpdatesResponse inResponse) 
	{
		switch (inResponse.getPurchaseUpdatesRequestStatus())
		{
			case SUCCESSFUL:
			{
				//check for existing managed purchases
				for (final Receipt receipt : inResponse.getReceipts()) 
				{
                    switch (receipt.getItemType()) 
                    {
                        case ENTITLED:
                        {
        		            if (inResponse.getUserId().equals(mstrUserID) == true)
        		            {
        		            	ProcessPurchase(PurchaseTransaction.kdwPurchaseRestored, receipt.getSku(), receipt.getItemType(), inResponse.getRequestId(), receipt.getPurchaseToken(), false);
        		            }
        		            else
        		            {
        		            	ProcessPurchaseForUser(inResponse.getUserId(), PurchaseTransaction.kdwPurchaseRestored, receipt.getSku(), receipt.getItemType(), inResponse.getRequestId(), receipt.getPurchaseToken());
        		            }
                        	break;
                        }
                        default:
                        	break;
                    }
                }
				
				//update the transaction offset.
				final Offset newOffset = inResponse.getOffset();
				mDataStore.SetPurchaseUpdateOffset(newOffset);
				if (inResponse.isMore() == true)
				{
					PurchasingManager.initiatePurchaseUpdatesRequest(mDataStore.GetPurchaseUpdateOffset());
				}
				break;
			}
			case FAILED:
			{
				Logging.logError("Amazon IAP System: Failed to receive purchase updates response.");
				break;
			}
		}
	}
	//-----------------------------------------------------
	/// Refresh Data Store
	///
	/// Refreshes the data store, updating to a new user
	/// if it has changed.
	//-----------------------------------------------------
	private synchronized void RefreshDataStore()
	{
		String strUserIDHash = Integer.toString(Math.abs(HashCRC32.Generate(StringUtils.StringToUTF8ByteArray(mstrUserID))));
		mDataStore = new DataStore(kstrCacheNamePrefix + strUserIDHash + kstrCacheExtension, mstrPrivateKey, mstrUDID, mstrUserID);
	}
	//-----------------------------------------------------
	/// Build Purchase Receipt
	///
	/// Builds JSON containing the user id that made the
	/// purchase and the purchase token. This is returned
	/// in string format.
	///
	/// @param The user ID.
	/// @param The Purchase Token.
	/// @return The receipt string.
	//-----------------------------------------------------
	private String BuildPurchaseReceipt(final String instrUserID, final String instrToken)
	{
		String strReceipt = "";
		try
		{
			JSONObject root = new JSONObject();
			root.put(kstrReceiptUserIDKey, instrUserID);
			root.put(kstrReceiptTokenKey, instrToken);
			strReceipt = root.toString();
		}
		catch (Exception e)
		{
			Logging.logError(ExceptionUtils.ConvertToString(e));
		}
		return strReceipt;
	}
	//-----------------------------------------------------
	/// Process Pending Transactions
	///
	/// Sends all pending transactions down to the native
	/// side of the engine.
	//-----------------------------------------------------
	private void ProcessPendingTransactions()
	{
		if (mbListeningForPurchaseTransactions == true)
		{
			final LinkedList<PurchaseTransaction> astrTransactions = mDataStore.GetPendingPurchaseTransactions();
			if (astrTransactions.size() > 0)
			{
				for (final PurchaseTransaction transaction : astrTransactions)
				{
					int dwResult = transaction.GetResult();
					
					//if the transaction is a success, then change it to resumed to the application can tell that this is a successful
					//transaction from a previous attempt at purchasing.
					if (dwResult == PurchaseTransaction.kdwPurchaseSuccessful)
					{
						dwResult = PurchaseTransaction.kdwPurchaseResumed;
					}
					NativeOnTransactionStatusUpdated(dwResult, transaction.GetSKU(), transaction.GetTransactionID(), transaction.GetPurchaseReceipt());
				}
			}
		}
	}
	//-----------------------------------------------------
	/// Process Purchase
	///
	/// Processes a single purchase transaction. This will
	/// create a transaction, add it to the data store if
	/// it is not an error. It will then send it down to
	/// native if the application is currently listening
	/// for transactions. It will also add the sku to the
	/// entitled list if it is a managed purchase.
	///
	/// @param The result.
	/// @param The sku.
	/// @param The item type.
	/// @param The request id.
	/// @param The purchase token.
	/// @param Whether or not to send this event if the
	/// purchase has already been entitled.
	//-----------------------------------------------------
	private void ProcessPurchase(int indwResult, final String instrSKU, final ItemType ineItemType, final String instrRequestId, final String instrPurchaseToken, boolean inbEvenIfEntitled)
	{
		if (indwResult == PurchaseTransaction.kdwPurchaseSuccessful || indwResult == PurchaseTransaction.kdwPurchaseRestored)
		{
			//get the product type.
			ProductType eProductType = ProductType.NONE;
			switch (ineItemType)
			{
				case CONSUMABLE:
					eProductType = ProductType.CONSUMABLE;
					break;
				case ENTITLED:
					eProductType = ProductType.ENTITLEMENT;
					break;
				default:
					Logging.logError("Amazon IAP System: Cannot process purchase, item type is unknown.");
					return;
			}
	         
			//get the purchase receipt
			String strReceipt = BuildPurchaseReceipt(mstrUserID, instrPurchaseToken);

			if (eProductType == ProductType.ENTITLEMENT)
			{
				if (inbEvenIfEntitled == false && mDataStore.IsEntitled(instrSKU) == true)
				{
					//Already entitled, so don't bother sending again.
					return;
				}
				
				mDataStore.AddEntitledSKU(instrSKU);
			}
			
			PurchaseTransaction transaction = new PurchaseTransaction(indwResult, instrSKU, eProductType, instrRequestId, strReceipt);
	     	mDataStore.AddPendingPurchaseTransaction(transaction);
	     	
	     	if (mbListeningForPurchaseTransactions == true)
	     	{
	     		NativeOnTransactionStatusUpdated(transaction.GetResult(), transaction.GetSKU(), transaction.GetTransactionID(), transaction.GetPurchaseReceipt());
	     	}
		}
		else if (indwResult == PurchaseTransaction.kdwPurchaseFailed && mbListeningForPurchaseTransactions == true)
		{
	     	NativeOnTransactionStatusUpdated(indwResult, instrSKU, "", "");
		}
	}
	//-----------------------------------------------------
	/// Process Purchase For User
	///
	/// Processes a single purchase transaction for a user
	/// that is not currently logged in. This will store the
	/// information in the user specfic data store for
	/// resuming later.
	///
	/// This should never be called for the user that is
	/// currently logged in or the data store will become
	/// out of sync.
	///
	/// @param The 
	//-----------------------------------------------------
	private void ProcessPurchaseForUser(final String instrUserID, int indwResult, final String instrSKU, final ItemType ineItemType, final String instrRequestId, final String instrPurchaseToken)
	{
		if (indwResult == PurchaseTransaction.kdwPurchaseSuccessful || indwResult == PurchaseTransaction.kdwPurchaseRestored)
		{
			//get the product type.
			ProductType eProductType = ProductType.NONE;
			switch (ineItemType)
			{
				case CONSUMABLE:
					eProductType = ProductType.CONSUMABLE;
					break;
				case ENTITLED:
					eProductType = ProductType.ENTITLEMENT;
					break;
				default:
					Logging.logError("Amazon IAP System: Cannot process purchase, item type is unknown.");
					return;
			}
			
			String strUserIDHash = "" + Math.abs(HashCRC32.Generate(StringUtils.StringToUTF8ByteArray(instrUserID)));
			DataStore dataStore = new DataStore(kstrCacheNamePrefix + strUserIDHash + kstrCacheExtension, mstrPrivateKey, mstrUDID, instrUserID);
	         
			//get the purchase receipt
			String strReceipt = BuildPurchaseReceipt(instrUserID, instrPurchaseToken);

			if (eProductType == ProductType.ENTITLEMENT)
			{
				if (dataStore.IsEntitled(instrSKU) == true)
				{
					//Already entitled, so don't bother sending again.
					return;
				}
				
				dataStore.AddEntitledSKU(instrSKU);
			}
			
			PurchaseTransaction transaction = new PurchaseTransaction(indwResult, instrSKU, eProductType, instrRequestId, strReceipt);
			dataStore.AddPendingPurchaseTransaction(transaction);
		}
	}
	//-----------------------------------------------------
	/// Native On Products Descriptions Request Complete
	///
	/// Calls down to native with information on each
	/// product as requested.
	///
	/// @param The array of product IDs.
	/// @param The array of product names.
	/// @param The array of product descriptions.
	/// @param The array of formatted prices.
	//-----------------------------------------------------
	private native void NativeOnProductsDescriptionsRequestComplete(final String[] inastrIDs, final String[] inastrNames, final String[] inastrDescriptions, final String[] inastrFormattedPrices);
	//-----------------------------------------------------
	/// Native On Transaction Status Updated
	///
	/// Calls down to native to update the status of a
	/// purchase transaction.
	///
	/// @param The transaction result. This possible values
	/// for this are defined as contants in CPurchaseTransaction
	/// @param The product ID.
	/// @param The transaction ID.
	/// @param The receipt.
	//-----------------------------------------------------
	private native void NativeOnTransactionStatusUpdated(int indwResult, final String instrProductID, final String instrTransactionID, final String instrReceipt);
	//-----------------------------------------------------
	/// Native On Transaction Closed
	///
	/// Calls down to native to confirm that the transaction
	/// was successfully closed.
	///
	/// @param The product ID.
	/// @param The transaction ID.
	//-----------------------------------------------------
	private native void NativeOnTransactionClosed(final String instrProductID, final String instrTransactionID);
}
