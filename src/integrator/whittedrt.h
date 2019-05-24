/*
   FileName:      whittedrt.h

   Created Time:  2011-08-04 12:49:06

   Auther:        Cao Jiayin

   Email:         soraytrace@hotmail.com

   Location:      China, Shanghai

   Description:   SORT is short for Simple Open-source Ray Tracing. Anyone could checkout the source code from
                'sourceforge', https://soraytrace.svn.sourceforge.net/svnroot/soraytrace. And anyone is free to
                modify or publish the source code. It's cross platform. You could compile the source code in 
                linux and windows , g++ or visual studio 2008 is required.
*/

#ifndef	SORT_WHITTEDRT
#define	SORT_WHITTEDRT

// include the header file
#include "integrator.h"

/////////////////////////////////////////////////////////////////////////////
// definition of whittedrt
// note : Whitted ray tracer only takes direct light into consideration,
//		  there are also specular reflection and refraction. While indirect 
//		  light, like color bleeding , is not supported.
class	WhittedRT : public Integrator
{
// public method
public:
	DEFINE_CREATOR( WhittedRT , "whitted" );

	// return the radiance of a specific direction
	// para 'scene' : scene containing geometry data
	// para 'ray'   : ray with specific direction
	// result       : radiance along the ray from the scene<F3>
	virtual Spectrum	Li( const Ray& ray , const PixelSample& ps ) const;

	// output log information
	virtual void OutputLog() const;
};

#endif
