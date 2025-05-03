cmake_minimum_required(VERSION 3.23)
project(NKFrameworkDecomp VERSION 1.0 LANGUAGES C CXX)
add_library(NKFrameworkDecomp)

set_target_properties(NKFrameworkDecomp
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

set(AD_SYSTEMS "" CACHE STRING "The ad systems to enable in the ad interface, semicolon separated (IronSource, Tapjoy, etc.).")
set(TARGET_PLATFORM "steam" CACHE STRING "The platform that the game will be distributed to.")

target_compile_definitions(NKFrameworkDecomp PUBLIC TARGET_PLATFORM="${TARGET_PLATFORM}")

# Sources
target_sources(NKFrameworkDecomp
    PRIVATE
        NewFramework/AdServer/AdInterface.cpp
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
        NewFramework/Events/EventManager.cpp
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
        NewFramework/Networking/NKAPI/Impl/NKFileClientImpl.cpp
        NewFramework/Networking/NKAPI/Impl/NKMessages.cpp
        NewFramework/Networking/NKAPI/Impl/NKSessionImpl.cpp
        NewFramework/Networking/NKAPI/Impl/FileClientActions/NKAction_AppendFileSavePath.cpp
        NewFramework/Networking/NKAPI/Impl/FileClientActions/NKAction_DownloadFile.cpp
        NewFramework/Networking/NKAPI/Impl/FileClientActions/NKAction_FileConflict.cpp
        NewFramework/Networking/NKAPI/Impl/FileClientActions/NKAction_FileWebRequest.cpp
        NewFramework/Networking/NKAPI/Impl/FileClientActions/NKAction_UploadFile.cpp
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
        NewFramework/Networking/NKAPI/Modules/NKModule.cpp
        NewFramework/Networking/NKAPI/Modules/NKModuleUtility.cpp
        NewFramework/Networking/NKAPI/NKCrossPromo.cpp
        NewFramework/Networking/NKAPI/NKEndpoints.cpp
        NewFramework/Networking/NKAPI/NKEnums.cpp
        NewFramework/Networking/NKAPI/NKError.cpp
        NewFramework/Networking/NKAPI/NKFileClient.cpp
        NewFramework/Networking/NKAPI/NKManager.cpp
        NewFramework/Networking/NKAPI/NKModules.cpp
        NewFramework/Networking/NKAPI/NKSession.cpp
        NewFramework/Networking/NKAPI/NKSignature.cpp
        NewFramework/Networking/Protocols/HTTP/HttpCallbackFunctor.cpp
        NewFramework/Networking/Protocols/HTTP/HttpRequest.cpp
        NewFramework/Platform/Shared/Assert.cpp
        NewFramework/Platform/Shared/Core.cpp
        NewFramework/Platform/Shared/Logging.cpp
        NewFramework/ThirdParty/libpng/load_png.cpp
        NewFramework/Utilities/Encoding.cpp
        NewFramework/Utilities/HashHelper.cpp
        NewFramework/Utilities/JSON/JSON_PropertyReader/JSON_Read.cpp
        NewFramework/Utilities/JSON/JSONWrapper.cpp
        NewFramework/Utilities/Misc/AssetBag.cpp
        NewFramework/Utilities/StringHelper.cpp
        NewFramework/Utilities/URLParser.cpp
        NewFramework/Utilities/Utilities.cpp
        Uncategorized/BehaviourTree.cpp
        Uncategorized/Encryption.cpp
        Uncategorized/GlobalVariables.cpp
        Uncategorized/NKLoginService.cpp
        Uncategorized/NKLoginWebview.cpp
        Uncategorized/NKRequestFactory.cpp
        Uncategorized/ProfanityFilter.cpp
        Uncategorized/RGBA.cpp
        Uncategorized/SpriteInformation.cpp
        Uncategorized/StephenEncryption.cpp
        Uncategorized/Version.cpp
    PUBLIC
        FILE_SET HEADERS FILES
            NewFramework/AdServer/AdInterface.h
            NewFramework/Analytics/DGAnalytics.h
            NewFramework/Analytics/DGAnalyticsData.h
            NewFramework/Analytics/DGAnalyticsEventGroups.h
            NewFramework/BaseApplication/AppClass/AppClass.h
            NewFramework/BaseApplication/AppClass/NKApplication.h
            NewFramework/BaseApplication/AppClass/NKSDKApp.h
            NewFramework/BaseApplication/LegacyFeatures/LegacyFeatures.h
            NewFramework/BaseApplication/Screens/BaseScreen/BaseScreen.h
            NewFramework/BaseApplication/Screens/ScreenData.h
            NewFramework/BaseApplication/BasePointers.h
            NewFramework/Console/ConsoleVariable.h
            NewFramework/Events/EventManager.h
            NewFramework/Events/Observer.h
            NewFramework/Graphics/Font/Font.h
            NewFramework/Graphics/Font/FontManager.h
            NewFramework/Graphics/Management/JPEG/JPEGTextures.h
            NewFramework/Graphics/Management/JPNG/JPNGTextures.h
            NewFramework/Graphics/Management/PVR/PVRTextures.h
            NewFramework/Graphics/Management/TextureLoader.h
            NewFramework/Graphics/Management/TextureManager.h
            NewFramework/Graphics/Objects/BasePositionableObject.h
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
            NewFramework/Networking/NKAPI/Impl/NKFileClientImpl.h
            NewFramework/Networking/NKAPI/Impl/NKMessages.h
            NewFramework/Networking/NKAPI/Impl/NKSessionImpl.h
            NewFramework/Networking/NKAPI/Impl/FileClientActions/NKAction_AppendFileSavePath.h
            NewFramework/Networking/NKAPI/Impl/FileClientActions/NKAction_DownloadFile.h
            NewFramework/Networking/NKAPI/Impl/FileClientActions/NKAction_FileConflict.h
            NewFramework/Networking/NKAPI/Impl/FileClientActions/NKAction_FileWebRequest.h
            NewFramework/Networking/NKAPI/Impl/FileClientActions/NKAction_UploadFile.h
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
            NewFramework/Networking/NKAPI/Modules/NKModule.h
            NewFramework/Networking/NKAPI/Modules/NKModuleUtility.h
            NewFramework/Networking/NKAPI/NKCrossPromo.h
            NewFramework/Networking/NKAPI/NKEndpoints.h
            NewFramework/Networking/NKAPI/NKEnums.h
            NewFramework/Networking/NKAPI/NKError.h
            NewFramework/Networking/NKAPI/NKFileClient.h
            NewFramework/Networking/NKAPI/NKManager.h
            NewFramework/Networking/NKAPI/NKModules.h
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
            NewFramework/ThirdParty/libpng/load_png.h
            NewFramework/Utilities/Bitmask.h
            NewFramework/Utilities/Encoding.h
            NewFramework/Utilities/HashHelper.h
            NewFramework/Utilities/JSON/JSON_PropertyReader/JSON_PropertyException.h
            NewFramework/Utilities/JSON/JSON_PropertyReader/JSON_Read.h
            NewFramework/Utilities/JSON/JSONWrapper.h
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
            Uncategorized/NKRequestFactory.h
            Uncategorized/ProfanityFilter.h
            Uncategorized/ReferenceCounted.h
            Uncategorized/RGBA.h
            Uncategorized/SpriteInformation.h
            Uncategorized/StephenEncryption.h
            Uncategorized/StuffToRefactor.h
            Uncategorized/types.h
            Uncategorized/Version.h
)

## Platform-specific
if("IronSource" IN_LIST AD_SYSTEMS)
    target_compile_definitions(NKFrameworkDecomp PUBLIC USING_IRONSOURCE=1)
    if(ANDROID)
        target_sources(NKFrameworkDecomp PRIVATE NewFramework/AdServer/Android/IronSource/IronSource.cpp)
        target_sources(NKFrameworkDecomp PUBLIC FILE_SET HEADERS FILES NewFramework/AdServer/Android/IronSource/IronSource.h)
    elseif(APPLE)
        target_sources(NKFrameworkDecomp PRIVATE NewFramework/AdServer/iOS/IronSource/IronSource.mm)
        target_sources(NKFrameworkDecomp PUBLIC FILE_SET HEADERS FILES NewFramework/AdServer/iOS/IronSource/IronSource.h)
    endif()
endif()

if("Tapjoy" IN_LIST AD_SYSTEMS)
    target_compile_definitions(NKFrameworkDecomp PUBLIC USING_TAPJOY=1)
    if(ANDROID)
        target_sources(NKFrameworkDecomp PRIVATE NewFramework/AdServer/Android/Tapjoy/Tapjoy.cpp)
        target_sources(NKFrameworkDecomp PUBLIC FILE_SET HEADERS FILES NewFramework/AdServer/Android/Tapjoy/Tapjoy.h)
    elseif(APPLE)
        target_sources(NKFrameworkDecomp PRIVATE NewFramework/AdServer/iOS/Tapjoy/Tapjoy.mm)
        target_sources(NKFrameworkDecomp PUBLIC FILE_SET HEADERS FILES NewFramework/AdServer/iOS/Tapjoy/Tapjoy.h)
    endif()
endif()

if(UNIX)
    target_sources(NKFrameworkDecomp PRIVATE NewFramework/Platform/iOS/File.cpp)
    target_sources(NKFrameworkDecomp PUBLIC FILE_SET HEADERS FILES NewFramework/Platform/iOS/File.h)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_sources(NKFrameworkDecomp PRIVATE NewFramework/Platform/OSX/OSXCore.mm)
endif()

# Libraries

## Boost
### for utmost accuracy, you can have this link to boost 1.63, the version used in the framework.
### because boost is massive, it's not brought in by default, and we just use the system one.
### NK also kind of does this, linking to one global boost install on a development machine.
find_package(Boost REQUIRED)
target_include_directories(NKFrameworkDecomp PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(NKFrameworkDecomp PUBLIC ${Boost_LIBRARIES})

## easyzlib
add_subdirectory(NewFramework/ThirdParty/easyzlib)
target_link_libraries(NKFrameworkDecomp PUBLIC easyzlib)

## hashlibpp
add_subdirectory(NewFramework/ThirdParty/hashlib++)
target_link_libraries(NKFrameworkDecomp PUBLIC hashlibpp)

## libjpeg
add_subdirectory(NewFramework/ThirdParty/libjpeg)
target_include_directories(NKFrameworkDecomp PUBLIC NewFramework/ThirdParty/libjpeg)
target_link_libraries(NKFrameworkDecomp PUBLIC libjpeg)

## libpng
set(PNG_SHARED OFF CACHE BOOL "")
set(PNG_TESTS OFF CACHE BOOL "")

add_subdirectory(NewFramework/ThirdParty/libpng)
target_include_directories(NKFrameworkDecomp PUBLIC NewFramework/ThirdParty/libpng)
target_link_libraries(NKFrameworkDecomp PUBLIC png16_static)

## json_spirit
set(BUILD_STATIC_LIBRARIES ON CACHE BOOL "")
set(JSON_SPIRIT_MVALUE_ENABLED ON CACHE BOOL "")
set(JSON_SPIRIT_DEMOS OFF CACHE BOOL "")
set(JSON_SPIRIT_TESTS OFF CACHE BOOL "")

add_subdirectory(NewFramework/ThirdParty/json_spirit)
target_include_directories(NKFrameworkDecomp PUBLIC NewFramework/ThirdParty/json_spirit)
target_link_libraries(NKFrameworkDecomp PUBLIC json_spirit)

if(JSON_SPIRIT_MVALUE_ENABLED)
    target_compile_definitions(NKFrameworkDecomp PRIVATE JSON_SPIRIT_MVALUE_ENABLED)
endif()

## jsoncpp
set(JSONCPP_WITH_TESTS OFF CACHE BOOL "")
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE BOOL "")
set(JSONCPP_WITH_STRICT_ISO OFF CACHE BOOL "")
set(JSONCPP_WITH_PKGCONFIG_SUPPORT OFF CACHE BOOL "")
set(JSONCPP_WITH_CMAKE_PACKAGE OFF CACHE BOOL "")

add_subdirectory(NewFramework/ThirdParty/jsoncpp)
target_link_libraries(NKFrameworkDecomp PUBLIC jsoncpp_lib_static)

## TinyXML++
add_subdirectory(NewFramework/ThirdParty/TinyXML++)
target_link_libraries(NKFrameworkDecomp PUBLIC ticpp)

## utfcpp
target_include_directories(NKFrameworkDecomp PUBLIC NewFramework/ThirdParty/utfcpp)

# Include dirs
target_include_directories(NKFrameworkDecomp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Installation
include(GNUInstallDirs)
install(
    TARGETS NKFrameworkDecomp
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NKFrameworkDecomp
)
