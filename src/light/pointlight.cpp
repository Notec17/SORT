/*
   FileName:      pointlight.cpp

   Created Time:  2011-08-04 12:48:32

   Auther:        Cao Jiayin

   Email:         soraytrace@hotmail.com

   Location:      China, Shanghai

   Description:   SORT is short for Simple Open-source Ray Tracing. Anyone could checkout the source code from
                'sourceforge', https://soraytrace.svn.sourceforge.net/svnroot/soraytrace. And anyone is free to
                modify or publish the source code. It's cross platform. You could compile the source code in 
                linux and windows , g++ or visual studio 2008 is required.
*/

// include header
#include "pointlight.h"
#include "geometry/intersection.h"
#include "utility/samplemethod.h"
#include "sampler/sample.h"

IMPLEMENT_CREATOR( PointLight );

// sample ray from light
Spectrum PointLight::sample_l( const Intersection& intersect , const LightSample* ls , Vector& wi , float delta , float* pdf , Visibility& visibility ) const 
{
	Point pos( light2world.matrix.m[3] , light2world.matrix.m[7] , light2world.matrix.m[11] );
	Vector vec = pos - intersect.intersect;
	wi = Normalize( vec );
	if( pdf ) *pdf = 1.0f;

	float len = vec.Length();
	visibility.ray = Ray( pos , -wi , 0 , delta , len - delta );

	return intensity / ( len * len );
}

// sample a ray from light
Spectrum PointLight::sample_l( const LightSample& ls , Ray& r , Vector& n , float* pdf , float* area_pdf ) const
{
	r.m_fMin = 0.0f;
	r.m_fMax = FLT_MAX;
	r.m_Ori = Point( light2world.matrix.m[3] , light2world.matrix.m[7] , light2world.matrix.m[11] );
	r.m_Dir = UniformSampleSphere( ls.u , ls.v ); 
	n = r.m_Dir;

	if( pdf ) *pdf = UniformSpherePdf(r.m_Dir);
	if( area_pdf ) *area_pdf = 1.0f;

	return intensity;
}

// register property
void PointLight::_registerAllProperty()
{
	Light::_registerAllProperty();
	_registerProperty( "pos" , new PosProperty(this) );
}
