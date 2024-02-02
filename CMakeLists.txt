cmake_minimum_required(VERSION 3.5)
project(NKFrameworkDecomp VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost COMPONENTS function preprocessor smart_ptr spirit threads utility)
add_library(${PROJECT_NAME})

set(SOURCE_FILES
    NewFramework/Analytics/DGAnalytics.cpp
    NewFramework/BaseApplication/BasePointers.cpp
    NewFramework/IO/File/BaseFileIO.cpp
    NewFramework/IO/File/FilePolicy.cpp
    NewFramework/Platform/Shared/Assert.cpp
    NewFramework/Platform/Shared/Core.cpp
    NewFramework/Platform/Shared/Logging.cpp
    NewFramework/Utilities/Json/Json_PropertyReader/JSON_Read.cpp
    NewFramework/Utilities/StringHelper.cpp
    Uncategorized/Version.cpp
)

set(HEADER_FILES
    NewFramework/Analytics/DGAnalytics.h
    NewFramework/BaseApplication/AppClass/AppClass.h
    NewFramework/BaseApplication/BasePointers.h
    NewFramework/IO/File/BaseFileIO.h
    NewFramework/IO/File/FilePlatformData.h
    NewFramework/IO/File/FilePolicy.h
    NewFramework/IO/File/IFile.h
    NewFramework/Platform/Shared/Assert.h
    NewFramework/Platform/Shared/Core.h
    NewFramework/Platform/Shared/Logging.h
    NewFramework/Platform/Shared/LoggingComponent.h
    NewFramework/Utilities/Json/Json_PropertyReader/JSON_PropertyException.h
    NewFramework/Utilities/Json/Json_PropertyReader/JSON_Read.h
    NewFramework/Utilities/StringHelper.h
    Uncategorized/Version.h
)

if(UNIX)
    list(APPEND SOURCE_FILES NewFramework/Platform/iOS/File.cpp)
    list(APPEND HEADER_FILES NewFramework/Platform/iOS/File.h)
endif()

if(APPLE)
    list(APPEND SOURCE_FILES NewFramework/Platform/iOS/OSXCore.mm)
endif()

cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0077 NEW)

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

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${jsonspirit_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})

include(GNUInstallDirs)
install(
    TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)
