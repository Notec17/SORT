/*
   FileName:      plyloader.cpp

   Created Time:  2011-08-04 12:48:11

   Auther:        Cao Jiayin

   Email:         soraytrace@hotmail.com

   Location:      China, Shanghai

   Description:   SORT is short for Simple Open-source Ray Tracing. Anyone could checkout the source code from
                'sourceforge', https://soraytrace.svn.sourceforge.net/svnroot/soraytrace. And anyone is free to
                modify or publish the source code. It's cross platform. You could compile the source code in 
                linux and windows , g++ or visual studio 2008 is required.
*/

// include the header file
#include "plyloader.h"
#include "managers/meshmanager.h"
#include "thirdparty/ply/ply.h"

static char x_prop[] = "x";
static char y_prop[] = "y";
static char z_prop[] = "z";
static char vertex_index_prop[] = "vertex_indices";

static PlyProperty vert_props[] = 
{
  {x_prop, PLY_FLOAT, PLY_FLOAT, 0, 0, 0, 0, 0},
  {y_prop, PLY_FLOAT, PLY_FLOAT, 4, 0, 0, 0, 0},
  {z_prop, PLY_FLOAT, PLY_FLOAT, 8, 0, 0, 0, 0},
};

static PlyProperty face_props[] = 
{
  {vertex_index_prop, PLY_UINT, PLY_UINT, 4 , 1, 4, PLY_UINT, 0},
};

// load obj from file
bool PlyLoader::LoadMesh( const string& str , BufferMemory* mem )
{
	// some variable that will be used later
	int nelems;
	char **elist;
	int file_type;
	float version;
	int nprops;
	int num_elems;
	char *elem_name;

	// open file for reading
	PlyFile *ply = ply_open_for_reading( str.c_str() , &nelems, &elist, &file_type, &version);
	if( ply == 0 )
		return false;

	// set the memory
	mem->m_filename = str;

	// ply format doesn't support mutiple trunks , so there is only one trunk named 'default'
	Trunk*	trunk = new Trunk("default");
	mem->m_TrunkBuffer.push_back( trunk );

	for ( int i = 0; i < nelems; i++) 
	{
		/* get the description of the first element */
		elem_name = elist[i];
		PlyProperty** properties = ply_get_element_description (ply, elem_name, &num_elems, &nprops);

		if (equal_strings ("vertex", elem_name))
		{
			ply_get_property (ply, elem_name, &vert_props[0]);
			ply_get_property (ply, elem_name, &vert_props[1]);
			ply_get_property (ply, elem_name, &vert_props[2]);

			for ( int j = 0; j < num_elems; j++) 
			{
				Point p;
				ply_get_element(ply, (void *)&p);
				mem->m_PositionBuffer.push_back( p );
			}
		}

		/* if we're on face elements, read them in */
		if (equal_strings ("face", elem_name)) 
		{
			/* set up for getting face elements */
			ply_get_property (ply, elem_name, &face_props[0]);

			/* grab all the face elements */
			for ( int j = 0; j < num_elems; j++)
			{
				PlyIndex index;
				ply_get_element (ply, (void *)&index);

				int curId = 1;
				for( unsigned i = 0 ; i < index.count - 2 ; i++ )
				{
					VertexIndex vid;
					vid.posIndex = index.index[0];
					trunk->m_IndexBuffer.push_back( vid );
					vid.posIndex = index.index[curId];
					trunk->m_IndexBuffer.push_back( vid );
					vid.posIndex = index.index[curId+1];
					trunk->m_IndexBuffer.push_back( vid );

					curId++;
				}
			}
		}

		for( int k = 0 ; k < nprops ; k++ )
		{
			delete[] properties[k]->name;
			delete properties[k];
		}
		delete[] properties;
	}

	for( int i = 0 ; i < nelems ; i++ )
		delete[] elist[i];
	delete[] elist;

	// close ply file
	ply_free_file( ply );

	return true;
}
