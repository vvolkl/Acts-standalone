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
plbConfig.approachSurfaceEnvelope = 5.;

plbConfig.centralLayerBinMultipliers = {1, 1};
plbConfig.centralLayerRadii = {25., 60, 100, 205, 325, 450,};
plbConfig.centralModuleBinningSchema = {{14, 20}, {16, 20}, {26,20}, {26,20}, {42, 20}, {56, 20}};
plbConfig.centralModuleHalfX = {6.4, 12.8, 12.8, 25.6, 25.6, 25.6};

size_t l_numLayers = plbConfig.centralModuleBinningSchema.size();
plbConfig.centralLayerEnvelopes = {pcEnvelope, pcEnvelope, pcEnvelope, pcEnvelope, pcEnvelope, pcEnvelope};
plbConfig.centralLayerMaterialConcentration = std::vector<int>(l_numLayers, 1);
plbConfig.centralLayerMaterialProperties = {pcmProperties, pcmProperties, pcmProperties, pcmProperties, pcmProperties, pcmProperties};
plbConfig.centralModuleTiltPhi = std::vector<double>(l_numLayers,0.);
plbConfig.centralModuleHalfY = std::vector<double>(l_numLayers, 41.);
plbConfig.centralModuleThickness = {0.15, 0.05, 0.15, 0.15, 0.15, 0.15};
plbConfig.centralModuleReadoutBinsX = {336, 336, 226, 226, 226,226};
plbConfig.centralModuleReadoutBinsY = {1280, 1280, 1280, 1280, 1280, 1280};
plbConfig.centralModuleReadoutSide = {-1, -1, -1, -1, -1,-1};
plbConfig.centralModuleLorentzAngle = {0.12, 0.12, 0.12, 0.12, 0.12, 0.12};
// no frontside/backside
plbConfig.centralModuleFrontsideStereo = {};
plbConfig.centralModuleBacksideStereo = {};
plbConfig.centralModuleBacksideGap = {};
// mPositions
std::vector<std::vector<Vector3D>> centralModulePositions;
for (size_t plb = 0; plb < plbConfig.centralLayerRadii.size(); ++plb) {
  // call the helper function
  centralModulePositions.push_back(modulePositionsCylinder(plbConfig.centralLayerRadii[plb],
                                                           0.5,  // 1 mm stagger
                                                           plbConfig.centralModuleHalfY[plb],
                                                           1.,  // 2 mm module overlap
                                                           1.,
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

  // OUTER BARREL
  // ----------------------------------------------------------------------------
  // configure short strip layer builder
  GenericLayerBuilder::Config olbConfig;
  olbConfig.layerCreator        = layerCreator;
  olbConfig.layerIdentification = "OuterBarrel";
  // fill necessary vectors for configuration
  //-------------------------------------------------------------------------------------
  // some prep work
  // envelope double
  std::pair<double, double> oEnvelope(2., 2.);
  // Layer material properties - thickness, X0, L0, A, Z, Rho
  MaterialProperties omProperties(1., 95.7, 465.2, 28.03, 14., 2.32e-3);
  // Module material - X0, L0, A, Z, Rho
  Material oMaterial(95.7, 465.2, 28.03, 14., 2.32e-3);
  olbConfig.approachSurfaceEnvelope = 5.;

  // configure the central barrel
  olbConfig.centralLayerBinMultipliers = {1, 1};
  olbConfig.centralLayerRadii          = {607.6, 807.3, 969.7};
  olbConfig.centralModuleBinningSchema = {{38, 30}, {50, 30}, {60, 30}};
  olbConfig.centralLayerMaterialConcentration = {1, 1, 1};
  olbConfig.centralLayerMaterialProperties
      = {omProperties, omProperties, omProperties};
  l_numLayers = olbConfig.centralLayerRadii.size();
  olbConfig.centralLayerEnvelopes      = std::vector<std::pair<double,double>>(l_numLayers, oEnvelope);
  olbConfig.centralModuleTiltPhi       = {0.0, 0.0, 0.0};
  olbConfig.centralModuleHalfX         = std::vector<double>(l_numLayers, 51.2);
  olbConfig.centralModuleHalfY         = std::vector<double>(l_numLayers, 51.2);
  olbConfig.centralModuleThickness     = {0.25, 0.25, 0.25};

  olbConfig.centralModuleReadoutBinsX = {728, 728, 728};  // 50 um pitch
  olbConfig.centralModuleReadoutBinsY = {85, 85, 85};     // 1.6 mm strixels
  olbConfig.centralModuleReadoutSide  = {1, 1, 1};
  olbConfig.centralModuleLorentzAngle = {0.12, 0.12, 0.12};

  olbConfig.centralModuleMaterial
    = {oMaterial, oMaterial, oMaterial};
  olbConfig.centralModuleFrontsideStereo = {-0.02, -0.02, -0.02};
  olbConfig.centralModuleBacksideStereo  = {0.02, 0.02, 0.02};
  olbConfig.centralModuleBacksideGap     = {2., 2., 2.};
  // mPositions
  //std::vector<std::vector<Vector3D>> centralModulePositions;
  centralModulePositions.clear();
  for (size_t olb = 0; olb < olbConfig.centralLayerRadii.size(); ++olb) {
    // call the helper function
    centralModulePositions.push_back(
        modulePositionsCylinder(olbConfig.centralLayerRadii[olb],
                                0.5,  // 1 mm stagger
                                olbConfig.centralModuleHalfY[olb],

                                1.,  // 2 mm module overlap
                                5.,
                                olbConfig.centralModuleBinningSchema[olb]));
  }
  olbConfig.centralModulePositions = centralModulePositions;

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
  std::vector<Material> mMaterial  = {oMaterial, oMaterial, oMaterial};
  std::vector<double>   mfStereo   = {-0.02, -0.02, -0.02};
  std::vector<double>   mbStereo   = {0.02, 0.02, 0.02};
  std::vector<double>   mfbGap     = {2., 2., 2.};

  olbConfig.posnegLayerBinMultipliers = {1, 2};
  olbConfig.posnegLayerPositionsZ = {1880., 2100., 2300., 2550., 3800., 3200.};
  size_t nposnegs                  = olbConfig.posnegLayerPositionsZ.size();
  olbConfig.posnegLayerEnvelopeR  = std::vector<double>(nposnegs, 5.);
  olbConfig.posnegLayerMaterialConcentration = std::vector<int>(nposnegs, 1);
  olbConfig.posnegLayerMaterialProperties
      = std::vector<MaterialProperties>(nposnegs, omProperties);
  olbConfig.posnegModuleMinHalfX
      = std::vector<std::vector<double>>(nposnegs, mrMinHx);
  olbConfig.posnegModuleMaxHalfX
      = std::vector<std::vector<double>>(nposnegs, mrMaxHx);
  olbConfig.posnegModuleHalfY
      = std::vector<std::vector<double>>(nposnegs, mrHy);
  olbConfig.posnegModulePhiBins
      = std::vector<std::vector<int>>(nposnegs, mPhiBins);
  olbConfig.posnegModuleThickness
      = std::vector<std::vector<double>>(nposnegs, mThickness);

  olbConfig.posnegModuleReadoutBinsX
      = std::vector<std::vector<size_t>>(nposnegs, mrReadoutBinsX);
  olbConfig.posnegModuleReadoutBinsY
      = std::vector<std::vector<size_t>>(nposnegs, mrReadoutBinsY);
  olbConfig.posnegModuleReadoutSide
      = std::vector<std::vector<int>>(nposnegs, mrReadoutSide);
  olbConfig.posnegModuleLorentzAngle
      = std::vector<std::vector<double>>(nposnegs, mrLorentzAngle);

  olbConfig.posnegModuleMaterial
      = std::vector<std::vector<Material>>(nposnegs, mMaterial);
  olbConfig.posnegModuleFrontsideStereo
      = std::vector<std::vector<double>>(nposnegs, mfStereo);
  olbConfig.posnegModuleBacksideStereo
      = std::vector<std::vector<double>>(nposnegs, mbStereo);
  olbConfig.posnegModuleBacksideGap
      = std::vector<std::vector<double>>(nposnegs, mfbGap);
  // mPositions
  std::vector<std::vector<std::vector<Vector3D>>> posnegModulePositions;
  for (size_t id = 0; id < olbConfig.posnegLayerPositionsZ.size(); ++id) {
    posnegModulePositions.push_back(
        modulePositionsDisc(olbConfig.posnegLayerPositionsZ[id],
                            2.0,
                            0.5,
                            220.,
                            500.,
                            olbConfig.posnegModulePhiBins[id],
                            olbConfig.posnegModuleHalfY[id]));
  }
  olbConfig.posnegModulePositions = posnegModulePositions;

  // define the builder
  auto obLayerBuilder = std::make_shared<GenericLayerBuilder>(
      olbConfig, getDefaultLogger("OuterBarrelLayerBuilder", layerLLevel));
  //-------------------------------------------------------------------------------------
  // build the pixel volume
  CylinderVolumeBuilder::Config ovbConfig;
  ovbConfig.trackingVolumeHelper = cylinderVolumeHelper;
  ovbConfig.volumeName           = "SStrip";
  ovbConfig.buildToRadiusZero    = false;
  ovbConfig.layerBuilder         = obLayerBuilder;
  ovbConfig.volumeSignature      = 0;
  auto obVolumeBuilder        = std::make_shared<CylinderVolumeBuilder>(
      ovbConfig, getDefaultLogger("OuterBarrelVolumeBuilder", volumeLLevel));

  //-------------------------------------------------------------------------------------
  // add to the list of builders
  volumeBuilders.push_back(obVolumeBuilder);
