

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
#include <iostream>
#include <vector>

#include "SuperSimpleDetector.hpp"

using namespace Acts;

std::shared_ptr<Acts::TrackingGeometry> buildSuperSimpleDetector() {
  std::shared_ptr<Acts::Transform3D> identityTransform = std::make_shared<Acts::Transform3D>();
  identityTransform->setIdentity();

  std::vector<Acts::LayerPtr> layerVector;
  std::vector<double> layerRadii { 10, 20 ,30, 40, 50,60, 70, 80, 90, 100, 1000};
  for (double radius: layerRadii) { 
    std::cout << radius << "\t" << layerVector.size() << std::endl;
    std::shared_ptr<Acts::CylinderBounds> cylinderBounds = std::make_shared<Acts::CylinderBounds>(radius, 100);
    const Acts::CylinderSurface* cylinderSurface  = new Acts::CylinderSurface(nullptr, radius, 100);
    auto bp = std::make_unique<BinnedArrayXD<const Surface*>>(cylinderSurface);
    Acts::LayerPtr cylinderLayer = Acts::CylinderLayer::create(identityTransform, cylinderBounds, std::move(bp), 0, nullptr, Acts::passive);
    layerVector.push_back(cylinderLayer);
  }
  // Module material - X0, L0, A, Z, Rho
  std::shared_ptr<Acts::Material> material = std::make_shared<Acts::Material>(95.7, 465.2, 28.03, 14., 2.32e-3);
  std::shared_ptr<Acts::CylinderVolumeBounds> cylinderVolumeBounds = std::make_shared<Acts::CylinderVolumeBounds>(200, 100);
  // create (std::shared_ptr< Transform3D > htrans, VolumeBoundsPtr volumeBounds, std::shared_ptr< Material > matprop, std::unique_ptr< const LayerArray > cLayerArray=nullptr, const LayerVector cLayerVector={}, const TrackingVolumeVector cVolumeVector={}, const DetachedVolumeVector dVolumeVector={}, const std::string &volumeName="undefined")
  Acts::TrackingVolumePtr trackingVolume = Acts::TrackingVolume::create(identityTransform, cylinderVolumeBounds, material, nullptr, layerVector, {}, {}, "MyVolume");
  trackingVolume->sign(Acts::Global);
  std::shared_ptr<Acts::TrackingGeometry> trackingGeometry = std::make_shared<Acts::TrackingGeometry>(trackingVolume);
  return trackingGeometry;
}
  
