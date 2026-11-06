#!/bin/bash

MODE="Release"
while getopts "m:" opt; do
	case $opt in
		m) MODE="$OPTARG" ;;
		*) echo "Usage: $0 [-m MODE]"; exit 1 ;;
	esac
done
shift $((OPTIND-1))

if command -v ./tools/uv &> /dev/null; then
	echo "UV is installed"
else
	echo "UV not installed"
	curl -LsSf https://astral.sh/uv/install.sh | env UV_INSTALL_DIR="./tools" sh
fi

./tools/uv venv --allow-existing
source .venv/bin/activate
./tools/uv pip install conan

# check if there is an existing profile
if [ -z $(conan profile list | grep default) ]; then 
	echo "No default profile for conan"
	conan profile detect
fi

conan install . --profile ~/.conan2/profiles/default -s build_type=$MODE --output-folder=build --build=missing -c tools.system.package_manager:mode=install 
cmake -B build -DCMAKE_TOOLCHAIN_FILE=build/build/$MODE/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=$MODE .
cmake --build build
