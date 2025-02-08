#include "NKModuleUtility.h"
#include "NewFramework/BaseApplication/LegacyFeatures/LegacyFeatures.h"

#include <boost/bind/bind.hpp>
using namespace boost::placeholders;

void NKModuleUtility::GetTime(NKMessageUtilityTime& time, TimeResponseCallback callback) {
    if (LegacyFeatures::bUseFastTime) {
        Request(
            NKEndpoints::GetUtilityTimeFast(),
            NKJSON::Serialise<NKMessageUtilityTime>(time),
            HTTP_METHOD::METHOD_POST,
            boost::bind(&NKModuleUtility::GetTimeResponse, this, _1, _2, _3),
            boost::make_shared<TimeResponseCallback>(callback));
    } else {
        Request(
            NKEndpoints::GetUtilityTime(),
            NKJSON::Serialise<NKMessageUtilityTime>(time),
            HTTP_METHOD::METHOD_POST,
            boost::bind(&NKModuleUtility::GetTimeResponse, this, _1, _2, _3),
            boost::make_shared<TimeResponseCallback>(callback));
    }
}

void NKModuleUtility::GetTimeResponse(const NKError* error, NKMessageResponse response, RequestContext context) {
    NKResponseUtilityTime r;
    NKJSON::TryParse<NKResponseUtilityTime>(r, response.data);

    auto responseCallback = boost::static_pointer_cast<TimeResponseCallback>(context.responseCallback);
    if (responseCallback) {
        (*responseCallback)(error, r);
    }
}

void NKModuleUtility::GetGeoIP(NKMessageUtilityGeoIP& geoIP, GeoIPResponseCallback callback) {
    Request(
        NKEndpoints::GetUtilityGeoIP(),
        NKJSON::Serialise<NKMessageUtilityGeoIP>(geoIP),
        HTTP_METHOD::METHOD_POST,
        boost::bind(&NKModuleUtility::GetGeoIPResponse, this, _1, _2, _3),
        boost::make_shared<GeoIPResponseCallback>(callback));
}

void NKModuleUtility::GetGeoIPResponse(const NKError* error, NKMessageResponse response, RequestContext context) {
    NKResponseUtilityGeoIP r;
    NKJSON::TryParse<NKResponseUtilityGeoIP>(r, response.data);

    auto responseCallback = boost::static_pointer_cast<GeoIPResponseCallback>(context.responseCallback);
    if (responseCallback) {
        (*responseCallback)(error, r);
    }
}

void NKModuleUtility::GetCountry(NKMessageUtilityCountry& country, CountryResponseCallback callback) {
    Request(
        NKEndpoints::GetUtilityCountry(),
        NKJSONCPP::Serialise<NKMessageUtilityCountry>(country),
        HTTP_METHOD::METHOD_POST,
        boost::bind(&NKModuleUtility::GetCountryResponse, this, _1, _2, _3),
        boost::make_shared<CountryResponseCallback>(callback));
}

void NKModuleUtility::GetCountryResponse(const NKError* error, NKMessageResponse response, RequestContext context) {
    NKResponseUtilityCountry r;
    NKJSONCPP::TryParse<NKResponseUtilityCountry>(r, response.data);

    auto responseCallback = boost::static_pointer_cast<CountryResponseCallback>(context.responseCallback);
    if (responseCallback) {
        (*responseCallback)(error, r);
    }
}

void NKModuleUtility::GetIsSessionActive(NKMessageUtilitySession& session, SessionResponseCallback callback) {
    Request(
        NKEndpoints::GetUtilitySession(),
        NKJSON::Serialise<NKMessageUtilitySession>(session),
        HTTP_METHOD::METHOD_POST,
        boost::bind(&NKModuleUtility::GetIsSessionActiveResponse, this, _1, _2, _3),
        boost::make_shared<SessionResponseCallback>(callback));
}

void NKModuleUtility::GetIsSessionActiveResponse(const NKError* error, NKMessageResponse response, RequestContext context) {
    NKResponseUtilitySession r;
    NKJSON::TryParse<NKResponseUtilitySession>(r, response.data);

    auto responseCallback = boost::static_pointer_cast<SessionResponseCallback>(context.responseCallback);
    if (responseCallback) {
        (*responseCallback)(error, r);
    }
}

void NKModuleUtility::Search(const NKMessageUtilitySearch& search, SearchResponseCallback callback) {
    Request(
        NKEndpoints::GetUtilitySearch(),
        NKJSON::Serialise<NKMessageUtilitySearch>(search),
        HTTP_METHOD::METHOD_POST,
        boost::bind(&NKModuleUtility::SearchResponse, this, _1, _2, _3),
        boost::make_shared<SearchResponseCallback>(callback));
}

void NKModuleUtility::SearchResponse(const NKError* error, NKMessageResponse response, RequestContext context) {
    NKResponseUtilitySearch r;
    NKJSON::TryParse<NKResponseUtilitySearch>(r, response.data);

    auto responseCallback = boost::static_pointer_cast<SearchResponseCallback>(context.responseCallback);
    if (responseCallback) {
        (*responseCallback)(error, r);
    }
}

void NKModuleUtility::EsCreate(const NKMessageUtilityEsCreate& esCreate, EsCreateResponseCallback callback) {
    Request(
        NKEndpoints::GetUtilityEsCreate(),
        NKJSONCPP::Serialise<NKMessageUtilityEsCreate>(esCreate),
        HTTP_METHOD::METHOD_POST,
        boost::bind(&NKModuleUtility::EsCreateResponse, this, _1, _2, _3),
        boost::make_shared<EsCreateResponseCallback>(callback));
}

void NKModuleUtility::EsCreateResponse(const NKError* error, NKMessageResponse response, RequestContext context) {
    NKResponseUtilityEsCreate r;
    NKJSONCPP::TryParse<NKResponseUtilityEsCreate>(r, response.data);

    auto responseCallback = boost::static_pointer_cast<EsCreateResponseCallback>(context.responseCallback);
    if (responseCallback) {
        (*responseCallback)(error, r);
    }
}

void NKModuleUtility::EsUpdate(const NKMessageUtilityEsUpdate& esUpdate, EsUpdateResponseCallback callback) {
    Request(
        NKEndpoints::GetUtilityEsUpdate(),
        NKJSONCPP::Serialise<NKMessageUtilityEsUpdate>(esUpdate),
        HTTP_METHOD::METHOD_POST,
        boost::bind(&NKModuleUtility::EsUpdateResponse, this, _1, _2, _3),
        boost::make_shared<EsUpdateResponseCallback>(callback));
}

void NKModuleUtility::EsUpdateResponse(const NKError* error, NKMessageResponse response, RequestContext context) {
    NKResponseUtilityEsUpdate r;
    NKJSONCPP::TryParse<NKResponseUtilityEsUpdate>(r, response.data);

    auto responseCallback = boost::static_pointer_cast<EsUpdateResponseCallback>(context.responseCallback);
    if (responseCallback) {
        (*responseCallback)(error, r);
    }
}
