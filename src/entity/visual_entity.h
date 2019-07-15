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

#include "entity.h"
#include <vector>
#include "geometry/primitive.h"
#include "managers/meshmanager.h"
#include "math/transform.h"

class	Material;

//! @brief Visual entity is the basic visible unit can be seen in SORT.
/**
 * VisualEntity could be a single shape, like sphere, triangle. It could also be a set of triangles,
 * triangle mesh. Basically, visual entity is the most fundamental unit in SORT that is visible.
 */
class VisualEntity : public Entity{
public:
    //! Empty virtual destructor
    ~VisualEntity() {}
};

//! @brief Triangle Mesh entity.
/**
 * MeshEntity is the most common VisualEntity in a ray tracer. It is composited with a set of
 * triangles. Most of the objects in a scene uses this entity.
 */
class MeshEntity : public VisualEntity{
public:
    //! @brief  Fill the scene with triangles.
    //!
    //! @param  scene       The scene to be filled.
    void        FillScene( Scene& scene ) override;

    // ------------------------------------------------------------------------
    // Temporary solution before serialization is done.
    bool LoadMesh( const string& filename , const Transform& transform );
    void ResetMaterial( const string& setname , const string& matname );
	int  _getSubsetID( const string& setname );
	void _copyMaterial();
    // ------------------------------------------------------------------------

public:
    std::shared_ptr<BufferMemory>               m_pMemory;      /**< Memory for the mesh. */
    std::vector<std::shared_ptr<Material>>      m_Materials;    /**< Material referenced in the mesh */
};