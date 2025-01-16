# Raytracer - 3D Scene Renderer Configuration Program

## Overview
This project is a 3D scene rendering program that allows users to define and render 3D environments through a configuration file. Users can customize camera settings, shapes, materials, and lighting to create visually compelling scenes.

## Features
- **Camera Configuration**: Set up the camera's aspect ratio, field of view, and position.
- **Renderer Settings**: Configure background colors, anti-aliasing, and recursion depths for advanced visual effects.
- **Materials**: Define material properties like color and reflectivity for realistic rendering.
- **Geometric Shapes**: Build scenes with planes, spheres, cylinders, and more.
- **Lighting**: Add dynamic lighting to enhance realism.
- **Transformations**: Apply translation, rotation, and scaling to objects.

## Compilation Guide
### General Instructions
1. Run the provided script to configure and compile the project:
   ```bash
   ./run_cmake.sh
   ```
2. Execute the program with the desired configuration file:
   ```bash
   ./raytracer configFiles/main.cfg
   ```

### macOS Compilation
To compile on macOS, ensure you have GCC installed via Homebrew and set the following environment variables:
```bash
export CC=/opt/homebrew/Cellar/gcc/13.2.0/bin/gcc-13
export CXX=/opt/homebrew/Cellar/gcc/13.2.0/bin/g++-13
```
Then follow the general instructions.

## Usage
1. Edit the configuration file to define your scene.
2. Run the program as described in the compilation guide.
3. Adjust settings as needed to refine the output.
