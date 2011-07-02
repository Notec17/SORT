/*
 * filename :	bxdf.h
 *
 * programmer :	Cao Jiayin
 */

#ifndef	SORT_BXDF
#define	SORT_BXDF

// include the header
#include "spectrum/spectrum.h"

// pre-declera class
class Vector;

/////////////////////////////////////////////////////////////////////////
// definition of bxdf
class	Bxdf
{
// public method
public:
	// default constructor
	Bxdf(){}
	// destructor
	virtual ~Bxdf(){}

	// evaluate bxdf
	// para 'wo' : out going direction
	// para 'wi' : in direction
	// result    : the portion that comes along 'wo' from 'wi'
	virtual Spectrum f( const Vector& wo , const Vector& wi ) const = 0;

	// sample a direction randomly
	// para 'wo'  : out going direction
	// para 'wi'  : in direction generated randomly
	// para 'pdf' : property density function value of the specific 'wi'
	virtual void Sample_f( const Vector& wo , Vector& wi , float* pdf ) const = 0;

// private field
private:

};

#endif
