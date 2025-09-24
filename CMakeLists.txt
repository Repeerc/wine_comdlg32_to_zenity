
# i686-w64-mingw32-gcc -shared -o comdlg32.dll hook.c d.def -municode -Wl,--enable-stdcall-fixup

cmake_minimum_required(VERSION 4.1)

project(comdlg_hook C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
# set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(${PROJECT_NAME} SHARED hook.c)
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "comdlg32"
    PREFIX ""
    SUFFIX ".dll"
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${CMAKE_CURRENT_SOURCE_DIR}/d.def" )
target_link_options(${PROJECT_NAME} PRIVATE -municode -Wl,--kill-at -Wl,--enable-stdcall-fixup)
target_include_directories(${PROJECT_NAME} PRIVATE /usr/i686-w64-mingw32/include)
target_link_directories(${PROJECT_NAME} PRIVATE /usr/i686-w64-mingw32/lib)


set_target_properties(${PROJECT_NAME} PROPERTIES
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1
)





