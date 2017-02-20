/**
 * IAPSystemNativeInterface.java
 * ChilliSource
 * Created by Scott Downie on 12/06/2013.
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

package com.chilliworks.chillisource.networking;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.json.JSONException;
import org.json.JSONObject;

import android.content.Intent;

import com.chilliworks.chillisource.core.*;

public class GooglePlayIAPNativeInterface  extends com.chilliworks.chillisource.core.System
{
	//--------------------------------------------------------------
	/// Member data
	//--------------------------------------------------------------
    public static InterfaceId INTERFACE_ID = new InterfaceId();
	
	private static final int k_maxProductIDsPerRequest = 20;
	
	private IabHelper m_IABHelper = null;
	private boolean m_cancelProductDescRequest = false;
	private boolean m_isPurchasingEnabled = false;
	private boolean m_requestDescriptionsInProgress = false;
	private List<Purchase> m_currentPendingTransactions = new ArrayList<Purchase>();
	private Inventory m_inventory = null;
	
	//---------------------------------------------------------------------
	/// Native Methods
	//---------------------------------------------------------------------
	public native void NativeOnProductsDescriptionsRequestComplete(String[] in_IDs, String[] in_names, String[] in_descriptions, String[] in_formattedPrices, String[] in_currencyCodes, String[] in_unformattedPrices);
	public native void NativeOnTransactionStatusUpdated(int in_result, String in_productID, String in_transactionID, String in_receipt);
	public native void NativeOnTransactionClosed(String in_productID, String in_transactionID, boolean in_success);
	
	/**
	 * Constructor
	 * 
	 * @author Scott Downie
	 */
	public GooglePlayIAPNativeInterface()
	{
        init();
	}
    /**
     * Allows querying of whether or not the system implements the interface described by the
     * given interface id.
     *
     * @author Ian Copland
     *
     * @param in_interfaceId - The interface id to check
     *
     * @return Whether the system implements the given interface
     */
    @Override public boolean isA(InterfaceId in_interfaceId)
    {
        return (in_interfaceId == INTERFACE_ID);
    }
	/**
	 * Init
	 * 
	 * Prepares and starts the IAB system. This should be the first method
	 * called and it should only be called once. 
	 * 
	 * @author Scott Downie
	 */
	public void Init()
	{
		m_IABHelper = new IabHelper(CSApplication.get().getActivityContext(), GooglePlayLicensing.getLvlPublicKey());
		m_IABHelper.startSetup(new IabHelper.OnIabSetupFinishedListener()
		{
			@Override public void onIabSetupFinished(IabResult result) 
			{
				if(result.isSuccess())
				{
					m_isPurchasingEnabled = true;
				}
				else
				{
					m_isPurchasingEnabled = false;
					Logging.logError("Cannot setup Google Play IAB: " + result.getMessage());
				}
			}
		});
	}
	/**
	 * Request Product Descriptions
	 * 
	 * Request the descriptions of all the products on the store
	 * 
	 * @author HMcLaughlin
	 * 
	 * @param in_productIDs
	 */
	public void RequestProductDescriptions(final String[] in_productIDs)
	{
		if(m_requestDescriptionsInProgress && m_cancelProductDescRequest) // previously requested cancel but new request incoming
		{
			//This this will cause the system to received the previously requested product descriptions. This
			//means it will only work if the new request is identical to the previous one. 
			m_cancelProductDescRequest = false;
			return;
		}
		else if(m_requestDescriptionsInProgress) // just called twice, bail
			return;
		
		
		if(m_inventory != null)
		{
			OnProductsRequestComplete(m_inventory, in_productIDs);
			return;
		}
		
		// default flags for new request
		m_cancelProductDescRequest = false;
		m_requestDescriptionsInProgress = true;
			
		Thread descriptionRequestThread = new Thread(new Runnable()
		{
			@Override public void run() 
			{
				List<String> productIDs = new ArrayList<String>(Arrays.asList(in_productIDs));
				
				try 
				{
					Inventory itemsInventory = null;
					
					while(productIDs.size() > 0)
					{
						//Gets a sublist of max size|listsize
						ArrayList<String> idBatch = new ArrayList<String>(productIDs.subList(0, Math.min(k_maxProductIDsPerRequest, productIDs.size())));
						
						//Remove the sublist from the master list
						productIDs.removeAll(idBatch);
						
						//Make request with the sublist, this is a blocking call
						Inventory batchItems = m_IABHelper.queryInventory(true, idBatch);
						
						if(batchItems != null)
						{
							//Merge the inventory contents
							itemsInventory = MergeInventories(itemsInventory, batchItems);
						}
						else 
						{
							Logging.logError("Could not retrieve batch items - " + idBatch);
						}
					}
					
					m_requestDescriptionsInProgress = false;
					
					if(m_cancelProductDescRequest)
						return;
					
					m_inventory = itemsInventory;
					
					OnProductsRequestComplete(m_inventory, in_productIDs);
				} 
				catch (IabException in_exception) 
				{
					Logging.logError("Cannot query Google IAB inventory: " + in_exception.getMessage());
					OnProductsRequestComplete(null, in_productIDs);
					return;
				}
			}
		});
		
		descriptionRequestThread.start();
    }
	/**
	 * Merges the contents of two Inventories
	 * 
	 * @author HMcLaughlin
	 * 
	 * @param Inventory to combine
	 * @param Inventory to combine
	 * 
	 * @return Merged Inventory
	 */
	private Inventory MergeInventories(Inventory in_lhs, Inventory in_rhs)
	{
		Inventory mergedInventory = new Inventory();
		
		if(in_lhs != null)
		{
			mergedInventory.mPurchaseMap.putAll(in_lhs.mPurchaseMap);
			mergedInventory.mSkuMap.putAll(in_lhs.mSkuMap);
		}
		
		if(in_rhs != null)
		{
			mergedInventory.mPurchaseMap.putAll(in_rhs.mPurchaseMap);
			mergedInventory.mSkuMap.putAll(in_rhs.mSkuMap);
		}
		
		return mergedInventory;
	}
	/**
	 * On Products Request Complete
	 * 
	 * Triggered when the products description request completes
	 * 
	 * @author Scott Downie
	 * 
	 * @param Inventory
	 * @param Product IDs
	 */
    public void OnProductsRequestComplete(Inventory in_inventory, final String[] in_productIDs) 
    {
		List<IAPProductDescription> results = new ArrayList<IAPProductDescription>();
        List<String> currencyCodesList = new ArrayList<String>();
        List<String> unformattedPricesList = new ArrayList<String>();

		if(m_inventory != null)
		{
			for(int i=0; i<in_productIDs.length; ++i)
			{
				SkuDetails details = in_inventory.getSkuDetails(in_productIDs[i]);
				if(details != null)
				{
					IAPProductDescription desc = new IAPProductDescription();
					desc.ID = details.getSku();
					desc.Name = details.getTitle();
					desc.Description = details.getDescription();
					desc.FormattedPrice = details.getPrice();
                    results.add(desc);

                    String currencyCode = details.getPriceCurrencyCode();
                    currencyCodesList.add(currencyCode);

                    String unformattedPrice = Double.toString((double)details.getPriceAmountMicros() / 1000000.0);
                    unformattedPricesList.add(unformattedPrice);
				}
			}
		}

        int resultsSize = results.size();
    	String[] ids = new String[resultsSize];
    	String[] names = new String[resultsSize];
    	String[] descriptions = new String[resultsSize];
    	String[] formattedPrices = new String[resultsSize];
        String[] currencyCodes = new String[resultsSize];
        String[] unformattedPrices = new String[resultsSize];

    	for (int i = 0; i < resultsSize; ++i)
    	{
    		ids[i] = results.get(i).ID;
    		names[i] = results.get(i).Name;
    		descriptions[i] = results.get(i).Description;
            formattedPrices[i] = results.get(i).FormattedPrice;
            currencyCodes[i] = currencyCodesList.get(i);
            unformattedPrices[i] = unformattedPricesList.get(i);
    	}
  
    	NativeOnProductsDescriptionsRequestComplete(ids, names, descriptions, formattedPrices, currencyCodes, unformattedPrices);
    }
    /**
	 * Cancel Product Descriptions Request
	 * 
	 * Prevent the delegate being called with descriptions and
     * attempt to cancel the pending request
	 * 
	 * @author Scott Downie
	 */
    public void CancelProductDescriptionsRequest()
    {
    	m_cancelProductDescRequest = true;
    }
    /**
	 * Is Purchasing Enabled
	 * 
	 * @author Scott Downie
	 * 
	 * @return whether or not IAB V3 is supported on this device/OS
	 */
	public boolean IsPurchasingEnabled()
	{
		return m_isPurchasingEnabled;
	}
	 /**
	 * Get Purchase From Inventory
	 * 
	 * @author Scott Downie
	 * 
	 * @param Product ID
	 * 
	 * @return The product if owned by the user or null
	 */
	private Purchase GetPurchaseFromInventory(String in_productID)
	{
		if(m_inventory == null)
			return null;
		
		return m_inventory.getPurchase(in_productID);
	}
	/**
	 * Request Product Purchase
	 * 
	 * Start a transaction of an object with the given product ID.
	 * The transaction listener will receive updates on the transaction
	 * status
	 *  
	 * @author Scott Downie
	 * 
	 * @param Product ID
	 * @param Type (managed = 0 or unmanaged = 1 (from java interface))
	 */
	public void RequestProductPurchase(final String in_productID, final int in_type)
	{
		CSApplication.get().scheduleUIThreadTask(new Runnable()
		{
			@Override public void run() 
			{
				IabHelper.OnIabPurchaseFinishedListener listener = new IabHelper.OnIabPurchaseFinishedListener()
				{
					@Override public void onIabPurchaseFinished(IabResult in_result, Purchase in_info) 
					{
						int resultStatus = IAPTransactionDescription.SUCCEEDED;

						if(in_result.isFailure())
						{
							Logging.logError("Google IAB purchase error: " + in_result.getMessage() + " for product: " + in_productID);

							switch(in_result.getResponse())
							{
							case IabHelper.BILLING_RESPONSE_RESULT_USER_CANCELED:
							case IabHelper.IABHELPER_USER_CANCELLED:
								resultStatus = IAPTransactionDescription.CANCELLED;
								break;
							case IabHelper.BILLING_RESPONSE_RESULT_ITEM_ALREADY_OWNED:
								resultStatus = IAPTransactionDescription.FAILED;
								in_info = GetPurchaseFromInventory(in_productID);

								if(in_info != null)
								{
									//If this is a managed purchase then we are simply restoring the item
									if(in_info.getDeveloperPayload().compareTo("managed") == 0)
										resultStatus = IAPTransactionDescription.RESTORED;
									//If this is unmanaged then we are resuming an incomplete purchase
									else
										resultStatus = IAPTransactionDescription.RESUMED;
								}

								break;
							default:
								resultStatus = IAPTransactionDescription.FAILED;
								break;
							}
						}

						OnTransactionStatusUpdated(resultStatus, in_info, in_productID);
					}
				};
				
				//This uses an arbitrary value because the Google docs tell me to. The value represents the request code
				final int MANAGED = 0;
				final int UNMANAGED = 1;
				
				String payload = "unmanaged";
				switch(in_type)
				{
					case MANAGED:
						payload = "managed";
						break;
					case UNMANAGED:
						payload = "unmanaged";
						break;
				}
				
				m_IABHelper.launchPurchaseFlow(CSApplication.get().getActivity(), in_productID, 250102680, listener, payload);
			}
		});
	}
	/**
	 * On Transaction Status Updated
	 * 
	 * Is triggered whenever the status of a transaction changes i.e.
	 * completes or fails. This can be called at any point once the
	 * listener is attached and can be used to push through older 
	 * outstanding transactions
	 *  
	 * @author Scott Downie
	 * 
	 * @param Result
	 * @param Purchase
	 * @param Product ID
	 */
	public void OnTransactionStatusUpdated(int in_result, Purchase in_purchase, String in_productID) 
	{
		if(in_purchase != null)
		{
			m_currentPendingTransactions.add(in_purchase);
			
			JSONObject jsonSig = new JSONObject();
        	try 
        	{
        		jsonSig.put("SignedData", in_purchase.getOriginalJson());
				jsonSig.put("Signature", in_purchase.getSignature());
			}
        	catch (JSONException e) 
			{
				e.printStackTrace();
			}
     
			NativeOnTransactionStatusUpdated(in_result, in_productID, in_purchase.getOrderId(), jsonSig.toString());
		}
		else
		{
			NativeOnTransactionStatusUpdated(in_result, in_productID, "", "");
		}
	}
	/**
	 * Close Transaction
	 * 
	 * Consumes the transaction from the purchase list so that it may
	 * be purchased again
	 *  
	 * @author Scott Downie
	 * 
	 * @param Product ID
	 * @param Transaction ID
	 */
	public void CloseTransaction(final String in_productID, final String in_transactionID)
	{
		CSApplication.get().scheduleUIThreadTask(new Runnable()
		{
			@Override public void run() 
			{
				int count = 0;
				for(Purchase purchase : m_currentPendingTransactions)
				{
					if(purchase.getOrderId().compareTo(in_transactionID) == 0)
					{
						//Only consume consumables
						if(purchase.getDeveloperPayload().compareTo("unmanaged") == 0)
						{
							m_IABHelper.consumeAsync(purchase, new IabHelper.OnConsumeFinishedListener()
							{
								@Override public void onConsumeFinished(Purchase in_purchase, IabResult in_result) 
								{
									boolean success = in_result.isFailure() == false;
									if(success == false)
									{
										Logging.logError("Google IAB consume error: " + in_result.getMessage() + " for product: " + in_purchase.getSku());
									}
									
									NativeOnTransactionClosed(in_productID, in_transactionID, success);
								}
							});
						}

						m_currentPendingTransactions.remove(count);
						return;
					}

					count++;
				}
				
				NativeOnTransactionClosed(in_productID, in_transactionID, true);
			}
		});
	}
	/**
	 * Restore Pending Managed Transactions
	 * 
	 * Restore any pending transactions from the last session
	 *  
	 * @author Scott Downie
	 * 
	 * @param List of managed pending transactions
	 */
	public void RestorePendingManagedTransactions(final String[] in_pendingManagedTransactionIDs)
	{
		if(m_inventory == null)
			return;
		
		for(String transactionID : in_pendingManagedTransactionIDs)
		{
			List<Purchase> purchases = m_inventory.getAllPurchases();
			
			Purchase pendingPurchase = null;
			
			for(Purchase purchase : purchases)
			{
				if((purchase.getDeveloperPayload().compareTo("managed") == 0) && (purchase.getOrderId().compareTo(transactionID) == 0))
				{
					pendingPurchase = purchase;
					break;
				}
			}
			
			if(pendingPurchase != null)
			{
				OnTransactionStatusUpdated(IAPTransactionDescription.RESUMED, pendingPurchase, pendingPurchase.getSku());
			}
		}
	}
	/**
	 * Restore Pending Unmanaged Transactions
	 * 
	 * Restore any pending transactions from the last session
	 *  
	 * @author Scott Downie
	 */
	public void RestorePendingUnmanagedTransactions()
	{
		//Loop through the purchase list and pass on any managed products as new transactions but with the original receipts
		if(m_inventory == null)
			return;
		
		List<Purchase> purchases = m_inventory.getAllPurchases();
		
		for(Purchase purchase : purchases)
		{
			//If any consumables are in the list then they have no been consumed and are pending
			if(purchase.getDeveloperPayload().compareTo("unmanaged") == 0)
			{
				m_currentPendingTransactions.add(purchase);

				//Apparently 0 is good, 1 is cancelled and 2 is refunded
				switch(purchase.getPurchaseState())
				{
				case 0:
					OnTransactionStatusUpdated(IAPTransactionDescription.RESUMED, purchase, purchase.getSku());
					break;
				case 1:
					OnTransactionStatusUpdated(IAPTransactionDescription.CANCELLED, purchase, purchase.getSku());
					break;
				case 2:
					OnTransactionStatusUpdated(IAPTransactionDescription.REFUNDED, purchase, purchase.getSku());
					break;
				}
			}
		}
	}
	/**
	 * Restore Managed Purchases
	 * 
	 * Restores all previous managed purchase by injecting them as new
	 * transactions
	 *  
	 * @author Scott Downie
	 */
	public void RestoreManagedPurchases()
	{
		//Loop through the purchase list and pass on any managed products as new transactions but with the original receipts
		if(m_inventory == null)
		{
			Logging.logError("IAPSystem: Products must be registered and requested before any other actions");
			throw new NullPointerException("Products must be registered and requested before any other actions");
		}
		
		List<Purchase> purchases = m_inventory.getAllPurchases();
		
		for(Purchase purchase : purchases)
		{
			if(purchase.getDeveloperPayload().compareTo("managed") == 0)
			{
				//Apparently 0 is good, 1 is cancelled and 2 is refunded
				switch(purchase.getPurchaseState())
				{
				case 0:
					OnTransactionStatusUpdated(IAPTransactionDescription.RESTORED, purchase, purchase.getSku());
					break;
				case 1:
					OnTransactionStatusUpdated(IAPTransactionDescription.CANCELLED, purchase, purchase.getSku());
					break;
				case 2:
					OnTransactionStatusUpdated(IAPTransactionDescription.REFUNDED, purchase, purchase.getSku());
					break;
				}
			}
		}
	}
	/**
	 * On Destroy
	 * 
	 * Called when the ChilliSource activity is destroyed.
	 *  
	 * @author Scott Downie
	 */
	@Override public void onDestroy()
	{
		if(m_IABHelper != null)
		{
			m_IABHelper.dispose();
			m_IABHelper = null;
		}
	}
	/**
	 * On Activity Result
	 * 
	 * Called when the ChilliSource activity is "onActivityResult" is called.
	 *  
	 * @author Scott Downie
	 */
	@Override public void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		if(m_IABHelper != null)
		{
			m_IABHelper.handleActivityResult(requestCode, resultCode, data);
		}
	}
}
