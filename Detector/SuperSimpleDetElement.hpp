
#include "ACTS/Examples/GenericLayerBuilder.hpp"
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
#include "ACTS/Material/Material.hpp"
#include "ACTS/Surfaces/CylinderBounds.hpp"
#include "ACTS/Surfaces/DiscSurface.hpp"
#include "ACTS/Surfaces/RadialBounds.hpp"
#include "ACTS/Tools/LayerArrayCreator.hpp"
#include "ACTS/Tools/TrackingVolumeArrayCreator.hpp"
#include "ACTS/Utilities/BinUtility.hpp"
#include "ACTS/Utilities/BinnedArrayXD.hpp"
#include "ACTS/Volumes/CylinderVolumeBounds.hpp"
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
