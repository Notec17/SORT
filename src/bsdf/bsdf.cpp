/*
    This file is a part of SORT(Simple Open Ray Tracing), an open-source cross
    platform physically based renderer.

    Copyright (c) 2011-2019 by Jiayin Cao - All rights reserved.

    SORT is a free software written for educational purpose. Anyone can distribute
    or modify it under the the terms of the GNU General Public License Version 3 as
    published by the Free Software Foundation. However, there is NO warranty that
    all components are functional in a perfect manner. Without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License along with
    this program. If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
 */

#include "bsdf.h"
#include "bxdf.h"
#include "math/intersection.h"
#include "sampler/sample.h"
#include "core/sassert.h"

// constructor
Bsdf::Bsdf( const Intersection* _intersect , bool sub_bsdf ) :
    m_SubBSDF(sub_bsdf), intersect( *_intersect ){
    nn = Normalize(intersect.normal);
    btn = Normalize(Cross( nn , intersect.tangent ));
    tn = Normalize(Cross( btn , nn ));
}

// get the number of components in current bsdf
unsigned Bsdf::NumComponents( BXDF_TYPE type ) const{
    auto count = 0u;
    for(auto i = 0u ; i < m_bxdfCount ; i++ ){
        if( m_bxdf[i]->MatchFlag( type ) )
            count++;
    }
    return count;
}

// add a new bxdf
void Bsdf::AddBxdf( const Bxdf* bxdf ){
    if( m_bxdfCount == MAX_BXDF_COUNT || bxdf == 0 || bxdf->GetWeight().IsBlack() ) return;
    m_bxdf[m_bxdfCount++] = bxdf ;
}

// evaluate bxdf
Spectrum Bsdf::f( const Vector& wo , const Vector& wi , BXDF_TYPE type ) const{
    Spectrum r;

    auto swo = worldToLocal( wo );
    auto swi = worldToLocal( wi );

    for( unsigned i = 0 ; i < m_bxdfCount ; i++ ){
        if( m_bxdf[i]->MatchFlag( type ) )
            r += m_bxdf[i]->F( swo , swi ) * m_bxdf[i]->GetWeight();
    }

    return r;
}

// transform vector from world coordinate to shading coordinate
Vector Bsdf::worldToLocal( const Vector& v , bool force ) const{
    if( m_SubBSDF && !force ) return v;
    return Vector( Dot(v,tn) , Dot(v,nn) , Dot(v,btn) );
}

// transform vector from shading coordinate to world coordinate
Vector Bsdf::localToWorld( const Vector& v ) const{
    if( m_SubBSDF ) return v;
    return Vector(  v.x * tn.x + v.y * nn.x + v.z * btn.x ,
                    v.x * tn.y + v.y * nn.y + v.z * btn.y ,
                    v.x * tn.z + v.y * nn.z + v.z * btn.z );
}

// sample a ray from bsdf
Spectrum Bsdf::sample_f( const Vector& wo , Vector& wi , const BsdfSample& bs , float* pdf , BXDF_TYPE type , BXDF_TYPE* bxdf_type ) const{
    auto com_num = NumComponents(type);
    if( com_num == 0 )
    {
        if( pdf ) *pdf = 0.0f;
        if( bxdf_type ) *bxdf_type = BXDF_NONE;
        return 0.0f;
    }
    auto bsdf_id = std::min( (int)(bs.t*(float)com_num) , (int)(com_num-1) );
    const Bxdf* bxdf = nullptr;
    auto count = bsdf_id;
    for(auto i = 0u ; i < m_bxdfCount ; ++i )
        if( m_bxdf[i]->MatchFlag(type) ){
            if( count == 0 ){
                bxdf = m_bxdf[i];
                break;
            }
            count--;
        }

    sAssert( bxdf != 0 , GENERAL );

    // transform the 'wo' from world space to shading coordinate
    auto swo = worldToLocal( wo );

    // sample the direction
    auto t = bxdf->Sample_F( swo , wi , bs , pdf ) * bxdf->GetWeight();

    // if there is no properbility of sampling that direction , just return 0.0f
    if( pdf && *pdf == 0.0f ) return 0.0f;
    if( bxdf_type ) *bxdf_type = bxdf->GetType();

    // setup pdf
    if( pdf  && ( com_num > 1 ) )
    {
        for( unsigned i = 0; i < m_bxdfCount ; ++i )
            if( m_bxdf[i] != bxdf && m_bxdf[i]->MatchFlag( type ) )
                *pdf += m_bxdf[i]->Pdf( wo , wi );
    }
    if( pdf ) *pdf /= com_num;

    for(auto i = 0u ; i < m_bxdfCount ; ++i )
        if( bxdf != m_bxdf[i] && m_bxdf[i]->MatchFlag(type) )
            t += m_bxdf[i]->F(wo,wi) * m_bxdf[i]->GetWeight();

    // transform the direction back
    wi = localToWorld( wi );

    return t;
}

// get the pdf according to sampled direction
float Bsdf::Pdf( const Vector& wo , const Vector& wi , BXDF_TYPE type ) const{
    auto lwo = worldToLocal( wo );
    auto lwi = worldToLocal( wi );

    auto count = 0u;
    float pdf = 0.0f;
    for( unsigned i = 0 ; i < m_bxdfCount ; ++i )
        if( m_bxdf[i]->MatchFlag( type ) ){
            ++count;
            pdf += m_bxdf[i]->Pdf( lwo , lwi );
        }
    if( count )
        pdf /= count;
    return pdf;
}
