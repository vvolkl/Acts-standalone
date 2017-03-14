#ifndef ACTSTESTS_SUPERSIMPLEDETECTOR_H
#define ACTSTESTS_SUPERSIMPLEDETECTOR_H

namespace Acts {
class TrackingGeometry;

std::shared_ptr<Acts::TrackingGeometry> buildSuperSimpleDetector();
}
#endif
