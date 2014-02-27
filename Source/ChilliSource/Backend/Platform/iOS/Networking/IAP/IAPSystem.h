//
//  IAPSystem.h
//  MoFlow
//
//  Created by Scott Downie on 12/06/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#ifndef _MOFLOW_PLATFORM_IOS_IAPSYSTEM_
#define _MOFLOW_PLATFORM_IOS_IAPSYSTEM_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Networking/IAP/IAPSystem.h>
#include <ChilliSource/Backend/Platform/iOS/Networking/IAP/StoreKitIAPSystem.h>

namespace ChilliSource
{
    namespace iOS
    {
        class CIAPSystem : public Networking::IAPSystem
        {
        public:
            CIAPSystem();
            ~CIAPSystem();

            //---------------------------------------------------------------
            /// Register Products
            ///
            /// Inform the system of which products are available for
            /// purchase and whether they are managed or unmanaged
            ///
            /// @param List of products
            //---------------------------------------------------------------
            void RegisterProducts(const std::vector<Networking::IAPProductRegInfo>& inaProducts);
            
            //---------------------------------------------------------------
			/// Get Provider ID
			///
			/// @return The ID off the IAP provider as a string.
            //---------------------------------------------------------------
			std::string GetProviderID() const;
            //---------------------------------------------------------------
			/// Get Provider Name
			///
			/// @return A displayable name for the IAP Provider.
            //---------------------------------------------------------------
			std::string GetProviderName() const;
            //---------------------------------------------------------------
			/// Is Purchasing Enabled
			///
			/// @return Whether the purchasing is allowed by the device/OS
            //---------------------------------------------------------------
            bool IsPurchasingEnabled();
            
            //---------------------------------------------------------------
			/// Start Listening For Transaction Updates
			///
			/// Calling this function will set the listener to which any
            /// transaction events are directed. This is not necessarily
            /// in response to a user action it may be previously outstanding
            /// transactions.
            ///
            /// @param Delegate
            //---------------------------------------------------------------
            void StartListeningForTransactionUpdates(const Networking::IAPTransactionDelegate& inRequestDelegate);
            //---------------------------------------------------------------
			/// Stop Listening For Transaction Updates
            ///
            /// Prevent any more transaction uppdates from being triggered.
            //---------------------------------------------------------------
            void StopListeningForTransactionUpdates();
            
            //---------------------------------------------------------------
			/// Request Product Descriptions
			///
            /// Starts a request to the store for details of the products.
            /// These details are name, description and price
            ///
			/// @param List of product IDs to request descriptions for
            /// @param Delegate to invoke when the request completes
            //---------------------------------------------------------------
            void RequestProductDescriptions(const std::vector<std::string>& inaProductIDs, const Networking::IAPProductDescDelegate& inRequestDelegate);
            //---------------------------------------------------------------
			/// Request All Product Descriptions
			///
            /// Starts a request to the store for details of all registered
            /// products. These details are name, description and price
            ///
            /// @param Delegate to invoke when the request completes
            //---------------------------------------------------------------
            void RequestAllProductDescriptions(const Networking::IAPProductDescDelegate& inRequestDelegate);
            //---------------------------------------------------------------
			/// Cancel Product Descriptions Request
			///
			/// Prevent the completion delegate being called for
            /// any pending product description requests and attempt to
            /// cancel the request to the store.
            //---------------------------------------------------------------
            void CancelProductDescriptionsRequest();
            
            //---------------------------------------------------------------
			/// Request Product Purchase
			///
			/// Make a request to the store to purchase the item.
            /// This will trigger a call to the transaction listener delegate
            ///
            /// @param Product ID
            //---------------------------------------------------------------
            void RequestProductPurchase(const std::string& instrProductID);
            
            //---------------------------------------------------------------
			/// Close Transaction
			///
			/// Tell the store to close the transaction as complete.
            /// NOTE: This should only be called after the goods have been
            /// awarded.
            ///
            /// @param Transaction to close
            /// @param Delegate to call when closed successfully
            //---------------------------------------------------------------
            void CloseTransaction(const Networking::IAPTransactionPtr& inpTransaction, const Networking::IAPTransactionCloseDelegate& inDelegate);
            
            //---------------------------------------------------------------
			/// Restore Managed Purchases
			///
            /// Request that the store trigger new purchase requests for
            /// owned non-consumable items
            //---------------------------------------------------------------
            void RestoreManagedPurchases();
            
        private:
            
            //---------------------------------------------------------------
            /// On Product Description Request Complete
            ///
            /// @param List of StoreKit product informtation
            //---------------------------------------------------------------
            void OnProductDescriptionRequestComplete(NSArray* inProducts);
            //---------------------------------------------------------------
            /// On Transaction Update
            ///
            /// Triggered by StoreKit with transactions update
            ///
            /// @param Product ID
            /// @param Result
            /// @param Transaction
            //---------------------------------------------------------------
            void OnTransactionUpdate(NSString* inProductID, StoreKitIAP::TransactionResult ineResult, SKPaymentTransaction* inpTransaction);
            
        private:
            
            std::vector<Networking::IAPProductRegInfo> mProductRegInfos;
            
            StoreKitIAPSystem* mpStoreKitSystem;
            
            Networking::IAPProductDescDelegate mProductDescDelegate;
            Networking::IAPTransactionDelegate mTransactionDelegate;
        };
    }
}

#endif 
