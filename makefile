spring: spring.cpp
	module load gnu/6.3.0; \
	mpic++ -std=c++11 -openmp -g -o spring spring.cpp
