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

#include "material_node.h"

//! @brief  Composite SORT material node.
class SORTNodeComposite : public MaterialNode{
public:
    DEFINE_RTTI( SORTNodeComposite , MaterialNode );
    DEFINE_OUTPUT_COLOR_SOCKET( Result , SORTNodeComposite );

    //! @brief  Get the type of the material node.
    //!
    //! @return         Material node type.
    MATERIAL_NODE_PROPERTY_TYPE GetNodeReturnType() const override {
        return MNPT_COLOR;
    }
    
    //! @brief  Serialization interface. Loading data from stream.
    //!
    //! Serialize the material. Loading from an IStreamBase, which could be coming from file, memory or network.
    //!
    //! @param  stream      Input stream for data.
    //! @param  cache       Cache for avoiding creating duplicated node.
    void Serialize( IStreamBase& stream , MaterialNodeCache& cache ) override {
        R.Serialize( stream , cache );
        G.Serialize( stream , cache );
        B.Serialize( stream , cache );
	}

private:
    SORT_MATERIAL_DEFINE_PROP_FLOAT( R );
    SORT_MATERIAL_DEFINE_PROP_FLOAT( G );
    SORT_MATERIAL_DEFINE_PROP_FLOAT( B );
};

//! @brief  Extracting red channel of the source code.
class SORTNodeExtractRed : public MaterialNode{
public:
    DEFINE_RTTI( SORTNodeExtractRed , MaterialNode );
    DEFINE_OUTPUT_FLOAT_SOCKET( Result , SORTNodeExtractRed );

    //! @brief  Get the type of the material node.
    //!
    //! @return         Material node type.
    MATERIAL_NODE_PROPERTY_TYPE GetNodeReturnType() const override {
        return MNPT_FLOAT;
    }
    
    //! @brief  Serialization interface. Loading data from stream.
    //!
    //! Serialize the material. Loading from an IStreamBase, which could be coming from file, memory or network.
    //!
    //! @param  stream      Input stream for data.
    //! @param  cache       Cache for avoiding creating duplicated node.
    void Serialize( IStreamBase& stream , MaterialNodeCache& cache ) override {
        color.Serialize( stream , cache );
	}

private:
    SORT_MATERIAL_DEFINE_PROP_COLOR( color );
};

//! @brief  Extracting green channel of the source color.
class SORTNodeExtractGreen : public MaterialNode{
public:
    DEFINE_RTTI( SORTNodeExtractGreen , MaterialNode );
    DEFINE_OUTPUT_FLOAT_SOCKET( Result , SORTNodeExtractGreen );

    //! @brief  Get the type of the material node.
    //!
    //! @return         Material node type.
    MATERIAL_NODE_PROPERTY_TYPE GetNodeReturnType() const override {
        return MNPT_FLOAT;
    }
    
    //! @brief  Serialization interface. Loading data from stream.
    //!
    //! Serialize the material. Loading from an IStreamBase, which could be coming from file, memory or network.
    //!
    //! @param  stream      Input stream for data.
    //! @param  cache       Cache for avoiding creating duplicated node.
    void Serialize( IStreamBase& stream , MaterialNodeCache& cache ) override {
        color.Serialize( stream , cache );
    }

private:
    SORT_MATERIAL_DEFINE_PROP_COLOR( color );
};

//! @brief  Extracting blue channel of the source color.
class SORTNodeExtractBlue : public MaterialNode{
public:
    DEFINE_RTTI( SORTNodeExtractBlue , MaterialNode );
    DEFINE_OUTPUT_FLOAT_SOCKET( Result , SORTNodeExtractBlue );

    //! @brief  Get the type of the material node.
    //!
    //! @return         Material node type.
    MATERIAL_NODE_PROPERTY_TYPE GetNodeReturnType() const override {
        return MNPT_FLOAT;
    }

    //! @brief  Serialization interface. Loading data from stream.
    //!
    //! Serialize the material. Loading from an IStreamBase, which could be coming from file, memory or network.
    //!
    //! @param  stream      Input stream for data.
    //! @param  cache       Cache for avoiding creating duplicated node.
    void Serialize( IStreamBase& stream , MaterialNodeCache& cache ) override {
        color.Serialize( stream , cache );
	}

private:
    SORT_MATERIAL_DEFINE_PROP_COLOR( color );
};

//! @brief  Extracting intensity of the source spectrum.
class SORTNodeExtractIntensity : public MaterialNode{
public:
    DEFINE_RTTI( SORTNodeExtractIntensity , MaterialNode );
    DEFINE_OUTPUT_FLOAT_SOCKET( Result , SORTNodeExtractIntensity );

    //! @brief  Get the type of the material node.
    //!
    //! @return         Material node type.
    MATERIAL_NODE_PROPERTY_TYPE GetNodeReturnType() const override {
        return MNPT_FLOAT;
    }
    
    //! @brief  Serialization interface. Loading data from stream.
    //!
    //! Serialize the material. Loading from an IStreamBase, which could be coming from file, memory or network.
    //!
    //! @param  stream      Input stream for data.
    //! @param  cache       Cache for avoiding creating duplicated node.
    void Serialize( IStreamBase& stream , MaterialNodeCache& cache ) override {
        color.Serialize( stream , cache );
	}

private:
    SORT_MATERIAL_DEFINE_PROP_COLOR( color );
};
