/*
   FileName:      bidirpath.cpp

   Created Time:  2011-09-03 10:18:18

   Auther:        Cao Jiayin

   Email:         soraytrace@hotmail.com

   Location:      China, Shanghai

   Description:   SORT is short for Simple Open-source Ray Tracing. Anyone could checkout the source code from
                'sourceforge', https://soraytrace.svn.sourceforge.net/svnroot/soraytrace. And anyone is free to
                modify or publish the source code. It's cross platform. You could compile the source code in 
                linux and windows , g++ or visual studio 2008 is required.
*/

#include "sort.h"
#include "bidirpath.h"
#include "geometry/scene.h"
#include "light/light.h"
#include "bsdf/bsdf.h"
#include "integratormethod.h"
#include "camera/camera.h"
#include "imagesensor/imagesensor.h"

IMPLEMENT_CREATOR( BidirPathTracing );

// use power heuristic as default
#define MIS(x) ((x)*(x))	// power heuristic
//#define MIS(x) (x)		// balance heuristic
//#define MIS(x) 1.0f		// naive bidirectional path tracing

// return the radiance of a specific direction
Spectrum BidirPathTracing::Li( const Ray& ray , const PixelSample& ps ) const
{
	// pick a light randomly
	float pdf;
	const Light* light = scene.SampleLight( sort_canonical() , &pdf );
	if( light == 0 || pdf == 0.0f )
		return 0.0f;

	Spectrum li;

	float	light_emission_pdf = 0.0f;
	float	light_pdfa = 0.0f;
	Ray		light_ray;
    float   cosAtLight = 1.0f;
	LightSample light_sample(true);
	Spectrum le = light->sample_l( light_sample , light_ray , &light_emission_pdf , &light_pdfa , &cosAtLight );
	
	//-----------------------------------------------------------------------------------------------------
	// Trace light path from light source
	vector<BDPT_Vertex> light_path;
	Ray wi = light_ray;
	float vc = (light->IsDelta())?0.0f: MIS(cosAtLight / light_emission_pdf);
	float vcm = MIS(light_pdfa / light_emission_pdf);
	Spectrum throughput = le * cosAtLight / (light_emission_pdf * pdf);
	float rr = 1.0f;
	while ((int)light_path.size() < path_per_pixel)
	{
		BDPT_Vertex vert;
		if (false == scene.GetIntersect(wi, &vert.inter))
			break;

		const float distSqr = vert.inter.t * vert.inter.t;
		const float cosIn = AbsDot( wi.m_Dir , vert.inter.normal );
		if( light_path.size() > 0 || ( light_path.size() == 0 && !light->IsInfinite() ) )
			vcm *= MIS( distSqr );
		vcm /= MIS( cosIn );
		vc /= MIS( cosIn );

		rr = 1.0f;
		if (light_path.size() > 4 || throughput.GetIntensity() < 0.01f)
			rr = 0.5f;

		vert.p = vert.inter.intersect;
		vert.n = vert.inter.normal;
		vert.wi = -wi.m_Dir;
		vert.bsdf = vert.inter.primitive->GetMaterial()->GetBsdf(&vert.inter);
		vert.throughput = throughput;
		vert.vcm = vcm;
		vert.vc = vc;
		vert.rr = rr;

		light_path.push_back(vert);

		//-----------------------------------------------------------------------------------------------------
		// Path evaluation: light tracing
		_ConnectCamera( vert , light_path.size() , light );

		// russian roulette
		if (sort_canonical() > rr)
			break;

		float bsdf_pdf;
		Spectrum bsdf_value = vert.bsdf->sample_f(vert.wi, vert.wo, BsdfSample(true), &bsdf_pdf, BXDF_ALL);
		bsdf_pdf *= rr;
		const float cosOut = AbsDot(vert.wo, vert.n);
		throughput *= bsdf_value * ( cosOut / bsdf_pdf );

		if (bsdf_pdf == 0 || throughput.IsBlack())
			break;

		const float rev_bsdf_pdfw = vert.bsdf->Pdf( vert.wo , vert.wi ) * rr;
		vc = MIS(cosOut/bsdf_pdf) * ( MIS(rev_bsdf_pdfw) * vc + vcm ) ;
		vcm = MIS(1.0f/bsdf_pdf);

		wi = Ray(vert.inter.intersect, vert.wo, 0, 0.001f);
	}

	//-----------------------------------------------------------------------------------------------------
	// Trace light path from eye point
	const unsigned lps = light_path.size();
	const unsigned int total_pixel = camera->GetImageSensor()->GetWidth() * camera->GetImageSensor()->GetHeight();
	wi = ray;
	throughput = 1.0f;
	int light_path_len = 0;
	vc = 0.0f;
	vcm = MIS(total_pixel / ray.m_fPDF);
	rr = 1.0f;
	while (light_path_len < path_per_pixel)
	{
		BDPT_Vertex vert;
		if (false == scene.GetIntersect(wi, &vert.inter))
		{
			// the following code needs to be modified
			if (scene.GetSkyLight() == light)
			{
				if( light_path_len )
				{
					float emissionPdf;
					float directPdfA;
					Spectrum _li = light->Le( vert.inter, -wi.m_Dir , &directPdfA , &emissionPdf ) * throughput / light->PickPDF();
					float weight = 1.0f / ( 1.0f + MIS( directPdfA ) * vcm + MIS( emissionPdf ) * vc );
					li += _li * weight;
				}
			}

			if( light_path_len == 0 )
				li += scene.Le( wi );

			break;
		}

		const float distSqr = vert.inter.t * vert.inter.t;
		const float cosIn = AbsDot( wi.m_Dir , vert.inter.normal );
		vcm *= MIS( distSqr );
		vcm /= MIS( cosIn );
		vc /= MIS( cosIn );

		//-----------------------------------------------------------------------------------------------------
		// Path evaluation: it hits a light source
		if (vert.inter.primitive->GetLight() == light)
		{
			if( light_path_len )
			{
				float emissionPdf;
				float directPdfA;
				Spectrum _li = vert.inter.Le(-wi.m_Dir , &directPdfA , &emissionPdf ) * throughput / pdf;
				li += _li / ( 1.0f + MIS( directPdfA ) * vcm + MIS( emissionPdf ) * vc );
			}
			else
				li += vert.inter.Le(-wi.m_Dir) / pdf;
		}
		if( light_tracing_only )
			return li;

		rr = 1.0f;
		if (light_path_len > 4 || throughput.GetIntensity() < 0.01f )
			rr = 0.5f;

		vert.p = vert.inter.intersect;
		vert.n = vert.inter.normal;
		vert.wi = -wi.m_Dir;
		vert.bsdf = vert.inter.primitive->GetMaterial()->GetBsdf(&vert.inter);
		vert.throughput = throughput;
		vert.vc = vc;
		vert.vcm = vcm;
		vert.rr = rr;

		//-----------------------------------------------------------------------------------------------------
		// Path evaluation: connect light sample first
		li += _ConnectLight(vert, light) / pdf;

		//-----------------------------------------------------------------------------------------------------
		// Path evaluation: connect vertices
		for (unsigned j = 0; j < lps; ++j)
			li += _ConnectVertices( light_path[j] , vert , light );

		++light_path_len;

		// russian roulette
		if (sort_canonical() > rr)
			break;

		float bsdf_pdf;
		Spectrum bsdf_value = vert.bsdf->sample_f(vert.wi, vert.wo, BsdfSample(true), &bsdf_pdf, BXDF_ALL);
		bsdf_pdf *= rr;
		const float cosOut = AbsDot(vert.wo, vert.n);
		throughput *= bsdf_value * cosOut / bsdf_pdf;

		if (bsdf_pdf == 0 || throughput.IsBlack())
			break;

		const float rev_bsdf_pdfw = vert.bsdf->Pdf( vert.wo , vert.wi ) * rr;
		vc = MIS( cosOut / bsdf_pdf ) * ( MIS( rev_bsdf_pdfw ) * vc + vcm );
		vcm = MIS( 1.0f / bsdf_pdf );

		wi = Ray(vert.inter.intersect, vert.wo, 0, 0.001f);
	}

	return li;
}

void BidirPathTracing::RequestSample( Sampler* sampler , PixelSample* ps , unsigned ps_num )
{
	Integrator::RequestSample( sampler, ps , ps_num );

    sample_per_pixel = ps_num;
}

// connnect vertices
Spectrum BidirPathTracing::_ConnectVertices( const BDPT_Vertex& p0 , const BDPT_Vertex& p1 , const Light* light ) const
{
	const Vector delta = p0.p - p1.p;
	const float invDistcSqr = 1.0f / delta.SquaredLength();
	const Vector n_delta = delta * sqrt(invDistcSqr);

	const float cosAtP0 = AbsDot( p0.n , n_delta );
	const float cosAtP1 = AbsDot( p1.n , n_delta );
	const Spectrum g = p1.bsdf->f( p1.wi , n_delta ) * p0.bsdf->f( -n_delta, p0.wi ) * ( cosAtP0 * cosAtP1 * invDistcSqr );
	if( g.IsBlack() )
		return 0.0f;

	const float p0_bsdf_pdfw = p0.bsdf->Pdf( p0.wi , -n_delta ) * p0.rr;
	const float p0_bsdf_rev_pdfw = p0.bsdf->Pdf( -n_delta , p0.wi ) * p0.rr;
	const float p1_bsdf_pdfw = p1.bsdf->Pdf( p1.wi , n_delta ) * p1.rr;
	const float p1_bsdf_rev_pdfw = p1.bsdf->Pdf( n_delta , p1.wi ) * p1.rr;

	const float p0_a = p1_bsdf_pdfw * cosAtP0 * invDistcSqr;
	const float p1_a = p0_bsdf_pdfw * cosAtP1 * invDistcSqr;

	const float mis_0 = MIS( p0_a ) * ( p0.vcm + p0.vc * MIS( p0_bsdf_rev_pdfw ) );
	const float mis_1 = MIS( p1_a ) * ( p1.vcm + p1.vc * MIS( p1_bsdf_rev_pdfw ) );

	const float weight = 1.0f / (mis_0 + 1.0f + mis_1);

	const Spectrum li = p0.throughput * p1.throughput * g * weight;
	if( li.IsBlack() )
		return li;

	Visibility visible( scene );
	visible.ray = Ray( p1.p , n_delta  , 0 , 0.001f , delta.Length() - 0.001f );
	if( visible.IsVisible() == false )
		return 0.0f;

	return li;
}

// connect light sample
Spectrum BidirPathTracing::_ConnectLight(const BDPT_Vertex& eye_vertex , const Light* light ) const
{
	// drop the light vertex, take a new sample here
	const LightSample sample(true);
	Vector wi;
	Visibility visibility(scene);
	float directPdfW;
	float emissionPdfW;
	float cosAtLight;
	Spectrum li = light->sample_l(eye_vertex.inter, &sample, wi, 0 , &directPdfW, &emissionPdfW , &cosAtLight , visibility);
	const float cosAtEyeVertex = AbsDot(eye_vertex.n, wi);
	li *= eye_vertex.throughput * eye_vertex.bsdf->f(eye_vertex.wi, wi) * ( cosAtEyeVertex / directPdfW );

	if (li.IsBlack())
		return 0.0f;

	if (visibility.IsVisible() == false)
		return 0.0f;

	const float eye_bsdf_pdfw = eye_vertex.bsdf->Pdf( eye_vertex.wi , wi ) * eye_vertex.rr;
	const float eye_bsdf_rev_pdfw = eye_vertex.bsdf->Pdf( wi , eye_vertex.wi ) * eye_vertex.rr;

	const float mis0 = light->IsDelta()?0.0f:MIS(eye_bsdf_pdfw / directPdfW);
	const float mis1 = MIS( cosAtEyeVertex * emissionPdfW / ( cosAtLight * directPdfW ) ) * ( eye_vertex.vcm + eye_vertex.vc * MIS( eye_bsdf_rev_pdfw ) );

	const float weight = 1.0f / ( mis0 + mis1 + 1.0f );

	return li * weight;
}

// connect camera point
void BidirPathTracing::_ConnectCamera(const BDPT_Vertex& light_vertex, int len , const Light* light ) const
{
	const Vector delta = light_vertex.p - camera->GetEye();
	const float sqrLen = delta.SquaredLength();
	const float invSqrLen = 1.0f / sqrLen;
	const Vector n_delta = delta * sqrt(invSqrLen);

	// make sure it is in front of the eye
	if( Dot( delta , camera->GetForward() ) <= 0.0f )
		return;

	Visibility visible( scene );
	float camera_pdfA;
	const Vector2i coord = camera->GetScreenCoord(light_vertex.inter.intersect, &camera_pdfA , &visible );
	camera_pdfA *= AbsDot( light_vertex.n, n_delta ) * invSqrLen;

	if (coord.x < 0.0f || coord.y < 0.0f ||
		coord.x >= (int)camera->GetImageSensor()->GetWidth() ||
		coord.y >= (int)camera->GetImageSensor()->GetHeight() ||
		camera_pdfA == 0.0f )
		return;

	const Spectrum bsdf_value = light_vertex.bsdf->f( light_vertex.wi , -n_delta );
	if( bsdf_value.IsBlack() )
		return;

	if( visible.IsVisible() == false )
		return;

	const float total_pixel = (float)(camera->GetImageSensor()->GetWidth() * camera->GetImageSensor()->GetHeight());
	Spectrum radiance = light_vertex.throughput * bsdf_value * ( camera_pdfA / (float)( sample_per_pixel * total_pixel ) );

	if( !light_tracing_only )
	{
		const float bsdf_rev_pdfw = light_vertex.bsdf->Pdf( -n_delta , light_vertex.wi ) * light_vertex.rr;
		const float mis0 = ( light_vertex.vcm + light_vertex.vc * MIS( bsdf_rev_pdfw ) ) * MIS( camera_pdfA / total_pixel );
		const float weight = 1.0f / ( 1.0f + mis0 );

		radiance *= weight;
	}

	// update image sensor
	ImageSensor* is = camera->GetImageSensor();
	if (!is)
		return;
	is->UpdatePixel(coord.x , coord.y , radiance);
}