/*
    This file is a part of SORT(Simple Open Ray Tracing), an open-source cross
    platform physically based renderer.
 
    Copyright (c) 2011-2019 by Cao Jiayin - All rights reserved.
 
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

#include "imagesensor.h"

// generate output
class RenderTargetImage : public ImageSensor{
public:
    // constructor
    RenderTargetImage( int w , int h ):ImageSensor(w,h){}

	// store pixel information
	void StorePixel( int x , int y , const Spectrum& color , const Render_Task& rt ) override;

	// post process
	void PostProcess() override;
};