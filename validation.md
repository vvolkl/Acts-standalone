# Validation of the ACTS-Fitting Package

## Description of test setup:

* ACTS at revision `4f20562371cdd44fde5a5cdadd7999b3b5f6c3e8`, from June 8th 2017 ( newer releases cannot be used to test because of issue ACTS-333 )
* Platform: `x86_64-ubuntu1604-gcc62`, dependencies taken from ACTS-supplied docker image
* Geometries and initial parameters: see below

## Tests with Cylindrical Barrel

For a first round of tests the simplest relevant geometry is used: a set of centered cylinder barrels to represent equidistant detector layers. The concrete dimensions are chosen to be in the same order of those a realistic detector, so the fit results need not be scaled. Figure 1. shows the layout of this test.

![](data/exampleSuperSimpleBarrel-phiScan_hits_x-y.png)
![](data/exampleSuperSimpleBarrel-etaScan_hits_r-z.png)

### Validation of Extrapolation Track Curvature

A simple but essential test is comparing the extrapolated track to the analytical result for the curvature $R$ of the track of a particle with charge $q$ and transverse momentum $p_T$ in a magnetic field in Z-direction with magnitude $B$

$$ R [\text{cm}] = \frac{p_T[\text{GeV}]}{0.003 \cdot q[\text{e}] \cdot B [\text{T}]}, $$


compare reference [1]. Choosing $B = 2T$, $p_T = 0.1[\text{GeV}]$, we expect the track in the x-y plane to resemble a circle of radius $\approx 166[mm]$. The following circle fit of a single track with the previously mentioned parameters shows that the ACTS extrapolation is consistent with these expectations:

![](data/exampleSuperSimpleBarrel-singleTrackLowPt_hits_x-y.png)
![](data/exampleSuperSimpleBarrel-singleTrackLowPt_circle.png)

The test can be scaled to a high-momentum $p_T = 100$ [GeV] track, which also gives reasonable results
 
![](data/exampleSuperSimpleBarrel-singleTrack_hits_x-y.png)
![](data/exampleSuperSimpleBarrel-singleTrack_hits_circle.png)

### Fitting perfect tracks

The extrapolation capabilities of ACTS can be used to quickly generate pseudo-measurements as input to the fitter. For this test, the parameters chosen are:

* $p_T$: .1, 1, 10, 100 [GeV]
* $\varphi$: $0.5 \cdot \pi$
* $\eta$ $0\dots2.5$ in steps of 0.1
* vertex location: (0,0,0)

The covariance matrix corresponding to these track parameters is initalized as a $5 \times 5$ unit diagonal matrix. This gives the following result for the fit of tracks composed of six pseudo-measurements on the cylinder barrels:


![](data/exampleSuperSimpleBarrel-etaScan_parameters_covPt.png)
![](data/exampleSuperSimpleBarrel-etaScan_parameters_deltaP.png)


### Effect of initial covariance

The Kalman Filter requires a covariance for the initial track parameters. This must be estimated somehow, but can usually safely be set to a very large number. The following figure shows that the magnitude of the initial covariance matrix has no effect on the fit result except when the measurmement has a very big uncertainty

![](data/exampleSuperSimpleBarrel-uncertainties_parameters_covPt.png)
