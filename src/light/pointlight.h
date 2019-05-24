/*
    This file is a part of SORT(Simple Open Ray Tracing), an open-source cross
    platform physically based renderer.
 
    Copyright (c) 2011-2016 by Cao Jiayin - All rights reserved.
 
    SORT is a free software written for educational purpose. Anyone can distribute
    or modify it under the the terms of the GNU General Public License Version 3 as
    published by the Free Software Foundation. However, there is NO warranty that
    all components are functional in a perfect manner. Without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.
 
    You should have received a copy of the GNU General Public License along with
    this program. If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
 */

#ifndef	SORT_POINTLIGHT
#define	SORT_POINTLIGHT

// include the header
#include "light.h"
#include "utility/strhelper.h"

//////////////////////////////////////////////////////////////////////
// definition of point light
class PointLight : public Light
{
// public method
public:
	DEFINE_CREATOR( PointLight , "point" );

	// default constructor
	PointLight(){_registerAllProperty();}
	// destructor
	~PointLight(){}

	// sample ray from light
	// para 'intersect' : intersection information
	// para 'wi'		: input vector in world space
	// para 'delta'		: a delta to offset the original point
	// para 'pdf'		: property density function value of the input vector
	// para 'visibility': visibility tester
    virtual Spectrum sample_l( const Intersection& intersect , const LightSample* ls , Vector& dirToLight , float* distance , float* pdfw , float* emissionPdf , float* cosAtLight , Visibility& visibility ) const;

	// sample a ray from light
	// para 'ls'       : light sample
	// para 'r'       : the light vector
	// para 'pdf'      : the properbility density function
	virtual Spectrum sample_l( const LightSample& ls , Ray& r , float* pdfW , float* pdfA , float* cosAtLight ) const;

	// total power of the light
	virtual Spectrum Power() const
	{return 4 * PI * intensity;}

// private field
private:
    // light position
    Point light_pos;
    
	// register property
	void _registerAllProperty();

	class PosProperty : public PropertyHandler<Light>
	{
	public:
		PH_CONSTRUCTOR(PosProperty,Light);

		// set value
		void SetValue( const string& str )
		{
			PointLight* light = CAST_TARGET(PointLight);
			light->light_pos = PointFromStr( str );
			light->light2world.matrix.m[3] = light->light_pos.x;
			light->light2world.matrix.m[7] = light->light_pos.y;
			light->light2world.matrix.m[11] = light->light_pos.z;
			light->light2world.invMatrix.m[3] = -light->light_pos.x;
			light->light2world.invMatrix.m[7] = -light->light_pos.y;
			light->light2world.invMatrix.m[11] = -light->light_pos.z;
		}
	};
};

#endif
