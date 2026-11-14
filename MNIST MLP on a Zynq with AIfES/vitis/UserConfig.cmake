# UserConfig.cmake - aifes_mnist

# Sources: main, platform and all AIfES C files (cmsis and avr_pgm are for other targets)
file(GLOB_RECURSE AIFES_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/basic/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/*.c")
list(FILTER AIFES_SOURCES EXCLUDE REGEX "/(cmsis|avr_pgm)/")
set(USER_COMPILE_SOURCES
"main.c"
"platform.c"
)
list(APPEND USER_COMPILE_SOURCES ${AIFES_SOURCES})

# Compiler settings
set(USER_INCLUDE_DIRECTORIES
"${CMAKE_CURRENT_SOURCE_DIR}"
"${CMAKE_CURRENT_SOURCE_DIR}/basic"
"${CMAKE_CURRENT_SOURCE_DIR}/core"
)
set(USER_COMPILE_DEFINITIONS "")
set(USER_COMPILE_OPTIONS " -O2 -Wall -fmessage-length=0")

# Linker settings
set(USER_LINK_LIBRARIES "m")
set(USER_LINK_DIRECTORIES "")
set(USER_LINK_OPTIONS "")
set(USER_LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/lscript.ld")
