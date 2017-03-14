

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
#include "ACTS/Utilities/Identifier.hpp"
#include "ACTS/Utilities/Units.hpp"

#include "ACTS/Detector/DetectorElementBase.hpp"
#include "ACTS/Detector/TrackingGeometry.hpp"
#include "ACTS/Detector/TrackingVolume.hpp"
#include "ACTS/Layers/CylinderLayer.hpp"
#include "ACTS/Material/Material.hpp"
#include "ACTS/Surfaces/CylinderBounds.hpp"
#include "ACTS/Tools/LayerArrayCreator.hpp"
#include "ACTS/Tools/TrackingVolumeArrayCreator.hpp"
#include "ACTS/Utilities/BinUtility.hpp"
#include "ACTS/Utilities/BinnedArrayXD.hpp"
#include "ACTS/Volumes/CylinderVolumeBounds.hpp"
#include <iostream>
#include <vector>

#include "DetCylinderSurface.hpp"
#include "SuperSimpleDetElement.hpp"
#include "SuperSimpleDetector.hpp"

using namespace Acts;

namespace Acts {

std::shared_ptr<Acts::TrackingGeometry> buildSuperSimpleDetector() {

  // most placements are at the origin, without translations or rotations
  // create identity transformation to be used throughout
  std::shared_ptr<Acts::Transform3D> identityTransform = std::make_shared<Acts::Transform3D>();
  identityTransform->setIdentity();

  // inner barrel
  std::vector<Acts::LayerPtr> layerVector;
  std::vector<double> layerRadii{70, 80, 90, 100, 110, 120, 130};
  for (double radius : layerRadii) {
    std::cout << radius << "\t" << layerVector.size() << std::endl;
    std::shared_ptr<Acts::CylinderBounds> cylinderBounds =
        std::make_shared<Acts::CylinderBounds>(radius - 5, radius + 5, 500);
    SuperSimpleDetElement* myDetElem =
        new SuperSimpleDetElement(Identifier(radius), identityTransform, cylinderBounds, 0.0001);

    const Acts::Surface* cylinderSurface = &(myDetElem->surface());
    auto bp = std::make_unique<BinnedArrayXD<const Surface*>>(cylinderSurface);
    Acts::LayerPtr cylinderLayer =
        Acts::CylinderLayer::create(identityTransform, cylinderBounds, std::move(bp), 0, nullptr, Acts::passive);

    layerVector.push_back(cylinderLayer);
  }
  // Module material - X0, L0, A, Z, Rho
  std::shared_ptr<Acts::Material> material = std::make_shared<Acts::Material>(95.7, 465.2, 28.03, 14., 2.32e-3);
  std::shared_ptr<Acts::CylinderVolumeBounds> cylinderVolumeBounds =
      std::make_shared<Acts::CylinderVolumeBounds>(50, 135, 500);
  // create (std::shared_ptr< Transform3D > htrans, VolumeBoundsPtr volumeBounds, std::shared_ptr< Material > matprop,
  // std::unique_ptr< const LayerArray > cLayerArray=nullptr, const LayerVector cLayerVector={}, const
  // TrackingVolumeVector cVolumeVector={}, const DetachedVolumeVector dVolumeVector={}, const std::string
  // &volumeName="undefined")
  Acts::LayerArrayCreator lc;
  auto la = lc.layerArray(layerVector, 0, 500, Acts::arbitrary, Acts::binR);
  Acts::TrackingVolumePtr trackingVolume = Acts::TrackingVolume::create(
      identityTransform, cylinderVolumeBounds, material, std::move(la), {}, {}, {}, "MyVolume");
  trackingVolume->sign(Acts::Global);
  /*
  volVec.push_back(bp_trackingVolume);
  volVec.push_back(trackingVolume);

  Acts::TrackingVolumeArrayCreator tv_creator;
  auto tv_array = tv_creator.trackingVolumeArray(volVec, Acts::binR);
  std::shared_ptr<Acts::CylinderVolumeBounds> worldCylinderVolumeBounds =
  std::make_shared<Acts::CylinderVolumeBounds>(0, 300, 1000);
  std::cout << "tv array size: " <<  tv_array->arrayObjects().size() << std::endl;
  Acts::TrackingVolumePtr worldTrackingVolume = Acts::TrackingVolume::create(identityTransform,
  worldCylinderVolumeBounds, tv_array, "WorldVolume");
  */

  std::shared_ptr<Acts::TrackingGeometry> trackingGeometry = std::make_shared<Acts::TrackingGeometry>(trackingVolume);
  return trackingGeometry;
}
}
