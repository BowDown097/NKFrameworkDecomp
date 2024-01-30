cmake_minimum_required(VERSION 3.5)
project(NKFrameworkDecomp VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost COMPONENTS function preprocessor smartptr threads utility)
add_library(${PROJECT_NAME})

set(SOURCE_FILES
    NewFramework/Analytics/DGAnalytics.cpp
    NewFramework/BaseApplication/BasePointers.cpp
    NewFramework/IO/File/BaseFileIO.cpp
    NewFramework/IO/File/FilePolicy.cpp
    NewFramework/Platform/Shared/Assert.cpp
    NewFramework/Platform/Shared/Core.cpp
    NewFramework/Platform/Shared/Logging.cpp
    Uncategorized/StringHelper.cpp
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
    Uncategorized/StringHelper.h
    Uncategorized/Version.h
)

if(UNIX)
    list(APPEND SOURCE_FILES NewFramework/Platform/iOS/File.cpp)
    list(APPEND HEADER_FILES NewFramework/Platform/iOS/File.h)
endif()

if(APPLE)
    list(APPEND SOURCE_FILES NewFramework/Platform/iOS/OSXCore.mm)
endif()

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})

include(GNUInstallDirs)
install(
    TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)
