#ifndef _MOFLO_CORE_TWEAKABLECONSTANTS_H
#define _MOFLO_CORE_TWEAKABLECONSTANTS_H

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/XML/tinyxml.h>

#include <string>

#define USETWEAKABLECONSTANTS

#ifdef USETWEAKABLECONSTANTS

//Constants in root namespace
#define TC_FLOAT(name) ::ChilliSource::Core::TweakableConstants::GetSingleton().GetFloat(#name)
#define TC_INT(name) ::ChilliSource::Core::TweakableConstants::GetSingleton().GetInt(#name)
#define TC_STR(name) ::ChilliSource::Core::TweakableConstants::GetSingleton().GetString(#name)
#define TC_BOOL(name) ::ChilliSource::Core::TweakableConstants::GetSingleton().GetBool(#name)
#define TC_SAMPLECURVE(name,value) ::ChilliSource::Core::TweakableConstants::GetSingleton().SampleCurve(#name,value)

//Constants in namespace
#define TC_FLOATNS(namespace,name) ::ChilliSource::Core::TweakableConstants::GetSingleton().GetFloat(#namespace,#name)
#define TC_INTNS(namespace,name) ::ChilliSource::Core::TweakableConstants::GetSingleton().GetInt(#namespace,#name)
#define TC_STRNS(namespace,name) ::ChilliSource::Core::TweakableConstants::GetSingleton().GetString(#namespace,#name)
#define TC_BOOLNS(namespace,name) ::ChilliSource::Core::TweakableConstants::GetSingleton().GetBool(#namespace,#name)
#define TC_SAMPLECURVENS(namespace,name,value) ::ChilliSource::Core::TweakableConstants::GetSingleton().SampleCurve(#namespace,#name,value)

namespace ChilliSource
{
	namespace Core
	{
		
		class TweakableConstants
		{
		public: 
			TweakableConstants();
			~TweakableConstants();
			static TweakableConstants & GetSingleton();
			static TweakableConstants * GetSingletonPtr();
			
			void AddSourceFile(StorageLocation ineStorageLocation, const std::string & inFileName, bool inbLoadNow = true);
			
			void Reload();
			
			f32 GetFloat(const std::string & inNameSpace,const std::string & inConstName) const;
			f32 GetFloat(const std::string & inConstName) const;
			
			s32 GetInt(const std::string & inNameSpace, const std::string & inConstName) const;
			s32 GetInt(const std::string & inConstName) const;
			
			const std::string & GetString(const std::string & inNameSpace, const std::string & inConstName) const;
			const std::string & GetString(const std::string & inConstName) const;
			
			bool GetBool(const std::string & inNameSpace, const std::string & inConstName) const;
			bool GetBool(const std::string & inConstName) const;
			
			f32 SampleCurve(const std::string & inNameSpace, const std::string & inCurveName, f32 infXValue) const;
			f32 SampleCurve(const std::string & inCurveName, f32 infXValue) const;
			
			struct CurvePoint {
				f32 X;
				f32 Y;
			};
		protected:
			static TweakableConstants * msSingleton;
			void LoadValues();
			void LoadFile(StorageLocation ineStorageLocation, const std::string & inFileName);
			
			void ParseFileElement(TiXmlElement * inpElement, const std::string & outCurrentNamespace);
			

			
			typedef std::vector<CurvePoint> CurvePointVector;
			
			typedef std::unordered_map<std::string,float> MapNameToFloat;
			typedef MapNameToFloat::const_iterator MapNameToFloatItr;
			typedef std::unordered_map<std::string,int> MapNameToInt;
			typedef MapNameToInt::const_iterator MapNameToIntItr;
			typedef std::unordered_map<std::string,std::string> MapNameToString;
			typedef MapNameToString::const_iterator MapNameToStringItr;
			typedef std::unordered_map<std::string,bool> MapNameToBool;
			typedef MapNameToBool::const_iterator MapNameToBoolItr;
			
			typedef std::unordered_map<std::string,CurvePointVector> MapNameToCurvePoints;
			typedef MapNameToCurvePoints::const_iterator MapNameToCurvePointsItr;

			struct ConstantsGroup{
				MapNameToFloat Floats;
				MapNameToInt Ints;
				MapNameToString Strings;
				MapNameToBool Bools;
				MapNameToCurvePoints Curves;
			};
			
			typedef std::unordered_map<std::string,ConstantsGroup> MapNamespaceToConstantsGroup;
			typedef MapNamespaceToConstantsGroup::iterator MapNamespaceToConstantsGroupItr;
			typedef MapNamespaceToConstantsGroup::const_iterator MapNamespaceToConstantsGroupCItr;

			MapNamespaceToConstantsGroup mNamespaces;
			
			std::vector<Core::StorageLocationAndFilenamePtr> mFileNames;
			std::string mBlank;//blank string to return for failed string constant lookups
		};
		
		
	}
}

#else
#include "BakedConstants.h"
#define TC_FLOAT(x) x
#define TC_INT(x) x
#define TC_STR(x) x
#define TC_BOOL(x) x

#endif //USETWEAKABLECONSTANTS

#endif //TWEAKABLECONSTANTS_H
