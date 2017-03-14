

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
#include "SuperSimpleDetector.hpp"

using namespace Acts;

namespace Acts {

class SuperSimpleDetElement : public DetectorElementBase {
public:
  /// Constructor for single sided detector element
  /// - bound to a Plane Surface
  ///
  /// @param identifier is the module identifier
  /// @param transform is the transform that element the layer in 3D frame
  /// @param pBounds is the planar bounds for the planar detector element
  /// @param thickness is the module thickness
  /// @param material is the (optional) Surface material associated to it
  SuperSimpleDetElement(const Identifier identifier,
                        std::shared_ptr<Transform3D>
                            transform,
                        std::shared_ptr<const CylinderBounds>
                            pBounds,
                        double thickness,
                        std::shared_ptr<const SurfaceMaterial> material = nullptr)
      : DetectorElementBase(),
        m_elementIdentifier(std::move(identifier)),
        m_elementTransform(std::move(transform)),
        m_elementSurface(new DetCylinderSurface(pBounds, *this, identifier)),
        m_elementThickness(thickness),
        m_elementSurfaces({m_elementSurface}) {
    m_elementSurface->setAssociatedMaterial(material);
  }
  void assignIdentifier(const Identifier& /*identifier*/) const {
    // m_elementIdentifier=identifier; would not be const
  }

  ///  Destructor
  ~SuperSimpleDetElement() { /*nop */
  }

  /// Identifier
  Identifier identify() const override { return m_elementIdentifier; };

  /// Return local to global transform associated with this identifier
  ///
  /// @note this is called from the surface().transform() in the PROXY mode
  ///
  /// @param identifier is ignored for this simple detector element
  const Transform3D& transform(const Identifier& identifier = Identifier()) const override {
    return *m_elementTransform;
  };

  /// Return surface associated with this identifier,
  ///
  /// @param identifier is ignored in this case
  ///
  /// @param identifier is ignored for this simple detector element
  const Surface& surface(const Identifier& identifier = Identifier()) const override { return *m_elementSurface; };

  /// Returns the full list of all detection surfaces associated
  /// to this detector element
  const std::vector<std::shared_ptr<const Surface>>& surfaces() const override { return m_elementSurfaces; };

  /// The maximal thickness of the detector element wrt normal axis
  double thickness() const override { return m_elementThickness; };

private:
  /// the element representation
  /// identifier
  Identifier m_elementIdentifier;
  /// the transform for positioning in 3D space
  std::shared_ptr<Transform3D> m_elementTransform;
  /// the surface represented by it
  std::shared_ptr<const Surface> m_elementSurface;
  /// the element thickness
  double m_elementThickness;

  /// the cache
  std::vector<std::shared_ptr<const Surface>> m_elementSurfaces;
  /// store either
};

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
