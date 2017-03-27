
#include <map>
#include <iostream>
#include <fstream>
#include "Utilities/KalmanFitUtils.hpp"
void read_config(std::string filename, std::map<std::string, std::vector<double>>& config) {
  std::ifstream fin(filename);
  std::string key;
  std::string vtype;
  double begin;
  double end;
  double stride;
  for (std::string line; std::getline(fin, line);) {
  std::istringstream is(line);
  is >> vtype >> key;
  std::getline(fin, line);

  std::istringstream is2(line);
  std::vector<double> values;
  if (vtype == "range") {
    //while (fin >> key >> begin >> end >> stride) {
     is2 >> begin >> end >> stride;
       
      for(double i = begin; i < end; i += stride) {
        values.push_back(i);
      }
      } else if (vtype == "vector") {

      std::copy(std::istream_iterator<double>(is2), std::istream_iterator<double>(), std::back_inserter(values));
      }
      config[key] = values;

  }
}

