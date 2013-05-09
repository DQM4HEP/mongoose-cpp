cmake_minimum_required (VERSION 2.6)
project (mongoose)
find_package (Threads)

option (EXAMPLES
    "Compile examples" OFF)

option (WESOCKET
    "Enables websocket" ON)

option (CPP_BINDING
    "Enables C++ binding" ON)

set (SOURCES
    mongoose.c
    )

set (MONGOOSE_CPP "${PROJECT_SOURCE_DIR}/mongoose")

include_directories ("${PROJECT_SOURCE_DIR}")

if (CPP_BINDING)
    set (SOURCES
        ${SOURCES}
        ${MONGOOSE_CPP}/Controller.cpp
        ${MONGOOSE_CPP}/Mutex.cpp
        ${MONGOOSE_CPP}/Request.cpp
        ${MONGOOSE_CPP}/Response.cpp
        ${MONGOOSE_CPP}/Server.cpp
        ${MONGOOSE_CPP}/Session.cpp
        ${MONGOOSE_CPP}/Sessions.cpp
        ${MONGOOSE_CPP}/WebController.cpp
        )

    include_directories ("${MONGOOSE_CPP}")
endif (CPP_BINDING)

if (WEBSOCKET)
    add_definitions("-DUSE_WEBSOCKET")
endif (WEBSOCKET)

# Adding dl
set (EXTRA_LIBS ${EXTRA_LIBS} dl)

# Adding sockets for Win32
if (WIN32)
    set (EXTRA_LIBS ${EXTRA_LIBS} ws2_32)
endif (WIN32)

# Compiling library
add_library (_mongoose ${SOURCES})
target_link_libraries (_mongoose ${EXTRA_LIBS} ${CMAKE_THREAD_LIBS_INIT})

# Compiling executable
add_executable (main main.c)
target_link_libraries (main _mongoose)

# Compiling tests
if (EXAMPLES)
    add_executable (hello examples/hello.c)
    target_link_libraries (hello _mongoose)

    add_executable (post examples/post.c)
    target_link_libraries (post _mongoose)

    add_executable (chat examples/chat.c)
    target_link_libraries (chat _mongoose)
    
    add_executable (websocket examples/websocket.c)
    target_link_libraries (websocket _mongoose)
    
    add_executable (cpp examples/main.cpp)
    target_link_libraries (cpp  _mongoose)
endif (EXAMPLES)