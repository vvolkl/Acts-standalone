
#include "bin/main.h"
#include "ACTS/Detector/TrackingGeometry.hpp"
#include "Utilities/KalmanFitUtils.hpp"
#include "ACTS/Examples/BuildGenericDetector.hpp"

int main() {
    std::shared_ptr<const Acts::TrackingGeometry> geo = 
    Acts::buildGenericDetector(Acts::Logging::VERBOSE, Acts::Logging::VERBOSE, Acts::Logging::VERBOSE, 1);
    run(geo, "hitfile_example1.dat", "parameterfile_example1.dat");
    run_etaslice(geo, "hitfile_example1_etaslice.dat", "parameterfile_example1_etaslice.dat");

  return 0;
}
