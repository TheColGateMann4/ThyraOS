cmake_minimum_required(VERSION 4.2)
project(bootloader LANGUAGES CXX)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.efi")

target_compile_options(${PROJECT_NAME} PRIVATE
    -target x86_64-pc-win32-coff
    -ffreestanding
    -fshort-wchar
    -fno-stack-protector
    -mno-red-zone
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    EFI_FUNCTION_WRAPPER
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	/mnt/c/Libraries/gnu-efi/x64/Release/efilib.lib
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    /mnt/c/Libraries/gnu-efi/inc
    /mnt/c/Libraries/gnu-efi/inc/x86_64
)

target_link_options(${PROJECT_NAME} PRIVATE
    -entry:efi_main
    -subsystem:efi_application
    -nodefaultlib
    -dll
)

set(CMAKE_CXX_LINK_EXECUTABLE
	"lld-link <OBJECTS> <LINK_LIBRARIES> <LINK_FLAGS> <FLAGS> -out:<TARGET>"
)
