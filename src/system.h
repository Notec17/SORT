/*
   FileName:      system.h

   Created Time:  2011-08-04 12:42:00

   Auther:        Cao Jiayin

   Email:         soraytrace@hotmail.com

   Location:      China, Shanghai

   Description:   SORT is short for Simple Open-source Ray Tracing. Anyone could checkout the source code from
                'sourceforge', https://soraytrace.svn.sourceforge.net/svnroot/soraytrace. And anyone is free to
                modify or publish the source code. It's cross platform. You could compile the source code in 
                linux and windows , g++ or visual studio 2008 is required.
*/

#ifndef	SORT_SYSTEM
#define	SORT_SYSTEM

// include the header file
#include "geometry/scene.h"
#include "integrator/integrator.h"
#include "output/imageoutput.h"
#include "output/blenderoutput.h"

// declare classes
class Camera;
class RenderTarget;
class Sampler;
class PixelSample;
class SORTOutput;

/////////////////////////////////////////////////////////////////////
//	definition of the system
class	System
{
// public method
public:
	// default constructor
	System();
	// destructor
	~System();

	// pre-process before rendering
	void PreProcess();
	// render the image
	void Render();
	// output the render target
	void OutputRT();

	// setup system from file
	bool Setup( const char* str );
	
	// load the scene
	bool LoadScene( const string& str );
	// get scene
	const Scene& GetScene() const { return m_Scene; }

	// get elapsed time
	unsigned GetRenderingTime() const;

	// output log information
	void OutputLog() const;

	// uninitialize
	void Uninit();

	// get resource path
	const string& GetResourcePath() const { return m_ResourcePath; }
	// set resource path
	void SetResourcePath( const string& str ) { m_ResourcePath = str; }

//private field:
private:
	// the render target for the system
	RenderTarget*	m_rt;
	// the output methods
	vector<SORTOutput*>	m_outputs;
	// image output
	ImageOutput		image_output;
	// blender output
	BlenderOutput	blender_output;

	// the camera for the system
	Camera*			m_camera;

	unsigned		m_totalTask;
	bool*			m_taskDone;
	char*			m_pProgress;

	// the integrator type
	string			m_integratorType;
	// integrator properties
	struct Property
	{
		string _name;
		string _property;
	};
	vector<Property>	m_integratorProperty;
	// the scene for rendering
	Scene			m_Scene;
	// the sampler
	Sampler*		m_pSampler;
	// sample number per pixel
	unsigned		m_iSamplePerPixel;

	// rendering time
	unsigned		m_uRenderingTime;
	// pre-processing time
	unsigned		m_uPreProcessingTime;

	// path for the resource
	string			m_ResourcePath;
	string			m_OutputFileName;

	// number of thread to allocate
	unsigned		m_thread_num;

	// pre-Initialize
	void	_preInit();
	// post-Uninitialize
	void	_postUninit();
	// output progress
	void	_outputProgress();
	// uninitialize 3rd party library
	void	_uninit3rdParty();
	// do ray tracing in a multithread enviroment
	void	_executeRenderingTasks();
	// output preprocessing information
	void	_outputPreprocess();
	// push rendering task
	void	_pushRenderTask();
	// allocate integrator
	Integrator*	_allocateIntegrator();
};

#endif
