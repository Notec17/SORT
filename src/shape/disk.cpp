/*
   FileName:      disk.cpp

   Created Time:  2011-08-19 22:08:45

   Auther:        Cao Jiayin

   Email:         soraytrace@hotmail.com

   Location:      China, Shanghai

   Description:   SORT is short for Simple Open-source Ray Tracing. Anyone could checkout the source code from
                'sourceforge', https://soraytrace.svn.sourceforge.net/svnroot/soraytrace. And anyone is free to
                modify or publish the source code. It's cross platform. You could compile the source code in 
                linux and windows , g++ or visual studio 2008 is required.
*/

// include header file
#include "disk.h"
#include "utility/samplemethod.h"
#include "sampler/sample.h"
#include "math/vector3.h"
#include "geometry/intersection.h"
#include "utility/rand.h"

IMPLEMENT_CREATOR( Disk );

// sample a point on shape
Point Disk::sample_l( const LightSample& ls , const Point& p , Vector& wi , float* pdf ) const
{
	float u , v;
	UniformSampleDisk( ls.u , ls.v , u , v );

	Point lp = transform( Point( u * radius , 0.0f , v * radius ) );
	Vector normal = transform( Vector( 0 , 1 , 0 ) );
	Vector delta = lp - p;
	wi = Normalize( delta );

	float dot = Dot( -wi , normal );
	if( pdf )
	{
		if( dot <= 0.0f )
			*pdf = 0.0f;
		else
			*pdf = delta.SquaredLength() / ( SurfaceArea() * dot );
	}

	return lp;
}

// sample a ray from light
void Disk::sample_l( const LightSample& ls , Ray& r , Vector& n , float* pdf ) const
{
	float u , v;
	UniformSampleDisk( ls.u , ls.v , u , v );
	r.m_fMin = 0.0f;
	r.m_fMax = FLT_MAX;
	r.m_Ori = transform(Point( u * radius , 0.0f , v * radius ));
	Vector wi = UniformSampleHemisphere(sort_canonical() , sort_canonical());
	r.m_Dir = transform(wi);
	n = transform.invMatrix.Transpose()( Vector( 0.0f , 1.0f , 0.0f ) );

	if( pdf ) *pdf = 1.0f / ( radius * radius * PI * TWO_PI );
}

// the surface area of the shape
float Disk::SurfaceArea() const
{
	return PI * radius * radius;
}

// get intersected point between the ray and the shape
float Disk::_getIntersect( const Ray& ray , Point& p , float limit , Intersection* intersect ) const
{
	if( ray.m_Dir.y == 0.0f )
		return -1.0f;

	float t = -ray.m_Ori.y / ray.m_Dir.y;
	if( t > limit || t <= 0.0f || t > ray.m_fMax )
		return -1.0f;
	p = ray(t);
	float sqLength = p.x * p.x + p.z * p.z;
	if( sqLength > radius * radius )
		return -1.0f;

	if( intersect )
	{
		intersect->t = t;
		intersect->intersect = transform( p );
		intersect->normal = transform.invMatrix.Transpose()(Vector( 0.0f , 1.0f , 0.0f ));
		intersect->tangent = transform(Vector( 0.0f , 0.0f , 1.0f ));
		intersect->primitive = const_cast<Disk*>(this);
	}

	return t;
}

// get the bounding box of the primitive
const BBox&	Disk::GetBBox() const
{
	if( !m_bbox )
	{
		m_bbox = new BBox();
		m_bbox->Union( transform( Point( radius , 0.0f , radius ) ) );
		m_bbox->Union( transform( Point( radius , 0.0f , -radius ) ) );
		m_bbox->Union( transform( Point( -radius , 0.0f , radius ) ) );
		m_bbox->Union( transform( Point( -radius , 0.0f , -radius ) ) );
	}

	return *m_bbox;
}