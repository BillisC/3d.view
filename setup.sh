#!/bin/bash
echo "-- THE SYSTEM IS EXPECTED TO HAVE GRAPHICS DRIVERS INSTALLED --"
echo "by BillisCh"

echo "Updating repositories.."
sudo apt update

echo "Installing prerequisites.."
sudo apt install git clang cmake libgl1-mesa-dev libglu1-mesa-dev

echo "Setting up glfw3.."
sudo apt install libwayland-dev libxkbcommon-dev xorg-dev
#git clone https://github.com/glfw/glfw.git libs/glfw/
#cmake -S libs/glfw/ -B libs/glfw/build/

echo "Building project.."
CORES=$(grep -c processor /proc/cpuinfo)
echo "Detected $CORES cores, using them all >:)"
cmake --build . --config Debug -j $CORES

echo "If the script ran successfully you may start the project with:"
echo "                  ./path-to-project/myogl"
echo ""
echo "Finito."
