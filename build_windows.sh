#!/bin/bash

# Cross-compile script for Windows exe with SFML
echo "Cross-compiling Who Wants to Be a Millionaire for Windows..."

# Path to Windows SFML
SFML_WIN_PATH=~/sfml-windows/SFML-2.5.1

# Create Windows build directory
mkdir -p windows_build

# Cross-compile using MinGW
x86_64-w64-mingw32-g++ -o windows_build/millionaire.exe main.cpp \
  $(find src -name "*.cpp" 2>/dev/null) \
  -I include \
  -I src \
  -I${SFML_WIN_PATH}/include \
  -L${SFML_WIN_PATH}/lib \
  -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio \
  -static-libgcc -static-libstdc++ \
  -std=c++17

# Check if compilation was successful
if [ $? -ne 0 ]; then
  echo "Compilation failed!"
  exit 1
fi

# Copy SFML DLLs to the build directory
cp ${SFML_WIN_PATH}/bin/*.dll windows_build/

# Copy assets and data
cp -r assets windows_build/
cp -r data windows_build/

# Create a simple readme file
echo "Who Wants to Be a Millionaire Game
----------------------------
1. Make sure all files remain in the same folder
2. Double-click millionaire.exe to start the game
3. Enjoy!
" > windows_build/README.txt

echo "Build complete! Windows executable is in windows_build/millionaire.exe"
echo "Creating zip file for distribution..."

# Create a zip file for easy distribution
cd windows_build
zip -r ../millionaire_windows.zip *
cd ..

echo "Done! Your Windows distribution is ready in millionaire_windows.zip"
