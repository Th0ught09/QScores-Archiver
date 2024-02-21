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
    \file external-software-local.hpp
    Header file for variables not part of the class and not included by other classes.
*/
/*******************************************************************/

#ifndef EXTERNAL_SOFTWARE_LOCAL_HPP
#define EXTERNAL_SOFTWARE_LOCAL_HPP

//!  Size of the buffers
const unsigned int g_INIT_BUFFER_SIZE = 131072;

//!  Temporary input file
const string g_TEMP_IN_FILENAME = "input.tmp";

//!  Temporary output file
const string g_TEMP_OUT_FILENAME = "output.tmp";

//!  Constant returned from system () indicating success
const unsigned int g_COMMAND_SUCCESS = 0;

//!  String to output for commands that aren't found
const string g_COMMAND_NOT_FOUND_STR = "N/A";

//!  Path to main /bin directory
const string g_PATH_BIN = "/bin";

//!  gzip command
const string g_COMMAND_GZIP = "gzip";

//!  gunzip command
const string g_COMMAND_GUNZIP = "gunzip";

//!  bzip2 command
const string g_COMMAND_BZIP = "bzip2";

//!  bunzip2 command
const string g_COMMAND_BUNZIP = "bunzip2";

//!  Re-Pair command
const string g_COMMAND_REPAIR = "repair";

//!  Des-Pair command
const string g_COMMAND_DESPAIR = "despair";

//!  shuff command
const string g_COMMAND_SHUFF = "shuff";

#endif

