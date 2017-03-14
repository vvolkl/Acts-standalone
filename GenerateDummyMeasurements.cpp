
#include "KalmanFitUtils.h"

std::vector<FitMeas_t> generateDummyMeasurements(BoundParameters theTrackParameters,
                                                 std::shared_ptr<IExtrapolationEngine> theExtrapolationEngine,
                                                 std::shared_ptr<const Acts::TrackingGeometry> geo,
                                                 const Acts::Layer* startLayer, std::ostream& log) {
  log << "starting extrapolation ... " << std::endl;

  ExtrapolationCell<TrackParameters> exCell(theTrackParameters);
  exCell.addConfigurationMode(ExtrapolationMode::CollectSensitive);
  // exCell.addConfigurationMode(ExtrapolationMode::Destination);
  // exCell.startLayer = startLayer;
  exCell.addConfigurationMode(ExtrapolationMode::StopAtBoundary);

  // const Acts::Layer* theLayer = geo->highestTrackingVolume()->confinedLayers()->arrayObjects()[5].get();
  // const Acts::Surface* sf =
  // (geo->highestTrackingVolume()->confinedLayers()->arrayObjects()[5]->surfaceArray()->arrayObjects()[0]);
  theExtrapolationEngine->extrapolate(exCell);

  std::vector<FitMeas_t> vMeasurements;
  vMeasurements.reserve(exCell.extrapolationSteps.size());

  // identifier
  long int id = 0;

  for (const auto& step : exCell.extrapolationSteps) {
    const auto& tp = step.parameters;

    log << "New extrapolationStep!" << std::endl;

    log << &(tp->referenceSurface()) << std::endl;

    auto lPtr = tp->referenceSurface().associatedLayer();
    auto detPtr = tp->referenceSurface().associatedDetectorElement();
    log << "the surface has an assoc layer: " << lPtr << std::endl;
    log << "the surface has an assoc detel: " << detPtr << std::endl;
    if (tp->referenceSurface().type() == Surface::Plane || tp->referenceSurface().type() == Surface::Disc) {
      log << "it is a plane or a disc" << std::endl;
    }
    if (detPtr != nullptr) {
      /*
      if (tp->referenceSurface().type() != Surface::Cylinder) {
          //continue;
      }
      if (lPtr == nullptr) {
      //continue;
      }
      */

      double std1 = 0.01;
      double std2 = 0.01;
      double l1 = tp->get<eLOC_0>();  // +  g(e);
      double l2 = tp->get<eLOC_1>();  // +  g(e);
      std::cout << l1 << "\t" << l2 << std::endl;
      ActsSymMatrixD<2> cov;
      cov << std1 * std1, 0, 0, std2 * std2;
      // cov << 0.00000001, 0, 0, 0.0000001;

      vMeasurements.push_back(Meas_t<eLOC_0, eLOC_1>(tp->referenceSurface(), id, std::move(cov), l1, l2));
      ++id;
    }
  }

  log << "created " << vMeasurements.size() << " pseudo-measurements" << std::endl;
  for (const auto& m : vMeasurements)
    log << m << std::endl << std::endl;
  return vMeasurements;
}
