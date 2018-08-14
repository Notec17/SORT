# SORT

## Introduction
SORT, short for Simple Open-source Ray Tracing, is my personal cross platform ray tracing renderer. It is a standalone ray tracing program, while works well in Blender as a renderer plugin. Simliar to other open source ray tracer, like PBRT, luxrenderer, SORT is also a physically based renderer. However, since it is a solo project that I worked on in my spare time, it is way simpler comparing with its peers.

## Features

Here are the features implemented so far:
  - Integrator. (Whitted ray tracing, direct lighting, path tracing, light tracing, bidirectional path tracing, instant radiosity, ambient occlusion)
  - Spatial acceleration structure. (KD-Tree, BVH, Uniform grid, OcTree)
  - BXDF. (Disney BRDF, Lambert, LambertTransmission, Oran Nayar, MicroFacet Reflection, Microfacet Transmission, MERL, Fourier, AshikhmanShirley, Modified Phong, Coat)
  - Node graph based material system.
  - Camera. (DOF)
  - Multi-thread rendering.
  - Blender plugin.

## Build Status

| Operating System | Build Status |
| ----------------------- | --------------- |
| Windows (Visual Studio) | [![Build status](https://ci.appveyor.com/api/projects/status/6kiio0dak0wc3ics?svg=true)](https://ci.appveyor.com/project/JerryCao1985/sort) |
| Ubuntu & MacOS | [![Build status](https://travis-ci.org/JerryCao1985/SORT.svg?branch=master)](https://travis-ci.org/JerryCao1985/SORT) |

## Images
Following are two examples of images generated by SORT
![Image](https://agraphicsguy.files.wordpress.com/2018/08/cup.png)

![Image](https://lh3.googleusercontent.com/MwyRCmfWzjzmN4w9Fea3rckJousA2eomEZHDKkWK6hxsr-tuT_rFO2pMWFNQKRcMTTPvEe9YB-jakHpmfwVqFit98CngnLdbwmyfNSaFecLRc_m4MlKEwyW5ZCXa2V1_HIbtEhu0j-lLvGawu3j1QGI_qC7p4AEJNjMWk1qcVjdtSzNggUEprUl8mhL3RR5ZaY_5x2UJms2q0YgJKAuG-ZE-arkKIJKguUC9SCtG4cB6hE_OpJ4fcxX73R73QSV5GP3B8UejhNfeNSMm0aPiPUOPXWwTYd-9cxmUOoFN752UYwl8kwWfJIpWufxsVX80a0c9ntwBRH3Z_pa0872TFZTsZ8DsMM67WFM7iKg9v5Vy_s0VU2f1b4ogMEsaco-emsl5kzoz3TGGhvd6qGaKpR--zioXGC2ev-6wKH8cHAbGx_dnl-4H7U0WdomU-S7VFehEUQ1pQD7ggs7bUkE2BWewxiM2K_BYKzgAdSoXrZKQDRzaHBTfXx9v4IHcK7EL1lavxJ8d1JyrI-KajoB5V6esBf0J1GGrVCQCrrGOof3PN3VC6De5yb11LzerbF7gnteFp1SX5bRqtsg_uTlYHE0eTgewBj6j39RSOXA=w1858-h1045-no)
