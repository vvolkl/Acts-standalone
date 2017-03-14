
#include "ACTS/Detector/TrackingGeometry.hpp"
#include "ACTS/EventData/Measurement.hpp"
#include "ACTS/Examples/BuildGenericDetector.hpp"
#include "ACTS/Extrapolation/ExtrapolationCell.hpp"
#include "ACTS/Extrapolation/ExtrapolationEngine.hpp"
#include "ACTS/Extrapolation/IExtrapolationEngine.hpp"
#include "ACTS/Extrapolation/MaterialEffectsEngine.hpp"
#include "ACTS/Extrapolation/RungeKuttaEngine.hpp"
#include "ACTS/Extrapolation/StaticEngine.hpp"
#include "ACTS/Extrapolation/StaticNavigationEngine.hpp"
#include "ACTS/Fitter/KalmanFitter.hpp"
#include "ACTS/Fitter/KalmanUpdator.hpp"
#include "ACTS/MagneticField/ConstantBField.hpp"
#include "ACTS/Surfaces/PerigeeSurface.hpp"
#include "ACTS/Utilities/Definitions.hpp"
#include "ACTS/Utilities/Logger.hpp"
#include "ACTS/Utilities/Units.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>

#include "SuperSimpleDetector.hpp"

using namespace Acts;

std::shared_ptr<IExtrapolationEngine> initExtrapolator(const std::shared_ptr<const TrackingGeometry>& geo) {
  auto propConfig = RungeKuttaEngine<>::Config();
  propConfig.fieldService = std::make_shared<ConstantBField>(0, 0, 0.001);
  auto propEngine = std::make_shared<RungeKuttaEngine<>>(propConfig);

  auto matConfig = MaterialEffectsEngine::Config();
  auto materialEngine = std::make_shared<MaterialEffectsEngine>(matConfig);

  auto navConfig = StaticNavigationEngine::Config();
  navConfig.propagationEngine = propEngine;
  navConfig.materialEffectsEngine = materialEngine;
  navConfig.trackingGeometry = geo;
  auto navEngine = std::make_shared<StaticNavigationEngine>(navConfig);

  auto statConfig = StaticEngine::Config();
  statConfig.propagationEngine = propEngine;
  statConfig.navigationEngine = navEngine;
  statConfig.materialEffectsEngine = materialEngine;
  auto statEngine = std::make_shared<StaticEngine>(statConfig);

  auto exEngineConfig = ExtrapolationEngine::Config();
  exEngineConfig.trackingGeometry = geo;
  exEngineConfig.propagationEngine = propEngine;
  exEngineConfig.navigationEngine = navEngine;
  exEngineConfig.extrapolationEngines = {statEngine};
  auto exEngine = std::make_shared<ExtrapolationEngine>(exEngineConfig);
  exEngine->setLogger(getDefaultLogger("ExtrapolationEngine", Logging::VERBOSE));

  return exEngine;
}

int main(int argc, char** argv) {
  std::istringstream ss(argv[1]);
  float par_qt;
  ss >> par_qt;
  std::ofstream myfile;
  myfile.open("test.dat");
  // std::shared_ptr<const Acts::TrackingGeometry> geo
  //    = Acts::buildGenericDetector(Acts::Logging::DEBUG,
  //                                 Acts::Logging::DEBUG,
  //                                 Acts::Logging::DEBUG,
  //                                 0);
  std::shared_ptr<const Acts::TrackingGeometry> geo = buildSuperSimpleDetector();
  std::uniform_real_distribution<double> std_loc1(1, 5);
  std::uniform_real_distribution<double> random_angle(0.0, 1.57);
  std::normal_distribution<double> g(0, 1);
  std::default_random_engine m_e;
  for (int i = 0; i < 1; ++i) {

    double phi = 0;
    double theta = M_PI * 0.3;
    double pT = par_qt;
    double x = 0;
    double y = 0;
    double z = 0;
    double px = pT * cos(phi);
    double py = pT * sin(phi);
    double pz = pT / tan(theta);
    double q = 1;  //(charge != 0) ? charge : +1;
    Vector3D pos(x, y, z);
    Vector3D mom(px, py, pz);

    auto startCov =
        std::make_unique<ActsSymMatrix<ParValue_t, NGlobalPars>>(ActsSymMatrix<ParValue_t, NGlobalPars>::Identity());

    // const Surface* pSurf   = geo->getBeamline();
    auto startTP = std::make_unique<SingleCurvilinearTrackParameters<Acts::ChargedPolicy>>(
        std::move(startCov), std::move(pos), std::move(mom), q);

    ExtrapolationCell<TrackParameters> exCell(*startTP);
    exCell.addConfigurationMode(ExtrapolationMode::CollectSensitive);
    exCell.addConfigurationMode(ExtrapolationMode::StopAtBoundary);

    auto exEngine = initExtrapolator(geo);
    exEngine->extrapolate(exCell);

    for (const auto& step : exCell.extrapolationSteps) {

      const auto& tp = step.parameters;
      // if (tp->associatedSurface().type() != Surface::Cylinder) continue;
      myfile << tp->position().x() << "\t" << tp->position().y() << "\t" << tp->position().z() << std::endl;
      std::cout << tp->position().x() << "\t" << tp->position().y() << "\t" << tp->position().z() << std::endl;
    }
  }

  myfile.close();
  return 0;
}
