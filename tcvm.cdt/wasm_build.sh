#!/bin/bash
#Thunderchain wasm contract build tools


TCVM_CDT_DIR=$(pwd)
WORK_PATH=$(pwd)/build

if [ ! -d "build"  ];then
	mkdir build
fi

cd build
if [ $? != 0 ];then
	echo "cd build error"
	exit 1
fi

EMSDK_PATH=$WORK_PATH/emsdk
WABT_PATH=$WORK_PATH/wabt
LLVM_PATH=$WORK_PATH/llvm
TOOLS_PATH=$WORK_PATH/tools/

echo
echo "=========   Thunderchain Wasm Compiler    ========== "
echo


MEM_MEG=$( free -m | sed -n 2p | tr -s ' ' | cut -d\  -f2 || cut -d' ' -f2 ) 
CPU_SPEED=$( lscpu | grep -m1 "MHz" | tr -s ' ' | cut -d\  -f3 || cut -d' ' -f3 | cut -d'.' -f1 ) 
CPU_CORE=$( lscpu -pCPU | grep -v "#" | wc -l ) 

MEM_GIG=$(( ((MEM_MEG / 1500)) )) 
JOBS=$(( MEM_GIG > CPU_CORE ? CPU_CORE : MEM_GIG ))
#JOBS=8
DISK_INSTALL=$(df -h . | tail -1 | tr -s ' ' | cut -d\  -f1 || cut -d' ' -f1) 
DISK_TOTAL_KB=$(df . | tail -1 | awk '{print $2}') 
DISK_AVAIL_KB=$(df . | tail -1 | awk '{print $4}') 
DISK_TOTAL=$(( DISK_TOTAL_KB / 1048576 )) 
DISK_AVAIL=$(( DISK_AVAIL_KB / 1048576 ))

printf "\\tCPU speed: %sMhz\\n" "${CPU_SPEED}"
printf "\\tCPU cores: %s\\n" "${CPU_CORE}"
printf "\\tPhysical Memory: %s Mgb\\n" "${MEM_MEG}"
printf "\\tDisk install: %s\\n" "${DISK_INSTALL}"
printf "\\tDisk space total: %sG\\n" "${DISK_TOTAL%.*}"
printf "\\tDisk space available: %sG\\n" "${DISK_AVAIL%.*}"
printf "\\tMake jobs: %s\\n" "${JOBS%.*}"
echo

echo "Please confirm"
echo "1.The installation of the following software:" 
echo "    [ git  cmake>3.4.3   make   python2.7.x ]"
echo "2.No other files under the install folder"
echo "3.Disk free space is greater than 5G"
echo

while true; do
read -p "Do you wish to continue building the compile system?[y/n] " yn
	case $yn in
		[Yy]* ) break;;
		[Nn]* ) exit;;
		* ) echo "Please enter yes or no.";;
    esac
done

echo
echo -e "\033[31mInstall LLVM && clang\033[0m"
echo

git clone --depth 1 --single-branch --branch release_70 https://github.com/llvm-mirror/llvm.git
if [ $? != 0 ];then
	cd llvm
	git pull
	cd ..
fi

cd $LLVM_PATH/tools

git clone --depth 1 --single-branch --branch release_70 https://github.com/llvm-mirror/clang.git
if [ $? != 0 ];then
	cd clang
	git pull
    cd ..
fi


git clone --depth 1 --single-branch --branch release_70 https://github.com/llvm-mirror/lld.git
if [ $? != 0 ];then
	cd lld
	git pull
	cd ..
fi

cd ${TCVM_CDT_DIR}/build/llvm
if [ $? != 0 ];then
	exit 1
fi

if [ ! -d "build"  ];then
	mkdir build
fi

cd build
if [ $? != 0 ];then
	echo "cd build error"
	exit 1
fi

cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX=.. -DLLVM_TARGETS_TO_BUILD="WebAssembly" -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD=WebAssembly -DCMAKE_BUILD_TYPE=Release ../
if [ $? != 0 ];then
	exit 1
fi

ln -s ${TCVM_CDT_DIR}/tcabi ${TCVM_CDT_DIR}/build/llvm/tools/clang/examples/tcabi
cp ${TCVM_CDT_DIR}/CMakeLists.txt.tcabi ${TCVM_CDT_DIR}/build/llvm/tools/clang/examples/CMakeLists.txt

make -j${JOBS} install
if [ $? != 0 ];then
	exit 1
fi

cd ${TCVM_CDT_DIR}/build/llvm/build
make TCabigen

echo
echo -e "\033[31mInstall The WebAssembly Binary Toolkit\033[0m"
echo
cd $WORK_PATH

git clone --recursive https://github.com/WebAssembly/wabt.git
if [ $? != 0 ];then
	cd wabt
	git pull
	cd ..
fi

if [ ! -d "$WABT_PATH" ];then
	echo "Error: wabt no founded"
fi

cd $WABT_PATH
if [ $? != 0 ];then
	exit 1
fi

make -j${JOBS} gcc-release
if [ $? != 0 ];then
	exit 1
fi

cd $WABT_PATH
if [ ! -d "$TOOLS_PATH" ];then
	mkdir $WORK_PATH/tools/
fi


cd ${TCVM_CDT_DIR}
if [ ! -d "bin"  ];then
	mkdir bin
fi

cp $WABT_PATH/out/gcc/Release/wat2wasm  $TCVM_CDT_DIR/bin/
cp $WABT_PATH/out/gcc/Release/wasm2wat  $TCVM_CDT_DIR/bin/
cp $LLVM_PATH/bin/clang++               $TCVM_CDT_DIR/bin/
cp $LLVM_PATH/bin/clang                 $TCVM_CDT_DIR/bin/
cp $LLVM_PATH/bin/wasm-ld               $TCVM_CDT_DIR/bin/
cp $LLVM_PATH/build/bin/TCabigen        $TCVM_CDT_DIR/bin/
