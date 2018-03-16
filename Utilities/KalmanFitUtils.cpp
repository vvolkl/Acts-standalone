#include "KalmanFitUtils.hpp"

void dumpTrackingVolume(const Acts::TrackingVolume* vol) {
  std::cout << "Dumping TrackingVolume" << vol->volumeName() << std::endl;
  ;
  auto volVec = vol->confinedVolumes();
  if (nullptr != volVec) {
    auto objVec = volVec->arrayObjects();
    for (auto o : objVec) {
      dumpTrackingVolume(o.get());
    }
  }
  auto layArr = vol->confinedLayers();
  if (nullptr != layArr) {
    auto layVec = layArr->arrayObjects();
    std::cout << "\t containing " << layVec.size() << " Layers" << std::endl;
    for (auto l : layVec) {
      dumpTrackingLayer(l.get());
    }
  }
}

void dumpTrackingLayer(const Acts::Layer* layer) {
  std::cout << "\t\t layer has geoID " << layer->geoID() << std::endl;
  auto surfArr = layer->surfaceArray();

  if (nullptr != surfArr) {
    auto surfVec = surfArr->arrayObjects();
    std::cout << "\t\t layer contains " << surfVec.size() << " TrackingSurfaces" << std::endl;
    for (auto s : surfVec) {
      std::cout << "\t\t\t" << s << std::endl;
    }
  }
}

MyExtrapolator::MyExtrapolator(std::shared_ptr<const IExtrapolationEngine> exEngine)
    : m_exEngine(std::move(exEngine)){};

MyCache MyExtrapolator::operator()(const FitMeas_t& m, const TrackParameters& tp) const {
  auto exCell = std::make_unique<ExtrapolationCell<TrackParameters>>(tp);
  /*
  exCell.addConfigurationMode(ExtrapolationMode::CollectMaterial);
  exCell->addConfigurationMode(ExtrapolationMode::CollectSensitive);
  exCell->addConfigurationMode(ExtrapolationMode::Destination);
  exCell->addConfigurationMode(ExtrapolationMode::StopAtBoundary);
  exCell->addConfigurationMode(ExtrapolationMode::Destination);
  exCell->addConfigurationMode(ExtrapolationMode::FATRAS);
  exCell->addConfigurationMode(ExtrapolationMode::CollectJacobians);
  exCell->addConfigurationMode(ExtrapolationMode::CollectPassive);
  exCell->addConfigurationMode(ExtrapolationMode::CollectBoundary);
  exCell->addConfigurationMode(ExtrapolationMode::CollectMaterial);
  exCell->addConfigurationMode(ExtrapolationMode::CollectSensitive);
  */

  exCell->addConfigurationMode(ExtrapolationMode::Destination);
  exCell->addConfigurationMode(ExtrapolationMode::FATRAS);
  exCell->addConfigurationMode(ExtrapolationMode::CollectJacobians);
  //(*exCell).pathLimit=500;
  const Surface& sf = getSurface(m);

  m_exEngine->extrapolate(*exCell, &sf);
  MyCache c;
  auto j = exCell->extrapolationSteps.back().transportJacobian.release();
  c.jacobian.reset(new KF::Step<long int>::JacobianMatrix(*j));
  auto pars = static_cast<const BoundParameters*>(exCell->leadParameters->clone());
  c.parameters.reset(pars);

  return c;
};

std::unique_ptr<const FitMeas_t> NoCalibration::operator()(const FitMeas_t& m, const BoundParameters&) const {
  return std::make_unique<const FitMeas_t>(m);
};

std::unique_ptr<KF::Step<long int>> CacheGenerator::operator()(MyCache m) const {
  auto step = std::make_unique<KF::Step<long int>>();
  step->setPredictedState(std::move(m.parameters));
  step->setJacobian(std::move(m.jacobian));

  return step;
};

std::shared_ptr<IExtrapolationEngine> initExtrapolator(const std::shared_ptr<const TrackingGeometry>& geo) {
  auto propConfig = RungeKuttaEngine<>::Config();
  propConfig.fieldService = std::make_shared<ConstantBField>(0, 0, 2 * Acts::units::_T);
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
  exEngine->setLogger(getDefaultLogger("ExtrapolationEngine", Logging::INFO));

  return exEngine;
};
