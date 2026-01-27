add_library(serzcore STATIC)
set_target_properties(serzcore PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_sources(serzcore
  PRIVATE
    serzcore.c
  PUBLIC
    serzcore.h
    serzcore_utils.h
  PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} FILES
      serzcore.h
      serzcore_utils.h
)
install(TARGETS serzcore
  FILE_SET HEADERS DESTINATION include
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include
)

if(BUILD_SERZCORE_LUA)
add_library(serzcore-lua STATIC)
target_compile_options(serzcore-lua PRIVATE -undefined -DSERZCORE_LUA)
set_target_properties(serzcore-lua PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_include_directories(serzcore-lua
  PRIVATE
    ${LUA_INCLUDE_DIR}
)
target_sources(serzcore-lua
  PRIVATE
    serzcore.c
  PUBLIC
    serzcore.h
    serzcore_utils.h
  PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} FILES
      serzcore.h
      serzcore_utils.h
)
install(TARGETS serzcore-lua
  FILE_SET HEADERS DESTINATION include
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include
)
endif()
