

#include "ACTS/Detector/TrackingGeometry.hpp"
#include "ACTS/Examples/GenericLayerBuilder.hpp"
#include "ACTS/Material/Material.hpp"
#include "ACTS/Tools/CylinderVolumeBuilder.hpp"
#include "ACTS/Tools/CylinderVolumeHelper.hpp"
#include "ACTS/Tools/LayerArrayCreator.hpp"
#include "ACTS/Tools/LayerCreator.hpp"
#include "ACTS/Tools/PassiveLayerBuilder.hpp"
#include "ACTS/Tools/SurfaceArrayCreator.hpp"
#include "ACTS/Tools/TrackingGeometryBuilder.hpp"
#include "ACTS/Tools/TrackingVolumeArrayCreator.hpp"
#include "ACTS/Utilities/Units.hpp"


#include "ACTS/Detector/TrackingGeometry.hpp"
#include "ACTS/Surfaces/CylinderBounds.hpp"
#include "ACTS/Layers/CylinderLayer.hpp"
#include "ACTS/Utilities/BinUtility.hpp"
#include "ACTS/Utilities/BinnedArrayXD.hpp"
#include "ACTS/Volumes/CylinderVolumeBounds.hpp"
#include "ACTS/Material/Material.hpp"
#include "ACTS/Detector/TrackingVolume.hpp"
#include "ACTS/Tools/LayerArrayCreator.hpp"
#include "ACTS/Tools/TrackingVolumeArrayCreator.hpp"
#include <iostream>
#include <vector>

#include "SuperSimpleDetector.hpp"

using namespace Acts;

TrackingVolumePtr
constructCylinderVolume(double surfaceHalfLengthZ,
                        double surfaceRadius,
                        double surfaceRstagger,
                        double surfaceZoverlap,
                        double layerEnvelope,
                        double volumeEnvelope,
                        double innerVolumeR,
                        double outerVolumeR,
                        const std::string& name)
{
  ///  the surface transforms
  auto sfnPosition
      = Vector3D(0., 0., -3 * surfaceHalfLengthZ - surfaceZoverlap);
  auto sfnTransform = std::make_shared<Transform3D>(Translation3D(sfnPosition));
  auto sfcTransform = nullptr;
  auto sfpPosition = Vector3D(0., 0., 3 * surfaceHalfLengthZ - surfaceZoverlap);
  auto sfpTransform = std::make_shared<Transform3D>(Translation3D(sfpPosition));
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
  auto   bUtility
      = std::make_unique<BinUtility>(surfaces.size(), bUmin, bUmax, open, binZ);
  std::unique_ptr<SurfaceArray> bArray
      = std::make_unique<BinnedArrayXD<const Surface*>>(surfaces,
                                                        std::move(bUtility));

  ///  now create the Layer
  auto layer0bounds = std::make_shared<CylinderBounds>(surfaceRadius, bUmax);
  auto layer0       = CylinderLayer::create(nullptr,
                                      layer0bounds,
                                      std::move(bArray),
                                      surfaceRstagger + 2 * layerEnvelope);
  std::unique_ptr<LayerArray> layerArray
      = std::make_unique<BinnedArrayXD<LayerPtr>>(layer0);

  ///  create the volume
  auto volumeBounds = std::make_shared<CylinderVolumeBounds>(
      innerVolumeR, outerVolumeR, bUmax + volumeEnvelope);
  TrackingVolumePtr volume = TrackingVolume::create(
      nullptr, volumeBounds, nullptr, std::move(layerArray), {}, {}, {}, name);
  ///  return the volume
  return volume;
}

///  helper function to create a container
TrackingVolumePtr
constructContainerVolume(TrackingVolumePtr  iVolume,
                         TrackingVolumePtr  oVolume,
                         double             hVolumeRadius,
                         double             hVolumeHalflength,
                         const std::string& name)
{
  ///  create the volume array
  typedef std::pair<TrackingVolumePtr, Vector3D> VAP;
  std::vector<VAP> volumes = {{iVolume, iVolume->binningPosition(binR)},
                              {oVolume, oVolume->binningPosition(binR)}};
  ///  the bounds for the container
  auto hVolumeBounds = std::make_shared<CylinderVolumeBounds>(
      0., hVolumeRadius, hVolumeHalflength);
  ///  create the BinUtility & the BinnedArray
  auto vUtility = std::make_unique<BinUtility>(
      volumes.size(), 0., hVolumeRadius, open, binR);
  std::shared_ptr<const TrackingVolumeArray> vArray
      = std::make_shared<const BinnedArrayXD<TrackingVolumePtr>>(
          volumes, std::move(vUtility));
  ///  create the container volume
  auto hVolume = TrackingVolume::create(nullptr, hVolumeBounds, vArray, name);
  // return the container
  return hVolume;
}

std::shared_ptr<Acts::TrackingGeometry> buildSuperSimpleDetector() {

  // most placements are at the origin, without translations or rotations
  // create identity transformation to be used throughout
  std::shared_ptr<Acts::Transform3D> identityTransform = std::make_shared<Acts::Transform3D>();
  identityTransform->setIdentity();

  //  beampipe
  std::shared_ptr<Acts::Material> bp_material = std::make_shared<Acts::Material>(95.7, 465.2, 28.03, 14., 2.32e-3);
  // Module material - X0, L0, A, Z, Rho
  std::vector<Acts::LayerPtr> bp_layerVector;
  std::vector<double> bp_layerRadii {20,30,40};
  for (double bp_radius: bp_layerRadii) { 
  std::shared_ptr<Acts::CylinderBounds> bp_cylinderBounds = std::make_shared<Acts::CylinderBounds>(bp_radius - 5, bp_radius + 5, 500);
  const Acts::CylinderSurface* bp_cylinderSurface  = new Acts::CylinderSurface(nullptr, bp_radius, 500);
  auto bp_bp = std::make_unique<BinnedArrayXD<const Surface*>>(bp_cylinderSurface);
  Acts::LayerPtr bp_cylinderLayer = Acts::CylinderLayer::create(identityTransform, bp_cylinderBounds, std::move(bp_bp), 0, nullptr, Acts::active);
  bp_layerVector.push_back(bp_cylinderLayer);
  }
  std::shared_ptr<Acts::CylinderVolumeBounds> bp_cylinderVolumeBounds = std::make_shared<Acts::CylinderVolumeBounds>(10, 50, 500);
  // create (std::shared_ptr< Transform3D > htrans, VolumeBoundsPtr volumeBounds, std::shared_ptr< Material > matprop, std::unique_ptr< const LayerArray > cLayerArray=nullptr, const LayerVector cLayerVector={}, const TrackingVolumeVector cVolumeVector={}, const DetachedVolumeVector dVolumeVector={}, const std::string &volumeName="undefined")
  Acts::TrackingVolumePtr bp_trackingVolume = Acts::TrackingVolume::create(identityTransform, bp_cylinderVolumeBounds, bp_material, nullptr, bp_layerVector, {}, {}, "MyVolume1");
  bp_trackingVolume->sign(Acts::Global);
  Acts::TrackingVolumeVector volVec;

  // inner barrel
  std::vector<Acts::LayerPtr> layerVector;
  std::vector<double> layerRadii {70, 80, 90, 100, 110, 120, 130};
  for (double radius: layerRadii) { 
    std::cout << radius << "\t" << layerVector.size() << std::endl;
    std::shared_ptr<Acts::CylinderBounds> cylinderBounds = std::make_shared<Acts::CylinderBounds>(radius - 5, radius + 5, 500);
    const Acts::CylinderSurface* cylinderSurface  = new Acts::CylinderSurface(nullptr, radius, 500);
    auto bp = std::make_unique<BinnedArrayXD<const Surface*>>(cylinderSurface);
    Acts::LayerPtr cylinderLayer = Acts::CylinderLayer::create(identityTransform, cylinderBounds, std::move(bp), 0, nullptr, Acts::active);
    layerVector.push_back(cylinderLayer);
  }
  // Module material - X0, L0, A, Z, Rho
  std::shared_ptr<Acts::Material> material = std::make_shared<Acts::Material>(95.7, 465.2, 28.03, 14., 2.32e-3);
  std::shared_ptr<Acts::CylinderVolumeBounds> cylinderVolumeBounds = std::make_shared<Acts::CylinderVolumeBounds>(50, 150, 500);
  // create (std::shared_ptr< Transform3D > htrans, VolumeBoundsPtr volumeBounds, std::shared_ptr< Material > matprop, std::unique_ptr< const LayerArray > cLayerArray=nullptr, const LayerVector cLayerVector={}, const TrackingVolumeVector cVolumeVector={}, const DetachedVolumeVector dVolumeVector={}, const std::string &volumeName="undefined")
  Acts::TrackingVolumePtr trackingVolume = Acts::TrackingVolume::create(identityTransform, cylinderVolumeBounds, material, nullptr, layerVector, {}, {}, "MyVolume");
  trackingVolume->sign(Acts::Global);
  volVec.push_back(bp_trackingVolume);
  volVec.push_back(trackingVolume);
 
  Acts::TrackingVolumeArrayCreator tv_creator; 
  auto tv_array = tv_creator.trackingVolumeArray(volVec, Acts::binR);
  std::shared_ptr<Acts::CylinderVolumeBounds> worldCylinderVolumeBounds = std::make_shared<Acts::CylinderVolumeBounds>(0, 300, 1000);
  std::cout << "tv array size: " <<  tv_array->arrayObjects().size() << std::endl;
  Acts::TrackingVolumePtr worldTrackingVolume = Acts::TrackingVolume::create(identityTransform, worldCylinderVolumeBounds, tv_array, "WorldVolume");


  ///  create three cylinder surfaces
  ///  the surface radius (will also be the layer radius)
  double iv_surfaceHalfLengthZ = 50 * Acts::units::_mm;
  double iv_surfaceRadius      = 25. * Acts::units::_mm;
  double iv_surfaceRstagger    = 5. * Acts::units::_mm;
  double iv_surfaceZoverlap    = 10. * Acts::units::_mm;
  double iv_layerEnvelope      = 0.5 * Acts::units::_mm;
  double iv_volumeEnvelope     = 10. * Acts::units::_mm;
  double iv_volumeRadius       = iv_surfaceRadius + 0.5 * iv_surfaceRstagger
      + iv_layerEnvelope + iv_volumeEnvelope;

  ///  the surface radius (will also be the layer radius)
  double ov_surfaceHalfLengthZ = 50. * Acts::units::_mm;
  double ov_surfaceRadius      = 100. * Acts::units::_mm;
  double ov_surfaceRstagger    = 5. * Acts::units::_mm;
  double ov_surfaceZoverlap    = 10. * Acts::units::_mm;
  double ov_layerEnvelope      = 0.5 * Acts::units::_mm;
  double ov_volumeEnvelope     = 10. * Acts::units::_mm;
  double ov_volumeRadius       = ov_surfaceRadius + 0.5 * ov_surfaceRstagger
      + ov_layerEnvelope + ov_volumeEnvelope;

  ///  inner volume
  auto iVolume = constructCylinderVolume(iv_surfaceHalfLengthZ,
                                         iv_surfaceRadius,
                                         iv_surfaceRstagger,
                                         iv_surfaceZoverlap,
                                         iv_layerEnvelope,
                                         iv_volumeEnvelope,
                                         0.,
                                         iv_volumeRadius,
                                         "InnerVolume");

  ///  outer volume
  auto oVolume = constructCylinderVolume(ov_surfaceHalfLengthZ,
                                         ov_surfaceRadius,
                                         ov_surfaceRstagger,
                                         ov_surfaceZoverlap,
                                         ov_layerEnvelope,
                                         ov_volumeEnvelope,
                                         iv_volumeRadius,
                                         ov_volumeRadius,
                                         "OuterVolume");

  std::shared_ptr<Acts::TrackingGeometry> trackingGeometry = std::make_shared<Acts::TrackingGeometry>(worldTrackingVolume);
  return trackingGeometry;
}
  
