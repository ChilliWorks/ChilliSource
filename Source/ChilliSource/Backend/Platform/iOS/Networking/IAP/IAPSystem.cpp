//
//  IAPSystem.cpp
//  MoFlow
//
//  Created by Scott Downie on 12/06/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Networking/IAP/IAPSystem.h>

#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>

namespace moFlo
{
    namespace iOSPlatform
    {
        CIAPSystem::CIAPSystem()
        {
            mpStoreKitSystem = [[StoreKitIAPSystem alloc] init];
        }
        //---------------------------------------------------------------
        /// Register Products
        //---------------------------------------------------------------
        void CIAPSystem::RegisterProducts(const DYNAMIC_ARRAY<Networking::IAPProductRegInfo>& inaProducts)
        {
            mProductRegInfos = inaProducts;
        }
        //---------------------------------------------------------------
        /// Get Provider ID
        //---------------------------------------------------------------
        std::string CIAPSystem::GetProviderID() const
        {
            return "Apple";
        }
        //---------------------------------------------------------------
        /// Get Provider Name
        //---------------------------------------------------------------
        std::string CIAPSystem::GetProviderName() const
        {
            return "iTunes";
        }
        //---------------------------------------------------------------
        /// Is Purchasing Enabled
        //---------------------------------------------------------------
        bool CIAPSystem::IsPurchasingEnabled()
        {
            return [mpStoreKitSystem isPurchasingEnabled] == YES;
        }
        //---------------------------------------------------------------
        /// Start Listening For Transaction Updates
        //---------------------------------------------------------------
        void CIAPSystem::StartListeningForTransactionUpdates(const Networking::IAPTransactionDelegate& inRequestDelegate)
        {
            mTransactionDelegate = inRequestDelegate;
            
            if(mTransactionDelegate == NULL)
                return;
            
            [mpStoreKitSystem startListeningForTransactions:fastdelegate::MakeDelegate(this, &CIAPSystem::OnTransactionUpdate)];
        }
        //---------------------------------------------------------------
        /// On Transaction Update
        //---------------------------------------------------------------
        void CIAPSystem::OnTransactionUpdate(NSString* inProductID, StoreKitIAP::TransactionResult ineResult, SKPaymentTransaction* inpTransaction)
        {
            if(mTransactionDelegate == NULL)
                return;
            
            Networking::IAPTransactionPtr pTransaction(new Networking::IAPTransaction());
            pTransaction->strProductID = Core::CStringUtils::NSStringToString(inProductID);
            
            bool bHasReceipt = false;
            Networking::IAPTransaction::Status eResult = Networking::IAPTransaction::FAILED;
            switch(ineResult)
            {
                case StoreKitIAP::SUCCEEDED:
                    bHasReceipt = true;
                    eResult = Networking::IAPTransaction::SUCCEEDED;
                    break;
                case StoreKitIAP::FAILED:
                    eResult = Networking::IAPTransaction::FAILED;
                    break;
                case StoreKitIAP::CANCELLED:
                    eResult = Networking::IAPTransaction::CANCELLED;
                    break;
                case StoreKitIAP::RESTORED:
                    bHasReceipt = true;
                    eResult = Networking::IAPTransaction::RESTORED;
                    break;
                case StoreKitIAP::RESUMED:
                    bHasReceipt = true;
                    eResult = Networking::IAPTransaction::RESUMED;
                    break;
            }
            
            if(pTransaction)
            {
                pTransaction->strTransactionID = Core::CStringUtils::NSStringToString(inpTransaction.transactionIdentifier);
                
                if(bHasReceipt)
                {
                    pTransaction->strReceipt = moFlo::CBaseEncoding::Base64Encode((s8*)[inpTransaction.transactionReceipt bytes], [inpTransaction.transactionReceipt length]);
                }
            }
            
            mTransactionDelegate(eResult, pTransaction);
        }
        //---------------------------------------------------------------
        /// Stop Listening For Transaction Updates
        //---------------------------------------------------------------
        void CIAPSystem::StopListeningForTransactionUpdates()
        {
            [mpStoreKitSystem stopListeningForTransactions];
        }
        //---------------------------------------------------------------
        /// Request Product Descriptions
        //---------------------------------------------------------------
        void CIAPSystem::RequestProductDescriptions(const DYNAMIC_ARRAY<std::string>& inaProductIDs, const Networking::IAPProductDescDelegate& inRequestDelegate)
        {
            mProductDescDelegate = inRequestDelegate;
            
            if(mProductDescDelegate == NULL)
                return;
            
            if(inaProductIDs.empty())
            {
                mProductDescDelegate(DYNAMIC_ARRAY<Networking::IAPProductDesc>());
                return;
            }
            
            NSMutableSet* idSet = [[NSMutableSet alloc] initWithCapacity:inaProductIDs.size()];
			
			for (u32 nID = 0; nID < inaProductIDs.size(); nID++)
            {
				[idSet addObject:Core::CStringUtils::StringToNSString(inaProductIDs[nID])];
			}
			
            [mpStoreKitSystem requestProducts:idSet forDelegate:fastdelegate::MakeDelegate(this, &CIAPSystem::OnProductDescriptionRequestComplete)];
            
            [idSet release];
        }
        //---------------------------------------------------------------
        /// Request All Product Descriptions
        //---------------------------------------------------------------
        void CIAPSystem::RequestAllProductDescriptions(const Networking::IAPProductDescDelegate& inRequestDelegate)
        {
            DYNAMIC_ARRAY<std::string> aIDs;
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
        void CIAPSystem::OnProductDescriptionRequestComplete(NSArray* inProducts)
        {
            if(mProductDescDelegate == NULL)
                return;
 
            DYNAMIC_ARRAY<Networking::IAPProductDesc> aResults;
            
            if(inProducts != nil)
            {
                NSNumberFormatter *pFormatter = [[NSNumberFormatter alloc] init];
                [pFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
                [pFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
                
                for(SKProduct* pProduct in inProducts)
                {
                    Networking::IAPProductDesc sDesc;
                    sDesc.strID = Core::CStringUtils::NSStringToString(pProduct.productIdentifier);
                    sDesc.strName = Core::CStringUtils::NSStringToString(pProduct.localizedTitle);
                    sDesc.strDescription = Core::CStringUtils::NSStringToString(pProduct.localizedDescription);
                    
                    [pFormatter setLocale:pProduct.priceLocale];
                    sDesc.strFormattedPrice = Core::CStringUtils::NSStringToString([pFormatter stringFromNumber:pProduct.price]);
                    
                    NSLocale* storeLocale = pProduct.priceLocale;
                    sDesc.strCountryCode = Core::CStringUtils::NSStringToString((NSString*)CFLocaleGetValue((CFLocaleRef)storeLocale, kCFLocaleCountryCode));
                    
                    aResults.push_back(sDesc);
                }
                
                [pFormatter release];
            }
            

            mProductDescDelegate(aResults);
        }
        //---------------------------------------------------------------
        /// Cancel Product Descriptions Request
        //---------------------------------------------------------------
        void CIAPSystem::CancelProductDescriptionsRequest()
        {
            [mpStoreKitSystem cancelProductsRequest];
        }
        //---------------------------------------------------------------
        /// Is Product ID Registered
        //---------------------------------------------------------------
        bool IsProductIDRegistered(const DYNAMIC_ARRAY<Networking::IAPProductRegInfo>& inProductRegInfos, const std::string& instrProductID)
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
        void CIAPSystem::RequestProductPurchase(const std::string& instrProductID)
        {
            MOFLOW_ASSERT(IsProductIDRegistered(mProductRegInfos, instrProductID), "Products must be registered with the IAP system before purchasing");
            NSString* productID = Core::CStringUtils::StringToNSString(instrProductID);
            [mpStoreKitSystem requestPurchaseWithProductID:productID andQuantity:1];
        }
        //---------------------------------------------------------------
        /// Close Transaction
        //---------------------------------------------------------------
        void CIAPSystem::CloseTransaction(const Networking::IAPTransactionPtr& inpTransaction, const Networking::IAPTransactionCloseDelegate& inDelegate)
        {
            [mpStoreKitSystem closeTransactionWithID:Core::CStringUtils::StringToNSString(inpTransaction->strTransactionID)];
            
            if(inDelegate)
            {
                inDelegate(inpTransaction->strProductID, inpTransaction->strTransactionID);
            }
        }
        //---------------------------------------------------------------
        /// Restore Non Consumable Purchases
        //---------------------------------------------------------------
        void CIAPSystem::RestoreManagedPurchases()
        {
            [mpStoreKitSystem restoreNonConsumablePurchases];
        }
        CIAPSystem::~CIAPSystem()
        {
            [mpStoreKitSystem release];
        }
    }
}