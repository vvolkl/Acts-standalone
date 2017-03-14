cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)

project(acts_tests)
find_package(ACTS COMPONENTS Core Examples TGeoPlugin DD4hepPlugin)

add_library (TestACTS SHARED SuperSimpleDetector.cpp SimpleDetector.cpp)
target_include_directories(TestACTS PUBLIC ACTS::ACTSCore)
target_link_libraries (TestACTS PUBLIC  ACTS::ACTSCore)

add_library (KalmanFitUtils SHARED KalmanFitUtils.cpp)
target_include_directories(KalmanFitUtils PUBLIC ACTS::ACTSCore ACTS::ACTSExamples)
target_link_libraries (KalmanFitUtils PUBLIC  ACTS::ACTSCore)

add_executable(acts_tests fittest.cpp)
target_link_libraries(acts_tests TestACTS ACTS::ACTSCore ACTS::ACTSDD4hepPlugin ACTS::ACTSDD4hepPlugin ACTS::ACTSExamples)

add_executable(main main.cpp)
target_link_libraries(main KalmanFitUtils TestACTS ACTS::ACTSCore ACTS::ACTSDD4hepPlugin ACTS::ACTSDD4hepPlugin ACTS::ACTSExamples)



