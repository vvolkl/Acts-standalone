// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

//-------------------------------------------------------------------------------------
// beam pipe
//-------------------------------------------------------------------------------------
// configure the beam pipe layer builder
PassiveLayerBuilder::Config bplConfig;
bplConfig.layerIdentification = "BeamPipe";
bplConfig.centralLayerRadii = std::vector<double>(1, 15.);
bplConfig.centralLayerHalflengthZ = std::vector<double>(1, 400.);
bplConfig.centralLayerThickness = std::vector<double>(1, 0.00001);
bplConfig.centralLayerMaterial = {Material(352.8, 407., 9.012, 4., 1.848e-3)};
auto beamPipeBuilder =
    std::make_shared<PassiveLayerBuilder>(bplConfig, getDefaultLogger("BeamPipeLayerBuilder", layerLLevel));
// create the volume for the beam pipe
CylinderVolumeBuilder::Config bpvConfig;
bpvConfig.trackingVolumeHelper = cylinderVolumeHelper;
bpvConfig.volumeName = "BeamPipe";
bpvConfig.layerBuilder = beamPipeBuilder;
bpvConfig.layerEnvelopeR = {0. * Acts::units::_mm, 0. * Acts::units::_mm};
bpvConfig.buildToRadiusZero = true;
bpvConfig.volumeSignature = 0;
auto beamPipeVolumeBuilder =
    std::make_shared<CylinderVolumeBuilder>(bpvConfig, getDefaultLogger("BeamPipeVolumeBuilder", volumeLLevel));
// add to the list of builders
volumeBuilders.push_back(beamPipeVolumeBuilder);
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
// pixel detector
//-------------------------------------------------------------------------------------
// configure pixel layer builder
GenericLayerBuilder::Config plbConfig;
plbConfig.layerCreator = layerCreator;
plbConfig.layerIdentification = "Pixel";
// fill necessary vectors for configuration
//-------------------------------------------------------------------------------------
// some prep work
// envelope double
std::pair<double, double> pcEnvelope(0.000001, 0.000001);
// Layer material properties - thickness, X0, L0, A, Z, Rho
MaterialProperties pcmProperties(1., 95.7, 465.2, 28.03, 14., 2.32e-3);
// Module material - X0, L0, A, Z, Rho
Material pcMaterial(95.7, 465.2, 28.03, 14., 2.32e-3);

// standard, an approach envelope
plbConfig.approachSurfaceEnvelope = 0.5;

plbConfig.centralLayerBinMultipliers = {1, 1};
plbConfig.centralLayerRadii = {25., 60, 100};
plbConfig.centralLayerEnvelopes = {pcEnvelope, pcEnvelope, pcEnvelope};
plbConfig.centralLayerMaterialConcentration = {1, 1, 1};
plbConfig.centralLayerMaterialProperties = {pcmProperties, pcmProperties, pcmProperties};
plbConfig.centralModuleBinningSchema = {{14, 13}, {16, 13}, {26,13}};
plbConfig.centralModuleTiltPhi = {0.1, 0.0, 0.1};
//plbConfig.centralModuleTiltPhi = {0.0, 0.0, 0.0};
plbConfig.centralModuleHalfX = {6.4, 10.5, 12.8};//{6.4, 12.8, 12.8};
plbConfig.centralModuleHalfY = {32., 32., 32};
plbConfig.centralModuleThickness = {0.15, 0.05, 0.15};
plbConfig.centralModuleReadoutBinsX = {336, 336, 226};
plbConfig.centralModuleReadoutBinsY = {1280, 1280, 1280};
plbConfig.centralModuleReadoutSide = {-1, -1, -1};
plbConfig.centralModuleLorentzAngle = {0.12, 0.12, 0.12};
// no frontside/backside
plbConfig.centralModuleFrontsideStereo = {};
plbConfig.centralModuleBacksideStereo = {};
plbConfig.centralModuleBacksideGap = {};
// mPositions
std::vector<std::vector<Vector3D>> centralModulePositions;
for (size_t plb = 0; plb < plbConfig.centralLayerRadii.size(); ++plb) {
  // call the helper function
  centralModulePositions.push_back(modulePositionsCylinder(plbConfig.centralLayerRadii[plb],
                                                           0.0,  // 1 mm stagger
                                                           plbConfig.centralModuleHalfY[plb],
                                                           0.,  // 2 mm module overlap
                                                           0.,
                                                           plbConfig.centralModuleBinningSchema[plb]));
}
plbConfig.centralModulePositions = centralModulePositions;

// define the builder
auto pixelLayerBuilder =
    std::make_shared<GenericLayerBuilder>(plbConfig, getDefaultLogger("PixelLayerBuilder", layerLLevel));
//-------------------------------------------------------------------------------------
// build the pixel volume
CylinderVolumeBuilder::Config pvbConfig;
pvbConfig.trackingVolumeHelper = cylinderVolumeHelper;
pvbConfig.volumeName = "Pixel";
pvbConfig.buildToRadiusZero = false;
pvbConfig.layerEnvelopeR = {3* Acts::units::_mm, 5. * Acts::units::_mm};
pvbConfig.layerBuilder = pixelLayerBuilder;
pvbConfig.volumeSignature = 0;
auto pixelVolumeBuilder =
    std::make_shared<CylinderVolumeBuilder>(pvbConfig, getDefaultLogger("PixelVolumeBuilder", volumeLLevel));
// add to the list of builders
volumeBuilders.push_back(pixelVolumeBuilder);
  // STRIPS
  // ----------------------------------------------------------------------------
  // configure short strip layer builder
  GenericLayerBuilder::Config sslbConfig;
  sslbConfig.layerCreator        = layerCreator;
  sslbConfig.layerIdentification = "SStrip";
  // fill necessary vectors for configuration
  //-------------------------------------------------------------------------------------
  // some prep work
  // envelope double
  std::pair<double, double> ssEnvelope(2., 2.);
  // Layer material properties - thickness, X0, L0, A, Z, Rho
  MaterialProperties ssmProperties(1., 95.7, 465.2, 28.03, 14., 2.32e-3);
  // Module material - X0, L0, A, Z, Rho
  Material ssMaterial(95.7, 465.2, 28.03, 14., 2.32e-3);

  // configure the central barrel
  sslbConfig.centralLayerBinMultipliers = {1, 1};
  sslbConfig.centralLayerRadii          = {205, 325, 450};
  sslbConfig.centralLayerEnvelopes      = {ssEnvelope, ssEnvelope, ssEnvelope};
  sslbConfig.centralLayerMaterialConcentration = {1, 1, 1};
  sslbConfig.centralLayerMaterialProperties
      = {ssmProperties, ssmProperties, ssmProperties};
  sslbConfig.centralModuleBinningSchema = {{26, 12}, {42, 12}, {56, 12}};
  sslbConfig.centralModuleTiltPhi       = {0.0, 0.0, 0.0};
  sslbConfig.centralModuleHalfX         = {25.6, 25.6, 25.6};
  sslbConfig.centralModuleHalfY         = {68., 68., 68.};
  sslbConfig.centralModuleThickness     = {0.25, 0.25, 0.25};

  sslbConfig.centralModuleReadoutBinsX = {728, 728, 728};  // 50 um pitch
  sslbConfig.centralModuleReadoutBinsY = {85, 85, 85};     // 1.6 mm strixels
  sslbConfig.centralModuleReadoutSide  = {1, 1, 1};
  sslbConfig.centralModuleLorentzAngle = {0.12, 0.12, 0.12};

  sslbConfig.centralModuleMaterial
      = {ssMaterial, ssMaterial, ssMaterial, ssMaterial};
  sslbConfig.centralModuleFrontsideStereo = {-0.02, -0.02, -0.02};
  sslbConfig.centralModuleBacksideStereo  = {0.02, 0.02, 0.02};
  sslbConfig.centralModuleBacksideGap     = {2., 2., 2.};
  // mPositions
  //std::vector<std::vector<Vector3D>> centralModulePositions;
  centralModulePositions.clear();
  for (size_t sslb = 0; sslb < sslbConfig.centralLayerRadii.size(); ++sslb) {
    // call the helper function
    centralModulePositions.push_back(
        modulePositionsCylinder(sslbConfig.centralLayerRadii[sslb],
                                0.0,  // 1 mm stagger
                                sslbConfig.centralModuleHalfY[sslb],

                                0.,  // 2 mm module overlap
                                5.,
                                sslbConfig.centralModuleBinningSchema[sslb]));
  }
  sslbConfig.centralModulePositions = centralModulePositions;

  // configure the endcaps
  std::vector<double> mrMinHx = {16.4, 24.2, 32.2};
  std::vector<double> mrMaxHx = {24.2, 32.2, 40.0};
  std::vector<double> mrHy    = {48., 48., 48.};

  // simplified strixels readout
  std::vector<size_t> mrReadoutBinsX = {968, 1288, 1600};  // 50 um pitch
  std::vector<size_t> mrReadoutBinsY = {60, 60, 60};       // 1.6 mm strixels
  std::vector<int>    mrReadoutSide  = {1, 1, 1};
  std::vector<double> mrLorentzAngle = {0., 0., 0.};

  std::vector<int>      mPhiBins   = {42, 58, 72};
  std::vector<double>   mThickness = {0.2, 0.2, 0.2};
  std::vector<Material> mMaterial  = {ssMaterial, ssMaterial, ssMaterial};
  std::vector<double>   mfStereo   = {-0.02, -0.02, -0.02};
  std::vector<double>   mbStereo   = {0.02, 0.02, 0.02};
  std::vector<double>   mfbGap     = {2., 2., 2.};

  sslbConfig.posnegLayerBinMultipliers = {1, 2};
  sslbConfig.posnegLayerPositionsZ = {880., 1100., 1300., 1550., 1800., 2200.};
  size_t nposnegs                  = sslbConfig.posnegLayerPositionsZ.size();
  sslbConfig.posnegLayerEnvelopeR  = std::vector<double>(nposnegs, 5.);
  sslbConfig.posnegLayerMaterialConcentration = std::vector<int>(nposnegs, 1);
  sslbConfig.posnegLayerMaterialProperties
      = std::vector<MaterialProperties>(nposnegs, ssmProperties);
  sslbConfig.posnegModuleMinHalfX
      = std::vector<std::vector<double>>(nposnegs, mrMinHx);
  sslbConfig.posnegModuleMaxHalfX
      = std::vector<std::vector<double>>(nposnegs, mrMaxHx);
  sslbConfig.posnegModuleHalfY
      = std::vector<std::vector<double>>(nposnegs, mrHy);
  sslbConfig.posnegModulePhiBins
      = std::vector<std::vector<int>>(nposnegs, mPhiBins);
  sslbConfig.posnegModuleThickness
      = std::vector<std::vector<double>>(nposnegs, mThickness);

  sslbConfig.posnegModuleReadoutBinsX
      = std::vector<std::vector<size_t>>(nposnegs, mrReadoutBinsX);
  sslbConfig.posnegModuleReadoutBinsY
      = std::vector<std::vector<size_t>>(nposnegs, mrReadoutBinsY);
  sslbConfig.posnegModuleReadoutSide
      = std::vector<std::vector<int>>(nposnegs, mrReadoutSide);
  sslbConfig.posnegModuleLorentzAngle
      = std::vector<std::vector<double>>(nposnegs, mrLorentzAngle);

  sslbConfig.posnegModuleMaterial
      = std::vector<std::vector<Material>>(nposnegs, mMaterial);
  sslbConfig.posnegModuleFrontsideStereo
      = std::vector<std::vector<double>>(nposnegs, mfStereo);
  sslbConfig.posnegModuleBacksideStereo
      = std::vector<std::vector<double>>(nposnegs, mbStereo);
  sslbConfig.posnegModuleBacksideGap
      = std::vector<std::vector<double>>(nposnegs, mfbGap);
  // mPositions
  std::vector<std::vector<std::vector<Vector3D>>> posnegModulePositions;
  for (size_t id = 0; id < sslbConfig.posnegLayerPositionsZ.size(); ++id) {
    posnegModulePositions.push_back(
        modulePositionsDisc(sslbConfig.posnegLayerPositionsZ[id],
                            2.0,
                            0.5,
                            220.,
                            500.,
                            sslbConfig.posnegModulePhiBins[id],
                            sslbConfig.posnegModuleHalfY[id]));
  }
  sslbConfig.posnegModulePositions = posnegModulePositions;

  // define the builder
  auto sstripLayerBuilder = std::make_shared<GenericLayerBuilder>(
      sslbConfig, getDefaultLogger("SStripLayerBuilder", layerLLevel));
  //-------------------------------------------------------------------------------------
  // build the pixel volume
  CylinderVolumeBuilder::Config ssvbConfig;
  ssvbConfig.trackingVolumeHelper = cylinderVolumeHelper;
  ssvbConfig.volumeName           = "SStrip";
  ssvbConfig.buildToRadiusZero    = false;
  ssvbConfig.layerBuilder         = sstripLayerBuilder;
  ssvbConfig.volumeSignature      = 0;
  auto sstripVolumeBuilder        = std::make_shared<CylinderVolumeBuilder>(
      ssvbConfig, getDefaultLogger("SStripVolumeBuilder", volumeLLevel));

  //-------------------------------------------------------------------------------------
  // add to the list of builders
  //volumeBuilders.push_back(sstripVolumeBuilder);
