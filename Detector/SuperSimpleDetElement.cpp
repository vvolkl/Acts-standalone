#include "SuperSimpleDetElement.hpp"

SuperSimpleDetElement::SuperSimpleDetElement(const Identifier identifier, std::shared_ptr<Transform3D> transform, std::shared_ptr<const CylinderBounds> pBounds, double thickness, std::shared_ptr<const SurfaceMaterial> material)
    : DetectorElementBase(),
      m_elementIdentifier(std::move(identifier)),
      m_elementTransform(std::move(transform)),
      m_elementSurface(new DetCylinderSurface(pBounds, *this, identifier)),
      m_elementThickness(thickness),
      m_elementSurfaces({m_elementSurface}) {
      //auto mutableSurface = std::const_pointer_cast<Surface>(m_elementSurface);
       //   mutableSurface->setAssociatedMaterial(material);

}
SuperSimpleDetElement::SuperSimpleDetElement(const Identifier identifier, std::shared_ptr<Transform3D> transform, std::shared_ptr<const RadialBounds> pBounds, double thickness, std::shared_ptr<const SurfaceMaterial> material)
    : DetectorElementBase(),
      m_elementIdentifier(std::move(identifier)),
      m_elementTransform(std::move(transform)),
      m_elementSurface(new DiscSurface(pBounds, *this, identifier)),
      m_elementThickness(thickness),
      m_elementSurfaces({m_elementSurface}) {
      //auto mutableSurface = std::const_pointer_cast<Surface>(m_elementSurface);
       //   mutableSurface->setAssociatedMaterial(material);

}

///  Destructor
SuperSimpleDetElement::~SuperSimpleDetElement() { /*nop */
}

/// Identifier
Identifier SuperSimpleDetElement::identify() const { return m_elementIdentifier; };

/// Return local to global transform associated with this identifier
///
/// @note this is called from the surface().transform() in the PROXY mode
///
/// @param identifier is ignored for this simple detector element
const Transform3D& SuperSimpleDetElement::transform(const Identifier& identifier) const { return *m_elementTransform; };

/// Return surface associated with this identifier,
///
/// @param identifier is ignored in this case
///
/// @param identifier is ignored for this simple detector element
const Surface& SuperSimpleDetElement::surface(const Identifier& identifier) const { return *m_elementSurface; };

/// Returns the full list of all detection surfaces associated
/// to this detector element
const std::vector<std::shared_ptr<const Surface>>& SuperSimpleDetElement::surfaces() const {
  return m_elementSurfaces;
};

/// The maximal thickness of the detector element wrt normal axis
double SuperSimpleDetElement::thickness() const { return m_elementThickness; };
