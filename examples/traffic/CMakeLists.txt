add_executable(traffic_test1)
target_sources(traffic_test1
  PRIVATE
    tester.c
    trafficserz.c
)
target_link_libraries(traffic_test1
  PUBLIC
    serzcore
)
install(TARGETS traffic_test1
  DESTINATION bin
)
