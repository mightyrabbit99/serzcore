add_executable(hello_test1)
target_sources(hello_test1
  PRIVATE
    tester.c
    helloendec.c
)
target_link_libraries(hello_test1
  PUBLIC
    serzcore
)
target_include_directories(hello_test1
  PRIVATE
    ${LUA_INCLUDE_DIR}
)
install(TARGETS hello_test1
  DESTINATION bin
)

add_library(helloendec-lua SHARED)
set_target_properties(helloendec-lua PROPERTIES LIBRARY_OUTPUT_NAME "helloendec")
set_target_properties(helloendec-lua PROPERTIES PREFIX "")
set_target_properties(helloendec-lua PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_include_directories(helloendec-lua
  PRIVATE
    ${LUA_INCLUDE_DIR}
)
target_sources(helloendec-lua
  PRIVATE
    helloendec_lua.c
)
target_link_libraries(helloendec-lua
  PUBLIC
    serzcore-lua
)
install(TARGETS helloendec-lua
  FILE_SET HEADERS DESTINATION include
  LIBRARY DESTINATION lib/lua
  PUBLIC_HEADER DESTINATION include
)
install(FILES base64.lua DESTINATION lib/lua)

install(FILES tester.lua DESTINATION bin RENAME hello_tester.lua)

add_library(lua STATIC IMPORTED)
set_target_properties(lua PROPERTIES IMPORTED_LOCATION ${LUA_LIBRARY})
find_library(MATH_LIBRARY m)
add_executable(hello_test2)
target_sources(hello_test2
  PRIVATE
    tester2.c
    helloendec.c
)
target_link_libraries(hello_test2
  PUBLIC
    serzcore
    lua
    ${MATH_LIBRARY}
)
target_include_directories(hello_test2
  PRIVATE
    ${LUA_INCLUDE_DIR}
)
install(TARGETS hello_test2
  DESTINATION bin
)


