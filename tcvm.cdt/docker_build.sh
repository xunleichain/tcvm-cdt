#execute wasm_build.sh first
#run as root
cp -r lib docker/data
cp -r include docker/data
cp build/llvm/build/bin/clang-7 docker/data
cp build/llvm/build/bin/clang++ docker/data
cp build/llvm/build/bin/wasm-ld docker/data
cp build/tools/* docker/data
cp thunder.imports docker/data

echo "build time" > docker/version
date >> docker/version

cd docker
docker build -t tcvm:v1 . 
