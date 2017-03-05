#include "KalmanFitUtils.h"

MyExtrapolator::MyExtrapolator(std::shared_ptr<const IExtrapolationEngine> exEngine)
    : m_exEngine(std::move(exEngine))
  {
  };

  MyCache
  MyExtrapolator::operator()(const FitMeas_t& m, const TrackParameters& tp) const
  {
    auto exCell = std::make_unique<ExtrapolationCell<TrackParameters>>(tp);
    exCell->addConfigurationMode(ExtrapolationMode::Destination);
    exCell->addConfigurationMode(ExtrapolationMode::FATRAS);
    exCell->addConfigurationMode(ExtrapolationMode::CollectJacobians);
    const Surface& sf = getSurface(m);

    m_exEngine->extrapolate(*exCell, &sf);
    MyCache c;
    auto    j = exCell->extrapolationSteps.back().transportJacobian.release();
    c.jacobian.reset(new KF::Step<long int>::JacobianMatrix(*j));
    auto pars
        = static_cast<const BoundParameters*>(exCell->leadParameters->clone());
    c.parameters.reset(pars);

    return c;
  };

  std::unique_ptr<const FitMeas_t>
  NoCalibration::operator()(const FitMeas_t& m, const BoundParameters&) const
  {
    return std::make_unique<const FitMeas_t>(m);
  };

  std::unique_ptr<KF::Step<long int>>
  CacheGenerator::operator()(MyCache m) const
  {
    auto step = std::make_unique<KF::Step<long int>>();
    step->setPredictedState(std::move(m.parameters));
    step->setJacobian(std::move(m.jacobian));

    return step;
  };

std::shared_ptr<IExtrapolationEngine>
initExtrapolator(const std::shared_ptr<const TrackingGeometry>& geo)
{
  auto propConfig         = RungeKuttaEngine<>::Config();
  propConfig.fieldService = std::make_shared<ConstantBField>(0, 0, 0.006);
  auto propEngine         = std::make_shared<RungeKuttaEngine<>>(propConfig);

  auto matConfig      = MaterialEffectsEngine::Config();
  auto materialEngine = std::make_shared<MaterialEffectsEngine>(matConfig);

  auto navConfig                  = StaticNavigationEngine::Config();
  navConfig.propagationEngine     = propEngine;
  navConfig.materialEffectsEngine = materialEngine;
  navConfig.trackingGeometry      = geo;
  auto navEngine = std::make_shared<StaticNavigationEngine>(navConfig);

  auto statConfig                  = StaticEngine::Config();
  statConfig.propagationEngine     = propEngine;
  statConfig.navigationEngine      = navEngine;
  statConfig.materialEffectsEngine = materialEngine;
  auto statEngine                  = std::make_shared<StaticEngine>(statConfig);

  auto exEngineConfig                 = ExtrapolationEngine::Config();
  exEngineConfig.trackingGeometry     = geo;
  exEngineConfig.propagationEngine    = propEngine;
  exEngineConfig.navigationEngine     = navEngine;
  exEngineConfig.extrapolationEngines = {statEngine};
  auto exEngine = std::make_shared<ExtrapolationEngine>(exEngineConfig);
  exEngine->setLogger(
      getDefaultLogger("ExtrapolationEngine", Logging::VERBOSE));

  return exEngine;
};
