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

#include "medium.h"

MediumStack::MediumStack(const MediumInteraction& mi) :m_mediumInteraction(mi) {
}

bool MediumStack::AddMedium(const Medium* medium) {
    // simply return false if there is no space, this should rarely happen unless there is more than 8 volumes overlap.
    if (m_mediumCnt >= MEDIUM_MAX_CNT)
        return false;

    m_mediums[m_mediumCnt++] = medium;
    return true;
}

bool MediumStack::RemoveMedium(const Medium* medium) {
    // if there is no medium in the container, simply return false
    if (0 == m_mediumCnt)
        return false;

    // find the medium to be removed. Unfortunately, due to the lack of detail in data, the medium to be removed could be any
    // medium in the data structure, it is necessary to iterate through everything to find it.
    const Medium* last_medium = m_mediums[m_mediumCnt - 1];
    for (int i = m_mediumCnt - 1; i >= 0; --i) {
        if (m_mediums[i] == medium) {
            m_mediums[i] = last_medium;
            --m_mediumCnt;
            return true;
        }
    }

    return false;
}