#include "FontManager.h"

#include <NewFramework/Graphics/Texture.h>
#include <NewFramework/IO/File/BaseFileIO.h>
#include <NewFramework/IO/File/IFile.h>
#include <NewFramework/Graphics/Font/Font.h>
#include <NewFramework/Platform/Shared/Assert.h>
#include <NewFramework/Platform/Shared/Logging.h>

#include <boost/smart_ptr/make_shared_object.hpp>

IFile* FindFontFile (CBaseFileIO* fileIO, eTextureSize maxSize, const std::string& directory, const std::string& fontName) {
	for (int i = static_cast<int>(maxSize); i > static_cast<int>(eTextureSize::Unknown); i--) {
		auto path = CTexture::TextureSizeToString(static_cast<eTextureSize>(i));
		path = directory + path;
		path += "/";
		path += fontName;
		path += ".fnt";
		if (fileIO->FileExists(path, fileIO->assetPolicy)) {
			return fileIO->OpenFile(path, fileIO->assetPolicy, eFileOpenMode::Read);
		}
	}

	return nullptr;
}




































































































































































































































































































const boost::shared_ptr<CFont> CFontManager::AddFont(const SLocalisedFontInfo& fontInfo) {
	auto definition = ImportFontDefinition(fontInfo.mFontFile, fontInfo.mD);
	if (definition == nullptr) {
		LOG_ERROR("Failed to import font definition \'%s\' for font \'%s\'", fontInfo.mFontFile.c_str(), fontInfo.mFontName.c_str());
		ENFORCE_LINE(0x142);
		return nullptr;
	}

	//TODO: The arguments passed here are shitty guesses
	auto result = boost::make_shared<CFont>(fontInfo.mFontName, fontInfo.mC, definition, fontInfo.mD, fontInfo.mColor, fontInfo.mAlpha);
	mFontDefinitions[fontInfo.mFontName] = definition;
	return result;
}
