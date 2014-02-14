/*
 *  Locale.h
 *  moFlo
 *
 *  Created by Ian Copland on 11/07/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Locale/MoFloLocale.h>

namespace moFlo
{
	namespace Core
	{
		CLocale::CLocale(std::string instrLanguageCode, std::string instrCountryCode)
		{
			mstrLanguageCode = instrLanguageCode;
			mstrCountryCode = instrCountryCode;
			mstrLocaleCode = mstrLanguageCode + "-" + mstrCountryCode;
		}
		CLocale::~CLocale()
		{

		}
		//---------------------------------------------------------------------
		/// Get Locale Code
		///
		/// @return The full locale code.
		//---------------------------------------------------------------------
		const std::string& CLocale::GetLocaleCode() const
		{
			return mstrLocaleCode;
		}
		//---------------------------------------------------------------------
		/// Get Country Code
		///
		/// @return The country code.
		//---------------------------------------------------------------------
		const std::string& CLocale::GetCountryCode() const
		{
			return mstrCountryCode;
		}
		//---------------------------------------------------------------------
		/// Get Language Code
		///
		/// @return The language code.
		//---------------------------------------------------------------------
		const std::string& CLocale::GetLanguageCode() const
		{
			return mstrLanguageCode;
		}
		//---------------------------------------------------------------------
		/// Set Locale Code
		///
		/// @param The full locale code.
		//---------------------------------------------------------------------
		void CLocale::SetLocaleCode(const std::string& instrCode)
		{
			mstrLocaleCode = instrCode;
		}
		//---------------------------------------------------------------------
		/// Set Country Code
		///
		/// @param The country code.
		//---------------------------------------------------------------------
		void CLocale::SetCountryCode(const std::string& instrCode)
		{
			mstrCountryCode = instrCode;
		}
		//---------------------------------------------------------------------
		/// Set Language Code
		///
		/// @param The language code.
		//---------------------------------------------------------------------
		void CLocale::SetLanguageCode(const std::string& instrCode)
		{
			mstrLanguageCode = instrCode;
		}
	}
}
