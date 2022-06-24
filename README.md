# ssao
An implementation of screen space ambient occlusion with deferred shading on a 3d rendered scene, using C++ and OpenGL.

This is my in-progress final year project for my bachelors in Computer Science, for Universidade Federal de Minas Gerais (UFMG).

### Screen Space Ambient Occlusion
SSAO is an alternative to raytracing for approximating ambient occlusion effect in real time. The technique implemented here was the one developed by Crytek (2007) using deferred shading and depth testing to determine the occlusion factor per pixel. This technique is widely known and documented, and there are many improvements and alternative algorithms for approximate ambient occlusion that came after.

Here are the current results, after the SSAO implementation (only ambient lighting):

https://user-images.githubusercontent.com/36515465/175450037-7333349d-3b69-4093-a9b8-b052f73043f8.mp4

The results are visually pleasing, and can be tweaked by changing certain parameters (kernel size for number of points to sample depth per pixel, radius to sample points from the pixel and a random noise rotation factor for the sampling).

### Goals
The final version of the project will have the SSAO implementation contributing to a lighting pass with Blinn-Phong shading, with a toggle to turn SSAO on/off for visual comparison.

In the future I also want to use most of the code for this project and repurpose it as a toy renderer for implementing other graphics algorithms and make engines on top of.
