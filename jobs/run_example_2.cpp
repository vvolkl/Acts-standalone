
#include "bin/main.h"
#include "ACTS/Detector/TrackingGeometry.hpp"
#include "Utilities/KalmanFitUtils.hpp"
#include "ACTS/Examples/BuildGenericDetector.hpp"

int main(int argn, char** argv) {
    std::shared_ptr<const Acts::TrackingGeometry> geo = 
    Acts::buildGenericDetector(Acts::Logging::VERBOSE, Acts::Logging::VERBOSE, Acts::Logging::VERBOSE, 3);

      std::string filename(argv[1]);
      std::map<std::string, std::vector<double>> config;
      read_config(filename, config);
      //extract filenam
      size_t index = 0;
      index = filename.find("/", index);
      if (index != std::string::npos) filename.replace( 0, index+1, "");
      std::cout << filename << std::endl;
      run(geo, config, "example2_" + filename);

    /*
    {
      std::map<std::string, std::vector<double>> config;
      #include "phi_slice.ipp"
      run(geo, config, "example2_phislice");
    }
    {
      std::map<std::string, std::vector<double>> config;
      #include "eta_slice.ipp"
      run(geo, config, "example2_etaslice");
    }
    */

  return 0;
}
