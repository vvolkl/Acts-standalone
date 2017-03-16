

#include "bin/main.h"
#include "ACTS/Detector/TrackingGeometry.hpp"
#include "Utilities/KalmanFitUtils.hpp"
#include "Detector/SuperSimpleDetector.hpp"

int main() {
    std::shared_ptr<const Acts::TrackingGeometry> geo = 
    Acts::buildSuperSimpleDetector();
    run(geo, "hitfile_example_supersimple.dat", "parameterfile_example_supersimple.dat");
    run_etaslice(geo, "hitfile_example_supersimple_etaslice.dat", "parameterfile_example_supersimple_etaslice.dat");

  return 0;
}
