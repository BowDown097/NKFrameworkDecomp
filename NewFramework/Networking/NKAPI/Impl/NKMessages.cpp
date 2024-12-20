#include "NKMessages.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include "NewFramework/Utilities/HashHelper.h"

// remove this undef stuff when the time comes
#undef ENFORCE_LINE
#define ENFORCE_LINE(expected)

#define ASSERT_OBJECT_HAS_MEMBER(it, member, line) \
if (it == obj.end()) \
{ \
std::string errorStr = "object has no member called '" member "'"; \
LOG_ERROR("%s", errorStr.c_str()); ENFORCE_LINE(line);\
throw std::runtime_error("object has no member called '" member "'"); \
}

uint64_t NKResponseUser::GetCreationTimeStamp() const
{
    return nkapiID.size() >= 8 ? HashHelper::HexStringToValue(nkapiID.substr(0, 8)) : 0;
}

void NKJSON::Serialise(const NKMessageAuth& val, json_spirit::mObject& obj)
{
    obj["session"] = val.session;
    obj["device"] = val.device;
    obj["appID"] = val.appID;
    obj["skuID"] = val.skuID;
}

void NKJSON::Serialise(const NKMessage& val, json_spirit::mObject& obj)
{
    json_spirit::mObject authObj;
    Serialise(val.auth, authObj);

    obj["auth"] = authObj;
    obj["sig"] = val.sig;
    obj["data"] = val.data;
    obj["nonce"] = val.nonce;
}

void NKJSON::Serialise(const NKMessageSession& val, json_spirit::mObject& obj)
{
    obj["sessionID"] = val.sessionID;
    obj["expires"] = val.expires;
}

void NKJSON::Serialise(const NKResponseLogin& val, json_spirit::mObject& obj)
{
    json_spirit::mObject sessionObj;
    Serialise(val.session, sessionObj);
    obj["session"] = sessionObj;

    json_spirit::mObject userObj;
    Serialise(val.user, userObj);
    obj["user"] = sessionObj;
}

void NKJSON::Serialise(const NKResponseUser& val, json_spirit::mObject& obj)
{
    obj["nkapiID"] = val.nkapiID;
    obj["shortcode"] = val.shortcode;
    obj["displayName"] = val.displayName;
    obj["clan"] = val.clan;
    obj["country"] = val.country;
    obj["continent"] = val.continent;
    obj["avatar"] = val.avatar;
    obj["online"] = val.online;
    obj["onlineApp"] = val.onlineApp;
    obj["age"] = val.age;

    json_spirit::mArray providersAvailable;
    for (int i = 0; i < val.providersAvailable.size(); ++i)
        providersAvailable.push_back(val.providersAvailable[i]);

    obj["providersAvailable"] = providersAvailable;
    obj["access"] = val.access;
}

void NKJSON::Serialise(const NKMessageStorageSaveOptions& val, json_spirit::mObject& obj)
{
    obj["isPublic"] = val.isPublic;
    obj["compress"] = val.compress;
    obj["decode"] = val.decode;
    obj["reducedRedundancy"] = val.reducedRedundancy;
    obj["expiresIn"] = val.expiresIn;
    obj["render"] = val.render;
    obj["contentType"] = val.contentType;
    obj["stephen"] = val.stephen;
}

void NKJSON::Serialise(const NKMessageStorageSave& val, json_spirit::mObject& obj)
{
    obj["filename"] = val.filename;
    obj["data"] = val.data;

    json_spirit::mObject optionsObj;
    Serialise(val.options, optionsObj);
    obj["options"] = optionsObj;
}

void NKJSON::Serialise(const NKMessageStorageSaveBufferOptions& val, json_spirit::mObject& obj)
{
    obj["isPublic"] = val.isPublic;
    obj["compress"] = val.compress;
    obj["decode"] = val.decode;
    obj["reducedRedundancy"] = val.reducedRedundancy;
    obj["expiresIn"] = val.expiresIn;
    obj["render"] = val.render;
    obj["contentType"] = val.contentType;
    obj["stephen"] = val.stephen;
}

void NKJSON::Serialise(const NKMessageStorageSaveBuffer& val, json_spirit::mObject& obj)
{
    obj["filename"] = val.filename;
    obj["data"] = val.data;

    json_spirit::mObject optionsObj;
    Serialise(val.options, optionsObj);
    obj["options"] = optionsObj;

    obj["storageEngine"] = val.storageEngine;
    obj["maximumBufferDuration"] = val.maximumBufferDuration;
}

void NKJSON::Serialise(const NKMessageStorageLoad& val, json_spirit::mObject& obj)
{
    obj["filename"] = val.filename;
    obj["owner"] = val.owner;
    obj["includeBody"] = val.includeBody;
}

const bool NKJSON::TryParse(NKResponseUser& out, const json_spirit::mObject& obj)
{
    return true;
}

const bool NKJSON::TryParse(NKResponseUserCurrent& out, const json_spirit::mObject& obj)
{
    auto userIt = obj.find("user");

    if (userIt == obj.end())
    {
        std::string error = "object has no member called 'user'";
        LOG_ERROR("%s", error.c_str()); ENFORCE_LINE(1680);
        throw std::runtime_error("object has no member called 'reason'");
    }

    return TryParse(out.user, obj);
}

const bool NKJSON::TryParse(NKMessageErrorDetails& out, const json_spirit::mObject& obj)
{
    auto reasonIt = obj.find("reason");

    if (reasonIt == obj.end())
    {
        std::string error = "object has no member called 'reason'";
        LOG_ERROR("%s", error.c_str()); ENFORCE_LINE(4807);
        throw std::runtime_error("object has no member called 'reason'");
    }

    out.reason = reasonIt->second.get_str();

    auto fixIt = obj.find("fix");

    if (fixIt == obj.end())
    {
        std::string error = "object has no member called 'fix'";
        LOG_ERROR("%s", error.c_str()); ENFORCE_LINE(4818);
        throw std::runtime_error("object has no member called 'fix'");
    }

    out.fix = fixIt->second.get_str();
    return true;
}





const bool NKJSON::TryParse(NKMessageError& out, const json_spirit::mObject& obj)
{
    auto typeIt = obj.find("type");

    if (typeIt == obj.end())
    {
        std::string error = "object has no member called 'type'";
        LOG_ERROR("%s", error.c_str()); ENFORCE_LINE(4836);
        throw std::runtime_error("object has no member called 'type'");
    }

    out.type = typeIt->second.get_str();

    auto detailsIt = obj.find("details");

    if (detailsIt == obj.end())
    {
        std::string error = "object has no member called 'details'";
        LOG_ERROR("%s", error.c_str()); ENFORCE_LINE(4847);
        throw std::runtime_error("object has no member called 'details'");
    }

    return TryParse(out.details, detailsIt->second.get_obj());
}









const bool NKJSON::TryParse(NKMessageResponse& out, const json_spirit::mObject& obj)
{
    auto errorIt = obj.find("error");

    if (errorIt == obj.end())
    {
        std::string error = "object has no member called 'error'";
        LOG_ERROR("%s", error.c_str()); ENFORCE_LINE(4869);
        throw std::runtime_error("object has no member called 'error'");
    }

    if (!errorIt->second.is_null())
    {
        const json_spirit::mObject& errorObj = errorIt->second.get_obj();
        TryParse(out.error, errorObj);
    }

    if (!out.error.type.empty())
    {
        return true;
    }

    auto dataIt = obj.find("data");
    auto sigIt = obj.find("sig");

    ASSERT_OBJECT_HAS_MEMBER(dataIt, "data", 4887);
    ASSERT_OBJECT_HAS_MEMBER(sigIt, "sig", 4888);

    out.data = dataIt->second.get_str();
    out.sig = sigIt->second.get_str();

    return true;
}










const bool NKJSON::TryParse(NKMessageSession& out, const json_spirit::mObject& obj)
{
    auto sessionIDIt = obj.find("sessionID");
    auto expiresIt = obj.find("expires");

    ASSERT_OBJECT_HAS_MEMBER(sessionIDIt, "sessionID", 4910);
    ASSERT_OBJECT_HAS_MEMBER(expiresIt, "expires", 4911);

    out.sessionID = sessionIDIt->second.get_str();
    out.expires = sessionIDIt->second.get_uint64();

    return true;
}






const bool NKJSON::TryParse(NKResponseLogin& out, const json_spirit::mObject& obj)
{
    auto sessionIt = obj.find("session");
    ASSERT_OBJECT_HAS_MEMBER(sessionIt, "session", 4931);

    if (!TryParse(out.session, sessionIt->second.get_obj())) {
        return false;
    }

    auto userIt = obj.find("user");
    ASSERT_OBJECT_HAS_MEMBER(userIt, "user", 4938);

    return TryParse(out.user, userIt->second.get_obj());
}









const bool NKJSON::TryParse(NKResponseCreate& out, const json_spirit::mObject& obj)
{
    auto userIt = obj.find("user");

    if (userIt == obj.end())
    {
        std::string error = "object has no member called 'user'";
        LOG_ERROR("%s", error.c_str()); ENFORCE_LINE(4958);
        throw std::runtime_error("object has no member called 'user'");
    }

    return TryParse(out.user, userIt->second.get_obj());
}

// this is a bit of a sus method. surely this has a bunch of code #define'd out?
const bool NKJSON::TryParse(NKResponseLink& out, const json_spirit::mObject& obj)
{
    return true;
}




















const bool NKJSON::TryParse(NKMessageResponseFileStorage& out, const json_spirit::mObject& obj)
{
    auto filenameIt = obj.find("filename");
    ASSERT_OBJECT_HAS_MEMBER(filenameIt, "filename", 4993);
    out.filename = filenameIt->second.get_str();

    auto successIt = obj.find("success");
    ASSERT_OBJECT_HAS_MEMBER(successIt, "success", 4997);
    out.success = successIt->second.get_bool();
    if (!out.success)
        return true;

    auto dataIt = obj.find("data");
    ASSERT_OBJECT_HAS_MEMBER(dataIt, "data", 5003);
    out.data = !dataIt->second.is_null() ? dataIt->second.get_str() : "";

    auto urlIt = obj.find("url");
    ASSERT_OBJECT_HAS_MEMBER(urlIt, "url", 5007);
    out.url = !urlIt->second.is_null() ? urlIt->second.get_str() : "";

    auto etagIt = obj.find("etag");
    ASSERT_OBJECT_HAS_MEMBER(etagIt, "etag", 5011);
    out.etag = !etagIt->second.is_null() ? etagIt->second.get_str() : "";

    auto storageOptionsIt = obj.find("storageOptions");
    ASSERT_OBJECT_HAS_MEMBER(storageOptionsIt, "storageOptions", 5015);
    return TryParse(out.storageOptions, storageOptionsIt->second.get_obj());
}













const bool NKJSON::TryParse(NKMessageResponseFile& out, const json_spirit::mObject& obj)
{
    auto storageIt = obj.find("storage");
    ASSERT_OBJECT_HAS_MEMBER(storageIt, "storage", 5034);
    return TryParse(out.storage, storageIt->second.get_obj());
}












const bool NKJSON::TryParse(NKEndpointFileOptions& out, const json_spirit::mObject& obj)
{
    auto isPublicIt = obj.find("isPublic");
    ASSERT_OBJECT_HAS_MEMBER(isPublicIt, "isPublic", 5052);
    out.isPublic = isPublicIt->second.get_bool();

    auto compressIt = obj.find("compress");
    ASSERT_OBJECT_HAS_MEMBER(compressIt, "compress", 5056);
    out.compress = compressIt->second.get_bool();

    auto decodeIt = obj.find("decode");
    ASSERT_OBJECT_HAS_MEMBER(decodeIt, "decode", 5060);
    out.decode = decodeIt->second.get_bool();

    auto reducedRedundancyIt = obj.find("reducedRedundancy");
    ASSERT_OBJECT_HAS_MEMBER(reducedRedundancyIt, "reducedRedundancy", 5064);
    out.reducedRedundancy = reducedRedundancyIt->second.get_bool();

    return true;
}
