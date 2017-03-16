
#include "Utilities/KalmanFitUtils.hpp"

std::vector<FitMeas_t> generateDummyMeasurements(BoundParameters theTrackParameters,
                                                 std::shared_ptr<IExtrapolationEngine> theExtrapolationEngine,
                                                 std::shared_ptr<const Acts::TrackingGeometry> geo,
                                                 const Acts::Layer* startLayer, std::ostream& log) {

  double pT = std::sqrt(theTrackParameters.momentum().x() *theTrackParameters.momentum().x() + theTrackParameters.momentum().y() * theTrackParameters.momentum().y());

  ExtrapolationCell<TrackParameters> exCell(theTrackParameters);
  exCell.addConfigurationMode(ExtrapolationMode::CollectSensitive);
  exCell.addConfigurationMode(ExtrapolationMode::StopAtBoundary);
  theExtrapolationEngine->extrapolate(exCell);

  std::vector<FitMeas_t> vMeasurements;
  vMeasurements.reserve(exCell.extrapolationSteps.size());

  // identifier
  long int id = 0;

  for (const auto& step : exCell.extrapolationSteps) {
    const auto& tp = step.parameters;

    auto lPtr = tp->referenceSurface().associatedLayer();
    auto detPtr = tp->referenceSurface().associatedDetectorElement();
    if (tp->referenceSurface().type() == Surface::Plane || tp->referenceSurface().type() == Surface::Disc) {
    }
    if (detPtr != nullptr) {
      log << tp->position().x() << "\t" << tp->position().y()
             << "\t" << tp->position().z() << "\t" << pT << "\t" <<  theTrackParameters.momentum().z() <<   std::endl;

      double std1 = 0.01;
      double std2 = 0.01;
      double l1 = tp->get<eLOC_0>();  // +  g(e);
      double l2 = tp->get<eLOC_1>();  // +  g(e);
      //std::cout << l1 << "\t" << l2 << std::endl;
      ActsSymMatrixD<2> cov;
      cov << std1 * std1, 0, 0, std2 * std2;

      vMeasurements.push_back(Meas_t<eLOC_0, eLOC_1>(tp->referenceSurface(), id, std::move(cov), l1, l2));
      ++id;
    }
  }
  return vMeasurements;
}
