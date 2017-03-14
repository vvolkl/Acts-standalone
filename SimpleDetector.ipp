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
bplConfig.centralLayerRadii = std::vector<double>(1, 19.);
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
plbConfig.centralLayerRadii = {29., 124};
plbConfig.centralLayerEnvelopes = {pcEnvelope, pcEnvelope};
plbConfig.centralLayerMaterialConcentration = {1, 1};
plbConfig.centralLayerMaterialProperties = {pcmProperties, pcmProperties};
plbConfig.centralModuleBinningSchema = {{16, 13}, {60, 13}};
plbConfig.centralModuleTiltPhi = {0.18, 0.18};
plbConfig.centralModuleHalfX = {8.4, 8.4};
plbConfig.centralModuleHalfY = {32., 32.};
plbConfig.centralModuleThickness = {0.15, 0.15};
plbConfig.centralModuleReadoutBinsX = {336, 336};
plbConfig.centralModuleReadoutBinsY = {1280, 1280};
plbConfig.centralModuleReadoutSide = {-1, -1};
plbConfig.centralModuleLorentzAngle = {0.12, 0.12};
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
                                                           2.,  // 2 mm module overlap
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
pvbConfig.layerEnvelopeR = {1. * Acts::units::_mm, 5. * Acts::units::_mm};
pvbConfig.layerBuilder = pixelLayerBuilder;
pvbConfig.volumeSignature = 0;
auto pixelVolumeBuilder =
    std::make_shared<CylinderVolumeBuilder>(pvbConfig, getDefaultLogger("PixelVolumeBuilder", volumeLLevel));
// add to the list of builders
volumeBuilders.push_back(pixelVolumeBuilder);
