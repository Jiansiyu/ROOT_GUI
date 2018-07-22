/*
 * Benchmark.cpp
 *
 *  Created on: May 22, 2018
 *      Author: newdriver
 */

#include "Benchmark.h"
#include "iostream"
#include "iomanip"

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

void Benchmark::Print(uint64_t evtcount=0){
	if(evtcount==0){
		std::cout<<std::endl<<"Time eclpise "<<GetElapaedTime()<<std::flush;
		Reset();
	}else{
		std::cout<<std::setw(16)<<"Time eclpise "<<GetElapaedTime()<<"ms ("<<GetElapaedTime()/evtcount<<" ms/event)"<<"\r"<<std::flush;
	}
}
