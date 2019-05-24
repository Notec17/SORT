/*
   FileName:      area.cpp

   Created Time:  2011-08-18 09:51:52

   Auther:        Cao Jiayin

   Email:         soraytrace@hotmail.com

   Location:      China, Shanghai

   Description:   SORT is short for Simple Open-source Ray Tracing. Anyone could checkout the source code from
                'sourceforge', https://soraytrace.svn.sourceforge.net/svnroot/soraytrace. And anyone is free to
                modify or publish the source code. It's cross platform. You could compile the source code in 
                linux and windows , g++ or visual studio 2008 is required.
*/

// include the header
#include "area.h"
#include "sampler/sample.h"

IMPLEMENT_CREATOR( AreaLight );

// initialize default value
void AreaLight::_init()
{
	_registerAllProperty();

	shape = 0;
	radius = 1.0f;
}

// release data
void AreaLight::_release()
{
}

// sample ray from light
Spectrum AreaLight::sample_l( const Intersection& intersect , const LightSample* ls , Vector& wi , float delta , float* pdf , Visibility& visibility ) const
{
	Sort_Assert( ls != 0 );
	Sort_Assert( shape != 0 );

	Point ps = shape->sample_l( *ls , intersect.intersect , wi , pdf );
	if( pdf && *pdf == 0.0f )
		return 0.0f;

	// get the delta
	Vector dlt = ps - intersect.intersect;
	// setup visibility tester
	visibility.ray = Ray( intersect.intersect , wi , 0 , delta , dlt.Length() - delta );

	return intensity;
}

// the pdf of the direction
float AreaLight::Pdf( const Point& p , const Vector& wi ) const
{
	Sort_Assert(shape!=0);

	return shape->Pdf( p , wi );
}

// total power of the light
Spectrum AreaLight::Power() const
{
	Sort_Assert( shape != 0 );
	return shape->SurfaceArea() * intensity.GetIntensity() * TWO_PI;
}

// register property
void AreaLight::_registerAllProperty()
{
	Light::_registerAllProperty();
	_registerProperty( "pos" , new PosProperty(this) );
	_registerProperty( "dir" , new DirProperty(this) );
	_registerProperty( "shape" , new ShapeProperty(this) );
	_registerProperty( "radius" , new RadiusProperty(this) );
}

// sample light density
Spectrum AreaLight::Le( const Intersection& intersect , const Vector& wo ) const
{
	if( Dot( wo , intersect.normal ) > 0.0f )
		return intensity;
	return 0.0f;
}

// get intersection between the light and the ray
bool AreaLight::Le( const Ray& ray , Intersection* intersect , Spectrum& radiance ) const
{
	Sort_Assert( shape != 0 );

	// get intersect
	bool result = shape->GetIntersect( ray , intersect );

	// transform the intersection result back to world coordinate
	if( result && intersect != 0 )
		radiance = Le( *intersect , -ray.m_Dir );

	return result;
}

// sample a ray from light
Spectrum AreaLight::sample_l( const LightSample& ls , Ray& r , Vector& n , float* pdf , float* area_pdf ) const
{
	Sort_Assert( shape != 0 );
	shape->sample_l( ls , r , n , pdf );

	if( area_pdf )
		*area_pdf = 1.0f / shape->SurfaceArea();

	// to avoid self intersection
	r.m_fMin = 0.01f;

	return intensity;
}
