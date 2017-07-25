
#include "bin/main.h"
#include "ACTS/Detector/TrackingGeometry.hpp"
#include "Utilities/KalmanFitUtils.hpp"
#include "Detector/SuperSimpleBarrel.hpp"

int main(int argn, char** argv) {
    std::shared_ptr<const Acts::TrackingGeometry> geo = 
    Acts::buildSuperSimpleBarrel();

      std::string filename(argv[1]);
      std::map<std::string, std::vector<double>> config;
      read_config(filename, config);
      //extract filename
      size_t index = 0;
      index = filename.find("/", index);
      if (index != std::string::npos) filename.replace( 0, index+1, "");
      filename.erase(filename.end()-4, filename.end());
      std::cout << "command file basename: " << filename << std::endl;
      run(geo, config, "exampleSuperSimpleBarrel-" + filename);

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
