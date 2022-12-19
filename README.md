# SSAO

![ssao_render_occlusion_only_64sp_blur_1080p](https://user-images.githubusercontent.com/36515465/208340474-f8525d67-ba9a-407b-81a6-0f0765e1a6ac.png)

An implementation of screen space ambient occlusion (SSAO) with deferred shading on a 3d real-time rendered scene, using C++ and OpenGL, accompanied by an offline ambient occlusion rendering of the same scene with raytraced ambient occlusion (RTAO).

This was my final year project for my bachelors in Computer Science, for Universidade Federal de Minas Gerais (UFMG).

### Screen Space Ambient Occlusion
SSAO is an alternative to raytracing for approximating ambient occlusion effect in real time. The technique implemented here was the one developed by Crytek (2007) using deferred shading and depth testing to determine the occlusion factor per pixel. This technique is widely known and documented, and there are many improvements and alternative algorithms for approximate ambient occlusion that came after.

Here are the final results, after the SSAO implementation:

[SSAO results](https://user-images.githubusercontent.com/36515465/208340200-6cd21b70-06a6-45c2-ab66-b00725c7f57c.webm)

The results are visually pleasing, and can be tweaked by changing certain parameters (kernel size for number of points to sample depth per pixel, radius to sample points from the pixel and a random noise rotation factor for the sampling).

## SSAO vs RTAO

For comparison purposes, I rendered the same scene in real-time with SSAO and an offline version using RTAO, to highlight visual differences between the algorithms. The final RTAO effect rendered (16 occlusion rays per primary ray):

![rtao_render_final_occlusion_only_16sp_1080p](https://user-images.githubusercontent.com/36515465/208340553-700c037a-23d4-4599-93f8-af20485482e2.png)

And here's a side-by-side comparison (radius 1, 16 SSAO samples per pixel and 16 RTAO occlusion rays per primary ray):

![rtao_ssao_compare_1](https://user-images.githubusercontent.com/36515465/208340603-dc2855ad-2c84-481d-a948-13d20d2f3dd5.jpg)
![rtao_ssao_compare_2](https://user-images.githubusercontent.com/36515465/208340606-fa8d9260-716d-49ae-a74c-03cebc039521.jpg)

## Credits

Sponza Atrium model by Frank Meinl, Crytek. License: [CC BY 3.0](https://creativecommons.org/licenses/by/3.0/)
