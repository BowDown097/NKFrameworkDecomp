#pragma once
#include "NKModule.h"

struct NKModuleUtility : NKModule {
    using TimeResponseCallback = boost::function2<void, const NKError*, NKResponseUtilityTime>;
    void GetTime(NKMessageUtilityTime& time, TimeResponseCallback callback);
    void GetTimeResponse(const NKError* error, NKMessageResponse response, RequestContext context);

    using GeoIPResponseCallback = boost::function2<void, const NKError*, NKResponseUtilityGeoIP>;
    void GetGeoIP(NKMessageUtilityGeoIP& geoIP, GeoIPResponseCallback callback);
    void GetGeoIPResponse(const NKError* error, NKMessageResponse response, RequestContext context);

    using CountryResponseCallback = boost::function2<void, const NKError*, NKResponseUtilityCountry>;
    void GetCountry(NKMessageUtilityCountry& country, CountryResponseCallback callback);
    void GetCountryResponse(const NKError* error, NKMessageResponse response, RequestContext context);

    using SessionResponseCallback = boost::function2<void, const NKError*, NKResponseUtilitySession>;
    void GetIsSessionActive(NKMessageUtilitySession& session, SessionResponseCallback callback);
    void GetIsSessionActiveResponse(const NKError* error, NKMessageResponse response, RequestContext context);

    using SearchResponseCallback = boost::function2<void, const NKError*, NKResponseUtilitySearch>;
    void Search(const NKMessageUtilitySearch& search, SearchResponseCallback callback);
    void SearchResponse(const NKError* error, NKMessageResponse response, RequestContext context);

    using EsCreateResponseCallback = boost::function2<void, const NKError*, NKResponseUtilityEsCreate>;
    void EsCreate(const NKMessageUtilityEsCreate& esCreate, EsCreateResponseCallback callback);
    void EsCreateResponse(const NKError* error, NKMessageResponse response, RequestContext context);

    using EsUpdateResponseCallback = boost::function2<void, const NKError*, NKResponseUtilityEsUpdate>;
    void EsUpdate(const NKMessageUtilityEsUpdate& esUpdate, EsUpdateResponseCallback callback);
    void EsUpdateResponse(const NKError* error, NKMessageResponse response, RequestContext context);
};
