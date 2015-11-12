##  Set the root path for Boost from the environment variable
IF (NOT Boost_FOUND)
  # SET (Boost_DEBUG OFF)

  SET (BOOST_ROOT $ENV{BOOST_ROOT})
  IF (NOT BOOST_ROOT)
    MESSAGE (FATAL_ERROR "EE\tThe environment variable BOOST_ROOT has to be set.")
  ENDIF ()

  SET (Boost_NO_SYSTEM_PATHS true)
  SET (Boost_USE_STATIC_LIBS   OFF)
  SET (Boost_USE_MULTITHREADED ON)
  SET (Boost_USE_STATIC_RUNTIME OFF)

  SET (BOOST_COMPILED_LIBRARIES program_options system filesystem serialization mpi)
  FIND_PACKAGE (Boost 1.59.0 REQUIRED COMPONENTS ${BOOST_COMPILED_LIBRARIES})

  ##  Make Boost_FOUND a cached internal variable so that we do not keep searching for Boost during one CMake run
  ##  Does *not* work; will try to figure it out some other day!
#   SET (Boost_FOUND ${Boost_FOUND} CACHE INTERNAL "Boost found!")
ENDIF ()

IF (Boost_FOUND)
  INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS})
  TARGET_INCLUDE_DIRECTORIES (${TARGET_NAME_EXEC} PUBLIC "${Boost_INCLUDE_DIRS}")
  TARGET_LINK_LIBRARIES (${TARGET_NAME_EXEC} ${Boost_LIBRARIES})
ENDIF ()

