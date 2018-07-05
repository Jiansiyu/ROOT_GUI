/*
 * Benchmark.cpp
 *
 *  Created on: May 22, 2018
 *      Author: newdriver
 */

#include "Benchmark.h"

Benchmark::Benchmark() {
	// TODO Auto-generated constructor stub
	Reset();
}

Benchmark::~Benchmark() {
	// TODO Auto-generated destructor stub
}

void Benchmark::Reset(){
	time_point=std::chrono::high_resolution_clock::now();
}

unsigned int Benchmark::GetElapaedTime(){
	auto time_end=std::chrono::high_resolution_clock::now();
	auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_point);
	return int_ms.count();
}
