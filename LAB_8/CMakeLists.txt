add_library(arrayheap STATIC arrayheap.h arrayheap_inst.cpp)
target_include_directories(arrayheap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(arrayheap PUBLIC cxx_std_17)
