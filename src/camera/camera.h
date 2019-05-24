/*
   FileName:      camera.h

   Created Time:  2011-08-04 12:51:59

   Auther:        Cao Jiayin

   Email:         soraytrace@hotmail.com

   Location:      China, Shanghai

   Description:   SORT is short for Simple Open-source Ray Tracing. Anyone could checkout the source code from
                'sourceforge', https://soraytrace.svn.sourceforge.net/svnroot/soraytrace. And anyone is free to
                modify or publish the source code. It's cross platform. You could compile the source code in 
                linux and windows , g++ or visual studio 2008 is required.
*/

#ifndef	SORT_CAMERA
#define	SORT_CAMERA

#include "geometry/point.h"
#include "geometry/ray.h"
#include "utility/enum.h"
#include "spectrum/spectrum.h"
#include "utility/propertyset.h"
#include "utility/strhelper.h"
#include "utility/creator.h"

// pre-decleration of render target
class PixelSample;
class ImageSensor;

////////////////////////////////////////////////////////////////////
//	definition of camera
class	Camera : public PropertySet<Camera>
{
// public method
public:
	// default constructor
	Camera(){ _init(); }
	// destructor
	virtual ~Camera(){}

	// generate a ray given a pixel
	virtual Ray	GenerateRay( unsigned pass_id , float x , float y , const PixelSample& ps ) const = 0;

	// set a render target
	void SetImageSensor( ImageSensor* is ) { m_imagesensor = is; }

	// get and set eye point
	const Point& GetEye() const { return m_eye; }
	virtual void SetEye( const Point& eye ) { m_eye = eye; }
	
	// get pass number. By default , all of the cameras have only one pass except stereo vision is enabled in perspective camera.
	virtual unsigned GetPassCount() const { return 1; }
	// get pass filter
	virtual Spectrum GetPassFilter( unsigned id ) const { return 1.0f; }
    
    // get image sensor
    ImageSensor* GetImageSensor(){
        return m_imagesensor;
    }

// protected field
protected:
	// the eye point
	Point	m_eye;
    // the image sensor
    ImageSensor*    m_imagesensor;
	// the type for the camera
	CAMERA_TYPE m_type;
	// the size of the sensor
	float	m_sensorW, m_sensorH;
	// aspect ratio
	float	m_aspectRatioW, m_aspectRatioH;
	// aspect fit
	int		m_aspectFit;
	// camera shift
	float	m_shiftX, m_shiftY;

// private method
	// initialize default data
	void _init() { m_imagesensor = 0; m_type = CT_NONE; m_sensorW = 0; m_sensorH = 0; }
	
	// property handler
	class EyeProperty : public PropertyHandler<Camera>
	{
	public:
		// constructor
		PH_CONSTRUCTOR(EyeProperty,Camera);
		
		// set value
		void SetValue( const string& str )
		{
			Camera* camera = CAST_TARGET(Camera);
			camera->SetEye( PointFromStr(str) );
		}
	};

	// property handler
	class SensorSizeProperty : public PropertyHandler<Camera>
	{
	public:
		// constructor
		PH_CONSTRUCTOR(SensorSizeProperty,Camera);
		
		// set value
		void SetValue( const string& str )
		{
			Camera* camera = CAST_TARGET(Camera);

			Point p = PointFromStr(str);

			camera->m_sensorW = p.x;
			camera->m_sensorH = p.y;
			camera->m_aspectFit = (int)p.z;
		}
	};

	// property handler
	class AspectProperty : public PropertyHandler<Camera>
	{
	public:
		// constructor
		PH_CONSTRUCTOR(AspectProperty,Camera);
		
		// set value
		void SetValue( const string& str )
		{
			Camera* camera = CAST_TARGET(Camera);

			string _str = str;
			string x = NextToken( _str , ' ' );
			string y = NextToken( _str , ' ' );

			camera->m_aspectRatioW = (float)atof( x.c_str() );
			camera->m_aspectRatioH = (float)atof( y.c_str() );
		}
	};

	// property handler
	class ShiftProperty : public PropertyHandler<Camera>
	{
	public:
		// constructor
		PH_CONSTRUCTOR(ShiftProperty,Camera);
		
		// set value
		void SetValue( const string& str )
		{
			Camera* camera = CAST_TARGET(Camera);

			string _str = str;
			string x = NextToken( _str , ' ' );
			string y = NextToken( _str , ' ' );

			camera->m_shiftX = (float)atof( x.c_str() );
			camera->m_shiftY = (float)atof( y.c_str() );
		}
	};
};

#endif
