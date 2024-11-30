cmake_minimum_required(VERSION 3.5)
project(NKFrameworkDecomp VERSION 1.0 LANGUAGES C CXX)
add_library(NKFrameworkDecomp)

set_target_properties(NKFrameworkDecomp
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON)

set(TARGET_PLATFORM "steam" CACHE STRING "The platform that the game will be distributed to.")
target_compile_definitions(NKFrameworkDecomp PUBLIC TARGET_PLATFORM="${TARGET_PLATFORM}")

target_sources(NKFrameworkDecomp
    PRIVATE
        NewFramework/Analytics/DGAnalytics.cpp
        NewFramework/Analytics/DGAnalyticsData.cpp
        NewFramework/Analytics/DGAnalyticsEventGroups.cpp
        NewFramework/BaseApplication/AppClass/AppClass.cpp
        NewFramework/BaseApplication/AppClass/NKApplication.cpp
        NewFramework/BaseApplication/LegacyFeatures/LegacyFeatures.cpp
        NewFramework/BaseApplication/Screens/BaseScreen/BaseScreen.cpp
        NewFramework/BaseApplication/Screens/ScreenData.cpp
        NewFramework/BaseApplication/BasePointers.cpp
        NewFramework/Console/ConsoleVariable.cpp
        NewFramework/Graphics/Font/FontManager.cpp
        NewFramework/Graphics/Font/Font.cpp
        NewFramework/Graphics/Management/JPEG/JPEGTextures.cpp
        NewFramework/Graphics/Management/JPNG/JPNGTextures.cpp
        NewFramework/Graphics/Management/PVR/PVRTextures.cpp
        NewFramework/Graphics/Management/TextureLoader.cpp
        NewFramework/Graphics/Management/TextureManager.cpp
        NewFramework/Graphics/Objects/BasePositionableObject.cpp
        NewFramework/Graphics/Render/RenderSystem.cpp
        NewFramework/Graphics/Texture.cpp
        NewFramework/IO/File/BaseFileIO.cpp
        NewFramework/IO/File/FilePolicy.cpp
        NewFramework/IO/File/PNGInMemory.cpp
        NewFramework/Math/MathematicTypes/VectorDataStructures.cpp
        NewFramework/Math/Matrix.cpp
        NewFramework/Networking/NKAPI/Impl/NKMessages.cpp
        NewFramework/Networking/NKAPI/Impl/NKSessionImpl.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_AuthenticateService.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_CacheSession.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_CheckErrorType.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_CheckLoginServiceType.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_CheckOnline.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_CheckServiceActionType.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_ClearSession.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_GetUser.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_HideWebView.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_IsServiceAuthenticated.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_OpenSessionUsingToken.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_ReportErrorCallback.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_ServiceAction.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_SessionHttpRequest.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_SetError.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_ShowWebView.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_StoreStateCallback.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_WebViewErrorToolTip.cpp
        NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_WebviewRequestListener.cpp
        NewFramework/Networking/NKAPI/NKEndpoints.cpp
        NewFramework/Networking/NKAPI/NKError.cpp
        NewFramework/Networking/NKAPI/NKSession.cpp
        NewFramework/Networking/NKAPI/NKSignature.cpp
        NewFramework/Networking/Protocols/HTTP/HttpCallbackFunctor.cpp
        NewFramework/Networking/Protocols/HTTP/HttpRequest.cpp
        NewFramework/Platform/Shared/Assert.cpp
        NewFramework/Platform/Shared/Core.cpp
        NewFramework/Platform/Shared/Logging.cpp
        NewFramework/ThirdParty/easyzlib/easyzlib.c
        NewFramework/ThirdParty/libpng/load_png.cpp
        NewFramework/Utilities/Encoding.cpp
        NewFramework/Utilities/Json/Json_PropertyReader/JSON_Read.cpp
        NewFramework/Utilities/Misc/AssetBag.cpp
        NewFramework/Utilities/StringHelper.cpp
        NewFramework/Utilities/URLParser.cpp
        NewFramework/Utilities/Utilities.cpp
        Uncategorized/BehaviourTree.cpp
        Uncategorized/Encryption.cpp
        Uncategorized/GlobalVariables.cpp
        Uncategorized/NKLoginService.cpp
        Uncategorized/NKLoginWebview.cpp
        Uncategorized/RGBA.cpp
        Uncategorized/SpriteInformation.cpp
        Uncategorized/StephenEncryption.cpp
        Uncategorized/Version.cpp
    PUBLIC
        FILE_SET HEADERS FILES
            NewFramework/Analytics/DGAnalytics.h
            NewFramework/Analytics/DGAnalyticsData.h
            NewFramework/Analytics/DGAnalyticsEventGroups.h
            NewFramework/Analytics/IAnalytics.h
            NewFramework/Analytics/SAnalyticsInitialisation.h
            NewFramework/BaseApplication/AppClass/AppClass.h
            NewFramework/BaseApplication/AppClass/NKApplication.h
            NewFramework/BaseApplication/AppClass/NKSDKApp.h
            NewFramework/BaseApplication/LegacyFeatures/LegacyFeatures.h
            NewFramework/BaseApplication/Screens/BaseScreen/BaseScreen.h
            NewFramework/BaseApplication/Screens/ScreenData.h
            NewFramework/BaseApplication/BasePointers.h
            NewFramework/Console/ConsoleVariable.h
            NewFramework/Graphics/Font/FontManager.h
            NewFramework/Graphics/Font/Font.h
            NewFramework/Graphics/Management/JPEG/JPEGTextures.h
            NewFramework/Graphics/Management/JPNG/JPNGTextures.h
            NewFramework/Graphics/Management/PVR/PVRTextures.h
            NewFramework/Graphics/Management/TextureLoader.h
            NewFramework/Graphics/Management/TextureManager.h
            NewFramework/Graphics/Render/RenderSystem.h
            NewFramework/Graphics/Texture.h
            NewFramework/IO/File/BaseFileIO.h
            NewFramework/IO/File/FilePlatformData.h
            NewFramework/IO/File/FilePolicy.h
            NewFramework/IO/File/IFile.h
            NewFramework/IO/File/PNGInMemory.h
            NewFramework/Math/MathematicTypes/VectorDataStructures.h
            NewFramework/Math/Maths.h
            NewFramework/Math/Matrix.h
            NewFramework/Networking/NKAPI/Impl/NKMessages.h
            NewFramework/Networking/NKAPI/Impl/NKSessionImpl.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_AuthenticateService.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_CacheSession.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_CheckErrorType.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_CheckLoginServiceType.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_CheckOnline.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_CheckServiceActionType.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_ClearSession.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_GetUser.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_HideWebView.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_IsServiceAuthenticated.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_OpenSessionUsingToken.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_ReportErrorCallback.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_ServiceAction.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_SessionHttpRequest.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_SetError.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_ShowWebView.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_StoreStateCallback.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_WebViewErrorToolTip.h
            NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_WebviewRequestListener.h
            NewFramework/Networking/NKAPI/NKEndpoints.h
            NewFramework/Networking/NKAPI/NKError.h
            NewFramework/Networking/NKAPI/NKManager.h
            NewFramework/Networking/NKAPI/NKSession.h
            NewFramework/Networking/NKAPI/NKSignature.h
            NewFramework/Networking/Protocols/HTTP/HttpCallbackFunctor.h
            NewFramework/Networking/Protocols/HTTP/HttpRequest.h
            NewFramework/Networking/Protocols/HTTP/IHttpCallback.h
            NewFramework/Networking/Protocols/HTTP/IHttpRequestManager.h
            NewFramework/Platform/Shared/Assert.h
            NewFramework/Platform/Shared/Core.h
            NewFramework/Platform/Shared/Logging.h
            NewFramework/Platform/Shared/LoggingComponent.h
            NewFramework/ThirdParty/easyzlib/easyzlib.h
            NewFramework/ThirdParty/libpng/load_png.h
            NewFramework/Utilities/Encoding.h
            NewFramework/Utilities/Json/Json_PropertyReader/JSON_PropertyException.h
            NewFramework/Utilities/Json/Json_PropertyReader/JSON_Read.h
            NewFramework/Utilities/Misc/AssetBag.h
            NewFramework/Utilities/StringHelper.h
            NewFramework/Utilities/URLParser.h
            NewFramework/Utilities/Utilities.h
            Uncategorized/BehaviourTree.h
            Uncategorized/Blackboards.h
            Uncategorized/Encryption.h
            Uncategorized/GlobalVariables.h
            Uncategorized/NKLoginQuery.h
            Uncategorized/NKLoginService.h
            Uncategorized/NKLoginWebview.h
            Uncategorized/ReferenceCounted.h
            Uncategorized/RGBA.h
            Uncategorized/SpriteInformation.h
            Uncategorized/StephenEncryption.h
            Uncategorized/StuffToRefactor.h
            Uncategorized/Version.h
)

## Platform-specific
if(UNIX)
    target_sources(NKFrameworkDecomp PRIVATE NewFramework/Platform/iOS/File.cpp)
    target_sources(NKFrameworkDecomp PUBLIC FILE_SET HEADERS FILES NewFramework/Platform/iOS/File.h)
endif()

if(APPLE)
    target_sources(NKFrameworkDecomp PRIVATE NewFramework/Platform/OSX/OSXCore.mm)
endif()

# Packages
find_package(Boost REQUIRED)
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)

# jsonspirit
include(FetchContent)
FetchContent_Declare(
    jsonspirit
    GIT_REPOSITORY https://github.com/png85/json_spirit.git
    GIT_TAG master
)

set(BUILD_STATIC_LIBRARIES ON)
set(JSON_SPIRIT_MVALUE_ENABLED ON CACHE BOOL "Build with support for ascii map types")
set(JSON_SPIRIT_DEMOS OFF CACHE BOOL "Enable building demos")
set(JSON_SPIRIT_TESTS OFF CACHE BOOL "Enable building tests")

FetchContent_MakeAvailable(jsonspirit)

if(JSON_SPIRIT_MVALUE_ENABLED)
    target_compile_definitions(NKFrameworkDecomp PRIVATE JSON_SPIRIT_MVALUE_ENABLED)
endif()

# ticpp
FetchContent_Declare(
    ticpp
    GIT_REPOSITORY https://github.com/wxFormBuilder/ticpp.git
    GIT_TAG master
)
FetchContent_MakeAvailable(ticpp)

# hashlibpp
FetchContent_Declare(
    hashlibpp
    GIT_REPOSITORY https://github.com/ErrorFlexXx/hashlibpp.git
    GIT_TAG master
)
FetchContent_MakeAvailable(hashlibpp)

target_include_directories(NKFrameworkDecomp
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${JPEG_INCLUDE_DIR}
        ${jsonspirit_SOURCE_DIR}
        ${PNG_PNG_INCLUDE_DIR}
        ${ticpp_SOURCE_DIR}
)

target_link_libraries(NKFrameworkDecomp PRIVATE hashlibpp ${Boost_LIBRARIES} ${JPEG_LIBRARY} ${PNG_LIBRARY})

# Installation
include(GNUInstallDirs)
install(
    TARGETS NKFrameworkDecomp
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NKFrameworkDecomp
)
