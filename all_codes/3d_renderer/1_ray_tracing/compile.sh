#!/bin/bash

# Check if the script received an input argument
if [ $# -eq 0 ]; then
    echo "No source file provided. Usage: ./compile.sh <source_file.c>"
    exit 1
fi

# File name of the C program
SOURCE_FILE=$1

# Extract the base name without the extension
OUTPUT_FILE="${SOURCE_FILE%.*}"

# Compilation command
gcc $SOURCE_FILE -o $OUTPUT_FILE -framework GLUT -framework OpenGL

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. You can run the program using ./$OUTPUT_FILE"
    ./$OUTPUT_FILE
else
    echo "Compilation failed. Please check the source code for errors."
fi