cmake_minimum_required (VERSION 3.5)

#set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR wasm)
set(CMAKE_CROSSCOMPILING 1)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

#set(TCVM_CDT_DIR "/home/root1/wasmexplorer-service/tcvm.cdt")
#set(LLVM_ROOT "/home/root1/wasmexplorer-service/tcvm.cdt")

set(TCVM_CDT_DIR $ENV{TCVM_CDT_DIR})
set(TCVM_CDT_BIN $ENV{TCVM_CDT_BIN})

set(CMAKE_C_COMPILER "${TCVM_CDT_BIN}/clang" CACHE PATH "cc" FORCE)
set(CMAKE_CXX_COMPILER "${TCVM_CDT_BIN}/clang++" CACHE PATH "cxx" FORCE)
set(WASM_LINKER "${TCVM_CDT_BIN}/wasm-ld")

project(test)

set(TCVM_C_FLAGS "-cc1 -triple wasm32 -emit-obj -disable-free -disable-llvm-verifier -discard-value-names -mrelocation-model static -mthread-model single -masm-verbose -mconstructor-aliases -ffreestanding -fuse-init-array -target-cpu generic -fvisibility hidden -dwarf-column-info -debugger-tuning=gdb -momit-leaf-frame-pointer -v -ferror-limit 0 -fmessage-length 147 -fno-builtin -fno-threadsafe-statics -fobjc-runtime=gnustep -fno-common -fdiagnostics-show-option -fcolor-diagnostics -vectorize-loops -vectorize-slp -mllvm -use-cfl-aa-in-codegen=both")
set(TCVM_C_FLAGS "${TCVM_C_FLAGS} -O3")
set(TCVM_C_FLAGS "${TCVM_C_FLAGS} -isysroot ${TCVM_CDT_DIR}")
set(TCVM_C_FLAGS "${TCVM_C_FLAGS} -internal-isystem ${TCVM_CDT_DIR}/include")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TCVM_C_FLAGS}")

set(CMAKE_C_COMPILE_OBJECT "<CMAKE_C_COMPILER> <FLAGS> <DEFINES> <INCLUDES> -o <OBJECT> <SOURCE>")

set(TCVM_CXX_FLAGS "${TCVM_CXX_FLAGS} ${TCVM_C_FLAGS}")
set(TCVM_CXX_FLAGS "${TCVM_CXX_FLAGS} -std=c++17 -fdeprecated-macro -fno-rtti")
set(TCVM_CXX_FLAGS "${TCVM_CXX_FLAGS} -DDISABLE_NEW_COUNT -DTEST_HAS_NO_EXCEPTIONS -DTEST_HAS_NO_RTTI")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TCVM_CXX_FLAGS}")
set(CMAKE_CXX_COMPILE_OBJECT "<CMAKE_CXX_COMPILER> <FLAGS> <DEFINES> <INCLUDES> -o <OBJECT> <SOURCE>")

set(TCVM_LINK_FLAGS "--allow-undefined-file=${TCVM_CDT_DIR}/thunder.imports -e thunderchain_main")
set(CMAKE_C_LINK_FLAGS "-zstack-size=16384 --no-threads -error-limit=0")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --stack-first --merge-data-segments --strip-all")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} ${TCVM_LINK_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_C_LINK_FLAGS}")


set(CMAKE_C_LINK_EXECUTABLE "${WASM_LINKER} <LINK_FLAGS> <OBJECTS> <LINK_LIBRARIES> -o <TARGET>")
set(CMAKE_CXX_LINK_EXECUTABLE "${WASM_LINKER} <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")


find_library(LIBC c HINTS "${TCVM_CDT_DIR}/lib")
if(NOT LIBC)
  message(FATAL_ERROR "Library `c` not found in ${TCVM_CDT_DIR}/lib")
endif()

find_library(LIBCXX c++ HINTS "${TCVM_CDT_DIR}/lib")
if(NOT LIBCXX)
  message(FATAL_ERROR "Library `c++` not found in ${TCVM_CDT_DIR}/lib")
endif()

link_libraries(${LIBC})
link_libraries(${LIBCXX})

include_directories(${TCVM_CDT_DIR}/include)
include_directories(${TCVM_CDT_DIR}/include/libcxx)
include_directories(${TCVM_CDT_DIR}/include/libc)
include_directories(${TCVM_CDT_DIR}/include/boost/include)

message(STATUS ${CON_DIR})
aux_source_directory(${CON_DIR} source_list)
message(STATUS ${source_list})

add_executable(output.wasm ${source_list})
add_custom_command(TARGET output.wasm POST_BUILD COMMAND ${TCVM_CDT_BIN}/TCabigen ${source_list} -I ${TCVM_CDT_DIR}/include -I ${TCVM_CDT_DIR}/include/libcxx -I ${TCVM_CDT_DIR}/include/libc -I ${TCVM_CDT_DIR}/include/boost/include)


#add_executable(${prjName}.wasm ${source_list})
#set_target_properties(${prjName}.wasm PROPERTIES LINKER_LANGUAGE C)
