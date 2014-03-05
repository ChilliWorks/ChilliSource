//
//  IAPSystem.cpp
//  MoFlow
//
//  Created by Scott Downie on 12/06/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Networking/IAP/IAPSystem.h>

#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>
#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
    namespace iOS
    {
        IAPSystem::IAPSystem()
        {
            mpStoreKitSystem = [[StoreKitIAPSystem alloc] init];
        }
        //---------------------------------------------------------------
        /// Register Products
        //---------------------------------------------------------------
        void IAPSystem::RegisterProducts(const std::vector<Networking::IAPProductRegInfo>& inaProducts)
        {
            mProductRegInfos = inaProducts;
        }
        //---------------------------------------------------------------
        /// Get Provider ID
        //---------------------------------------------------------------
        std::string IAPSystem::GetProviderID() const
        {
            return "Apple";
        }
        //---------------------------------------------------------------
        /// Get Provider Name
        //---------------------------------------------------------------
        std::string IAPSystem::GetProviderName() const
        {
            return "iTunes";
        }
        //---------------------------------------------------------------
        /// Is Purchasing Enabled
        //---------------------------------------------------------------
        bool IAPSystem::IsPurchasingEnabled()
        {
            return [mpStoreKitSystem isPurchasingEnabled] == YES;
        }
        //---------------------------------------------------------------
        /// Start Listening For Transaction Updates
        //---------------------------------------------------------------
        void IAPSystem::StartListeningForTransactionUpdates(const Networking::IAPTransactionDelegate& inRequestDelegate)
        {
            mTransactionDelegate = inRequestDelegate;
            
            if(mTransactionDelegate == nullptr)
                return;
            
            [mpStoreKitSystem startListeningForTransactions:Core::MakeDelegate(this, &IAPSystem::OnTransactionUpdate)];
        }
        //---------------------------------------------------------------
        /// On Transaction Update
        //---------------------------------------------------------------
        void IAPSystem::OnTransactionUpdate(NSString* inProductID, StoreKitIAP::TransactionResult ineResult, SKPaymentTransaction* inpTransaction)
        {
            if(mTransactionDelegate == nullptr)
                return;
            
            Networking::IAPTransactionPtr pTransaction(new Networking::IAPTransaction());
            pTransaction->strProductID = Core::StringUtils::NSStringToString(inProductID);
            
            bool bHasReceipt = false;
            Networking::IAPTransaction::Status eResult = Networking::IAPTransaction::Status::k_failed;
            switch(ineResult)
            {
                case StoreKitIAP::TransactionResult::k_succeeded:
                    bHasReceipt = true;
                    eResult = Networking::IAPTransaction::Status::k_succeeded;
                    break;
                case StoreKitIAP::TransactionResult::k_failed:
                    eResult = Networking::IAPTransaction::Status::k_failed;
                    break;
                case StoreKitIAP::TransactionResult::k_cancelled:
                    eResult = Networking::IAPTransaction::Status::k_cancelled;
                    break;
                case StoreKitIAP::TransactionResult::k_restored:
                    bHasReceipt = true;
                    eResult = Networking::IAPTransaction::Status::k_restored;
                    break;
                case StoreKitIAP::TransactionResult::k_resumed:
                    bHasReceipt = true;
                    eResult = Networking::IAPTransaction::Status::k_resumed;
                    break;
            }
            
            if(pTransaction)
            {
                pTransaction->strTransactionID = Core::StringUtils::NSStringToString(inpTransaction.transactionIdentifier);
                
                if(bHasReceipt)
                {
                    pTransaction->strReceipt = Core::BaseEncoding::Base64Encode((s8*)[inpTransaction.transactionReceipt bytes], [inpTransaction.transactionReceipt length]);
                }
            }
            
            mTransactionDelegate(eResult, pTransaction);
        }
        //---------------------------------------------------------------
        /// Stop Listening For Transaction Updates
        //---------------------------------------------------------------
        void IAPSystem::StopListeningForTransactionUpdates()
        {
            [mpStoreKitSystem stopListeningForTransactions];
        }
        //---------------------------------------------------------------
        /// Request Product Descriptions
        //---------------------------------------------------------------
        void IAPSystem::RequestProductDescriptions(const std::vector<std::string>& inaProductIDs, const Networking::IAPProductDescDelegate& inRequestDelegate)
        {
            mProductDescDelegate = inRequestDelegate;
            
            if(mProductDescDelegate == nullptr)
                return;
            
            if(inaProductIDs.empty())
            {
                mProductDescDelegate(std::vector<Networking::IAPProductDesc>());
                return;
            }
            
            NSMutableSet* idSet = [[NSMutableSet alloc] initWithCapacity:inaProductIDs.size()];
			
			for (u32 nID = 0; nID < inaProductIDs.size(); nID++)
            {
				[idSet addObject:Core::StringUtils::StringToNSString(inaProductIDs[nID])];
			}
			
            [mpStoreKitSystem requestProducts:idSet forDelegate:Core::MakeDelegate(this, &IAPSystem::OnProductDescriptionRequestComplete)];
            
            [idSet release];
        }
        //---------------------------------------------------------------
        /// Request All Product Descriptions
        //---------------------------------------------------------------
        void IAPSystem::RequestAllProductDescriptions(const Networking::IAPProductDescDelegate& inRequestDelegate)
        {
            std::vector<std::string> aIDs;
            aIDs.reserve(mProductRegInfos.size());
            
            for(u32 i=0; i<mProductRegInfos.size(); ++i)
            {
                aIDs.push_back(mProductRegInfos[i].strID);
            }
            
            RequestProductDescriptions(aIDs, inRequestDelegate);
        }
        //---------------------------------------------------------------
        /// On Product Description Request Complete
        //---------------------------------------------------------------
        void IAPSystem::OnProductDescriptionRequestComplete(NSArray* inProducts)
        {
            if(mProductDescDelegate == nullptr)
                return;
 
            std::vector<Networking::IAPProductDesc> aResults;
            
            if(inProducts != nil)
            {
                NSNumberFormatter *pFormatter = [[NSNumberFormatter alloc] init];
                [pFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
                [pFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
                
                for(SKProduct* pProduct in inProducts)
                {
                    Networking::IAPProductDesc sDesc;
                    sDesc.strID = Core::StringUtils::NSStringToString(pProduct.productIdentifier);
                    sDesc.strName = Core::StringUtils::NSStringToString(pProduct.localizedTitle);
                    sDesc.strDescription = Core::StringUtils::NSStringToString(pProduct.localizedDescription);
                    
                    [pFormatter setLocale:pProduct.priceLocale];
                    sDesc.strFormattedPrice = Core::StringUtils::NSStringToString([pFormatter stringFromNumber:pProduct.price]);
                    
                    NSLocale* storeLocale = pProduct.priceLocale;
                    sDesc.strCountryCode = Core::StringUtils::NSStringToString((NSString*)CFLocaleGetValue((CFLocaleRef)storeLocale, kCFLocaleCountryCode));
                    
                    aResults.push_back(sDesc);
                }
                
                [pFormatter release];
            }
            

            mProductDescDelegate(aResults);
        }
        //---------------------------------------------------------------
        /// Cancel Product Descriptions Request
        //---------------------------------------------------------------
        void IAPSystem::CancelProductDescriptionsRequest()
        {
            [mpStoreKitSystem cancelProductsRequest];
        }
        //---------------------------------------------------------------
        /// Is Product ID Registered
        //---------------------------------------------------------------
        bool IsProductIDRegistered(const std::vector<Networking::IAPProductRegInfo>& inProductRegInfos, const std::string& instrProductID)
        {
            for(u32 i=0; i<inProductRegInfos.size(); ++i)
            {
                if(inProductRegInfos[i].strID == instrProductID)
                {
                    return true;
                }
            }
            
            return false;
        }
        //---------------------------------------------------------------
        /// Request Product Purchase
        //---------------------------------------------------------------
        void IAPSystem::RequestProductPurchase(const std::string& instrProductID)
        {
            CS_ASSERT(IsProductIDRegistered(mProductRegInfos, instrProductID), "Products must be registered with the IAP system before purchasing");
            NSString* productID = Core::StringUtils::StringToNSString(instrProductID);
            [mpStoreKitSystem requestPurchaseWithProductID:productID andQuantity:1];
        }
        //---------------------------------------------------------------
        /// Close Transaction
        //---------------------------------------------------------------
        void IAPSystem::CloseTransaction(const Networking::IAPTransactionPtr& inpTransaction, const Networking::IAPTransactionCloseDelegate& inDelegate)
        {
            [mpStoreKitSystem closeTransactionWithID:Core::StringUtils::StringToNSString(inpTransaction->strTransactionID)];
            
            if(inDelegate)
            {
                inDelegate(inpTransaction->strProductID, inpTransaction->strTransactionID);
            }
        }
        //---------------------------------------------------------------
        /// Restore Non Consumable Purchases
        //---------------------------------------------------------------
        void IAPSystem::RestoreManagedPurchases()
        {
            [mpStoreKitSystem restoreNonConsumablePurchases];
        }
        IAPSystem::~IAPSystem()
        {
            [mpStoreKitSystem release];
        }
    }
}