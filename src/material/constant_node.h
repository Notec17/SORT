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

#include "material_node.h"

// constant color node
class ConstantColorNode : public MaterialNode
{
public:
	DEFINE_CREATOR( ConstantColorNode , MaterialNode , "SORTNodeConstant" );

    void GetMaterialProperty( Bsdf* bsdf , Spectrum& result ) override;
    
    MATERIAL_NODE_PROPERTY_TYPE GetNodeReturnType() const override { return MNPT_COLOR; }
    
    //! @brief  Serialization interface. Loading data from stream.
    //!
    //! Serialize the material. Loading from an IStreamBase, which could be coming from file, memory or network.
    //!
    //! @param  stream      Input stream for data.
    void Serialize( IStreamBase& stream ) override {
        src.Serialize( stream );
    }

    //! @brief  Serialization interface. Saving data to stream.
    //!
    //! Serialize the material. Saving to an OStreamBase, which could be file, memory or network streaming.
    //!
    //! @param  stream      Output stream.
    void Serialize( OStreamBase& stream ) override {
		// to be implemented
	}

private:
	SORT_MATERIAL_DEFINE_PROP_COLOR( "Color" , src );
};

// constant float node
class ConstantFloatNode : public MaterialNode
{
public:
    DEFINE_CREATOR( ConstantFloatNode , MaterialNode , "SORTNodeConstantFloat" );
    
    void GetMaterialProperty( Bsdf* bsdf , float& result ) override;
    
    MATERIAL_NODE_PROPERTY_TYPE GetNodeReturnType() const override { return MNPT_FLOAT; }
    
    //! @brief  Serialization interface. Loading data from stream.
    //!
    //! Serialize the material. Loading from an IStreamBase, which could be coming from file, memory or network.
    //!
    //! @param  stream      Input stream for data.
    void Serialize( IStreamBase& stream ) override {
        value.Serialize( stream );
    }

    //! @brief  Serialization interface. Saving data to stream.
    //!
    //! Serialize the material. Saving to an OStreamBase, which could be file, memory or network streaming.
    //!
    //! @param  stream      Output stream.
    void Serialize( OStreamBase& stream ) override {
		// to be implemented
	}

private:
    SORT_MATERIAL_DEFINE_PROP_FLOAT( "Value" , value );
};
