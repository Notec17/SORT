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

#pragma once

#include "core/define.h"
#include "medium.h"

//! @brief  Medium that only absorb lights without any scattering.
/**
 * This is not a physically based medium. There is only absorption considered, the scattering part is totally ignored.
 * However, since it could be useful for rendering glass, it is still an available medium to the renderer.
 */
class AbsorptionMedium : public Medium{
public:
    //! @brief  Constructor.
    //!
    //! @param a            Absorption coefficient.
    AbsorptionMedium( const Spectrum& a , const PhaseFunction* ph ):a(a){
    }

    //! @brief  Evaluation of beam transmittance.
    //!
    //! Beam transmittance is how much percentage of radiance get attenuated during
    //! traveling through the medium. It is a spectrum dependent attenuation.
    //!
    //! @param  ray         The ray, which it uses to evaluate beam transmittance.
    //!                     It is very important to setup the max of the ray to be correct
    //!                     that there is no occluder in between.
    //! @return             The attenuation of each spectrum channel.
    Spectrum Tr( const Ray& ray ) const override;

    //! @brief  Importance sampling a point along the ray in the medium.
    //!
    //! @param ray          The ray we use to take sample.
    //! @param mi           The interaction sampled.
    //! @return             The beam transmittance between the ray origin and the interaction.
    Spectrum Sample( const Ray& ray , MediumInteraction*& mi ) const override;

private:
    const Spectrum          a;
};