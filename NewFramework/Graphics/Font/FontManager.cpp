#include "FontManager.h"

#include "NewFramework/Platform/Shared/Assert.h"
#include "NewFramework/Platform/Shared/Logging.h"

#include <boost/smart_ptr/make_shared_object.hpp>

const boost::shared_ptr<CFont> CFontManager::AddFont(const SLocalisedFontInfo& fontInfo) {
	auto definition = ImportFontDefinition(fontInfo.mB, fontInfo.mD);
	NKAssert(definition != nullptr, "Failed to import font definition \'%s\' for font \'%s\'", fontInfo.mB.c_str(), fontInfo.mA.c_str());
	ENFORCE_LINE(0x142);

	auto result = boost::make_shared<CFont>(definition);
	mFontDefinitions[fontInfo.mA] = definition;
	return result;
}
