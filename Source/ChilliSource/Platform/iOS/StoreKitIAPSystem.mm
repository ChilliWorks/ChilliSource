//
//  StoreKitIAPSystem.mm
//  MoFlow
//
//  Created by Scott Downie on 12/06/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//


#include <moFlo/Platform/iOS/StoreKitIAPSystem.h>

@implementation StoreKitIAPSystem

//-------------------------------------------------------
/// Init
//-------------------------------------------------------
-(StoreKitIAPSystem*) init
{
    if ((self = [super init]))
    {
        mProductsDelegate = NULL;
        mTransactionUpdateDelegate = NULL;
        mProducts = nil;
        
        mOpenTransactions = [[NSMutableArray alloc] init];
        mUserPurchasedProductIDs = [[NSMutableArray alloc] init];
	}
    
	return self;
}
//-------------------------------------------------------
/// Is Purchasing Enabled
//-------------------------------------------------------
-(BOOL) isPurchasingEnabled
{
    return [SKPaymentQueue canMakePayments];
}
//-------------------------------------------------------
/// Request Products Fore Delegate
//-------------------------------------------------------
-(void) requestProducts:(NSSet*)inProductIDs forDelegate:(const StoreKitIAP::ProductsDelegate&) inDelegate
{
    //If we are already requesting the products then cancel and request the new product IDs instead
    if(mProductsDelegate)
    {
        [self cancelProductsRequest];
    }
    
    mProductsDelegate = inDelegate;
    
    if(mProducts != nil)
    {
        //Check if all the products already exist and if so then
        //we can return the cached version
        NSMutableArray* cachedProducts = [[NSMutableArray alloc] initWithCapacity:inProductIDs.count];
        for(NSString* productID in inProductIDs)
        {
            for(SKProduct* product in mProducts)
            {
                if([productID compare:product.productIdentifier] == NSOrderedSame)
                {
                    [cachedProducts addObject:product];
                    break;
                }
            }
        }
        
        if(cachedProducts.count == inProductIDs.count)
        {
            mProductsDelegate(cachedProducts);
            mProductsDelegate = NULL;
            return;
        }
    }
    
    mProductsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:inProductIDs];
    mProductsRequest.delegate = self;
    [mProductsRequest start];
}
//-------------------------------------------------------
/// Cancel Products Request
//-------------------------------------------------------
-(void) cancelProductsRequest 
{
    if(mProductsRequest != nil)
    {
        [mProductsRequest cancel];
        [mProductsRequest release];
    }
    mProductsRequest = nil;
	mProductsDelegate = NULL;
}
//-------------------------------------------------------
/// Products Request Did Receive Response
//-------------------------------------------------------
- (void)productsRequest:(SKProductsRequest*) inRequest didReceiveResponse:(SKProductsResponse *) inResponse
{
    //Clean up the product request as we allocated it on starting the request
    [mProductsRequest release];
    mProductsRequest = nil;
    
    if(inResponse.invalidProductIdentifiers.count > 0)
    {
        NSLog(@"Invalid Product IDs: %@", inResponse.invalidProductIdentifiers);
    }
    
    if(mProductsDelegate == NULL)
        return;
    
    //Store the products as we will need the objects to perform the purchases
    mProducts = [inResponse.products retain];
	
    mProductsDelegate(mProducts);
    mProductsDelegate = NULL;
}
//-------------------------------------------------------
/// Start Listening For Transactions
//-------------------------------------------------------
-(void) startListeningForTransactions:(const StoreKitIAP::TransactionUpdateDelegate&) inDelegate
{
    [self stopListeningForTransactions];
    
    mTransactionUpdateDelegate = inDelegate;
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
}
//-------------------------------------------------------
/// Stop Listening For Transactions
//-------------------------------------------------------
-(void) stopListeningForTransactions
{
    mTransactionUpdateDelegate = NULL;
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
}
//-------------------------------------------------------
/// Request Purchase With Product ID and Quantity
//-------------------------------------------------------
-(void) requestPurchaseWithProductID:(NSString*)inProductID andQuantity:(u32)inQuantity
{
    MOFLOW_ASSERT([mOpenTransactions count] == 0 && [mUserPurchasedProductIDs count] == 0, "IAPSystem: Cannot make multiple purchases at same time");
    
    for(SKProduct* pProduct in mProducts)
    {
        if([inProductID compare:pProduct.productIdentifier] == NSOrderedSame)
        {
            [mUserPurchasedProductIDs addObject:inProductID];
            
            SKMutablePayment * pPayment = [SKMutablePayment paymentWithProduct:pProduct];
            pPayment.quantity = inQuantity;
            [[SKPaymentQueue defaultQueue] addPayment:pPayment];
            return;
        }
    }
    
    //Invoke failure as we cannot find a product with that ID
    NSLog(@"ERROR: IAP: Purchase - Cannot find product with ID: %@", inProductID);
    
    if(mTransactionUpdateDelegate)
    {
        mTransactionUpdateDelegate(inProductID, StoreKitIAP::FAILED, nil);
    }
}
//-------------------------------------------------------
/// Payment Queue Updated Transactions
//-------------------------------------------------------
- (void)paymentQueue:(SKPaymentQueue *)inQueue updatedTransactions:(NSArray*)inTransactions
{
    if(mTransactionUpdateDelegate == NULL)
        return;
    
	for(SKPaymentTransaction * pTransaction in inTransactions)
    {
        if(SKPaymentTransactionStatePurchasing != pTransaction.transactionState)
        {
            [mOpenTransactions addObject:pTransaction];
        }
        
        StoreKitIAP::TransactionResult eResult = StoreKitIAP::FAILED;
        
		switch (pTransaction.transactionState)
        {
			case SKPaymentTransactionStatePurchased:
                //If this was not started by the user it is a past transaction resuming
                //otherwise it is a vanilla transaction
                eResult = StoreKitIAP::RESUMED;
                for(NSString* productID in mUserPurchasedProductIDs)
                {
                    if([productID compare:pTransaction.payment.productIdentifier] == NSOrderedSame)
                    {
                        eResult = StoreKitIAP::SUCCEEDED;
                        break;
                    }
                }
                break;
			case SKPaymentTransactionStateFailed:
				NSLog(@"%@", [pTransaction.error localizedDescription]);
                eResult = (pTransaction.error.code == SKErrorPaymentCancelled) ? StoreKitIAP::CANCELLED : StoreKitIAP::FAILED;
				break;
            case SKPaymentTransactionStateRestored:
                eResult = StoreKitIAP::RESTORED;
                break;
			default:
				continue;
				break;
		}
		
        mTransactionUpdateDelegate(pTransaction.payment.productIdentifier, eResult, pTransaction);
	}
}
//---------------------------------------------------------------
/// Restore Non Consumable Purchases
///
/// Request that the store trigger new purchase requests for
/// owned non-consumable items
//---------------------------------------------------------------
-(void) restoreNonConsumablePurchases
{
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}
//-------------------------------------------------------
/// Payment Queue Restore Completed Transactions Failed With Error
//-------------------------------------------------------
- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    NSLog(@"restoreCompletedTransactionsFailedWithError:");
    NSLog(@"%@", [error localizedDescription]);
}
//-------------------------------------------------------
/// Payment Queue Restore Completed Transactions Finished
//-------------------------------------------------------
- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
    NSLog(@"paymentQueueRestoreCompletedTransactionsFinished:");
}
//-------------------------------------------------------
/// Close Transaction With ID
///
/// Close the transaction with the given transaction ID
///
/// @param Transaction ID
//-------------------------------------------------------
-(void) closeTransactionWithID:(NSString*)inTransactionID
{
    for(SKPaymentTransaction* pTransaction in mOpenTransactions)
    {
        if([pTransaction.transactionIdentifier compare:inTransactionID] == NSOrderedSame)
        {
            u32 udwIndex = 0;
            for(NSString* productID in mUserPurchasedProductIDs)
            {
                if([productID compare:pTransaction.payment.productIdentifier] == NSOrderedSame)
                {
                    [mUserPurchasedProductIDs removeObjectAtIndex:udwIndex];
                    break;
                }
                ++udwIndex;
            }
           
            [[SKPaymentQueue defaultQueue] finishTransaction:pTransaction];
            [mOpenTransactions removeObject:pTransaction];
            return;
        }
    }
    
    NSLog(@"ERROR: IAP: Close Transaction - Cannot find transaction with ID: %@", inTransactionID);
}
//-------------------------------------------------------
/// dealloc
//-------------------------------------------------------
-(void) dealloc
{
    if(mProducts != nil)
        [mProducts release];
    
    if(mOpenTransactions != nil)
       [mOpenTransactions release];
    
    if(mUserPurchasedProductIDs != nil)
        [mUserPurchasedProductIDs release];
    
    [super dealloc];
}

@end
