#include "NKMessages.h"
#include <json_spirit/json_spirit_writer.h>

template<> std::string NKJSON::Serialise(const NKMessage& val)
{
    json_spirit::mObject obj;
    Serialise(val, obj);
    return json_spirit::write(obj);
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
