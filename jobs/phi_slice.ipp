

config["eta"] = {0.};

config["phi"] = {};
for (double phi=0; phi < M_PI / 2.; phi += M_PI / 200.) {
  config["phi"].push_back(phi);
}

config["uncertainty"] = {1.};
config["pT"] = {1., 1000.};
config["std1"] = {0.0001};
