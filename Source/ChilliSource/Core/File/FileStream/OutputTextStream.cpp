//
//  OutputTextStream.cpp
//  ChilliSource
//
//  Created by Hugh McLaughlin on 18/05/2016.
//  Copyright © 2016 Tag Games. All rights reserved.
//

#include "OutputTextStream.h"

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    OutputTextStream::OutputTextStream(const std::string& filePath) noexcept
    {
        m_filename = filePath;
        
        m_fileStream.open(m_filename.c_str(), std::ios_base::binary);
        m_isValid = m_fileStream.is_open() == true && m_fileStream.bad() == false && m_fileStream.fail() == false;
    }
    //------------------------------------------------------------------------------
    bool OutputTextStream::IsValid() const noexcept
    {
        return m_isValid;
    }
    //------------------------------------------------------------------------------
    void OutputTextStream::Write(const std::string& data) noexcept
    {
        
    }
}