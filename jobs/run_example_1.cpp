
#include "bin/main.h"
#include "ACTS/Detector/TrackingGeometry.hpp"
#include "Utilities/KalmanFitUtils.hpp"
#include "ACTS/Examples/BuildGenericDetector.hpp"

int main() {
    std::shared_ptr<const Acts::TrackingGeometry> geo = 
    Acts::buildGenericDetector(Acts::Logging::VERBOSE, Acts::Logging::VERBOSE, Acts::Logging::VERBOSE, 1);

    {
      std::map<std::string, std::vector<double>> config;
      #include "phi_slice.ipp"
      run(geo, config, "example1_phislice");
    }
    {
      std::map<std::string, std::vector<double>> config;
      #include "eta_slice.ipp"
      run(geo, config, "example1_etaslice");
    }

  return 0;
}
