#!/usr/bin/env bash


FRAGVIEW=$3
SAMPLES_ROOT=$2
echo $FRAGVIEW
echo $SAMPLES_ROOT
cd ..

echo "Samples with OpenGL\n"
./client/fragview --debug -f  samples/glsl/fractal.frag --renderer-opengl
./client/fragview --debug -f  samples/glsl/guassian.frag --texture=fragview.png --renderer-opengl
./client/fragview --debug -C  samples/glsl/sepia.comp --texture=fragview.png --renderer-opengl
./client/fragview --debug -C  samples/glsl/sobelEdgeDetection.comp --texture=fragview.png --renderer-opengl
./client/fragview --debug -f  samples/glsl/wave.frag --renderer-opengl
./client/fragview --debug -C  samples/glsl/sobelEdgeDetection.comp -f samples/glsl/guassian.frag --texture=fragview.png --renderer-opengl

echo "Samples with Vulkan\n"

./client/fragview --debug -f  samples/glsl/fractal.frag --renderer-vulkan