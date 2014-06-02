//
//  AmazonIAPSystem.h
//  Chilli Source
//
//  Created by Ian Copland on 10/12/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_ANDROID

#ifdef CS_ANDROIDEXTENSION_AMAZON

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_EXTENSIONS_AMAZON_AMAZONIAPSYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_EXTENSIONS_AMAZON_AMAZONIAPSYSTEM_H_

#include <ChilliSource/Backend/Platform/Android/Extensions/Amazon/AmazonIAPJavaInterface.h>
#include <ChilliSource/Networking/IAP/IAPSystem.h>

namespace ChilliSource
{
	namespace Android
	{
		//----------------------------------------------------------------------------------
		/// System that allows purchasing of IAPs from
		/// the Amazon App Store. System allows product info
		/// to be requested in ordrer to build displays and then
		/// for products to be purchased. NOTE: Must StartListeningForTransactionUpdates()
		/// before any purchases are made as this can be called with previously incomplete
		/// transactions.
		///
		/// Note: The CS pre-processor flag CS_ANDROIDEXTENSION_AMAZON should be set for this to be used.
		///
		/// @author I Copland
		//----------------------------------------------------------------------------------
		class AmazonIAPSystem final : public Networking::IAPSystem
		{
		public:

			CS_DECLARE_NAMEDTYPE(AmazonIAPSystem);

            //---------------------------------------------------------------
            /// @author I Copland
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
            /// @author I Copland
            ///
            /// @param List of products
            //---------------------------------------------------------------
            void RegisterProducts(const std::vector<ProductRegInfo>& in_productInfos) override;
            //---------------------------------------------------------------
			/// @author I Copland
			///
			/// @return The ID off the IAP provider as a string.
            //---------------------------------------------------------------
			std::string GetProviderID() const override;
            //---------------------------------------------------------------
			/// @author I Copland
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
            /// @author I Copland
            ///
            /// @param Delegate
            //---------------------------------------------------------------
            void StartListeningForTransactionUpdates(const TransactionStatusDelegate& in_delegate) override;
            //---------------------------------------------------------------
            /// Prevent any more transaction uppdates from being triggered.
            ///
            /// @author I Copland
            //---------------------------------------------------------------
            void StopListeningForTransactionUpdates() override;
            //---------------------------------------------------------------
            /// Starts a request to the store for details of the products.
            /// These details are name, description and price
            ///
            /// @author I Copland
            ///
			/// @param List of product IDs to request descriptions for
            /// @param Delegate to invoke when the request completes
            //---------------------------------------------------------------
            void RequestProductDescriptions(const std::vector<std::string>& in_productIds, const ProductDescDelegate& in_delegate) override;
            //---------------------------------------------------------------
            /// Starts a request to the store for details of all registered
            /// products. These details are name, description and price
            ///
            /// @author I Copland
            ///
            /// @param Delegate to invoke when the request completes
            //---------------------------------------------------------------
            void RequestAllProductDescriptions(const ProductDescDelegate& in_delegate) override;
            //---------------------------------------------------------------
			/// Prevent the completion delegate being called for
            /// any pending product description requests and attempt to
            /// cancel the request to the store.
            ///
            /// @author I Copland
            //---------------------------------------------------------------
            void CancelProductDescriptionsRequest() override;
            //---------------------------------------------------------------
			/// Make a request to the store to purchase the item.
            /// This will trigger a call to the transaction listener delegate
            ///
            /// @author I Copland
            ///
            /// @param Product ID
            //---------------------------------------------------------------
            void RequestProductPurchase(const std::string& in_productId) override;
            //---------------------------------------------------------------
            /// Tell the store to close the transaction as complete.
            /// NOTE: This should only be called after the goods have been
            /// awarded.
            ///
            /// @author I Copland
            ///
            /// @param Transaction to close
            /// @param Delegate to call when closed (either with success or failure)
            //---------------------------------------------------------------
            void CloseTransaction(const TransactionSPtr& inpTransaction, const TransactionCloseDelegate& in_delegate) override;
            //---------------------------------------------------------------
            /// Request that the store trigger new purchase requests for
            /// owned non-consumable items
            ///
            /// @author I Copland
            //---------------------------------------------------------------
            void RestoreManagedPurchases() override;

		private:

            friend Networking::IAPSystemUPtr Networking::IAPSystem::Create(const Core::ParamDictionary&);
            //---------------------------------------------------------------
            /// Private constructor to enforce use of factory method
            ///
            /// @author I Copland
			///
			/// @param A dictionary of platform specific parameters. The
			/// parameters that relate to the Amazon IAP System are as
			/// follows:
			/// 	AmazonPrivateKey	The private key used to encrypt
            ///							the on disk Amazon IAP cache.
            //---------------------------------------------------------------
			AmazonIAPSystem(const Core::ParamDictionary& in_params);
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

		private:

            std::vector<ProductRegInfo> m_productRegInfos;
            std::string m_privateKey;
            AmazonIAPJavaInterfaceSPtr m_javaInterface;
		};
	}
}

#endif

#endif

#endif