/*
    This file is a part of SORT(Simple Open Ray Tracing), an open-source cross
    platform physically based renderer.

    Copyright (c) 2011-2020 by Jiayin Cao - All rights reserved.

    SORT is a free software written for educational purpose. Anyone can distribute
    or modify it under the the terms of the GNU General Public License Version 3 as
    published by the Free Software Foundation. However, there is NO warranty that
    all components are functional in a perfect manner. Without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License along with
    this program. If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
 */

#include "accelerator.h"
#include "core/primitive.h"

SORT_STATS_DEFINE_COUNTER(sRayCount)
SORT_STATS_DEFINE_COUNTER(sShadowRayCount)
SORT_STATS_DEFINE_COUNTER(sIntersectionTest)

void Accelerator::computeBBox(){
    // reset bounding box
    m_bbox.InvalidBBox();

    // update bounding box again
    for( auto& primitive : *m_primitives )
        m_bbox.Union( primitive->GetBBox() );

    // enlarge the bounding box a little
    static const auto threshold = 0.001f;
    auto delta = (m_bbox.m_Max - m_bbox.m_Min ) * threshold;
    m_bbox.m_Min -= delta;
    m_bbox.m_Max += delta;
}

#ifdef ENABLE_TRANSPARENT_SHADOW
bool Accelerator::GetAttenuation( Ray& ray , Spectrum& attenuation , MediumStack* ms ) const {
    SurfaceInteraction intersection;
    intersection.query_shadow = true;
    if (!GetIntersect(ray, intersection)) {
        if (ms)
            attenuation *= ms->Tr(ray, ray.m_fMax);
        return false;
    }

    // primitive being null is a special coding meaning the ray is blocked by an opaque primitive.
    if( nullptr == intersection.primitive ){
        attenuation = 0.0f;
        return true;
    }

    const MaterialBase* material = intersection.primitive->GetMaterial();

    sAssert( nullptr != material , SPATIAL_ACCELERATOR );

    attenuation = 1.0f;

    // consider beam transmittance during ray traversal if medium is presented.
    if (ms) {
        attenuation *= ms->Tr(ray, intersection.t);

        const auto theta_wi = dot(ray.m_Dir, intersection.gnormal);
        const auto theta_wo = -theta_wi;
        const auto interaction_flag = update_interaction_flag(theta_wi, theta_wo);

        // at this point, we know for sure the ray pass through the surface.
        MediumInteraction mi;
        mi.intersect = intersection.intersect;
        material->UpdateMediumStack(mi, interaction_flag, *ms);
    }

    ray.m_Ori = intersection.intersect;
    ray.m_fMin = 0.001f;              // avoid self collision again.
    ray.m_fMax -= intersection.t;

    attenuation *= material->EvaluateTransparency(intersection);

    return true;
}
#endif