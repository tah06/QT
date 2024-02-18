# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/VisualisateurBaseDeDonnees_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/VisualisateurBaseDeDonnees_autogen.dir/ParseCache.txt"
  "VisualisateurBaseDeDonnees_autogen"
  )
endif()
