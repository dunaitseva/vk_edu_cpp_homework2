#!/bin/bash

BINARY_DIR="build"
CMAKE_OPTIONS=""
while getopts "tp" opt; do
    case $opt in
    t)
      CMAKE_OPTIONS="$CMAKE_OPTIONS -DTEST=ON "
      ;;
    p)
      CMAKE_OPTIONS="$CMAKE_OPTIONS -DPARALLEL=ON "
      ;;
    a*)
      echo "Unexpected flag $opt"
      exit 1
      ;;
    esac
done

echo "Enabled cmake options: $CMAKE_OPTIONS"
rm -rf $BINARY_DIR

cmake $CMAKE_OPTIONS -S . -B $BINARY_DIR
if [ $? -ne 0 ]; then
   echo "Build ends with errors. Binary directory was not created."
   exit 1
else
   echo FAIL
fi

echo "Binary directory was created with name $BINARY_DIR. Build finished success"