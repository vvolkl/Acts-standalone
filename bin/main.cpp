// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

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

#include "Utilities/KalmanFitUtils.hpp"
#include "Detector/SimpleDetector.hpp"
#include "Detector/SuperSimpleDetector.hpp"

using namespace Acts;

int main() {
  std::ofstream myfile;
  std::ofstream tpfile;
  myfile.open("test.dat");
  tpfile.open("tp.dat");
  std::default_random_engine e;
  e.seed(123124);
  std::uniform_real_distribution<double> std_loc1(1, 5);
  std::uniform_real_distribution<double> std_loc2(0.1, 2);
  std::shared_ptr<const Acts::TrackingGeometry> geo = Acts::buildSuperSimpleDetector();
  //= Acts::buildGenericDetector(Acts::Logging::VERBOSE, Acts::Logging::VERBOSE, Acts::Logging::VERBOSE, 1);
  //= Acts::buildSimpleDetector(Acts::Logging::VERBOSE, Acts::Logging::VERBOSE, Acts::Logging::VERBOSE, 0);

  dumpTrackingVolume(geo->highestTrackingVolume());

  const Surface* pSurf = geo->getBeamline();
  std::cout << "beamline ptr: " << pSurf << std::endl;
  std::cout << geo->highestTrackingVolume()->volumeName() << std::endl;
  double phi = 0.0;
  double theta = M_PI * 0.5;
  double std1, std2, l1, l2;
  // std::vector<double> uncertaintyVector {10, 1, 0.1, 0.01, 0.001};
  std::vector<double> uncertaintyVector{1};
  // std::vector<double> pTVector {1, 5, 10, 100, 1000};
  std::vector<double> pTVector{1000};
  std::vector<double> etaVector{};
  for (double eta = 0; eta < 0.2; eta += 0.2) {
    etaVector.push_back(eta);
  }
  for (double eta : etaVector) {
    std::cout << eta << std::endl;
    double tanhEtaSq = std::tanh(eta) * std::tanh(eta);
    for (double pT : pTVector) {

      double pZ = std::sqrt(pT * pT * tanhEtaSq / (1. - tanhEtaSq));
      for (double std1 : uncertaintyVector) {
        std2 = std1;
        std::normal_distribution<double> g(0, 0.000001);
        std::normal_distribution<double> g2(0, std2);
        // pT += g2(e);
        double x = 0;
        double y = 0;
        double z = 0;
        double px = pT * cos(phi);
        double py = pT * sin(phi);
        double pz = pZ;  // pT / tan(theta);
        double q = 1;    //(charge != 0) ? charge : +1;
        Vector3D pos(x, y, z);
        Vector3D mom(px, py, pz);

        auto startCov = std::make_unique<ActsSymMatrix<ParValue_t, NGlobalPars>>(
            ActsSymMatrix<ParValue_t, NGlobalPars>::Identity());
        // std::cout << "startcov" << std::endl;
        (*startCov)(4, 4) = std2 * std2;
        // std::cout << *startCov << std::endl;

        auto startTP =
            std::make_unique<BoundParameters>(std::move(startCov), std::move(pos), std::move(mom), q, *pSurf);

        auto exEngine = initExtrapolator(geo);
        // const Acts::Layer* startLayer = (geo->highestTrackingVolume()->confinedLayers()->arrayObjects()[0]).get();
        std::vector<FitMeas_t> vMeasurements = generateDummyMeasurements(*startTP, exEngine, geo);

        KalmanFitter<MyExtrapolator, CacheGenerator, NoCalibration, GainMatrixUpdator> KF;
        KF.m_oCacheGenerator = CacheGenerator();
        KF.m_oCalibrator = NoCalibration();
        KF.m_oExtrapolator = MyExtrapolator(exEngine);
        KF.m_oUpdator = GainMatrixUpdator();

        if (vMeasurements.size() > 1) {
          std::cout << "start fit" << std::endl;
          auto track = KF.fit(vMeasurements, std::move(startTP));
          std::cout << "dump track" << std::endl;

          // dump track
          int trackCounter = 0;
          for (const auto& p : track) {
            std::cout << "new track state! " << std::endl;
            auto smoothedState = *p->getSmoothedState();
            auto filteredState = *p->getFilteredState();
            std::cout << "\t LayerPtr: " << filteredState.referenceSurface().associatedLayer() << std::endl;
            std::cout << "\t assoc detel: " << filteredState.referenceSurface().associatedDetectorElement()
                      << std::endl;
            if (true) {  // trackCounter == track.size() - 1) {
              // std::cout << *p->getCalibratedMeasurement() << std::endl;
              std::cout << smoothedState << std::endl;
              // std::cout << smoothedState.parameters()[4] << std::endl;
              auto cov = *smoothedState.covariance();
              double fitres_pt = std::sqrt(smoothedState.momentum().x() * smoothedState.momentum().x() +
                                           smoothedState.momentum().y() * smoothedState.momentum().y());
              // tpfile << std1 << "\t" << std::sqrt(cov(4,4)) << "\t" << 1. / smoothedState.parameters()[4] <<
              // std::endl;
              tpfile << pT << "\t" << fitres_pt << "\t" << pz << "\t" << smoothedState.momentum().z() << "\t"
                     << std::sqrt(cov(4, 4)) << "\t" << std1 << "\t" << eta << "\t" << vMeasurements.size()
                     << std::endl;
            }
            myfile << eta << "\t" << pT << "\t" << filteredState.position().x() << "\t" << filteredState.position().y()
                   << "\t" << filteredState.position().z() << std::endl;
            trackCounter++;
          }
        }
      }
    }
  }
  myfile.close();
  tpfile.close();

  return 0;
}
