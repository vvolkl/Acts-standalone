
#include <memory>

namespace Acts {
  class TrackingGeometry;
}

void run(std::shared_ptr<const Acts::TrackingGeometry> geo, std::string hitfilename, std::string trackparameterfilename);
void run_etaslice(std::shared_ptr<const Acts::TrackingGeometry> geo, std::string hitfilename, std::string trackparameterfilename);

