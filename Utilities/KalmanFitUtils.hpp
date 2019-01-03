#ifndef KALMANFITUTILS_H
#define KALMANFITUTILS_H
#include "Acts/Detector/TrackingGeometry.hpp"
#include "Acts/EventData/Measurement.hpp"
#include "Acts/Extrapolation/ExtrapolationCell.hpp"
#include "Acts/Extrapolation/ExtrapolationEngine.hpp"
#include "Acts/Extrapolation/IExtrapolationEngine.hpp"
#include "Acts/Extrapolation/MaterialEffectsEngine.hpp"
#include "Acts/Extrapolation/RungeKuttaEngine.hpp"
#include "Acts/Extrapolation/StaticEngine.hpp"
#include "Acts/Extrapolation/StaticNavigationEngine.hpp"
#include "Acts/Fitter/KalmanFitter.hpp"
#include "Acts/Fitter/KalmanUpdator.hpp"
#include "Acts/MagneticField/ConstantBField.hpp"
#include "Acts/Surfaces/PerigeeSurface.hpp"
#include "Acts/Utilities/Definitions.hpp"
#include "Acts/Utilities/Logger.hpp"
#include "Acts/Utilities/Units.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

using namespace Acts;
typedef FittableMeasurement<long int> FitMeas_t;
template <ParID_t... pars>
using Meas_t = Measurement<long int, pars...>;

struct MyCache {
  std::unique_ptr<const KF::Step<long int>::JacobianMatrix> jacobian;
  std::unique_ptr<const BoundParameters> parameters;

  MyCache() = default;
  MyCache(const MyCache&) = delete;
  MyCache(MyCache&&) = default;
};

class MyExtrapolator {
public:
  MyExtrapolator(std::shared_ptr<const IExtrapolationEngine> exEngine = nullptr);

  MyCache operator()(const FitMeas_t& m, const TrackParameters& tp) const;

private:
  std::shared_ptr<const IExtrapolationEngine> m_exEngine;
};

class NoCalibration {
public:
  std::unique_ptr<const FitMeas_t> operator()(const FitMeas_t& m, const BoundParameters&) const;
};

class CacheGenerator {
public:
  std::unique_ptr<KF::Step<long int>> operator()(MyCache m) const;
};

std::shared_ptr<IExtrapolationEngine> initExtrapolator(const std::shared_ptr<const TrackingGeometry>& geo);

std::vector<FitMeas_t> generateDummyMeasurements(BoundParameters theTrackParameters,
                                                 std::shared_ptr<IExtrapolationEngine> theExtrapolationEngine,
                                                 std::shared_ptr<const Acts::TrackingGeometry> geo,
                                                 double std1,
                                                 const Acts::Layer* startLayer = nullptr,
                                                 std::ostream& log = std::cout
                                                 );

void dumpTrackingVolume(const Acts::TrackingVolume* vol);
void dumpTrackingLayer(const Acts::Layer* layer);

void saveDummyHits(std::ostream& log, std::vector<FitMeas_t> measurements);
void read_config(std::string filename, std::map<std::string, std::vector<double>>& config);
#endif
