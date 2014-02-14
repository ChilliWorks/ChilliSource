//
//  Locales.h
//  moFloTest
//
//  Created by Scott Downie on 18/03/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//


#ifndef _MO_FLO_CORE_LOCALES_H_
#define _MO_FLO_CORE_LOCALES_H_

#include <ChilliSource/Core/Locale/MoFloLocale.h>

namespace moFlo
{
    namespace Core
    {
    	//---------------------------------------------
		/// This provides some default locales to
    	/// compare against.
		//---------------------------------------------
        const CLocale kUnknownLocale("unknown");
        const CLocale kEnglishLocale("en");
        const CLocale kFrenchLocale("fr");
        const CLocale kGermanLocale("de");
        const CLocale kItalianLocale("it");
        const CLocale kSpanishLocale("es");
        const CLocale kPolishLocale("pl");
        const CLocale kCzechLocale("cs");
        const CLocale kRussianLocale("ru");
        const CLocale kJapaneseLocale("jp");
    }
}

#endif
