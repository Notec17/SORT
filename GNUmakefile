define HELP_TEXT

Available commands

    * final
        Build final version.
    * release
        Build release version.
    * debug
        Build debug version.
    * update
        Sync the latest code from Github
    * update_dep
        Update dependency files.
    * clean
    	Clean up all generated file, including the binaries.
    	But dependency will still exist.
    * clean_dep
    	Clean the dependency.
    * help
        Print this help message.
    * about
        Introduction about SORT and myself.

Convenience targets

    * final       
        The final version that has no debugging information. This is the
        best option for practical rendering.
    * release
        The relatively performant build version that provides reasonable 
        information, like statistics and log. However, there is no debug
        information.
    * debug
        Standard debugging version that has everything, which is only for
        debugging purposes.

endef

define DEP_TEXT

Following are dependencies used in SORT renderer

    * Open Shading Language
        OSL is what SORT is currently using as shading language. With the
        support of OSL, SORT can compile shaders during runtime to allow
        artist more flexible support in shader assets.
    * Easy Profiler
        Easy profiler is currently used as main profiling tool. It is more
        of a higher level profiling tool for tasks, instead of low level
        profiling tool.
    * stb_image
        Stb_image is used to read textures from file system. It is already
        included in the source code in the thirdparty folder.
    * Google Test Framework
        SORT uses Google test as its framework for unit testing. This is
        also included in the thirdparty folder in SORT source code.
    * Tiny Exr
        Tiny Exr is a open source library for reading and writting exr files.
        Like the other two libraries, the source code is also included in 
        'thirdpard' folder.

endef

define ABOUT_TEXT

SORT is short for Simple Open-source Ray Tracing. It is a stand along cross platform 
physically based renderer. It is my personal side project that I have been working 
on ever since 2011. Following are features supported in SORT renderer so far,

    * Path Tracing, Bidirectional Path Tracing, Light Tracing, Instant Radiosity, 
      Ambient Occlusion, Whitted Ray Tracing.
    * KD-Tree, BVH, OcTree, Uniform Grid.
    * Point Light, Spot Light, Directional Light, Area Light, HDR Image Based Lighting.
    * Disney BRDF (2015), Lambert, LambertTransmission, Oran Nayar, Microfacet Reflection,
      Microfacet Tranmission, MERL Measured BRDF, Fourier BRDF, AshikhmanShirley BRDF,
      Phong, Coat, Blend, Double-Sided, Ditribution BRDF, DreamWorks' Fabric BRDF.
    * Sursurface Scattering.
    * Hair, Fur.
    * Open Shading Language.
    * Depth of Field.
    * Multi-threaded Rendering.
    * Blender 2.8.

Please visit the following website for more information,

    * http://sort-renderer.com/

My name is Jiayin Cao. I'm currently working at Naughty Dog as a graphics engineer.
Prior to joining Naughty Dog, I worked at Ubisoft, NVIDIA, AMD and Microsoft before.
Following are my contacts, feel free to contact me for any reason,
	
    * Linkedin,   https://www.linkedin.com/in/caojiayin/
    * Twitter,    https://twitter.com/Jiayin_Cao
    * Tech blog,  https://agraphicsguy.wordpress.com/
    * Email,      caojiayin1985@icloud.com

endef

# SORT root directory
SORT_DIR		:=	$(shell pwd -P)

release: .FORCE
	@echo 'building release version.'
	cd $(SORT_DIR); mkdir proj_release; cd proj_release; cmake -DCMAKE_BUILD_TYPE=Release ..;make -j 4

final: .FORCE
	@echo 'building final version.'
	@echo 'Currently not supported.'

debug: .FORCE
	@echo 'building debug version.'
	cd $(SORT_DIR); mkdir proj_debug; cd proj_release; cmake -DCMAKE_BUILD_TYPE=Debug ..;make -j 4

update: .FORCE
	@echo 'Syncing source code from Github'
	git pull

update_dep: .FORCE
	@echo 'Syncing dependencies'
	ifeq ($(OS), Darwin)
		sh ./getdep_mac.sh

clean: .FORCE
	@echo 'Cleaning all generated files'
	cd $(SORT_DIR); rm -rf bin ; rm -rf proj_release ; rm -rf proj_debug

clean_dep: .FORCE
	@echo 'Cleaning all generated files'
	cd $(SORT_DIR); rm -rf dependencies

export HELP_TEXT
help: .FORCE
	@echo "$$HELP_TEXT"

export ABOUT_TEXT
about: .FORCE
	@echo "$$ABOUT_TEXT"

export DEP_TEXT
dep_info: .FORCE
	@echo "$$DEP_TEXT"

.FORCE: