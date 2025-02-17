/*
 * This file is part of the GROMACS molecular simulation package.
 *
 * Copyright 2014- The GROMACS Authors
 * and the project initiators Erik Lindahl, Berk Hess and David van der Spoel.
 * Consult the AUTHORS/COPYING files and https://www.gromacs.org for details.
 *
 * GROMACS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * GROMACS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GROMACS; if not, see
 * https://www.gnu.org/licenses, or write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
 *
 * If you want to redistribute modifications to GROMACS, please
 * consider that scientific software is very special. Version
 * control is crucial - bugs must be traceable. We will be happy to
 * consider code for inclusion in the official distribution, but
 * derived work must not be called official GROMACS. Details are found
 * in the README & COPYING files - if they are missing, get the
 * official version at https://www.gromacs.org.
 *
 * To help us fund GROMACS development, we humbly ask that you cite
 * the research papers on the package. Check out https://www.gromacs.org.
 */
/*! \internal
 * \file
 *
 * \brief This file declares inline utility functionality.
 *
 * \author Mark Abraham <mark.j.abraham@gmail.com>
 *
 * \ingroup module_listed_forces
 */
#ifndef GMX_LISTED_FORCES_UTILITIES_H
#define GMX_LISTED_FORCES_UTILITIES_H

#include "gromacs/topology/ifunc.h"

/*! \brief Return whether this is a listed interaction that actually
 * calculates a potential (not e.g. a connection).
 *
 * \todo This function could go away when idef is not a big bucket of
 * everything. */
static inline bool ftypeIsListedPotential(int ftype)
{
    return ((interaction_function[ftype].flags & IF_BOND) != 0U) && ftype != F_CONNBONDS;
}

/*! \brief Return whether this is an interaction that actually
 * calculates a potential and works on multiple atoms (not e.g. a
 * connection or a position restraint).
 *
 * \todo This function could go away when idef is not a big bucket of
 * everything. */
static inline bool ftype_is_bonded_potential(int ftype)
{
    return ((interaction_function[ftype].flags & IF_BOND) != 0U)
           && !(ftype == F_CONNBONDS || ftype == F_POSRES || ftype == F_FBPOSRES);
}

#endif
