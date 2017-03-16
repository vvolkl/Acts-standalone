all:
	cd build && make -j 4 && cd .. #&& ./build/run_example_1_etaslice > log && python scripts/plot_hits.py hitfile_example_1_etaslice.dat 
simple:
	cd build && make -j 4 && cd .. #&& ./build/run_example_simple > log && python scripts/plot_hits.py hitfile_example_simple.dat 
