#!/bin/bash

g++ -O3 make_spaces.cpp ../constraints/all-diff_concept.cpp ../constraints/less-than_concept.cpp ../constraints/linear-eq_concept.cpp ../constraints/connection-min-gt_concept.cpp ../constraints/overlap-1d_concept.cpp ../constraints/concept.cpp ../utils/random_draw.cpp ../utils/latin.cpp -I./include -o bin/make_spaces
