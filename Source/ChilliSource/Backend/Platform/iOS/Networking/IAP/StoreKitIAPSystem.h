//
//  StoreKitIAPSystem.h
//  MoFlow
//
//  Created by Scott Downie on 12/06/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/ChilliSource.h>

#import <StoreKit/StoreKit.h>

#include <functional>

namespace StoreKitIAP
{
    enum class TransactionResult
    {
        k_succeeded,
        k_failed,
        k_cancelled,
        k_restored,
        k_resumed
    };
    
    typedef std::function<void(NSArray*)> ProductsDelegate;
    typedef std::function<void(NSString*, TransactionResult, SKPaymentTransaction*)> TransactionUpdateDelegate;
}

@interface StoreKitIAPSystem : NSObject<SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
    StoreKitIAP::ProductsDelegate mProductsDelegate;
    NSArray* mProducts;
    NSMutableArray* mOpenTransactions;
    NSMutableArray* mUserPurchasedProductIDs;
    SKProductsRequest* mProductsRequest;
    StoreKitIAP::TransactionUpdateDelegate mTransactionUpdateDelegate;
}
//-------------------------------------------------------
/// Init
//-------------------------------------------------------
-(StoreKitIAPSystem*) init;

//-------------------------------------------------------
/// Is Purchasing Enabled
///
/// @return Whether purchasing is enabled or not
//-------------------------------------------------------
-(BOOL) isPurchasingEnabled;

//-------------------------------------------------------
/// Request Products Fore Delegate
///
/// Starts requesting the SKProducts from the store
/// based on the given product IDs and return the
/// results to the given delegate
///
/// @param Set of product IDs
/// @param Delegate to pass results to
//-------------------------------------------------------
-(void) requestProducts:(NSSet*)inProductIDs forDelegate:(const StoreKitIAP::ProductsDelegate&) inDelegate;
//-------------------------------------------------------
/// Cancel Products Request
///
/// Cancels the request.
//-------------------------------------------------------
-(void) cancelProductsRequest;

//-------------------------------------------------------
/// Start Listening For Transactions
///
/// Add the delegate as a listener to the payment
/// queue
///
/// @param Delegate to pass updates to
//-------------------------------------------------------
-(void) startListeningForTransactions:(const StoreKitIAP::TransactionUpdateDelegate&) inDelegate;
//-------------------------------------------------------
/// Stop Listening For Transactions
///
/// Remove the delegate as a listener to the payment
/// queue
//-------------------------------------------------------
-(void) stopListeningForTransactions;

//-------------------------------------------------------
/// Request Purchase With Product ID and Quantity
///
/// @param Product ID
/// @param Quantity
//-------------------------------------------------------
-(void) requestPurchaseWithProductID:(NSString*)inProductID andQuantity:(u32)inQuantity;

//-------------------------------------------------------
/// Close Transaction With ID
///
/// Close the transaction with the given transaction ID
///
/// @param Transaction ID
//-------------------------------------------------------
-(void) closeTransactionWithID:(NSString*)inTransactionID;

//---------------------------------------------------------------
/// Restore Non Consumable Purchases
///
/// Request that the store trigger new purchase requests for
/// owned non-consumable items
//---------------------------------------------------------------
-(void) restoreNonConsumablePurchases;

@end

