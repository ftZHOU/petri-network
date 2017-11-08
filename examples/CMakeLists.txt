include_directories (${CMAKE_SOURCE_DIR}/src)

file(GLOB 
  petri_examples
  example-*.cpp)

foreach(filename ${petri_examples})
  get_filename_component(exampleName ${filename} NAME_WE) 
  add_executable (${exampleName} ${filename}) 
  target_compile_options(${exampleName} PUBLIC -std=c++14 -Wall)
  target_link_libraries(${exampleName} petri)
  install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${exampleName}
    DESTINATION bin
    RENAME ${CMAKE_PROJECT_NAME}-${exampleName})
endforeach(filename)
