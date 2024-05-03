## Setup
**Debian:**

Install the required packages:
```
sudo apt install git clang cmake libgl1-mesa-dev libglu1-mesa-dev
sudo apt install libwayland-dev libxkbcommon-dev xorg-dev
```
Inside of the project's folder run the following to build it:
```
cmake .
cmake --build . --config Debug
```
Finally run the linked executable with:
```
./viewer
```
