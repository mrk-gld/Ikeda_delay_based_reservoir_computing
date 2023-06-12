COMP = g++
OPT = -std=gnu++17 -Ofast -DARMA_DONT_USE_WRAPPER
LIB = -I -lm -llapack -lopenblas -larmadillo -lstdc++
BUILDDIR = build/

delay_based_RC: delay_based_RC.cpp reservoirs.h
		$(COMP) $(OPT) delay_based_RC.cpp $(LIB) -o delay_based_RC