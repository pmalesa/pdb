cmake_minimum_required(VERSION 3.7.2)

project(pdbServer)
set(CMAKE_CXX_STANDARD 14)

# Platform specific variables
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(TARGET_SYSTEM "win64")
    endif()
elseif(UNIX)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(TARGET_SYSTEM "linux64")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(TARGET_SYSTEM "linux32")
    endif()
endif()

message(STATUS "Target system: ${TARGET_SYSTEM}")

# Find the Boost library
find_package(Boost REQUIRED COMPONENTS system log filesystem)
link_directories(${Boost_LIBRARY_DIRS} )

message(STATUS "BOOST LIB DIRS: ${Boost_LIBRARY_DIRS}, BOOST LIBS: ${Boost_LIBRARIES}")

# Threads
find_package(Threads REQUIRED)

# Gainput library
if (UNIX)
    set(GAINPUT_LIBRARIES "${PROJECT_SOURCE_DIR}/dll/${TARGET_SYSTEM}/libgainput.so")
endif()

if (UNIX)
    set(RTAUDIO_LIBRARIES "${PROJECT_SOURCE_DIR}/dll/${TARGET_SYSTEM}/librtaudio.so")
endif()

# Amazon Polly
find_package(AWSSDK REQUIRED COMPONENTS polly)

# Create sources variables
set(PDB_SERVER_MAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp")
set(PDB_SERVER_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/src/Server.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Server.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/Config.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Config.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/systems/input/InputManager.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/systems/input/InputManager.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/systems/audio/AudioManager.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/systems/audio/AudioManager.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/systems/audio/AudioTrack.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/systems/audio/AudioTrack.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/systems/audio/AudioStream.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/systems/audio/AudioStream.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/systems/audio/AudioStreamMp3.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/systems/audio/AudioStreamMp3.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/systems/audio/AudioStreamWav.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/systems/audio/AudioStreamWav.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/systems/voice/VoiceManager.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/systems/voice/VoiceManager.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/apps/App.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/apps/App.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/apps/network/NetworkApp.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/apps/network/NetworkApp.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/apps/audiobook/AudiobookApp.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/apps/audiobook/AudiobookApp.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/apps/audiobook/AudiobookPlayer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/apps/audiobook/AudiobookPlayer.h"

    "${CMAKE_CURRENT_LIST_DIR}/lib/audiofile/AudioFile.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/lib/audiofile/AudioFile.h"
)

add_compile_options(-DBOOST_LOG_DYN_LINK)
set(LINKER_FLAGS)
if(UNIX)
    set(LINKER_FLAGS ${LINKER_FLAGS} "-lX11 -lmpg123 -lboost_log -lboost_log_setup")
endif()

# Add logs directory
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/logs")

### MAIN APP
add_executable(pdbServer "")
target_sources(pdbServer
    PRIVATE
        ${PDB_SERVER_MAIN_FILE}
        ${PDB_SERVER_SOURCES}
)

target_include_directories(pdbServer PUBLIC "src/" "lib/audiofile/" "lib/gainput/include/" "lib/rtaudio/include/" ${Boost_INCLUDE_DIR})
target_link_libraries(pdbServer Threads::Threads ${AWSSDK_LINK_LIBRARIES}
    ${Boost_LIBRARIES} ${GAINPUT_LIBRARIES} ${RTAUDIO_LIBRARIES} ${LINKER_FLAGS})

if(WIN32)
    # Create a post-build DLL copy command
    add_custom_command(TARGET pdbServer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/dll/${TARGET_SYSTEM}/$<CONFIGURATION>/"
        $<TARGET_FILE_DIR:pdbServer>)
endif()

### TESTS APP
option(TESTS "Determines whether to build tests." OFF)
if(TESTS)
    enable_testing()

    set(PDB_SERVER_TESTS_MAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/test/main.cpp")
    set(PDB_SERVER_TESTS_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/test/AudiobookPlayer_test.cpp"
    )

    add_executable(pdbServerTests "")
    target_sources(pdbServerTests
        PRIVATE
            ${PDB_SERVER_TESTS_MAIN_FILE}
            ${PDB_SERVER_TESTS_SOURCES}
            ${PDB_SERVER_SOURCES}
    )

    
    target_include_directories(pdbServer PUBLIC "src/" "lib/audiofile/" "lib/gainput/include/" "lib/rtaudio/include/"
        "lib/catch/" ${Boost_INCLUDE_DIR})
    target_link_libraries(pdbServer Threads::Threads ${AWSSDK_LINK_LIBRARIES}
        ${Boost_LIBRARIES} ${GAINPUT_LIBRARIES} ${RTAUDIO_LIBRARIES} ${LINKER_FLAGS})

    add_test(NAME TestPdbServer COMMAND pdbServerTests)
endif()