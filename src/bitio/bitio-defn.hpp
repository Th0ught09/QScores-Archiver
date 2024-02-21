//  ###########################################################################
//  Copyright 2011-2015, 2024 by Raymond Wan (rwan.work@gmail.com)
//    https://github.com/rwanwork/QScores-Archiver
//
//  This file is part of QScores-Archiver.
//
//  QScores-Archiver is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public License
//  as published by the Free Software Foundation; either version
//  3 of the License, or (at your option) any later version.
//
//  QScores-Archiver is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with QScores-Archiver; if not, see
//  <http://www.gnu.org/licenses/>.
//  ###########################################################################


/*******************************************************************/
/*!
    \file bitio-defn.hpp
    Header file for including all encoding and decoding functions.
*/
/*******************************************************************/

#ifndef BITIO_HPP
#define BITIO_HPP

//!  Default parameter for Golomb and Rice coding that indicates they are not being used; basically used by other classes.
const unsigned int g_DEFAULT_GOLOMB_RICE_PARAM = UINT_MAX;

//!  Multiplication factor to determine Golomb/Rice coding parameter.
const double g_GOLOMB_RICE_CONSTANT = 0.69;

#include "unary.hpp"
#include "binary.hpp"
#include "gamma.hpp"
#include "delta.hpp"
#include "golomb.hpp"
#include "rice.hpp"
#include "bytecode.hpp"

#endif

