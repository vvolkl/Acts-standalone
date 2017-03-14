
#ifndef ACTS_SIMPLEDETECTOR_H
#define ACTS_SIMPLEDETECTOR_H 1

#include "ACTS/Utilities/Definitions.hpp"
#include "ACTS/Utilities/Logger.hpp"
#include <memory>
#include <vector>

namespace Acts {
class TrackingGeometry;

/// Global method to build the generic tracking geometry
/// @param lvl is the debug logging level
/// @param version is the detector version
std::unique_ptr<Acts::TrackingGeometry> buildSimpleDetector(Logging::Level surfaceLLevel = Logging::INFO,
                                                            Logging::Level layerLLevel = Logging::INFO,
                                                            Logging::Level volumeLLevel = Logging::INFO,
                                                            size_t version = 0);

/// Helper method for positioning
/// @param radius is the cylinder radius
/// @param tilt is the nominal layer radius
/// @param zStagger is the radial staggering along z
/// @param moduleHalfLength is the module length (longitudinal)
/// @param lOverlap is the overlap of the modules (longitudinal)
/// @binningSchema is the way the bins are laid out rphi x z
std::vector<Acts::Vector3D> modulePositionsCylinder(
    double radius, double zStagger, double moduleHalfLength, double lOverlap, const std::pair<int, int>& binningSchema);

/// Helper method for positioning
/// @param z is the z position of the ring
/// @param radius is the ring radius
/// @param phiStagger is the radial staggering along phi
/// @param lOverlap is the overlap of the modules
/// @parm nPhiBins is the number of bins in phi
std::vector<Acts::Vector3D> modulePositionsRing(double z, double radius, double phiStagger, int nPhiBins);

/// Helper method for positioning
/// @param z is the nominal z posiiton of the dis
/// @param ringStagger is the staggering of the different rings
/// @param phiStagger is the staggering on a ring when jumping R
/// @param innerRadius is the inner Radius for the disc
/// @param outerRadius is the outer Radius for the disc
/// @param discBinning is the binning setup in r, phi
/// @param moduleHalfLength is pair of phibins and module length
std::vector<std::vector<Acts::Vector3D>> modulePositionsDisc(double z,
                                                             double ringStagger,
                                                             double phiStagger,
                                                             double innerRadius,
                                                             double outerRadius,
                                                             const std::vector<int>& discBinning,
                                                             const std::vector<double>& moduleHalfLength);
}

#endif
