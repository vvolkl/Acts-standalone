

config["eta"] = {};
for (double eta = 0.00001; eta < 1.; eta += 0.1) {
  config["eta"].push_back(eta);
}

config["phi"] = {1.4};

config["uncertainty"] = {1.};
config["pT"] = { 0.0001, 0.001, 0.05, 0.07, 0.1}; //, 10., 100., 1000.};
config["std1"] = {0.0001};
