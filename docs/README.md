##### Build
```bash
# For linux
cmake -DBUILD_LINUX=ON -B ./build/ -S .
# For windows (does not work at the moment)
cmake -DBUILD_WINDOWS=ON -B ./build/ -S .

cmake --build build
```