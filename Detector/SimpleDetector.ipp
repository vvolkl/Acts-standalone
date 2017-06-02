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
  Material oMaterial(95.7, 465.2, 28.03, 14., 2.32e-3);
  MaterialProperties omProperties(1., 95.7, 465.2, 28.03, 14., 2.32e-3);
PassiveLayerBuilder::Config bplConfig;
bplConfig.layerIdentification = "BeamPipe";
bplConfig.centralLayerRadii = std::vector<double>(1, 15.);
bplConfig.centralLayerHalflengthZ = std::vector<double>(1, 810.);
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
plbConfig.centralModuleThickness = std::vector<double>(l_numLayers, 0.15);
plbConfig.centralModuleReadoutBinsX = std::vector<long unsigned int>(l_numLayers, 226);
plbConfig.centralModuleReadoutBinsY = std::vector<long unsigned int>(l_numLayers, 1280);
plbConfig.centralModuleReadoutSide = std::vector<int>(l_numLayers, -1);
plbConfig.centralModuleLorentzAngle = std::vector<double>(l_numLayers, 0.12);
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
  std::vector<double> imrMinHx = {16.4, 24.2, 32.2, 40., 48.};
  std::vector<double> imrMaxHx = {24.2, 32.2, 40.0, 48., 56.};

  // simplified strixels readout
  std::vector<size_t> imrReadoutBinsX = {968, 1288, 1600, 1800, 2000};  // 50 um pitch

  std::vector<int>      imPhiBins   = {42, 58, 72, 90, 102};
  size_t inRings = imPhiBins.size();
  std::vector<double>   imThickness = std::vector<double>(inRings, 0.2);
  std::vector<Material> imMaterial  = std::vector<Material>(inRings, oMaterial);
  std::vector<double>   imfStereo   = std::vector<double>(inRings, -0.02);
  std::vector<double>   imbStereo   = std::vector<double>(inRings, 0.02);
  std::vector<double>   imfbGap     = std::vector<double>(inRings,  2.);
  std::vector<size_t> imrReadoutBinsY = std::vector<size_t>(inRings, 60);       // 1.6 mm strixels
  std::vector<int>    imrReadoutSide  = std::vector<int>(inRings, 1.);
  std::vector<double> imrHy    = std::vector<double>(inRings, 48.);
  std::vector<double> imrLorentzAngle = std::vector<double>(inRings, 0.);

  plbConfig.posnegLayerBinMultipliers = {1, 2};
  plbConfig.posnegLayerPositionsZ = {1000, 1200, 1500, 1800, 2200};
  size_t nposnegs                  = plbConfig.posnegLayerPositionsZ.size();
  plbConfig.posnegLayerEnvelopeR  = std::vector<double>(nposnegs, 5.);
  plbConfig.posnegLayerMaterialConcentration = std::vector<int>(nposnegs, 1);
  plbConfig.posnegLayerMaterialProperties
      = std::vector<MaterialProperties>(nposnegs, omProperties);
  plbConfig.posnegModuleMinHalfX
      = std::vector<std::vector<double>>(nposnegs, imrMinHx);
  plbConfig.posnegModuleMaxHalfX
      = std::vector<std::vector<double>>(nposnegs,imrMaxHx);
  plbConfig.posnegModuleHalfY
      = std::vector<std::vector<double>>(nposnegs, imrHy);
  plbConfig.posnegModulePhiBins
      = std::vector<std::vector<int>>(nposnegs, imPhiBins);
  plbConfig.posnegModuleThickness
      = std::vector<std::vector<double>>(nposnegs, imThickness);

  plbConfig.posnegModuleReadoutBinsX
      = std::vector<std::vector<size_t>>(nposnegs, imrReadoutBinsX);
  plbConfig.posnegModuleReadoutBinsY
      = std::vector<std::vector<size_t>>(nposnegs, imrReadoutBinsY);
  plbConfig.posnegModuleReadoutSide
      = std::vector<std::vector<int>>(nposnegs, imrReadoutSide);
  plbConfig.posnegModuleLorentzAngle
      = std::vector<std::vector<double>>(nposnegs, imrLorentzAngle);

  plbConfig.posnegModuleMaterial
      = std::vector<std::vector<Material>>(nposnegs, imMaterial);
  plbConfig.posnegModuleFrontsideStereo
      = std::vector<std::vector<double>>(nposnegs, imfStereo);
  plbConfig.posnegModuleBacksideStereo
      = std::vector<std::vector<double>>(nposnegs, imbStereo);
  plbConfig.posnegModuleBacksideGap
      = std::vector<std::vector<double>>(nposnegs, imfbGap);
  // mPositions
  std::vector<std::vector<std::vector<Vector3D>>> iposnegModulePositions;
  for (size_t id = 0; id < plbConfig.posnegLayerPositionsZ.size(); ++id) {
    iposnegModulePositions.push_back(
        modulePositionsDisc(plbConfig.posnegLayerPositionsZ[id],
                            2.0,
                            0.5,
                            30.,
                            450.,
                            plbConfig.posnegModulePhiBins[id],
                            plbConfig.posnegModuleHalfY[id]));
  }
  plbConfig.posnegModulePositions = iposnegModulePositions;

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
  // Module material - X0, L0, A, Z, Rho
  olbConfig.approachSurfaceEnvelope = 5.;

  // configure the central barrel
  olbConfig.centralLayerBinMultipliers = {1, 1};
  olbConfig.centralLayerRadii          = {607.6, 807.3, 969.7, 1200, 1400, 1600};
  olbConfig.centralModuleBinningSchema = {{38, 40}, {50, 40}, {60, 40}, {70, 40}, {80, 40}, {90, 40}};
  olbConfig.centralLayerMaterialConcentration = {1, 1, 1, 1, 1, 1};
  olbConfig.centralLayerMaterialProperties
      = {omProperties, omProperties, omProperties, omProperties, omProperties, omProperties};
  l_numLayers = olbConfig.centralLayerRadii.size();
  olbConfig.centralLayerEnvelopes      = std::vector<std::pair<double,double>>(l_numLayers, oEnvelope);
  olbConfig.centralModuleTiltPhi       = std::vector<double>(l_numLayers, 0.);
  olbConfig.centralModuleHalfX         = std::vector<double>(l_numLayers, 51.2);
  olbConfig.centralModuleHalfY         = std::vector<double>(l_numLayers, 51.2);
  olbConfig.centralModuleThickness     = std::vector<double>(l_numLayers, 0.25);

  olbConfig.centralModuleReadoutBinsX =  std::vector<size_t>(l_numLayers, 728);  // 50 um pitch
  olbConfig.centralModuleReadoutBinsY = std::vector<size_t>(l_numLayers, 85);     // 1.6 mm strixels
  olbConfig.centralModuleReadoutSide  = std::vector<int>(l_numLayers,1);
  olbConfig.centralModuleLorentzAngle = std::vector<double>(l_numLayers, 0.12);

  olbConfig.centralModuleMaterial
    = std::vector<Material>(l_numLayers, oMaterial);
  olbConfig.centralModuleFrontsideStereo = std::vector<double>(l_numLayers,  -0.02);
  olbConfig.centralModuleBacksideStereo  = std::vector<double>(l_numLayers, 0.02);
  olbConfig.centralModuleBacksideGap     = std::vector<double>(l_numLayers,  2.);
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

  // simplified strixels readout

  std::vector<int>      mPhiBins   = std::vector<int>(6, 50); // {42, 58, 72, 90, 102,108,116};//,122,130,138, 146, 152};
  size_t nRings = mPhiBins.size();
  std::vector<double> mrMinHx = {};

  std::vector<double> mrMaxHx = {};
  for (size_t i = 0; i < nRings; ++i) {
    std::cout << i << std::endl;
    mrMinHx.push_back(12 + 18. * i);
    mrMaxHx.push_back(24 + 18. * i);
  }
  std::vector<size_t> mrReadoutBinsX = std::vector<size_t>(nRings, 2000);  // 50 um pitch
  std::vector<double>   mThickness = std::vector<double>(nRings, 0.2);
  std::vector<Material> mMaterial  = std::vector<Material>(nRings, oMaterial);
  std::vector<double>   mfStereo   = std::vector<double>(nRings, -0.02);
  std::vector<double>   mbStereo   = std::vector<double>(nRings, 0.02);
  std::vector<double>   mfbGap     = std::vector<double>(nRings,  2.);
  std::vector<size_t> mrReadoutBinsY = std::vector<size_t>(nRings, 60);       // 1.6 mm strixels
  std::vector<int>    mrReadoutSide  = std::vector<int>(nRings, 1.);
  std::vector<double> mrHy    = std::vector<double>(nRings, 140.);
  std::vector<double> mrLorentzAngle = std::vector<double>(nRings, 0.);

  olbConfig.posnegLayerBinMultipliers = {1, 2};
  olbConfig.posnegLayerPositionsZ = {2600, 2900., 3300., 3800., 4300., 5000.};
  //olbConfig.posnegLayerPositionsZ = {2600, 2900., 3300., 3800., 4300., 5000.};
  size_t onposnegs                  = olbConfig.posnegLayerPositionsZ.size();
  olbConfig.posnegLayerEnvelopeR  = std::vector<double>(onposnegs, 5.);
  olbConfig.posnegLayerMaterialConcentration = std::vector<int>(onposnegs, 1);
  olbConfig.posnegLayerMaterialProperties
      = std::vector<MaterialProperties>(onposnegs, omProperties);
  olbConfig.posnegModuleMinHalfX
      = std::vector<std::vector<double>>(onposnegs, mrMinHx);
  olbConfig.posnegModuleMaxHalfX
      = std::vector<std::vector<double>>(onposnegs, mrMaxHx);
  olbConfig.posnegModuleHalfY
      = std::vector<std::vector<double>>(onposnegs, mrHy);
  olbConfig.posnegModulePhiBins
      = std::vector<std::vector<int>>(onposnegs, mPhiBins);
  olbConfig.posnegModuleThickness
      = std::vector<std::vector<double>>(onposnegs, mThickness);

  olbConfig.posnegModuleReadoutBinsX
      = std::vector<std::vector<size_t>>(onposnegs, mrReadoutBinsX);
  olbConfig.posnegModuleReadoutBinsY
      = std::vector<std::vector<size_t>>(onposnegs, mrReadoutBinsY);
  olbConfig.posnegModuleReadoutSide
      = std::vector<std::vector<int>>(onposnegs, mrReadoutSide);
  olbConfig.posnegModuleLorentzAngle
      = std::vector<std::vector<double>>(onposnegs, mrLorentzAngle);

  olbConfig.posnegModuleMaterial
      = std::vector<std::vector<Material>>(onposnegs, mMaterial);
  olbConfig.posnegModuleFrontsideStereo
      = std::vector<std::vector<double>>(onposnegs, mfStereo);
  olbConfig.posnegModuleBacksideStereo
      = std::vector<std::vector<double>>(onposnegs, mbStereo);
  olbConfig.posnegModuleBacksideGap
      = std::vector<std::vector<double>>(onposnegs, mfbGap);
  // mPositions
  std::vector<std::vector<std::vector<Vector3D>>> posnegModulePositions;
  for (size_t id = 0; id < olbConfig.posnegLayerPositionsZ.size(); ++id) {
    posnegModulePositions.push_back(
        modulePositionsDisc(olbConfig.posnegLayerPositionsZ[id],
                            2.0,
                            0.5,
                            30.,
                            1650.,
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
