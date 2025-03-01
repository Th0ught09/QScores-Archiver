###########################################################################
##  Copyright 2011-2015, 2024-2025 by Raymond Wan (rwan.work@gmail.com)
##    https://github.com/rwanwork/QScores-Archiver
##
##  This file is part of QScores-Archiver.
##
##  QScores-Archiver is free software; you can redistribute it and/or
##  modify it under the terms of the GNU Lesser General Public License
##  as published by the Free Software Foundation; either version
##  3 of the License, or (at your option) any later version.
##
##  QScores-Archiver is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU Lesser General Public License for more details.
##
##  You should have received a copy of the GNU Lesser General Public
##  License along with QScores-Archiver; if not, see
##  <http://www.gnu.org/licenses/>.
###########################################################################


##  Add a target to generate Doxygen documentation
option (BUILD_DOCS "Create and install the HTML based API documentation (requires Doxygen)" OFF)
if (BUILD_DOCS)
  find_package (Doxygen REQUIRED dot OPTIONAL_COMPONENTS dia)

  message (STATUS "II\tDoxygen version ${DOXYGEN_VERSION} has been found!")

  set (DOXYGEN_GENERATE_HTML          YES)
  set (DOXYGEN_HAVE_DOT               YES)

  ##  Variables used here
  set (DOXYFILE_IN ${MAIN_SRC_PATH}/common/Doxyfile.in)
  set (DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  configure_file (${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

  doxygen_add_docs (${PROJECT_NAME}-doc
                    ALL
                    COMMENT "Generating API documentation with Doxygen"
                    CONFIG_FILE ${DOXYFILE_OUT})

  file (MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/doc/${TARGET_NAME_LIB}/")
endif ()


