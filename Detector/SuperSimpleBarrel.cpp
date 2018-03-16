

#include "ACTS/Detector/TrackingGeometry.hpp"
#include "ACTS/Material/Material.hpp"
#include "ACTS/Tools/CylinderVolumeBuilder.hpp"
#include "ACTS/Tools/CylinderVolumeHelper.hpp"
#include "ACTS/Tools/LayerArrayCreator.hpp"
#include "ACTS/Tools/LayerCreator.hpp"
#include "ACTS/Tools/SurfaceArrayCreator.hpp"
#include "ACTS/Tools/TrackingGeometryBuilder.hpp"
#include "ACTS/Tools/TrackingVolumeArrayCreator.hpp"
#include "ACTS/Utilities/Identifier.hpp"
#include "ACTS/Utilities/Units.hpp"

#include "ACTS/Detector/DetectorElementBase.hpp"
#include "ACTS/Detector/TrackingGeometry.hpp"
#include "ACTS/Detector/TrackingVolume.hpp"
#include "ACTS/Layers/CylinderLayer.hpp"
#include "ACTS/Layers/DiscLayer.hpp"
#include "ACTS/Material/Material.hpp"
#include "ACTS/Surfaces/CylinderBounds.hpp"
#include "ACTS/Surfaces/RadialBounds.hpp"
#include "ACTS/Tools/LayerArrayCreator.hpp"
#include "ACTS/Tools/TrackingVolumeArrayCreator.hpp"
#include "ACTS/Utilities/BinUtility.hpp"
#include "ACTS/Utilities/BinnedArrayXD.hpp"
#include "ACTS/Volumes/CylinderVolumeBounds.hpp"
#include <iostream>
#include <vector>

#include "DetCylinderSurface.hpp"
#include "SuperSimpleDetector.hpp"

using namespace Acts;

namespace Acts {

std::shared_ptr<Acts::TrackingGeometry> buildSuperSimpleBarrel() {

  // most placements are at the origin, without translations or rotations
  // create identity transformation to be used throughout
  std::shared_ptr<Acts::Transform3D> identityTransform = std::make_shared<Acts::Transform3D>();
  identityTransform->setIdentity();

  std::vector<Acts::LayerPtr> layerVector;
  // endcaps
  std::vector<double> layerRs{20, 40, 60, 80, 100};
  for (double r: layerRs) {
    Acts::Translation3D translation{0., 0, 0};
    std::shared_ptr<Acts::Transform3D>   transform = std::make_shared<Acts::Transform3D>(translation);

    std::shared_ptr<Acts::CylinderBounds> radialBounds =
        std::make_shared<Acts::CylinderBounds>(r, 1000.);
    //SuperSimpleDetElement* myDetElem =
    //    new SuperSimpleDetElement(Identifier(z), transform, radialBounds, 0.0001);


    const Acts::Surface* cylinderSurface = new Acts::CylinderSurface(identityTransform, r, 500);//&(myDetElem->surface());
    auto bp = std::make_unique<BinnedArrayXD<const Surface*>>(cylinderSurface);
    Acts::LayerPtr cylinderLayer =
        Acts::CylinderLayer::create(transform, radialBounds, std::move(bp), 0, nullptr, Acts::passive);


    layerVector.push_back(cylinderLayer);


  }



  // Module material - X0, L0, A, Z, Rho
  std::shared_ptr<Acts::Material> material = std::make_shared<Acts::Material>(95.7, 465.2, 28.03, 14., 2.32e-3);
  std::shared_ptr<Acts::CylinderVolumeBounds> cylinderVolumeBounds =
      std::make_shared<Acts::CylinderVolumeBounds>(120, 1000);
  // create (std::shared_ptr< Transform3D > htrans, VolumeBoundsPtr volumeBounds, std::shared_ptr< Material > matprop,
  // std::unique_ptr< const LayerArray > cLayerArray=nullptr, const LayerVector cLayerVector={}, const
  // TrackingVolumeVector cVolumeVector={}, const DetachedVolumeVector dVolumeVector={}, const std::string
  // &volumeName="undefined")
  Acts::LayerArrayCreator lc;
  auto la = lc.layerArray(layerVector, 0, 1020, Acts::arbitrary, Acts::binR);
  Acts::TrackingVolumePtr trackingVolume = Acts::TrackingVolume::create(
      identityTransform, cylinderVolumeBounds, nullptr,
      // nullptr, 
      std::move(la),
       //layerVector,
        {}, 
        {}, {}, "MyVolume");
  std::const_pointer_cast<TrackingVolume>(trackingVolume)->sign(Acts::Global);

  /*

  // outer Barrel
  std::vector<Acts::LayerPtr> olayerVector;
  std::vector<double> olayerRadii{310, 320, 330, 340};
  for (double radius : olayerRadii) {
    std::cout << radius << "\t" << olayerVector.size() << std::endl;
    std::shared_ptr<Acts::CylinderBounds> ocylinderBounds =
        std::make_shared<Acts::CylinderBounds>(radius - 5, radius + 5, 500);
    SuperSimpleDetElement* omyDetElem =
        new SuperSimpleDetElement(Identifier(radius), identityTransform, ocylinderBounds, 0.0001);

    const Acts::Surface* ocylinderSurface = &(omyDetElem->surface());
    auto obp = std::make_unique<BinnedArrayXD<const Surface*>>(ocylinderSurface);
    Acts::LayerPtr ocylinderLayer =
        Acts::CylinderLayer::create(identityTransform, ocylinderBounds, std::move(obp), 0, nullptr, Acts::passive);

    olayerVector.push_back(ocylinderLayer);
  }
  // Module material - X0, L0, A, Z, Rho
  std::shared_ptr<Acts::CylinderVolumeBounds> ocylinderVolumeBounds =
      std::make_shared<Acts::CylinderVolumeBounds>(250, 500, 500);
  // create (std::shared_ptr< Transform3D > htrans, VolumeBoundsPtr volumeBounds, std::shared_ptr< Material > matprop,
  // std::unique_ptr< const LayerArray > cLayerArray=nullptr, const LayerVector cLayerVector={}, const
  // TrackingVolumeVector cVolumeVector={}, const DetachedVolumeVector dVolumeVector={}, const std::string
  // &volumeName="undefined")
  auto ola = lc.layerArray(olayerVector, 250, 500, Acts::arbitrary, Acts::binR);
  Acts::TrackingVolumePtr otrackingVolume = Acts::TrackingVolume::create(
      identityTransform, ocylinderVolumeBounds, material, std::move(ola), {}, {}, {}, "MyOuterVolume");
  //otrackingVolume->sign(Acts::Global);

  std::vector<Acts::TrackingVolumePtr> volVec;
  volVec.push_back(trackingVolume);
  volVec.push_back(otrackingVolume);

  Acts::TrackingVolumeArrayCreator tv_creator;
  auto tv_array = tv_creator.trackingVolumeArray(volVec, Acts::binR);
  std::shared_ptr<Acts::CylinderVolumeBounds> worldCylinderVolumeBounds =
  std::make_shared<Acts::CylinderVolumeBounds>(0, 1000, 500);
  std::cout << "tv array size: " <<  tv_array->arrayObjects().size() << std::endl;
  Acts::TrackingVolumePtr worldTrackingVolume = Acts::TrackingVolume::create(identityTransform,
  worldCylinderVolumeBounds, tv_array, "WorldVolume");

  */

  std::shared_ptr<Acts::TrackingGeometry> trackingGeometry = std::make_shared<Acts::TrackingGeometry>(std::const_pointer_cast<TrackingVolume>(trackingVolume));
  return trackingGeometry;
}
}
