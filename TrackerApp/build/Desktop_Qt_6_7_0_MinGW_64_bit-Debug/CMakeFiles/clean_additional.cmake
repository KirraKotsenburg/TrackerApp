# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appTrackerApp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appTrackerApp_autogen.dir\\ParseCache.txt"
  "appTrackerApp_autogen"
  )
endif()
