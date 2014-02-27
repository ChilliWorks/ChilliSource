//
//  Utils.cpp
//  moFlo
//
//  Created by Andrew Glass on 09/08/2012.
//
//

#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/JSON/json.h>
#include <zlib.h>

using namespace ChilliSource::Core;

bool CUtils::ReadJson(ChilliSource::Core::StorageLocation ineStorageLocation, const std::string &instrPath, Json::Value *outpJson)
{
    if(!outpJson)
    {
    	CS_WARNING_LOG("CUtils::ReadJson| Output is NULL");
        return false;
    }
    
    std::string strJsonString;
    if(!FileToString(ineStorageLocation, instrPath, strJsonString))
        return false;
    
    Json::Reader jReader;
    
    *outpJson = Json::Value();
    
    if(!jReader.parse(strJsonString, *outpJson))
    {
        CS_WARNING_LOG("CUtils::ReadJson| Json could not be parsed:" + jReader.getFormatedErrorMessages());
        return false;
    }
    
    return true;
}

bool CUtils::FileToString(StorageLocation ineStorageLocation, const std::string & instrPath, std::string & outstrFileContent)
{
    FileStreamSPtr pFile = Application::GetFileSystemPtr()->CreateFileStream(ineStorageLocation, instrPath, FileMode::k_read);
    
    if(!pFile || pFile->IsOpen() == false)
    {
        CS_WARNING_LOG("CUtils::FileToString| Could not open file" + instrPath);
        return false;
    }

    pFile->GetAll(outstrFileContent);
    
    return true;
}

FileStreamSPtr CUtils::StringToFile(StorageLocation ineStorageLocation, const std::string & instrPath, const std::string& instrFileOut)
{
    FileStreamSPtr pFile = Application::GetFileSystemPtr()->CreateFileStream(ineStorageLocation, instrPath, FileMode::k_write);
    
    if(!pFile || pFile->IsOpen() == false)
    {
        CS_WARNING_LOG("CUtils::StringToFile| Could not open file" + instrPath);
        return FileStreamSPtr();
    }
    
    pFile->Write(instrFileOut);
    
    return pFile;    
}

bool CUtils::ZlibCompressString(const std::string &instrUncompressed, std::string &outstrCompressed)
{
    uLongf maxCompSize = instrUncompressed.size() * 1.1f + 12;
    
    outstrCompressed.clear();
    outstrCompressed.resize(maxCompSize);
    
    s32 zlibResult = compress2(
                              (Bytef*)(outstrCompressed.data()),
                              &maxCompSize,
                              (Bytef*)instrUncompressed.data(),
                              instrUncompressed.size(),
                               Z_BEST_COMPRESSION);
    
    
    if(zlibResult == Z_OK)
    {
        outstrCompressed.resize(maxCompSize);
        return true;
    }
    
    outstrCompressed.clear();
    return false;
}

bool CUtils::ZlibDecompressString(const std::string &instrCompressed, std::string& outstrUncompress,  u32 inOutputSizeGuess)
{
    outstrUncompress.clear();
    outstrUncompress.resize(inOutputSizeGuess);
    
    uLongf outBufferSize = inOutputSizeGuess;
    s32 zlibResult = uncompress(
                                (Bytef*)(outstrUncompress.data()) ,
                                &outBufferSize,
                                (const Bytef*)instrCompressed.data(),
                                instrCompressed.size());
    
    if(zlibResult == Z_OK)
    {
        outstrUncompress.resize(outBufferSize);
        return true;
    }
    
    outstrUncompress.clear();
    return false;
}

Vector2 CUtils::ScaleMaintainingAspectRatio(const Vector2& invCurrentSize, const Vector2& invTargetSize, bool inbFitInside)
{
    if(invCurrentSize == invTargetSize)
    {
        return invCurrentSize;
    }
    
    Vector2 vResult;
    
    Vector2 vDiff(invTargetSize.x - invCurrentSize.x, invTargetSize.y - invCurrentSize.y);
	
	// reverse the checks below so the smallest value is picked instead
	if(inbFitInside)
	{
		vDiff *= -1.0f;
	}
    
    if(vDiff.x > vDiff.y)
    {
        f32 fScaleFactor = invTargetSize.x / invCurrentSize.x;
        vResult.x = invTargetSize.x;
        vResult.y = invCurrentSize.y * fScaleFactor;
    }
    else
    {
        f32 fScaleFactor = invTargetSize.y / invCurrentSize.y;
        vResult.y = invTargetSize.y;
        vResult.x = invCurrentSize.x * fScaleFactor;
    }
    
    return vResult;
}
