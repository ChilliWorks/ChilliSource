//
//  StoreKitIAPSystem.h
//  Chilli Source
//  Created by Scott Downie on 12/06/2013.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2013 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifdef CS_TARGETPLATFORM_IOS

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

#endif