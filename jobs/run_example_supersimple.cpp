

#include "bin/main.h"
#include "ACTS/Detector/TrackingGeometry.hpp"
#include "Utilities/KalmanFitUtils.hpp"
#include "Detector/SuperSimpleDetector.hpp"

int main() {
    std::shared_ptr<const Acts::TrackingGeometry> geo = 
    Acts::buildSuperSimpleDetector();

    {
      std::map<std::string, std::vector<double>> config;
      #include "phi_slice.ipp"
      run(geo, config, "exampleSuperSimple_phislice");
    }
    {
      std::map<std::string, std::vector<double>> config;
      #include "eta_slice.ipp"
      run(geo, config, "exampleSuperSimple_etaslice");
    }

  return 0;
}
