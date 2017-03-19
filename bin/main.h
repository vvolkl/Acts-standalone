
#include <memory>
#include <map>
#include <string>
#include <vector>

namespace Acts {
  class TrackingGeometry;
}

void run(std::shared_ptr<const Acts::TrackingGeometry> geo, std::map<std::string, std::vector<double>> config, std::string name);
void run_etaslice(std::shared_ptr<const Acts::TrackingGeometry> geo, std::string hitfilename, std::string trackparameterfilename);

