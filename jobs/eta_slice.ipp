

config["eta"] = {};
for (double eta = 0.0; eta < 5.; eta += 0.01) {
  config["eta"].push_back(eta);
}

config["phi"] = {0.};

config["uncertainty"] = {1.};
config["pT"] = {1., 1000.};
config["std1"] = {0.0001};
