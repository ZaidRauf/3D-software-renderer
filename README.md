# 3D Software Renderer
A Software Rasterizer/Renderer written in C++ with SDL2.

![Demo Image](./img/dolphin.png)
*Low poly dolphin I modelled. It is rendered with a metal texture using Phong shading and Phong lighting*

## Demo Video
https://github.com/ZaidRauf/3D-software-renderer/assets/41550770/12fc3f38-ff77-48cd-a6ad-22001231f961

The demo video above show cases the demos included with the 3D renderer. Each one is highlighting a different feature. Please note the demo is recorded at a lower framerate which leads to a choppier video, the application itself runs pretty smoothly.

1. The first demo depicts a metal dolphin, and is meant to showcase the overall 3D rendering, texturing, and lighting ability of the renderer.
2. The second demo is similar to the first except with a simpler mesh.
3. The third demo showcases the perspective correct UV mapping that this render does (But it can also support affine mapping as an artistic choice).
4. The fourth demo showcases the differences between flat shading and phong shading to light a mesh and how this renderer supports both modes.
5. The fifth demo shows the first scene again except it shows off some of the first person camera movement.

## Features
- Uses a software rasterization (No 3D libraries used!) to render 3D objects on to the screen.
- Implements the phong shading and lighting model to light up objects.
- Performs UV mapping to texture objects
- Reads OBJ and TGA files for models and textures respectively.
- Also supports flat shading and full bright objects.
- Uses SDL2 for input and opening a window to display the rendered image.
- Implemented a first person like flying camera to move around the scene
- Can move with W, A, S, D to move forward, strafe left, strafe right, and move back respectively
- Can look by moving the mouse around
- Can alternatively look around with keys Q, E, R, F to look left, look right, look up, and look down respectively
- Press Space to cycle through the available demo scenes
- Clips vertices of the object outside of camera view.
- Performs back face culling to avoid drawing triangles not pointed towards the camera (Toggle with C)
- Uses a Z-Buffer to manage pixel draw priority.

## Motivation
I really wanted to develop a deeper understanding of how the real time 3D graphics pipeline works. Thus I went ahead and worked through several resources that taught me how to build a 3D rasterizer and have gone ahead and built my own. I began by implementing the basic linear algebra functionality eventually projecting vertices on to a 2D buffer. Once that was done I implemented line drawing and triangle filling algorithms to actually draw shapes on the screen, i then added some basic flat shading to add depth to the drawn models. To finish it off I implemented the phong shading and lighting model to render smooth objects.

I also used this as an opportunity to learn some more modern C++ features such as smart pointers and making heavy use of the standard template library when appropriate.

Finally I also learned about the SDL2 library and how it can be used for both input and displaying results as the output.

## To Build
### Linux
Make sure you have installed SDL2. Run `make` in the top level folder and run the code with `./3D_renderer` from the terminal. 

## References I Used
- [Pikuma: 3D Computer Graphics Programming](https://pikuma.com/courses/learn-3d-computer-graphics-programming) by Gustavo Pezzi - An excellent course that goes through teaching the theory and practice of building a software rasterizer
- [Scratchapixel](https://www.scratchapixel.com/index.html) - Several articles about the math and implementations of various Computer Graphics algorithms
- [tinyrenderer](https://github.com/ssloy/tinyrenderer) - Got the idea of using TGA for textures from here, although didn't look at their code to read TGA, wrote that by looking at the spec info myself. Looked at how their renderer worked in comparision to mine as well.
