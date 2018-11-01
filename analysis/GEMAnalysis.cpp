/*
 * GEMAnalysis.cpp
 *
 *  Created on: Sep 19, 2018
 *      Author: newdriver
 */

#include "GEMAnalysis.h"
#include "TApplication.h"

#include <thread>

#include <string>
#include <vector>
#include <algorithm>
#include <mutex>
#include "Config.h"
#include "GEMTree.h"
#include "GEMTracking.h"
#include "GEMFittingShape.h"
#include "../src/UserGuiGeneralDialogProcess.h"
#include "../GEMDetector/GEMConfigure.h"

GEMAnalysis::GEMAnalysis() {
	// TODO Auto-generated constructor stub

}

GEMAnalysis::~GEMAnalysis() {
	// TODO Auto-generated destructor stub
}

void GEMAnalysis::AnalysisMT(std::vector<std::string> const fnamelist){
	// try a thread pool
	std::vector<std::thread> threadpool;
	for(auto fname : fnamelist){
		threadpool.push_back(std::thread(&GEMAnalysis::Analysis,this,fname.c_str()));
		threadpool.back().join();
		while(threadpool.size()>=7){
			sleep(5);
			for(std::thread &t : threadpool){
				if(!(t.joinable())){

				}
			}
		}
	}
}

void GEMAnalysis::Analysis(std::string const fname){

	std::thread::id this_threadid=std::this_thread::get_id();
	//thread_mutex.lock();
	std::cout<<__FUNCTION__<<"["<<__LINE__<<"] thread :"<<this_threadid<<" file:"<<fname.c_str()<<"  start !! ..."<<std::endl;

	UserGuiGeneralDialogProcess *Filenamecheck =
			new UserGuiGeneralDialogProcess();
	TChain *fChain = new TChain("GEMHit", "");
	Filenamecheck->CheckAppendix((fname).c_str(), "root");
	TFile *ff = new TFile((fname).c_str());
	if (ff->IsOpen()) {
		TTree *theTree = (TTree *) ff->Get("GEMHit");
		if (!(theTree->IsZombie())) {
			fChain->AddFile((fname).c_str());
		} else
			printf("Tree is not found in the file\n");
	}
	UserGuiGeneralDialogProcess *a = new UserGuiGeneralDialogProcess();
	std::string file = fname;
	GEMConfigure *cfg = GEMConfigure::GetInstance();
	UserGuiGeneralDialogProcess *generalprocess =
			new UserGuiGeneralDialogProcess();

	//extract the number in the string
	std::string filebasename = basename(strdup(fname.c_str()));
	filebasename.substr(0, filebasename.find_first_of("."));
	std::string filename_temp = filebasename;
	int rundividednumber = atoi(
			filename_temp.substr(filename_temp.find_last_of("_") + 1).c_str());

	std::string test = filebasename.substr(0, filebasename.find_last_of("_"));
	std::cout << "Test :" << test.c_str() << std::endl;
	std::string savefilename = Form("Tracking_run%d_%d.root",
			generalprocess->GetNumberFromFilename(test.c_str()),
			rundividednumber);
	std::cout << "Working on file : " << file.c_str() << std::endl
			<< "Save file name   : " << savefilename.c_str() << std::endl;
	std::string savename = savefilename;
	printf("File will be save as : %s", savename.c_str());

	GEMTracking *pGEMTrack = new GEMTracking(fChain);

//	pGEMTrack->efficiency(-1,1, savename.c_str());
	pGEMTrack->produceTrainingData(-1,savename.c_str());

	ff->Close();
	std::cout<<__FUNCTION__<<"["<<__LINE__<<"] thread :"<<this_threadid<<" file:"<<fname.c_str() <<" finished !!"<<std::endl;
	delete pGEMTrack;
	delete ff;
	//thread_mutex.unlock();
 }

void GEMAnalysis::TrainingData(std::string const fname){

	std::thread::id this_threadid=std::this_thread::get_id();
	//thread_mutex.lock();
	std::cout<<__FUNCTION__<<"["<<__LINE__<<"] thread :"<<this_threadid<<" file:"<<fname.c_str()<<"  start !! ..."<<std::endl;

	UserGuiGeneralDialogProcess *Filenamecheck =
			new UserGuiGeneralDialogProcess();
	TChain *fChain = new TChain("GEMHit", "");
	Filenamecheck->CheckAppendix((fname).c_str(), "root");
	TFile *ff = new TFile((fname).c_str());
	if (ff->IsOpen()) {
		TTree *theTree = (TTree *) ff->Get("GEMHit");
		if (!(theTree->IsZombie())) {
			fChain->AddFile((fname).c_str());
		} else
			printf("Tree is not found in the file\n");
	}
	UserGuiGeneralDialogProcess *a = new UserGuiGeneralDialogProcess();
	std::string file = fname;
	GEMConfigure *cfg = GEMConfigure::GetInstance();
	UserGuiGeneralDialogProcess *generalprocess =
			new UserGuiGeneralDialogProcess();

	//extract the number in the string
	std::string filebasename = basename(strdup(fname.c_str()));
	filebasename.substr(0, filebasename.find_first_of("."));
	std::string filename_temp = filebasename;
	int rundividednumber = atoi(
			filename_temp.substr(filename_temp.find_last_of("_") + 1).c_str());

	std::string test = filebasename.substr(0, filebasename.find_last_of("_"));
	std::cout << "Test :" << test.c_str() << std::endl;
	std::string savefilename = Form("Training_run%04d_%04d.txt",
			generalprocess->GetNumberFromFilename(test.c_str()),
			rundividednumber);
	std::cout << "Working on file : " << file.c_str() << std::endl
			<< "Save file name   : " << savefilename.c_str() << std::endl;
	std::string savename = savefilename;
	printf("File will be save as : %s", savename.c_str());

	GEMTracking *pGEMTrack = new GEMTracking(fChain);
	pGEMTrack->produceTrainingData(-1,savename.c_str());

	ff->Close();
	std::cout<<__FUNCTION__<<"["<<__LINE__<<"] thread :"<<this_threadid<<" file:"<<fname.c_str() <<" finished !!"<<std::endl;
	delete pGEMTrack;
	delete ff;
	//thread_mutex.unlock();
 }
