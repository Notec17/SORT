/*
   FileName:      integratormethod.h

   Created Time:  2011-08-04 12:48:59

   Auther:        Cao Jiayin

   Email:         soraytrace@hotmail.com

   Location:      China, Shanghai

   Description:   SORT is short for Simple Open-source Ray Tracing. Anyone could checkout the source code from
                'sourceforge', https://soraytrace.svn.sourceforge.net/svnroot/soraytrace. And anyone is free to
                modify or publish the source code. It's cross platform. You could compile the source code in 
                linux and windows , g++ or visual studio 2008 is required.
*/

#ifndef	SORT_INTEGRATORMETHOD
#define	SORT_INTEGRATORMETHOD

// include the header
#include "integrator.h"
#include "utility/enum.h"

// pre-decleration
class Intersection;
class Light;

// evaluate direct lighting
Spectrum	EvaluateDirect( const Ray& r , const Scene& scene , const Light* light , const Intersection& ip , 
							const LightSample& ls ,	const BsdfSample& bs , BXDF_TYPE type = BXDF_ALL );

// mutilpe importance sampling factors
float		MisFactor( int nf, float fPdf, int ng, float gPdf );

#endif
