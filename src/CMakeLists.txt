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