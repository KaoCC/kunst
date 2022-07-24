# Kunst: A Collection of Algorithmic Artwork


## Build

### Requirements

- CMake
- Conan

An example: 
- `mkdir build && cd build`
- `conan install .. -s compiler.cppstd=17 --build missing`
- `cmake ..`
- `make` or open the project from Visual Studio depends on what compiler you are using.


## Algorithm 
### QuadTree
Quadtree is a tree data structure in which each node has zero or four children. It can be used to create a special encoding that transforms the original image into colorful grids.

The algorithm builds a Quadtree by recursively dividing the input image into four subimages. For each subimage, the average color and deviation are computed accordingly. A priority queue is utilized along with a traversal algorithm to decide which node should be processed and animated based on the deviation.
