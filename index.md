SORT, short for Simple Open-source Ray Tracing, is my personal cross platform ray tracing renderer. It is a standalone ray tracing program, while works well in Blender as a renderer plugin. Simliar to other open source ray tracer, like PBRT, luxrenderer, SORT is also a physically based renderer. However, since it is a solo project that I worked on in my spare time, it is way simpler comparing with others.  
For those who are interested in compiling SORT by themselves, please check out [this page](/compilation.md) for further deatail.

## Features

Here are the features implemented so far:
  - Integrator. (Whitted ray tracing, direct lighting, path tracing, light tracing, bidirectional path tracing, instant radiosity, ambient occlusion)
  - Spatial acceleration structure. (KD-Tree, BVH, Uniform grid, OcTree)
  - BXDF. (Disney BRDF, Lambert, LambertTransmission, Oran Nayar, MicroFacet Reflection, Microfacet Transmission, MERL, Fourier, AshikhmanShirley, Modified Phong, Coat, Blend, Double-Sided)
  - Node graph based material system.
  - Camera. (DOF)
  - Multi-thread rendering.
  - Blender plugin.
  - Fur.

## Images
Following are some examples of images generated by SORT
![Image](assets/dinning room.png)
![Image](assets/santa_hat.png)
![Image](assets/cups.png)
![Image](assets/coffee machine.png)

## Materials System in SORT
Featuring a flexible graph node material system, SORT is designed to support a wide variaty of materials. Most of the parameters in its BRDF could be driven by textures, providing quite some freedom in term of its material design. Following is a screenshot of SORT working in Blender
![](assets/material_system.png)

Here are some of the examples rendered by SORT  
<img src="assets/mat_matte.png" width="256" height="256" />
<img src="assets/mat_glass.png" width="256" height="256" />
<img src="assets/mat_mirror.png" width="256" height="256" />
<img src="assets/mat_coated.png" width="256" height="256" />
<img src="assets/mat_plastic.png" width="256" height="256" />
<img src="assets/mat_rough_glass.png" width="256" height="256" />
<img src="assets/mat_anisotropic.png" width="256" height="256" />
<img src="assets/mat_bumpped_metal.png" width="256" height="256" />
<img src="assets/mat_bumpped_metal2.png" width="256" height="256" />
<img src="assets/mat_cloth.png" width="256" height="256" />
<img src="assets/mat_blended.png" width="256" height="256" />
<img src="assets/mat_coated2.png" width="256" height="256" />
<img src="assets/mat_plastic2.png" width="256" height="256" />