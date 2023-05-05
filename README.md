## image-compressor
![demo](https://user-images.githubusercontent.com/99078453/236337902-c5affe70-d8d6-4736-b474-995d0fe7bb84.gif)

### Description
Compresses PNG images while preserving the main object’s features. Works best with square images.
This was a programming assignment for UBC CPSC 221, Basic Algorithms and Data Structures.

### Features
- Image is rendered using quadtrees, where each node holds the RGBA values of a single pixel
- Uses lossy image compression algorithm where pixels with less color variance are pruned
- Users can specify their preferred tolerance level, which determines the degree of pruning applied to pixels.

### Installation
1. Clone this repository
2. Run main.cpp (change input images as necessary)
