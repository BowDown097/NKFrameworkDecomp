#pragma once

#include "FontDefinition.h"

#include <boost/smart_ptr/shared_ptr.hpp>

class IFile;

struct IFontImporter {
    virtual ~IFontImporter() = default;
    virtual const bool CanImport(const std::string& sPath) const;
    virtual const boost::shared_ptr<const SFontDefinition> Import(IFile* const pFile, const bool bLuminanceAlpha);
};
