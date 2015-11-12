##  Software versions
SET (GLOBAL_VERSION_MAJOR 1)
SET (GLOBAL_VERSION_MINOR 00)

SET (QSCORES_VERSION_MAJOR 1)
SET (QSCORES_VERSION_MINOR 00)


##  Testing compilation flags, some of which are suggested by the Valgrind 3.3 book
# SET (MY_CXX_FLAGS "-pedantic -Wno-long-long -g -fno-inline -O0 -Wall")
##  Release compilation flags, suggested by the Valgrind 3.3 book
##  Added the following because of warnings in Boost 1.59.0:
##    -Wno-unused-variable
##    -Wno-unused-but-set-variable
SET (MY_CXX_FLAGS "-O3 -Wall -Wno-unused-variable -Wno-unused-but-set-variable")

