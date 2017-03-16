

#include "bin/main.h"
#include "ACTS/Detector/TrackingGeometry.hpp"
#include "Utilities/KalmanFitUtils.hpp"
#include "Detector/SimpleDetector.hpp"

int main() {
    std::shared_ptr<const Acts::TrackingGeometry> geo = 
    Acts::buildSimpleDetector();
    run(geo, "hitfile_example_simple.dat", "parameterfile_example_simple.dat");
    run_etaslice(geo, "hitfile_example_simple_etaslice.dat", "parameterfile_example_simple_etaslice.dat");

  return 0;
}
