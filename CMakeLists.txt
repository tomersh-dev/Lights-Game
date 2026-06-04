cmake_minimum_required (VERSION 3.26)

project (oop2_ex04)
set (MY_AUTHORS "ADD-YOUR-NAMES-HERE")

include (cmake/CompilerSettings.cmake)

add_executable (${CMAKE_PROJECT_NAME})

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address>)
if (NOT MSVC)
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address>)
endif()

add_subdirectory (include)
add_subdirectory (src)
add_subdirectory (resources)

include (cmake/Zip.cmake)

set (SFML_LOCATION "C:/SFML/SFML-3.0.0")
set (SFML_DIR "${SFML_LOCATION}/lib/cmake/SFML")

find_package (SFML 3.0 COMPONENTS Graphics Main REQUIRED)

target_link_libraries (${CMAKE_PROJECT_NAME} SFML::Graphics SFML::Main)

include (cmake/SFML.cmake)

