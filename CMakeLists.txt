cmake_minimum_required(VERSION 3.5)
project(NKFrameworkDecomp VERSION 1.0 LANGUAGES C CXX)
add_library(${PROJECT_NAME})

# Source files
set(SOURCE_FILES
    NewFramework/Analytics/DGAnalytics.cpp
    NewFramework/Analytics/DGAnalyticsData.cpp
    NewFramework/Analytics/DGAnalyticsEventGroups.cpp
    NewFramework/BaseApplication/AppClass/AppClass.cpp
    NewFramework/BaseApplication/BasePointers.cpp
    NewFramework/Graphics/Management/JPEG/JPEGTextures.cpp
    NewFramework/Graphics/Management/JPNG/JPNGTextures.cpp
    NewFramework/Graphics/Management/PVR/PVRTextures.cpp
    NewFramework/Graphics/Management/TextureLoader.cpp
    NewFramework/Graphics/Texture.cpp
    NewFramework/IO/File/BaseFileIO.cpp
    NewFramework/IO/File/FilePolicy.cpp
    NewFramework/IO/File/PNGInMemory.cpp
    NewFramework/Math/MathematicTypes/VectorDataStructures.cpp
    NewFramework/Platform/Shared/Assert.cpp
    NewFramework/Platform/Shared/Core.cpp
    NewFramework/Platform/Shared/Logging.cpp
    NewFramework/ThirdParty/libpng/load_png.cpp
    NewFramework/Utilities/Json/Json_PropertyReader/JSON_Read.cpp
    NewFramework/Utilities/StringHelper.cpp
    Uncategorized/Version.cpp
)

set(HEADER_FILES
    NewFramework/Analytics/DGAnalytics.h
    NewFramework/Analytics/DGAnalyticsData.h
    NewFramework/Analytics/DGAnalyticsEventGroups.h
    NewFramework/Analytics/IAnalytics.h
    NewFramework/Analytics/SAnalyticsInitialisation.h
    NewFramework/BaseApplication/AppClass/AppClass.h
    NewFramework/BaseApplication/BasePointers.h
    NewFramework/Graphics/Management/JPEG/JPEGTextures.h
    NewFramework/Graphics/Management/JPNG/JPNGTextures.h
    NewFramework/Graphics/Management/PVR/PVRTextures.h
    NewFramework/Graphics/Management/TextureLoader.h
    NewFramework/Graphics/Texture.h
    NewFramework/IO/File/BaseFileIO.h
    NewFramework/IO/File/FilePlatformData.h
    NewFramework/IO/File/FilePolicy.h
    NewFramework/IO/File/IFile.h
    NewFramework/IO/File/PNGInMemory.h
    NewFramework/Math/MathematicTypes/VectorDataStructures.h
    NewFramework/Platform/Shared/Assert.h
    NewFramework/Platform/Shared/Core.h
    NewFramework/Platform/Shared/Logging.h
    NewFramework/Platform/Shared/LoggingComponent.h
    NewFramework/ThirdParty/libpng/load_png.h
    NewFramework/Utilities/Json/Json_PropertyReader/JSON_PropertyException.h
    NewFramework/Utilities/Json/Json_PropertyReader/JSON_Read.h
    NewFramework/Utilities/StringHelper.h
    Uncategorized/ReferenceCounted.h
    Uncategorized/Version.h
)

## Platform-specific
if(UNIX)
    list(APPEND SOURCE_FILES NewFramework/Platform/iOS/File.cpp)
    list(APPEND HEADER_FILES NewFramework/Platform/iOS/File.h)
endif()

if(APPLE)
    list(APPEND SOURCE_FILES NewFramework/Platform/OSX/OSXCore.mm)
endif()

# Packages
find_package(Boost COMPONENTS function preprocessor smart_ptr spirit threads unordered utility)
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
    target_compile_definitions(${PROJECT_NAME} PRIVATE JSON_SPIRIT_MVALUE_ENABLED)
endif()

# Target files
target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${JPEG_INCLUDE_DIR}
        ${jsonspirit_SOURCE_DIR}
        ${PNG_PNG_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES} ${JPEG_LIBRARY} ${PNG_LIBRARY})

# Installation
include(GNUInstallDirs)
install(
    TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)
