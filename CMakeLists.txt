cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)

project(fcc-acts_standalone)
#find_package(ROOT)
find_package(Acts COMPONENTS Core IdentificationPlugin )
find_package(Eigen3)


include_directories(ActsCore ${CMAKE_CURRENT_SOURCE_DIR})
link_libraries(Eigen3::Eigen ActsCore )

add_executable(proptest PropTest/main.cpp)

#add_library (fcc-acts_standalone-detector SHARED # Detector/SuperSimpleDetector.cpp Detector/SuperSimpleDetElement.cpp Detector/DetCylinderSurface.cpp # Detector/SimpleDetector.cpp
#   Detector/SuperSimpleBarrel.cpp)
#target_include_directories(fcc-acts_standalone-detector PUBLIC ActsCore ${CMAKE_CURRENT_SOURCE_DIR}/Utilities ${CMAKE_CURRENT_SOURCE_DIR}/Detector)
#target_link_libraries (fcc-acts_standalone-detector PUBLIC  ActsCore)

#add_library (fcc-acts_standalone-utilities  SHARED Utilities/KalmanFitUtils.cpp Utilities/GenerateDummyMeasurements.cpp Utilities/read_config.cpp)
#target_include_directories(fcc-acts_standalone-utilities PUBLIC ActsCore  ${CMAKE_CURRENT_SOURCE_DIR}/Utilities)
#target_link_libraries (fcc-acts_standalone-utilities PUBLIC ActsCore )

#add_subdirectory(unittests)


#add_library(main bin/main.cpp)
#target_link_libraries(main fcc-acts_standalone-detector fcc-acts_standalone-utilities ActsCore)

#add_executable(run_example_0 jobs/run_example_0.cpp)
#target_link_libraries(run_example_0 main)

#add_executable(run_example_1 jobs/run_example_1.cpp)
#target_link_libraries(run_example_1 main)

#add_executable(run_example_2 jobs/run_example_2.cpp)
#target_link_libraries(run_example_2 main fcc-acts_standalone-utilities)

#add_executable(run_example_simple jobs/run_example_simple.cpp)
#target_link_libraries(run_example_simple main)

#add_executable(run_example_supersimple jobs/run_example_supersimple.cpp)
#target_link_libraries(run_example_supersimple main)


#add_executable(run_example_supersimple_barrel jobs/run_example_supersimple_barrel.cpp)
#target_link_libraries(run_example_supersimple_barrel main)

#add_executable(propagator_example bin/propagator_example.cpp)
#target_link_libraries(propagator_example main)

#add_custom_target(run
#                  COMMAND ./run_example_supersimple
#                  COMMAND ./run_example_simple)
#                 COMMAND ./run_example_0
#                 COMMAND ./run_example_1
#                  COMMAND ./run_example_2)

