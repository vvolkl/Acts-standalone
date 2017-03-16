
#include "bin/main.h"
#include "ACTS/Detector/TrackingGeometry.hpp"
#include "Utilities/KalmanFitUtils.hpp"
#include "ACTS/Examples/BuildGenericDetector.hpp"

int main() {
    std::shared_ptr<const Acts::TrackingGeometry> geo = 
    Acts::buildGenericDetector(Acts::Logging::VERBOSE, Acts::Logging::VERBOSE, Acts::Logging::VERBOSE, 2);
    run(geo, "hitfile_example2.dat", "parameterfile_example2.dat");
    run_etaslice(geo, "hitfile_example2_etaslice.dat", "parameterfile_example_2_etaslice.dat");

  return 0;
}
