/*
   FileName:      main.cpp

   Created Time:  2011-08-04 13:01:09

   Auther:        Cao Jiayin

   Email:         soraytrace@hotmail.com

   Location:      China, Shanghai

   Description:   SORT is short for Simple Open-source Ray Tracing. Anyone could checkout the source code from
                'sourceforge', https://soraytrace.svn.sourceforge.net/svnroot/soraytrace. And anyone is free to
                modify or publish the source code. It's cross platform. You could compile the source code in 
                linux and windows , g++ or visual studio 2008 is required.
*/

// include the header file
#include "sort.h"
#include "system.h"

// the global system
System g_System;

extern bool g_bBlenderMode;

// the main func
#ifdef SORT_IN_WINDOWS
int __cdecl main( int argc , char** argv )
#elif defined(SORT_IN_LINUX) || defined(SORT_IN_MAC)
int main( int argc , char** argv )
#endif
{
	// check if there is file argument
	if( argc < 2 )
	{
		cout<<"Miss file argument."<<endl;
		return 0;
	}

	// enable blender mode if possible
	if (argc > 2)
	{
		if (strcmp(argv[2], "blendermode") == 0)
			g_bBlenderMode = true;
	}

	// setup the system
	if( g_System.Setup( argv[1] ) )
	{
		// do ray tracing
		g_System.Render();

		// output rendering time
		cout<<g_System.GetRenderingTime()/1000.0f<<" second"<<endl;

		// output log information
		g_System.OutputLog();

		// output image
		g_System.OutputRT();
	}

	// unitialize the system
	g_System.Uninit();
	
	return 0;
}
