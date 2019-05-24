/*
   FileName:      scene.cpp

   Created Time:  2011-08-04 12:50:19

   Auther:        Cao Jiayin

   Email:         soraytrace@hotmail.com

   Location:      China, Shanghai

   Description:   SORT is short for Simple Open-source Ray Tracing. Anyone could checkout the source code from
                'sourceforge', https://soraytrace.svn.sourceforge.net/svnroot/soraytrace. And anyone is free to
                modify or publish the source code. It's cross platform. You could compile the source code in 
                linux and windows , g++ or visual studio 2008 is required.
*/

// include the header
#include "scene.h"
#include "geometry/intersection.h"
#include "accel/accelerator.h"
#include "utility/strhelper.h"
#include "utility/path.h"
#include "utility/samplemethod.h"
#include "utility/sassert.h"
#include "managers/matmanager.h"
#include "light/light.h"
#include "shape/shape.h"

// initialize default data
void Scene::_init()
{
	m_pAccelerator = 0;
	m_pLightsDis = 0;
	m_skyLight = 0;
}

// load the scene from script file
bool Scene::LoadScene( const string& str )
{
	// copy the filename
	m_filename = str;

	// load the xml file
	TiXmlDocument doc( str.c_str() );
	doc.LoadFile();

	// if there is error , return false
	if( doc.Error() )
	{
		LOG_ERROR<<doc.ErrorDesc()<<CRASH;
		return false;
	}

	// get the root of xml
	TiXmlNode*	root = doc.RootElement();

	// get the resource path if there is
	string oldpath = GetResourcePath();
	TiXmlElement* element = root->FirstChildElement( "Resource" );
	if( element )
	{
		const char* path = element->Attribute( "path" );
		if( path )	SetResourcePath( path );
	}

	// parse materials
	TiXmlElement* material = root->FirstChildElement( "Material" );
	while( material )
	{
		const char* mat_name = material->Attribute( "value" );
		if( mat_name != 0 )
			MatManager::GetSingleton().ParseMatFile( mat_name );
		material = material->NextSiblingElement( "Material" );
	}

	// parse the triangle mesh
	TiXmlElement* meshNode = root->FirstChildElement( "Model" );
	while( meshNode )
	{
		// get the name of the file
		const char* filename = meshNode->Attribute( "filename" );

		if( filename != 0 )
		{
			// the name of the model
			const char* model_name = meshNode->Attribute( "name" );
			if( model_name == 0 )
			{
				LOG_WARNING<<"There is a mesh \""<<filename<<"\" without a model name, it will be skipped."<<ENDL;
				break;
			}
			if( 0 != GetTriMesh( model_name ) )
			{
				LOG_WARNING<<"A model with name \""<<model_name<<"\" already exists, it will be skipped."<<ENDL;
				break;
			}

			// load the transform matrix
			Transform transform = _parseTransform( meshNode->FirstChildElement( "Transform" ) );

			// load the first mesh
			TriMesh* mesh = new TriMesh(model_name);
			if( mesh->LoadMesh( filename , transform ) )
			{
				// reset the material if neccessary
				TiXmlElement* meshMat = meshNode->FirstChildElement( "Material" );
				if( meshMat )
				{
					meshMat = meshMat->FirstChildElement( "MatSet" );
					do
					{
						const char* set_name = meshMat->Attribute( "name" );
						const char* mat_name = meshMat->Attribute( "mat" );

						if( set_name != 0 && mat_name != 0 )
							mesh->ResetMaterial( set_name , mat_name );

						meshMat = meshMat->NextSiblingElement( "MatSet" );
					}while( meshMat );
				}
				m_meshBuf.push_back( mesh );
			}
			else
				delete mesh;
		}

		// get to the next model
		meshNode = meshNode->NextSiblingElement( "Model" );
	}
	// generate triangle buffer after parsing from file
	_generateTriBuf();
	
	// parse the lights
	TiXmlElement* lightNode = root->FirstChildElement( "Light" );
	while( lightNode )
	{
		const char* type = lightNode->Attribute( "type" );
		if( type != 0 )
		{
			Light* light = CREATE_TYPE( type , Light );
			if( light )
			{
				// setup 
				light->SetupScene( this );

				// load the transform matrix
				light->SetTransform( _parseTransform( lightNode->FirstChildElement( "Transform" ) ) );

				// set the properties
				TiXmlElement* prop = lightNode->FirstChildElement( "Property" );
				while( prop )
				{
					const char* prop_name = prop->Attribute( "name" );
					const char* prop_value = prop->Attribute( "value" );
					if( prop_name != 0 && prop_value != 0 )
						light->SetProperty( prop_name , prop_value );
					prop = prop->NextSiblingElement( "Property" );
				}

				Shape* shape = light->GetShape();
				if( shape )
				{
					shape->SetID( m_triBuf.size() );
					m_triBuf.push_back( shape );
				}

				if( strcmp( type , "skylight" ) == 0 )
					m_skyLight = light;

				m_lights.push_back( light );
			}
		}else
			LOG_WARNING<<"There is no light with the type of \'"<<type<<"\'."<<ENDL;

		// get to the next light
		lightNode = lightNode->NextSiblingElement( "Light" );
	}
	_genLightDistribution();
	
	// get accelerator if there is, if there is no accelerator, intersection test is performed in a brute force way.
	TiXmlElement* accelNode = root->FirstChildElement( "Accel" );
	if( accelNode )
	{
		// set cooresponding type of accelerator
		const char* type = accelNode->Attribute( "type" );
		if( type != 0 )	m_pAccelerator = CREATE_TYPE( type , Accelerator );
	}

	// restore resource path
	SetResourcePath( oldpath );

	return true;
}

// get the intersection between a ray and the scene
bool Scene::GetIntersect( const Ray& r , Intersection* intersect ) const
{
	if( intersect )
		intersect->t = FLT_MAX;

	// brute force intersection test if there is no accelerator
	if( m_pAccelerator == 0 )
		return _bfIntersect( r , intersect );

	return m_pAccelerator->GetIntersect( r , intersect );
}

// get the intersection between a ray and the scene in a brute force way
bool Scene::_bfIntersect( const Ray& r , Intersection* intersect ) const
{
	if( intersect ) intersect->t = FLT_MAX;
	int n = (int)m_triBuf.size();
	for( int k = 0 ; k < n ; k++ )
	{
		bool flag = m_triBuf[k]->GetIntersect( r , intersect );
		if( flag && intersect == 0 )
			return true;
	}

	if( intersect == 0 )
		return false;
	return intersect->t < r.m_fMax && ( intersect->primitive != 0 );
}

// release the memory of the scene
void Scene::Release()
{
	SAFE_DELETE( m_pAccelerator );
	SAFE_DELETE( m_pLightsDis );

	vector<Primitive*>::iterator it = m_triBuf.begin();
	while( it != m_triBuf.end() )
	{
		delete *it;
		it++;
	}
	m_triBuf.clear();

	vector<TriMesh*>::iterator tri_it = m_meshBuf.begin();
	while( tri_it != m_meshBuf.end() )
	{
		delete *tri_it;
		tri_it++;
	}
	m_meshBuf.clear();

	vector<Light*>::iterator light_it = m_lights.begin();
	while( light_it != m_lights.end() )
	{
		delete *light_it;
		light_it++;
	}
	m_lights.clear();
}

// generate triangle buffer
void Scene::_generateTriBuf()
{
	// iterator the mesh
	vector<TriMesh*>::iterator it = m_meshBuf.begin();
	while( it != m_meshBuf.end() )
	{
		(*it)->FillTriBuf( m_triBuf );
		it++;
	}
}

// output log information
void Scene::OutputLog() const
{
	LOG_HEADER("Geometry Information");
	LOG<<"Scene File Name:\t"<<m_filename<<ENDL;
	LOG<<"Triangle Count :\t"<<(unsigned)m_triBuf.size()<<ENDL<<ENDL;

	if( m_pAccelerator )
		m_pAccelerator->OutputLog();
}

// preprocess
void Scene::PreProcess()
{
	// set uniform grid as acceleration structure as default
	if( m_pAccelerator )
	{
		m_pAccelerator->SetPrimitives( &m_triBuf );
		m_pAccelerator->Build();
	}
}

// parse transformation
Transform Scene::_parseTransform( const TiXmlElement* node )
{
	Transform transform;
	if( node )
	{
		node = node->FirstChildElement( "Matrix" );
		while( node )
		{
			const char* trans = node->Attribute( "value" );
			if( trans )	transform = TransformFromStr( trans ) * transform;
			node = node->NextSiblingElement( "Matrix" );
		}
	}
	return transform;
}

// get the bounding box for the scene
const BBox& Scene::GetBBox() const
{
	if( m_pAccelerator != 0 )
		return m_pAccelerator->GetBBox();

	// if there is no bounding box for the scene, generate one
	vector<Primitive*>::const_iterator it = m_triBuf.begin();
	while( it != m_triBuf.end() )
	{
		m_BBox.Union( (*it)->GetBBox() );
		it++;
	}
	return m_BBox;
}

// compute light cdf
void Scene::_genLightDistribution()
{
	unsigned count = m_lights.size();
	if( count == 0 )
		return ;

	float* pdf = new float[count];
	for( unsigned i = 0 ; i < count ; i++ )
		pdf[i] = m_lights[i]->Power().GetIntensity();

	float total_pdf = 0.0f;
	for( unsigned i = 0 ; i < count ; i++ )
		total_pdf += pdf[i];

	for( unsigned i = 0 ; i < count ; i++ )
		m_lights[i]->SetPickPDF( pdf[i] / total_pdf );

	SAFE_DELETE(m_pLightsDis);
	m_pLightsDis = new Distribution1D( pdf , count );
	delete[] pdf;
}

// get sampled light
const Light* Scene::SampleLight( float u , float* pdf ) const
{
	Sort_Assert( u >= 0.0f && u <= 1.0f );
	Sort_Assert( m_pLightsDis != 0 );

	float _pdf;
	int id = m_pLightsDis->SampleDiscrete( u , &_pdf );
	if( id >= 0 && id < (int)m_lights.size() && _pdf != 0.0f )
	{
		if( pdf ) *pdf = _pdf;
		return m_lights[id];
	}
	return 0;
}

// get light sample property
float Scene::LightProperbility( unsigned i ) const
{
	Sort_Assert( m_pLightsDis != 0 );
	return m_pLightsDis->GetProperty( i );
}

// get triangle mesh set with a specific name
TriMesh* Scene::GetTriMesh( const string& name ) const
{
	vector<TriMesh*>::const_iterator it = m_meshBuf.begin();
	while( it != m_meshBuf.end() )
	{
		if( (*it)->m_Name == name )
			return *it;
		it++;
	}
	return 0;
}

// evalute sky
Spectrum Scene::Le( const Ray& ray ) const
{
	if( m_skyLight )
	{
		Spectrum r;
		m_skyLight->Le( ray , 0 , r );
		return r;
	}
	return 0.0f;
}
