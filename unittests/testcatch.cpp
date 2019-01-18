#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "Acts/Detector/TrackingGeometry.hpp"
#include "Acts/Detector/TrackingVolume.hpp"
#include "Acts/Layers/CylinderLayer.hpp"
#include "Acts/Surfaces/CylinderBounds.hpp"
#include "Acts/Surfaces/CylinderSurface.hpp"
#include "Acts/Utilities/BinUtility.hpp"
#include "Acts/Utilities/BinnedArrayXD.hpp"
#include "Acts/Utilities/Definitions.hpp"
#include "Acts/Volumes/CylinderVolumeBounds.hpp"

#include "Acts/Utilities/Helpers.hpp"
#include "Acts/Utilities/Units.hpp"


namespace Acts {

///  helper function to create a cylinder
TrackingVolumePtr
constructCylinderVolume(double             surfaceHalfLengthZ,
                        double             surfaceRadius,
                        double             surfaceRstagger,
                        double             surfaceZoverlap,
                        double             layerEnvelope,
                        double             volumeEnvelope,
                        double             innerVolumeR,
                        double             outerVolumeR,
                        const std::string& name)
{
  ///  the surface transforms
  auto sfnPosition
      = Vector3D(0., 0., -3 * surfaceHalfLengthZ - surfaceZoverlap);
  auto sfnTransform
      = std::make_shared<const Transform3D>(Translation3D(sfnPosition));
  auto sfcTransform = nullptr;
  auto sfpPosition = Vector3D(0., 0., 3 * surfaceHalfLengthZ - surfaceZoverlap);
  auto sfpTransform
      = std::make_shared<const Transform3D>(Translation3D(sfpPosition));
  ///  the surfaces
  auto sfn = new CylinderSurface(
      sfnTransform, surfaceRadius - 0.5 * surfaceRstagger, surfaceHalfLengthZ);
  auto sfc = new CylinderSurface(
      sfcTransform, surfaceRadius + 0.5 * surfaceRstagger, surfaceHalfLengthZ);
  auto sfp = new CylinderSurface(
      sfpTransform, surfaceRadius - 0.5 * surfaceRstagger, surfaceHalfLengthZ);

  ///  prepare the surfaces
  typedef std::pair<const Surface*, Vector3D> TAP;
  std::vector<TAP> surfaces = {{sfn, sfn->binningPosition(binZ)},
                               {sfc, sfc->binningPosition(binZ)},
                               {sfp, sfp->binningPosition(binZ)}};

  ///  make the binned array
  double bUmin = sfnPosition.z() - surfaceHalfLengthZ;
  double bUmax = sfpPosition.z() + surfaceHalfLengthZ;

  std::vector<const Surface*> surfaces_only = {{sfn, sfc, sfp}};

  detail::Axis<detail::AxisType::Equidistant, detail::AxisBoundaryType::Bound>
       axis(bUmin, bUmax, surfaces.size());
  auto g2l = [](const Vector3D& glob) {
    return std::array<double, 1>({{glob.z()}});
  };
  auto l2g
      = [](const std::array<double, 1>& loc) { return Vector3D(0, 0, loc[0]); };
  auto sl = std::make_unique<SurfaceArray::SurfaceGridLookup<decltype(axis)>>(
      g2l, l2g, std::make_tuple(axis));
  sl->fill(surfaces_only);
  auto bArray = std::make_unique<SurfaceArray>(std::move(sl), surfaces_only);

  ///  now create the Layer
  auto layer0bounds
      = std::make_shared<const CylinderBounds>(surfaceRadius, bUmax);
  auto layer0 = CylinderLayer::create(nullptr,
                                      layer0bounds,
                                      std::move(bArray),
                                      surfaceRstagger + 2 * layerEnvelope);
  std::unique_ptr<const LayerArray> layerArray
      = std::make_unique<const BinnedArrayXD<LayerPtr>>(layer0);

  ///  create the volume
  auto volumeBounds = std::make_shared<const CylinderVolumeBounds>(
      innerVolumeR, outerVolumeR, bUmax + volumeEnvelope);
  TrackingVolumePtr volume = TrackingVolume::create(
      nullptr, volumeBounds, nullptr, std::move(layerArray), {}, {}, {}, name);
  ///  return the volume
  return volume;
}

///  helper function to create a container
MutableTrackingVolumePtr
constructContainerVolume(TrackingVolumePtr  iVolume,
                         TrackingVolumePtr  oVolume,
                         double             hVolumeRadius,
                         double             hVolumeHalflength,
                         const std::string& name)
{
  ///  create the volume array
  using VAP                = std::pair<TrackingVolumePtr, Vector3D>;
  std::vector<VAP> volumes = {{iVolume, iVolume->binningPosition(binR)},
                              {oVolume, oVolume->binningPosition(binR)}};
  ///  the bounds for the container
  auto hVolumeBounds = std::make_shared<const CylinderVolumeBounds>(
      0., hVolumeRadius, hVolumeHalflength);
  ///  create the BinUtility & the BinnedArray
  auto vUtility = std::make_unique<const BinUtility>(
      volumes.size(), 0., hVolumeRadius, open, binR);
  std::shared_ptr<const TrackingVolumeArray> vArray
      = std::make_shared<const BinnedArrayXD<TrackingVolumePtr>>(
          volumes, std::move(vUtility));
  ///  create the container volume
  auto hVolume = TrackingVolume::create(nullptr, hVolumeBounds, vArray, name);
  // return the container
  return hVolume;
}
}
using namespace Acts;


#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"


TEST_CASE( "Factorials are computed", "[factorial]" ) {
  double surfaceHalfLengthZ = 50 * Acts::units::_mm;
  double surfaceRstagger    = 5. * Acts::units::_mm;
  double surfaceZoverlap    = 10. * Acts::units::_mm;
  double layerEnvelope      = 0.5 * Acts::units::_mm;
  double volumeEnvelope     = 10. * Acts::units::_mm;

  // inner inner volume definitions
  double iiv_surfaceRadius = 25. * Acts::units::_mm;
  double iiv_volumeRadius  = iiv_surfaceRadius + 0.5 * surfaceRstagger
      + layerEnvelope + volumeEnvelope;

  ///  inner outer volume defininitions
  double iov_surfaceRadius = 100. * Acts::units::_mm;
  double iov_volumeRadius  = iov_surfaceRadius + 0.5 * surfaceRstagger
      + layerEnvelope + volumeEnvelope;

  ///  inner inner volume
  auto iiVolume = constructCylinderVolume(surfaceHalfLengthZ,
                                          iiv_surfaceRadius,
                                          surfaceRstagger,
                                          surfaceZoverlap,
                                          layerEnvelope,
                                          volumeEnvelope,
                                          0.,
                                          iiv_volumeRadius,
                                          "InnerInnerVolume");
  ///  inner outer volume
  auto ioVolume = constructCylinderVolume(surfaceHalfLengthZ,
                                          iov_surfaceRadius,
                                          surfaceRstagger,
                                          surfaceZoverlap,
                                          layerEnvelope,
                                          volumeEnvelope,
                                          iiv_volumeRadius,
                                          iov_volumeRadius,
                                          "InnerOuterVolume");

  // now create the Inner Container volume
  double volumeHalfZ
      = (4 * surfaceHalfLengthZ - surfaceZoverlap) + volumeEnvelope;
  /// the inner volume
  auto iVolume = constructContainerVolume(iiVolume,
                                          ioVolume,
                                          iov_volumeRadius,
                                          volumeHalfZ,
                                          "InnerVolume");

  // outer volume definitions
  double ov_surfaceRadius = 150. * Acts::units::_mm;
  double ov_volumeRadius  = ov_surfaceRadius + 0.5 * surfaceRstagger
      + layerEnvelope + volumeEnvelope;

  ///  inner outer volume
  auto oVolume = constructCylinderVolume(surfaceHalfLengthZ,
                                         ov_surfaceRadius,
                                         surfaceRstagger,
                                         surfaceZoverlap,
                                         layerEnvelope,
                                         volumeEnvelope,
                                         iov_volumeRadius,
                                         ov_volumeRadius,
                                         "OuterVolume");
  /// the inner volume
  auto volume = constructContainerVolume(iVolume,
                                         oVolume,
                                         ov_volumeRadius,
                                         volumeHalfZ,
                                         "WorldVolume");

  // creating a TrackingGeometry
  // -> closs the geometry, this should set the GeometryID
  TrackingGeometry tGeometry(volume);
  // get the world back
  auto world = tGeometry.highestTrackingVolume();

    size_t nSurfaces = 0;
    tGeometry.visitSurfaces([&nSurfaces](const auto*) { nSurfaces++; });

    REQUIRE(nSurfaces == 9);
}
