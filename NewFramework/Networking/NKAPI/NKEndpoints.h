#pragma once
#include "NewFramework/Networking/Protocols/HTTP/IHttpCallback.h"

enum class eNKLinkServerCluster { ALAN_DEV, ROSS_DEV, STAGING, LIVE, NONE };
enum class eNKPublicUrlChannel { STANDARD, PRIORITY };

// TODO: implement every method that's not sendMessage, and also come up with field names for Endpoint
namespace NKEndpoints
{
    struct Endpoint
    {
        std::string path; // 0x00
        bool isFullUrl{}; // 0x18

        Endpoint(std::string path, bool isFullUrl) : path(path), isFullUrl(isFullUrl) {}
        Endpoint() = default;
    };

    std::string GetBase(eNKLinkServerCluster cluster);
    std::string GetWebviewBase(eNKLinkServerCluster cluster);
    std::string GetLoginWebView(eNKLinkServerCluster cluster);
    std::string GetProfileWebView(eNKLinkServerCluster cluster);
    std::string GetContactWebView(eNKLinkServerCluster cluster);
    std::string GetDownloadDirectory();
    std::string GetDownloadFile();
    std::string GetStaticEndpointBase(eNKLinkServerCluster cluster, eNKPublicUrlChannel channel);
    std::string GetPublicFileBaseUrl(eNKLinkServerCluster cluster);
    std::string GetPublicFileBaseUrl(eNKLinkServerCluster cluster, eNKPublicUrlChannel channel);
    std::string GetPublicFileUrl(eNKLinkServerCluster cluster, const std::string& a2,
                                 const std::string& a3, const std::string& a4);
    std::string GetPublicFileUrl(eNKLinkServerCluster cluster, eNKPublicUrlChannel channel,
                                 const std::string& a2, const std::string& a3,
                                 const std::string& a4, const std::string& a5);
    std::string GetSessionCacheFilePath(eNKLinkServerCluster cluster);
    std::string GetReplayFileBaseUrl(eNKLinkServerCluster cluster);
    std::string GetReplayFileUrl(eNKLinkServerCluster cluster, const std::string& a2, const std::string& a3);
    std::string IsSDKPath(const std::string& path);
    std::string GetPubSubUserSend();
    std::string GetPubsubUserRemove();
    std::string GetPubsubUserRead();
    std::string GetPubsubGlobalSend();
    std::string GetPubsubGlobalRead();
    std::string GetStorageSave();
    std::string GetStorageSaveCloud();
    std::string GetStorageSaveBuffer();
    std::string GetStorageLoad();
    std::string GetStorageLoadCloud();
    std::string GetUserOnline();
    std::string GetUserOnlineCheck();
    std::string GetUserOnlineGamestate();
    std::string GetUserCurrent();
    std::string GetUserSearch();
    std::string GetUserLoginFacebook();
    std::string GetUserCreateFacebook();
    std::string GetUserLinkFacebookSession();
    std::string GetUserLoginGamecenter();
    std::string GetUserCreateGamecenter();
    std::string GetUserLinkGamecenterSession();
    std::string GetUserLinkGamecenterMerge();
    std::string GetUserLoginAppleSignIn();
    std::string GetUserCreateAppleSignIn();
    std::string GetUserLinkAppleSignInSession();
    std::string GetUserLoginGoogleplay();
    std::string GetUserCreateGoogleplay();
    std::string GetUserLinkGoogleplaySession();
    std::string GetUserGoogleplayMerge();
    std::string GetUserLinkGoogleplayIds();
    std::string GetUserLoginGamecircle();
    std::string GetUserLinkGamecircleSession();
    std::string GetUserLoginSteam();
    std::string GetUserCreateSteam();
    std::string GetUserLinkSteamSession();
    std::string GetUserSteamTicket();
    std::string GetUserFacebookLongToken();
    std::string GetMatchmakingCreateMatchAnon();
    std::string GetMatchmakingCreateMatch();
    std::string GetMatchmakingFindMatch();
    std::string GetMatchmakingJoinMatchAnon();
    std::string GetMatchmakingJoinMatch();
    std::string GetMatchmakingRemoveMatch();
    std::string GetMatchmakingLeave();
    std::string GetMatchmakingLeaveAnon();
    std::string GetMatchmakingMigrate();
    std::string GetMatchmakingPlay();
    std::string GetMatchmakingPurge();
    std::string GetMatchmakingRegion();
    std::string GetMatchmakingInviteSet();
    std::string GetMatchmakingInviteGet();
    std::string GetMatchmakingInviteRemove();
    std::string GetMatchmakingStart();
    std::string GetMatchmakingUpdate();
    std::string GetMatchmakingRelayHeartbeat();
    std::string GetMatchmakingGet();
    std::string GetMatchmakingJoinGroup();
    std::string GetLeaderboardTopAnon();
    std::string GetLeaderboardTop();
    std::string GetLeaderboardSet();
    std::string GetLeaderboardAnonSet();
    std::string GetLeaderboardSetEpoch();
    std::string GetLeaderboardSetMulti();
    std::string GetLeaderboardRank();
    std::string GetLeaderboardRankAnon();
    std::string GetLeaderboardRankEpoch();
    std::string GetLeaderboardRankEpochAnon();
    std::string GetLeaderboardRanks();
    std::string GetLeaderboardRanksAnon();
    std::string GetLeaderboardRanksEpoch();
    std::string GetLeaderboardRanksMulti();
    std::string GetLeaderboardNeighbours();
    std::string GetLeaderboardNeighboursEpoch();
    std::string GetLeaderboardEpoch();
    std::string GetLeaderboardFptp();
    std::string GetBankBalances();
    std::string GetBankUpdate();
    std::string GetBankGlobalBalances();
    std::string GetBankGlobalUpdate();
    std::string GetPushRegister();
    std::string GetPushSend();
    std::string GetPaymentVerifyIos();
    std::string GetPaymentVerifyAndroid();
    std::string GetPaymentVerifyWinRT();
    std::string GetPaymentSteamIaps();
    std::string GetPaymentSteamStart();
    std::string GetPaymentSteamEnd();
    std::string GetGuildCreate();
    std::string GetGuildUpdate();
    std::string GetGuildSearch();
    std::string GetGuildMultipleGet();
    std::string GetGuildListAll();
    std::string GetGuildJoin();
    std::string GetGuildFindMembers();
    std::string GetGuildListMembers();
    std::string GetGuildListMembersWithFilter();
    std::string GetGuildRemoveMember();
    std::string GetGuildAcceptMember();
    std::string GetGuildAcceptMemberMulti();
    std::string GetGuildAddPermissions();
    std::string GetGuildRemovePermissions();
    std::string GetGuildValidName();
    std::string GetGuildOwnerUpdate();
    std::string GetGuildGetMember();
    std::string GetGuildMigrateStatus();
    std::string GetGuildMigrateCreate();
    std::string GetTournamentSeasonOpenJoin();
    std::string GetTournamentSeasonOpenLeave();
    std::string GetTournamentSeasonOpenView();
    std::string GetUtilityTime();
    std::string GetUtilityTimeFast();
    std::string GetUtilityGeoIP();
    std::string GetUtilityCountry();
    std::string GetUtilitySession();
    std::string GetUtilitySearch();
    std::string GetUtilityEsCreate();
    std::string GetUtilityEsUpdate();
    std::string GetPaymentAppleAttribution();
    int sendMessage(const std::string& endpoint, std::map<std::string, std::string>& headers,
                    const std::string& data, const std::string& callbackKey, HTTP_METHOD method,
                    IHttpCallback* cb, SHttpTimeoutOptions timeoutOptions);
}
