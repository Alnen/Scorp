include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runPetriNetTest PetriNetTest.cpp TypeEnum.cpp)

target_link_libraries(runPetriNetTest gtest gtest_main)