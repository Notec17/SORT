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

#ifndef SORT_BLENDEROUTPUT
#define SORT_BLENDEROUTPUT

#include "imagesensor.h"
#include "texture/rendertarget.h"
#include "managers/smmanager.h"

// generate output
class BlenderImage : public ImageSensor
{
public:
	// store pixel information
	virtual void StorePixel( int x , int y , const Spectrum& color , const RenderTask& rt );

	// finish image tile
	virtual void FinishTile( int tile_x , int tile_y , const RenderTask& rt );

	// pre process
	virtual void PreProcess();

	// post process
	virtual void PostProcess();

private:
	int				m_header_offset;
	int				m_final_update_flag_offset;
	int				m_tilenum_x;
	int				m_tilenum_y;

	SharedMemory	m_sharedMemory;
};

#endif
