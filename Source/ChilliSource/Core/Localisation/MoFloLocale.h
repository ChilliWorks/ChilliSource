/*
 *  Locale.h
 *  moFlo
 *
 *  Created by Ian Copland on 11/07/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_LOCALE_H_
#define _MO_FLO_CORE_LOCALE_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Core
	{
		class Locale
		{
		public:
            Locale(std::string instrLanguageCode, std::string instrCountryCode = std::string());
            ~Locale();
			//---------------------------------------------------------------------
			/// Get Locale Code
			///
			/// @return The full locale code.
			//---------------------------------------------------------------------
            const std::string& GetLocaleCode() const;
			//---------------------------------------------------------------------
			/// Get Country Code
			///
			/// @return The country code.
			//---------------------------------------------------------------------
            const std::string& GetCountryCode() const;
			//---------------------------------------------------------------------
			/// Get Language Code
			///
			/// @return The language code.
			//---------------------------------------------------------------------
            const std::string& GetLanguageCode() const;
			//---------------------------------------------------------------------
			/// Set Locale Code
			///
			/// @param The full locale code.
			//---------------------------------------------------------------------
            void SetLocaleCode(const std::string& instrCode);
			//---------------------------------------------------------------------
			/// Set Country Code
			///
			/// @param The country code.
			//---------------------------------------------------------------------
			void SetCountryCode(const std::string& instrCode);
			//---------------------------------------------------------------------
			/// Set Language Code
			///
			/// @param The language code.
			//---------------------------------------------------------------------
			void SetLanguageCode(const std::string& instrCode);

		private:
			std::string mstrLocaleCode;
			std::string mstrCountryCode;
			std::string mstrLanguageCode;
		};
	}
}

#endif
