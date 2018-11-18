/*
    This file is a part of SORT(Simple Open Ray Tracing), an open-source cross
    platform physically based renderer.
 
    Copyright (c) 2011-2018 by Cao Jiayin - All rights reserved.
 
    SORT is a free software written for educational purpose. Anyone can distribute
    or modify it under the the terms of the GNU General Public License Version 3 as
    published by the Free Software Foundation. However, there is NO warranty that
    all components are functional in a perfect manner. Without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.
 
    You should have received a copy of the GNU General Public License along with
    this program. If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
 */

#pragma once

#include "rectangle.h"
#include "utility/creator.h"

/////////////////////////////////////////////////////////////////////////////////
//	definition of the square
class	Square : public Rectangle
{
// public method
public:
	DEFINE_CREATOR(Square, Shape, "square");

	// sample a point on shape
	// para 'ls': the light sample
	// para 'p'	: the target point for the light to lit.
	// para 'wi': the sampled vector ( output )
	// para 'pdf': the pdf of the light sample ( output )
	// result   : a sampled point from the light source
    Point sample_l( const LightSample& ls , const Point& p , Vector& wi , Vector& n , float* pdf ) const override;

	// sample a ray from light
	// para 'ls'       : light sample
	// para 'r'       : the light vector
	// para 'pdf'      : the properbility density function
    void sample_l( const LightSample& ls , Ray& r , Vector& n , float* pdf ) const override;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// methods inheriting from Primitive ( for geometry )

	// get the bounding box of the primitive
    const BBox&	GetBBox() const override;

	// the surface area of the shape
    float SurfaceArea() const override;
    
// protected method
protected:
	// get intersected point between the ray and the shape
	float getIntersect( const Ray& ray , Point& p , Intersection* intersect = nullptr ) const override;
};
