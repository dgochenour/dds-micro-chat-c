
cmake_minimum_required(VERSION 3.1)

PROJECT(example C)
set(SOURCE_EXTENSION_C c)

set_property(GLOBAL PROPERTY RULE_MESSAGES OFF)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/objs/${RTIME_TARGET_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/objs/${RTIME_TARGET_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/objs/${RTIME_TARGET_NAME})

set(RTI_LIB_PREFIX rti_me)
set(RTI_LIB_SUFFIX)

if (NOT (DEFINED RTI_SHARED_LIB))
    if (${CMAKE_BUILD_TYPE} MATCHES "[Rr]elease")
        set(RTI_LIB_SUFFIX "z")
        set(RTI_DEBUG_BUILD FALSE)
        add_definitions(-DNDEBUG)
    else()
        set(RTI_LIB_SUFFIX "zd")
        set(RTI_DEBUG_BUILD TRUE)
    endif()
else()
    if(${CMAKE_BUILD_TYPE} MATCHES "[Rr]elease")
        set(RTI_LIB_SUFFIX "")
        set(RTI_DEBUG_BUILD FALSE)
        ADD_DEFINITIONS(-DNDEBUG)
    else()
        set(RTI_LIB_SUFFIX "d")
        set(RTI_DEBUG_BUILD TRUE)
    endif()
endif()

set(IDL_GEN_C
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/chat.${SOURCE_EXTENSION_C}
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/chatPlugin.${SOURCE_EXTENSION_C}
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/chatSupport.${SOURCE_EXTENSION_C}
)
set(IDL_GEN_H
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/chat.h
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/chatPlugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/chatSupport.h
)

set(MICRO_C_LIBS 
    # rti_me_rhsm${RTI_LIB_SUFFIX} 
    # rti_me_whsm${RTI_LIB_SUFFIX} 
    # rti_me_discdpde${RTI_LIB_SUFFIX} 
    # rti_me${RTI_LIB_SUFFIX}
    rti_me${RTI_LIB_SUFFIX}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    $ENV{RTIMEHOME}/include 
)

link_directories($ENV{RTIMEHOME}/lib/${RTIME_TARGET_NAME})

################################################################################
# chat_client
################################################################################
add_executable(chat_client 
    ${CMAKE_CURRENT_SOURCE_DIR}/chat_client.${SOURCE_EXTENSION_C}
    ${IDL_GEN_C}
    ${IDL_GEN_H}
)

target_link_libraries(chat_client ${MICRO_C_LIBS} ${PLATFORM_LIBS})



