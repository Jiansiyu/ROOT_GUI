/*
 * Benchmark.h
 *
 *  Created on: May 22, 2018
 *      Author: newdriver
 */

#ifndef BENCHMARK_H_
#define BENCHMARK_H_

#include <chrono>

class Benchmark {
public:
	Benchmark();
	virtual ~Benchmark();
	void Reset();
	unsigned int GetElapaedTime();
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> time_point;
};

#endif /* BENCHMARK_H_ */
