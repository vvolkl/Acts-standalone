cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)

project(fcc-acts_standalone)
find_package(ACTS COMPONENTS Core Examples TGeoPlugin DD4hepPlugin)
include_directories(ACTS:ACTSCore ${CMAKE_CURRENT_SOURCE_DIR})

add_library (fcc-acts_standalone-detector SHARED Detector/SuperSimpleDetector.cpp Detector/SuperSimpleDetElement.cpp Detector/DetCylinderSurface.cpp Detector/SimpleDetector.cpp)
target_include_directories(fcc-acts_standalone-detector PUBLIC ACTS::ACTSCore ${CMAKE_CURRENT_SOURCE_DIR}/Utilities ${CMAKE_CURRENT_SOURCE_DIR}/Detector)
target_link_libraries (fcc-acts_standalone-detector PUBLIC  ACTS::ACTSCore)

add_library (fcc-acts_standalone-utilites  SHARED Utilities/KalmanFitUtils.cpp Utilities/GenerateDummyMeasurements.cpp)
target_include_directories(fcc-acts_standalone-utilites PUBLIC ACTS::ACTSCore ACTS::ACTSExamples ${CMAKE_CURRENT_SOURCE_DIR}/Utilities)
target_link_libraries (fcc-acts_standalone-utilites PUBLIC ACTS::ACTSCore)


add_executable(main bin/main.cpp)
target_link_libraries(main fcc-acts_standalone-detector fcc-acts_standalone-utilites ACTS::ACTSCore ACTS::ACTSDD4hepPlugin ACTS::ACTSDD4hepPlugin ACTS::ACTSExamples)


add_custom_target(run
                  COMMAND ./main)

