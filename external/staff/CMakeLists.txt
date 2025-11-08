file(GLOB_RECURSE STAFF_SOURCES
    src/api.cc
    src/internal.cc
)

file(GLOB_RECURSE STAFF_HEADERS
    include/api.hh
    include/core.hh
    include/internal.hh
)

add_library(staff_lib STATIC ${STAFF_SOURCES} ${STAFF_HEADERS})

target_include_directories(staff_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(staff_lib PUBLIC cxx_std_17)