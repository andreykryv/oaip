add_library(stringlib SHARED
    stringlib_global.h
    stringlib.h
    stringlib.cpp
)

set_target_properties(stringlib PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

target_include_directories(stringlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})