
#include "bin/main.h"
#include "ACTS/Detector/TrackingGeometry.hpp"
#include "Utilities/KalmanFitUtils.hpp"
#include "Detector/SimpleDetector.hpp"

int main() {
    std::shared_ptr<const Acts::TrackingGeometry> geo = 
    Acts::buildGenericDetector(Acts::Logging::VERBOSE, Acts::Logging::VERBOSE, Acts::Logging::VERBOSE, 0);
    run(geo, "hitfile_example0.dat", "parameterfile_example0.dat");

  return 0;
}
