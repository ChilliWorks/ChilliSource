//
//  GooglePlayIAPSystem.h
//  ChilliSource
//  Created by Scott Downie on 14/06/2013.
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

#ifdef CS_ANDROIDFLAVOUR_GOOGLEPLAY

#ifndef _CSBACKEND_PLATFORM_ANDROID_EXTENSIONS_GOOGLEPLAY_NETWORKING_GOOGLEPLAYIAPSYSTEM_H_
#define _CSBACKEND_PLATFORM_ANDROID_EXTENSIONS_GOOGLEPLAY_NETWORKING_GOOGLEPLAYIAPSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Networking/IAP/IAPSystem.h>

#include <CSBackend/Platform/Android/GooglePlay/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/GooglePlay/JNI/Networking/IAP/GooglePlayIAPJavaInterface.h>
#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>

namespace CSBackend
{
	namespace Android
	{
		//----------------------------------------------------------------------------------
		/// System that allows purchasing of IAPs from
		/// the Google Play Store. System allows product info
		/// to be requested in ordrer to build displays and then
		/// for products to be purchased. NOTE: Must StartListeningForTransactionUpdates()
		/// before any purchases are made as this can be called with previously incomplete
		/// transactions.
		///
		/// Note: The CS pre-processor flag CS_ANDROIDFLAVOUR_GOOGLEPLAY should be set for this to be used.
		///
		/// @author S Downie
		//----------------------------------------------------------------------------------
		class GooglePlayIAPSystem final : public ChilliSource::IAPSystem
		{
		public:

            CS_DECLARE_NAMEDTYPE(GooglePlayIAPSystem);

            //---------------------------------------------------------------
            /// This defines platform-specific extra product information.
            ///
            /// @author T Kane
            //---------------------------------------------------------------
            struct ExtraProductInfo final
            {
                std::string m_productId;            // Platform-specific product ID
                std::string m_unformattedPrice;     // Unformatted price e.g. 1.00
                std::string m_currencyCode;         // ISO 4217 currency code, e.g. GBP, USD
            };

            //---------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface ID to compare
            ///
            /// @return Whether the class is of the given type
            //---------------------------------------------------------------
            bool IsA(ChilliSource::InterfaceIDType in_interfaceId) const override;
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
            /// @param Purchasing enabled delegate
            //---------------------------------------------------------------
            void IsPurchasingEnabled(const PurchasingEnabledDelegate& in_delegate) override;
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
            //---------------------------------------------------------------
            /// @author T Kane
            ///
            /// @return A vector of extra product information, particular to
            /// this platform. Note that products must be requested from the
            /// store via RequestProductDescriptions or
            /// RequestProductDescriptions before being available so there is
            /// no guarantee that a particular product will be available. It is
            /// the application's responsibility to ensure the correct data
            /// has been requested up-front.
            //---------------------------------------------------------------
            const std::vector<GooglePlayIAPSystem::ExtraProductInfo>& GetExtraProductInfo() const { return m_extraProductsInfo; }

		private:
            friend ChilliSource::IAPSystemUPtr ChilliSource::IAPSystem::Create(const ChilliSource::ParamDictionary&);
            //---------------------------------------------------------------
            /// Private constructor to force the use of the factory method
            ///
            /// @author S Downie
			///
			/// @param A dictionary of platform specific parameters. The
			/// parameters that relate to the Google Play IAP System are as
			/// follows:
            /// 	GooglePlayPublicKey  	The public key used for connecting
            ///								to the Google Play store.
            //---------------------------------------------------------------
			GooglePlayIAPSystem(const ChilliSource::ParamDictionary& in_params);
            //-------------------------------------------------------
            /// Called when the system is created. Initialises
            /// the Java backend
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

            GooglePlayIAPJavaInterfaceSPtr m_javaInterface;

            std::string m_publicKey;
		    std::vector<ExtraProductInfo> m_extraProductsInfo;
		};
	}
}

#endif

#endif

#endif
