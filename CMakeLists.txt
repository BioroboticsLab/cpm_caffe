#-----------------------------------------------------------------------
# CPM configuration
#-----------------------------------------------------------------------
set(CPM_MODULE_NAME cpm_caffe)

if ((DEFINED CPM_DIR) AND (DEFINED CPM_UNIQUE_ID) AND (DEFINED CPM_TARGET_NAME))
  set(CPM_LIB_TARGET_NAME ${CPM_TARGET_NAME})
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CPM_DIR})
  include(CPM)
else()
  set(CPM_DIR "${CMAKE_CURRENT_BINARY_DIR}/cpm-packages" CACHE TYPE STRING)
  find_package(Git)
  if(NOT GIT_FOUND)
    message(FATAL_ERROR "CPM requires Git.")
  endif()
  if (NOT EXISTS ${CPM_DIR}/CPM.cmake)
    execute_process(
      COMMAND "${GIT_EXECUTABLE}" clone https://github.com/iauns/cpm ${CPM_DIR}
      RESULT_VARIABLE error_code
      OUTPUT_VARIABLE head_sha)
    if(error_code)
      message(FATAL_ERROR "CPM failed to get the hash for HEAD")
    endif()
  endif()
  include(${CPM_DIR}/CPM.cmake)
endif()

# All externals *must* define this.
CPM_ForceOnlyOneModuleVersion()

CPM_InitModule(${CPM_MODULE_NAME})

#------------------------------------------------------------------------------
# Caffe
#------------------------------------------------------------------------------

if(NOT DEFINED CAFFE_COMMIT)
    set(CAFFE_COMMIT "71e05876f644a08af4cb1c955d01c5a272539e96")
endif()

if(NOT DEFINED CAFFE_GIT_REPO)
    set(CAFFE_GIT_REPO "https://github.com/BVLC/caffe")
endif()

set(CAFFE_URL "${CAFFE_GIT_REPO}/archive/${CAFFE_COMMIT}.zip")

set(CAFFE_ZIP "${CMAKE_BINARY_DIR}/3rdParty/caffe.zip")
set(CAFFE_UNPACKED "${CMAKE_BINARY_DIR}/3rdParty/caffe-${CAFFE_COMMIT}")

set(REPO_SOURCE_DIR "${CMAKE_BINARY_DIR}/3rdParty/caffe")
set(REPO_BUILD_DIR "${CMAKE_BINARY_DIR}/3rdParty/caffe_build")
if(NOT EXISTS ${CAFFE_ZIP} OR NOT EXISTS ${REPO_SOURCE_DIR})
    message(${CAFFE_COMMIT})
    message("Downloading Caffe from ${CAFFE_URL}")
    file(DOWNLOAD ${CAFFE_URL} ${CAFFE_ZIP} SHOW_PROGRESS)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar -xf ${CAFFE_ZIP}
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/3rdParty/"
        RESULT_VARIABLE error_code)
    if(error_code)
        message(FATAL_ERROR "Failed to unpack Caffe")
    endif()
    file(RENAME ${CAFFE_UNPACKED} ${REPO_SOURCE_DIR})
endif()

if(DEFINED CPU_ONLY)
    CPM_ExportAdditionalDefinition("-DCPU_ONLY")
endif()

option(BUILD_SHARED_LIBS "BUILD_SHARED_LIBS" ON)
option(BUILD_EXAMPLES "BUILD_EXAMPLES" OFF)

add_subdirectory("${REPO_SOURCE_DIR}" "${REPO_BUILD_DIR}" EXCLUDE_FROM_ALL)
set_target_properties(caffe PROPERTIES EXCLUDE_FROM_ALL FALSE)
get_target_property(CAFFE_INCLUDE_DIRECTORIES caffe "INCLUDE_DIRECTORIES")

CPM_ExportAdditionalLibraryTarget("caffe")
CPM_ExportAdditionalIncludeDir("${CAFFE_INCLUDE_DIRECTORIES}")
