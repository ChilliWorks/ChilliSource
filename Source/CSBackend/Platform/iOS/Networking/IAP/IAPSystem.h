//
//  IAPSystem.h
//  Chilli Source
//
//  Created by Scott Downie on 12/06/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_IOS

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_IOS_NETWORKING_IAP_IAPSYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_IOS_NETWORKING_IAP_IAPSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Backend/Platform/iOS/Networking/IAP/StoreKitIAPSystem.h>
#include <ChilliSource/Networking/IAP/IAPSystem.h>

namespace ChilliSource
{
    namespace iOS
    {
        //----------------------------------------------------------------------------------
        /// System that allows purchasing of IAPs from
        /// the iOS App Store. System allows product info
        /// to be requested in ordrer to build displays and then
        /// for products to be purchased. NOTE: Must StartListeningForTransactionUpdates()
        /// before any purchases are made as this can be called with previously incomplete
        /// transactions.
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------
        class IAPSystem final : public Networking::IAPSystem
        {
        public:

            CS_DECLARE_NAMEDTYPE(IAPSystem);
            //---------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface ID to compare
            ///
            /// @return Whether the class is of the given type
            //---------------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //---------------------------------------------------------------
            /// Inform the system of which products are available for
            /// purchase and whether they are managed or unmanaged
            ///
            /// @author S Downie
            ///
            /// @param List of products
            //---------------------------------------------------------------
            void RegisterProducts(const std::vector<ProductRegInfo>& in_productInfos) override;
            //---------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The ID off the IAP provider as a string.
            //---------------------------------------------------------------
			std::string GetProviderID() const override;
            //---------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Whether the purchasing is allowed by the device/OS
            //---------------------------------------------------------------
            bool IsPurchasingEnabled() override;
            //---------------------------------------------------------------
			/// Calling this function will set the listener to which any
            /// transaction events are directed. This is not necessarily
            /// in response to a user action it may be previously outstanding
            /// transactions.
            ///
            /// @author S Downie
            ///
            /// @param Delegate
            //---------------------------------------------------------------
            void StartListeningForTransactionUpdates(const TransactionStatusDelegate& in_delegate) override;
            //---------------------------------------------------------------
            /// Prevent any more transaction uppdates from being triggered.
            ///
            /// @author S Downie
            //---------------------------------------------------------------
            void StopListeningForTransactionUpdates() override;
            //---------------------------------------------------------------
            /// Starts a request to the store for details of the products.
            /// These details are name, description and price
            ///
            /// @author S Downie
            ///
			/// @param List of product IDs to request descriptions for
            /// @param Delegate to invoke when the request completes
            //---------------------------------------------------------------
            void RequestProductDescriptions(const std::vector<std::string>& in_productIds, const ProductDescDelegate& in_delegate) override;
            //---------------------------------------------------------------
            /// Starts a request to the store for details of all registered
            /// products. These details are name, description and price
            ///
            /// @author S Downie
            ///
            /// @param Delegate to invoke when the request completes
            //---------------------------------------------------------------
            void RequestAllProductDescriptions(const ProductDescDelegate& in_delegate) override;
            //---------------------------------------------------------------
			/// Prevent the completion delegate being called for
            /// any pending product description requests and attempt to
            /// cancel the request to the store.
            ///
            /// @author S Downie
            //---------------------------------------------------------------
            void CancelProductDescriptionsRequest() override;
            //---------------------------------------------------------------
			/// Make a request to the store to purchase the item.
            /// This will trigger a call to the transaction listener delegate
            ///
            /// @author S Downie
            ///
            /// @param Product ID
            //---------------------------------------------------------------
            void RequestProductPurchase(const std::string& in_productId) override;
            //---------------------------------------------------------------
            /// Tell the store to close the transaction as complete.
            /// NOTE: This should only be called after the goods have been
            /// awarded.
            ///
            /// @author S Downie
            ///
            /// @param Transaction to close
            /// @param Delegate to call when closed (either with success or failure)
            //---------------------------------------------------------------
            void CloseTransaction(const TransactionSPtr& inpTransaction, const TransactionCloseDelegate& in_delegate) override;
            //---------------------------------------------------------------
            /// Request that the store trigger new purchase requests for
            /// owned non-consumable items
            ///
            /// @author S Downie
            //---------------------------------------------------------------
            void RestoreManagedPurchases() override;
            
        private:
            
            friend Networking::IAPSystemUPtr Networking::IAPSystem::Create(const Core::ParamDictionary&);
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            IAPSystem() = default;
            //-------------------------------------------------------
            /// Called when the system is created. Initialises
            /// the StoreKit backend
            ///
            /// @author S Downie
            //-------------------------------------------------------
            void OnInit() override;
            //-------------------------------------------------------
            /// Called when the system is shutdown.
            ///
            /// @author S Downie
            //-------------------------------------------------------
            void OnDestroy() override;
            //---------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param List of StoreKit product informtation
            //---------------------------------------------------------------
            void OnProductDescriptionRequestComplete(NSArray* in_products);
            //---------------------------------------------------------------
            /// Triggered by StoreKit with transactions update
            ///
            /// @author S Downie
            ///
            /// @param Product ID
            /// @param Result
            /// @param Transaction
            //---------------------------------------------------------------
            void OnTransactionUpdate(NSString* in_productId, StoreKitIAP::TransactionResult in_result, SKPaymentTransaction* in_transaction);
            
        private:
            
            std::vector<ProductRegInfo> m_productRegInfos;
            
            StoreKitIAPSystem* m_storeKitSystem;
            
            ProductDescDelegate m_productDescDelegate;
            TransactionStatusDelegate m_transactionStatusDelegate;
            TransactionCloseDelegate m_transactionCloseDelegate;
        };
    }
}

#endif 

#endif