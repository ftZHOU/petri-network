file(GLOB 
  lib_sources
  petri*.cpp)

file(GLOB 
  lib_headers
  petri*.hpp)

add_library(petri
  SHARED
  ${lib_sources})
target_include_directories(petri PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options    (petri PUBLIC -std=c++14 -Wall)

install(TARGETS petri          DESTINATION lib                          )
install(FILES   ${lib_headers} DESTINATION include/${CMAKE_PROJECT_NAME})

