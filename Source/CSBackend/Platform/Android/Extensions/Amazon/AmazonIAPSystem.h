//
//  AmazonIAPSystem.h
//  Chilli Source
//  Created by Ian Copland on 10/12/2013.
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifdef CS_ANDROIDEXTENSION_AMAZON

#ifndef _CSBACKEND_PLATFORM_ANDROID_EXTENSIONS_AMAZON_AMAZONIAPSYSTEM_H_
#define _CSBACKEND_PLATFORM_ANDROID_EXTENSIONS_AMAZON_AMAZONIAPSYSTEM_H_

#include <CSBackend/Platform/Android/Extensions/Amazon/AmazonIAPJavaInterface.h>
#include <ChilliSource/Networking/IAP/IAPSystem.h>

namespace CSBackend
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
		/// @author Ian Copland
		//----------------------------------------------------------------------------------
		class AmazonIAPSystem final : public CSNetworking::IAPSystem
		{
		public:

			CS_DECLARE_NAMEDTYPE(AmazonIAPSystem);

            //---------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param Interface ID to compare
            ///
            /// @return Whether the class is of the given type
            //---------------------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //---------------------------------------------------------------
            /// Inform the system of which products are available for
            /// purchase and whether they are managed or unmanaged
            ///
            /// @author Ian Copland
            ///
            /// @param List of products
            //---------------------------------------------------------------
            void RegisterProducts(const std::vector<ProductRegInfo>& in_productInfos) override;
            //---------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The ID off the IAP provider as a string.
            //---------------------------------------------------------------
			std::string GetProviderID() const override;
            //---------------------------------------------------------------
			/// @author Ian Copland
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
            /// @author Ian Copland
            ///
            /// @param Delegate
            //---------------------------------------------------------------
            void StartListeningForTransactionUpdates(const TransactionStatusDelegate& in_delegate) override;
            //---------------------------------------------------------------
            /// Prevent any more transaction uppdates from being triggered.
            ///
            /// @author Ian Copland
            //---------------------------------------------------------------
            void StopListeningForTransactionUpdates() override;
            //---------------------------------------------------------------
            /// Starts a request to the store for details of the products.
            /// These details are name, description and price
            ///
            /// @author Ian Copland
            ///
			/// @param List of product IDs to request descriptions for
            /// @param Delegate to invoke when the request completes
            //---------------------------------------------------------------
            void RequestProductDescriptions(const std::vector<std::string>& in_productIds, const ProductDescDelegate& in_delegate) override;
            //---------------------------------------------------------------
            /// Starts a request to the store for details of all registered
            /// products. These details are name, description and price
            ///
            /// @author Ian Copland
            ///
            /// @param Delegate to invoke when the request completes
            //---------------------------------------------------------------
            void RequestAllProductDescriptions(const ProductDescDelegate& in_delegate) override;
            //---------------------------------------------------------------
			/// Prevent the completion delegate being called for
            /// any pending product description requests and attempt to
            /// cancel the request to the store.
            ///
            /// @author Ian Copland
            //---------------------------------------------------------------
            void CancelProductDescriptionsRequest() override;
            //---------------------------------------------------------------
			/// Make a request to the store to purchase the item.
            /// This will trigger a call to the transaction listener delegate
            ///
            /// @author Ian Copland
            ///
            /// @param Product ID
            //---------------------------------------------------------------
            void RequestProductPurchase(const std::string& in_productId) override;
            //---------------------------------------------------------------
            /// Tell the store to close the transaction as complete.
            /// NOTE: This should only be called after the goods have been
            /// awarded.
            ///
            /// @author Ian Copland
            ///
            /// @param Transaction to close
            /// @param Delegate to call when closed (either with success or failure)
            //---------------------------------------------------------------
            void CloseTransaction(const TransactionSPtr& inpTransaction, const TransactionCloseDelegate& in_delegate) override;
            //---------------------------------------------------------------
            /// Request that the store trigger new purchase requests for
            /// owned non-consumable items
            ///
            /// @author Ian Copland
            //---------------------------------------------------------------
            void RestoreManagedPurchases() override;

		private:

            friend CSNetworking::IAPSystemUPtr CSNetworking::IAPSystem::Create(const CSCore::ParamDictionary&);
            //---------------------------------------------------------------
            /// Private constructor to enforce use of factory method
            ///
            /// @author Ian Copland
			///
			/// @param A dictionary of platform specific parameters. The
			/// parameters that relate to the Amazon IAP System are as
			/// follows:
			/// 	AmazonPrivateKey	The private key used to encrypt
            ///							the on disk Amazon IAP cache.
            //---------------------------------------------------------------
			AmazonIAPSystem(const CSCore::ParamDictionary& in_params);
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
