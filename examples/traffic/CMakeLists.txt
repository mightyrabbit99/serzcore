add_executable(traffic)
target_sources(traffic
  PRIVATE
    tester.c
    trafficserz.c
)
target_link_libraries(traffic
  PUBLIC
    serzcore
)
install(TARGETS traffic
  DESTINATION bin
)
