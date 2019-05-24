/*
   FileName:      skysphere.cpp

   Created Time:  2011-08-04 12:51:50

   Auther:        Cao Jiayin

   Email:         soraytrace@hotmail.com

   Location:      China, Shanghai

   Description:   SORT is short for Simple Open-source Ray Tracing. Anyone could checkout the source code from
                'sourceforge', https://soraytrace.svn.sourceforge.net/svnroot/soraytrace. And anyone is free to
                modify or publish the source code. It's cross platform. You could compile the source code in 
                linux and windows , g++ or visual studio 2008 is required.
*/

// include the header
#include "skysphere.h"
#include "bsdf/bsdf.h"
#include "geometry/ray.h"
#include "utility/samplemethod.h"
#include "managers/memmanager.h"

IMPLEMENT_CREATOR( SkySphere );

// initialize default value
void SkySphere::_init()
{
	_registerAllProperty();
	distribution = 0;
}
// release
void SkySphere::_release()
{
	SAFE_DELETE(distribution);
}

// evaluate value from sky
Spectrum SkySphere::Evaluate( const Vector& vec ) const
{
	Vector wi = m_transform.invMatrix(vec);

	float theta = SphericalTheta( wi );
	float phi = SphericalPhi( wi );

	float v = theta * INV_PI;
	float u = phi * INV_PI * 0.5f;

	return m_sky.GetColor( u , v );
}

// register property
void SkySphere::_registerAllProperty()
{
	_registerProperty( "image" , new ImageProperty( this ) );
}

// get the average radiance
Spectrum SkySphere::GetAverage() const
{
	return m_sky.GetAverage();
}

// generate 2d distribution
void SkySphere::_generateDistribution2D()
{
	SAFE_DELETE(distribution);

	unsigned nu = m_sky.GetWidth();
	unsigned nv = m_sky.GetHeight();
	Sort_Assert( nu != 0 && nv != 0 );
	float* data = new float[nu*nv];
	for( unsigned i = 0 ; i < nv ; i++ )
	{
		unsigned offset = i * nu;
		float sin_theta = sin( (float)i / (float)nv * PI );

		for( unsigned j = 0 ; j < nu ; j++ )
			data[offset+j] = max( 0.0f , m_sky.GetColor( (int)j , (int)i ).GetIntensity() * sin_theta );
	}

	distribution = new Distribution2D( data , nu , nv );

	delete[] data;
}

// sample direction
Vector SkySphere::sample_v( float u , float v , float* pdf , float* area_pdf ) const
{
	Sort_Assert( distribution != 0 );

	float uv[2] ;
	float apdf = 0.0f;
	distribution->SampleContinuous( u , v , uv , &apdf );
	if( area_pdf ) *area_pdf = apdf;
	if( apdf == 0.0f )
		return Vector();

	float theta = PI * uv[1];
	float phi = TWO_PI * uv[0];

	Vector wi = SphericalVec( theta , phi );
	if( pdf )
	{
		*pdf = apdf;
		float sin_theta = SinTheta( wi );
		if( sin_theta != 0.0f )
			*pdf /= TWO_PI * PI * SinTheta( wi );
		else
			*pdf = 0.0f;
	}

	return m_transform(wi);
}

// get the pdf
float SkySphere::Pdf( const Point& p , const Vector& wi ) const
{
	Vector lwi = m_transform.invMatrix( wi );

	float sin_theta = SinTheta(lwi);
	if( sin_theta == 0.0f ) return 0.0f;
	float u , v;
	float theta = SphericalTheta( lwi );
	float phi = SphericalPhi( lwi );
	u = theta * INV_PI;
	v = phi * INV_TWOPI;
	
	return distribution->Pdf( u , v ) / ( TWO_PI * PI * sin_theta );
}
