#include "Utilities/KalmanFitUtils.hpp"
void saveDummyHits(std::ostream& log, std::vector<FitMeas_t>& measurements) {
  for (FitMeas_t m: measurements) {
    std::cout << m.covariance() << std::endl;
  }
}
