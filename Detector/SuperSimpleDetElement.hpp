
#include "Acts/Examples/GenericLayerBuilder.hpp"
#include "Acts/Material/Material.hpp"
#include "Acts/Tools/CylinderVolumeBuilder.hpp"
#include "Acts/Tools/CylinderVolumeHelper.hpp"
#include "Acts/Tools/LayerArrayCreator.hpp"
#include "Acts/Tools/LayerCreator.hpp"
#include "Acts/Tools/SurfaceArrayCreator.hpp"
#include "Acts/Tools/TrackingGeometryBuilder.hpp"
#include "Acts/Tools/TrackingVolumeArrayCreator.hpp"
#include "Acts/Utilities/Identifier.hpp"
#include "Acts/Utilities/Units.hpp"

#include "Acts/Detector/DetectorElementBase.hpp"
#include "Acts/Detector/TrackingGeometry.hpp"
#include "Acts/Detector/TrackingVolume.hpp"
#include "Acts/Layers/CylinderLayer.hpp"
#include "Acts/Material/Material.hpp"
#include "Acts/Surfaces/CylinderBounds.hpp"
#include "Acts/Surfaces/DiscSurface.hpp"
#include "Acts/Surfaces/RadialBounds.hpp"
#include "Acts/Tools/LayerArrayCreator.hpp"
#include "Acts/Tools/TrackingVolumeArrayCreator.hpp"
#include "Acts/Utilities/BinUtility.hpp"
#include "Acts/Utilities/BinnedArrayXD.hpp"
#include "Acts/Volumes/CylinderVolumeBounds.hpp"
#include <iostream>
#include <vector>

#include "Detector/DetCylinderSurface.hpp"
using namespace Acts;

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
                        std::shared_ptr<const SurfaceMaterial> material = nullptr);
  SuperSimpleDetElement(const Identifier identifier,
                        std::shared_ptr<Transform3D>
                            transform,
                        std::shared_ptr<const RadialBounds>
                            pBounds,
                        double thickness,
                        std::shared_ptr<const SurfaceMaterial> material = nullptr);

  ///  Destructor
  ~SuperSimpleDetElement();

  /// Identifier
  Identifier identify() const override;
  void assignIdentifier(const Identifier& identifier)  override{};

  /// Return local to global transform associated with this identifier
  ///
  /// @note this is called from the surface().transform() in the PROXY mode
  ///
  /// @param identifier is ignored for this simple detector element
  const Transform3D& transform(const Identifier& identifier = Identifier()) const override;

  /// Return surface associated with this identifier,
  ///
  /// @param identifier is ignored in this case
  ///
  /// @param identifier is ignored for this simple detector element
  const Surface& surface(const Identifier& identifier = Identifier()) const override;

  /// Returns the full list of all detection surfaces associated
  /// to this detector element
  const std::vector<std::shared_ptr<const Surface>>& surfaces() const override;

  /// The maximal thickness of the detector element wrt normal axis
  double thickness() const override;

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
