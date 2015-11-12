##  Add a target to generate Doxygen documentation
OPTION (BUILD_DOCS "Create and install the HTML based API documentation (requires Doxygen)" OFF)
IF (BUILD_DOCS)
  FIND_PACKAGE (Doxygen)

  IF (NOT DOXYGEN_FOUND)
    MESSAGE (FATAL_ERROR "EE\tDoxygen is required in order for the documentation to be built.")
  ENDIF ()

  IF (NOT DOXYGEN_DOT_FOUND)
    MESSAGE (FATAL_ERROR "EE\tDot is also required in order for Doxygen to build documentation.")
  ENDIF ()

  MESSAGE (STATUS "II\tDoxygen version ${DOXYGEN_VERSION} has been found!")

  ##  Variables used here
  SET (DOXYFILE_IN ${MAIN_SRC_PATH}/cmake/Doxyfile.in)
  SET (DOXYFILE_OUT ${MAIN_BINARY_DIR}/Doxyfile)

  ##  Variables inserted using CONFIGURE_FILE
  SET (DOXYFILE_PROJECT_NAME ${PROJECT_NAME})
  SET (DOXYFILE_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
  SET (DOXYFILE_OUTPUT_DIR ${MAIN_BINARY_DIR}/doc/${PROJECT_NAME})
  SET (DOXYFILE_HTML_FILE ${MAIN_BINARY_DIR}/doc/${PROJECT_NAME}/html/index.html)

  CONFIGURE_FILE (${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)
  ADD_CUSTOM_COMMAND (OUTPUT ${DOXYFILE_HTML_FILE}
                      COMMAND ${DOXYGEN_EXECUTABLE} ${MAIN_BINARY_DIR}/Doxyfile
                      WORKING_DIRECTORY ${MAIN_BINARY_DIR}
                      MAIN_DEPENDENCY ${DOXYFILE_IN} ${DOXYFILE_OUT}
                      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
                      COMMENT "Generating API documentation with Doxygen")
  ADD_CUSTOM_TARGET (${TARGET_NAME}-doc ALL DEPENDS ${DOXYFILE_HTML_FILE})

  INSTALL (DIRECTORY ${MAIN_BINARY_PATH}/doc/${PROJECT_NAME} DESTINATION doc OPTIONAL)
ENDIF ()

