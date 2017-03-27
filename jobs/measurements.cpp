
#include "bin/main.h"
#include "ACTS/Detector/TrackingGeometry.hpp"
#include "Utilities/KalmanFitUtils.hpp"
#include "ACTS/Examples/BuildGenericDetector.hpp"

int main() {
    std::shared_ptr<const Acts::TrackingGeometry> geo = 
    Acts::buildGenericDetector(Acts::Logging::VERBOSE, Acts::Logging::VERBOSE, Acts::Logging::VERBOSE, 1);
    
 
  dumpTrackingVolume(geo->highestTrackingVolume());
  return 0;
}
