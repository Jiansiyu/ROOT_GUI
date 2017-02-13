/*
 * APVCrossTalkSearch.cpp
 *
 *  Created on: Feb 4, 2017
 *      Author: Siyu Jian
 */

#include "APVCrossTalkSearch.h"
#include "time.h"

int APVCrossTalkSearch::SingleSampleRun(){

	if(!cAPVMapping.size()){     // set the defult mapping if the mapping is empty
		std::map<int,int> a;
		APVMapping(a);    		//set the defult mapping
		Massage("string",Form("%s_Using Defult Mapping",__FUNCTION__));
	}
	if ((cRemoveTalkMethod == "regular")&&(!vAPVInputData.size())) {        // apply the crssponding method get the result
		fRemoveCrosstalk(); // apply the regular peocess method to calculate the
	}else{
		Massage("error", "NULL data is passed in");
	}
	return 1;
}

int APVCrossTalkSearch::SingleEventRun(){   //
	if(!vAPVSingleEventInputData.size()){
		Massage("error", Form("%s NULL data is passed in",__FUNCTION__));
		return -1;
	}else{
		for(std::map<int,std::map<int,int>>::iterator iter_sampleID=vAPVSingleEventInputData.begin();iter_sampleID!=vAPVSingleEventInputData.end();iter_sampleID++){
			vAPVInputData.clear();
			vAPVCrossTalkData.clear();
			vAPVRemoveCrossTalkData.clear();

			vAPVInputData=iter_sampleID->second;
			SingleEventRun();     // calculate the  crosstalk for this single time sample
			vAPVSingleEventCrossTalkData.insert(std::make_pair(iter_sampleID->first,vAPVCrossTalkData));
			vAPVSingleEventRemoveCrossTalkData.insert(std::make_pair(iter_sampleID->first, vAPVRemoveCrossTalkData));
		}
		return 1;
	}
}
APVCrossTalkSearch::~APVCrossTalkSearch() {
	// TODO Auto-generated destructor stub
}

APVCrossTalkSearch::APVCrossTalkSearch(TH1F * Raw_data) {
	// loading the raw data
	vAPVInputData.clear();
	for(int Strips_counter =1; Strips_counter<=Raw_data->GetXaxis()->GetXmax();Strips_counter++){
		vAPVInputData[Strips_counter-1]=Raw_data->GetBinContent(Strips_counter);
	}

}

APVCrossTalkSearch::APVCrossTalkSearch(std::vector<int> Raw_data) {

	// loading the raw data
	vAPVInputData.clear();
	for(unsigned int strips_counter=0;strips_counter!=Raw_data.size();strips_counter++) {
		vAPVInputData[strips_counter]=Raw_data[strips_counter];
	}
}

APVCrossTalkSearch::APVCrossTalkSearch(std::map<int,int> Raw_data) {
	// loading the raw data
	vAPVInputData.clear();
	std::map<int,int>::iterator iter_strips=Raw_data.begin();
	for(;iter_strips!=Raw_data.end();iter_strips++) {
		vAPVInputData[iter_strips->first]=iter_strips->second;
	}
}


// for
APVCrossTalkSearch::APVCrossTalkSearch(std::map<int,std::map<int,int> > Raw_data){

}

APVCrossTalkSearch::APVCrossTalkSearch(std::map<int,std::vector<int> > Raw_data) {
	for(std::map<int,std::vector<int> > ::iterator iter_stripsID=Raw_data.begin(); iter_stripsID!=Raw_data.end();iter_stripsID++){
		for(unsigned int Tsample_Counter=0;Tsample_Counter<iter_stripsID->second.size();Tsample_Counter++){
			vAPVSingleEventInputData[Tsample_Counter][iter_stripsID->first]=(iter_stripsID->second)[Tsample_Counter];
		}
	}
}

void APVCrossTalkSearch::SetRawAPVData(TH1F * Raw_data) {
	// loading the raw data
	vAPVInputData.clear();
	for (int Strips_counter = 1;
			Strips_counter <= Raw_data->GetXaxis()->GetXmax();
			Strips_counter++) {
		vAPVInputData[Strips_counter - 1] = Raw_data->GetBinContent(
				Strips_counter);
	}
}

void APVCrossTalkSearch::SetRawAPVData(std::vector<int> Raw_data) {

	// loading the raw data
	vAPVInputData.clear();
	for(unsigned int strips_counter=0;strips_counter!=Raw_data.size();strips_counter++) {
		vAPVInputData[strips_counter]=Raw_data[strips_counter];
	}
}

void APVCrossTalkSearch::SetRawAPVData(std::map<int,int> Raw_data) {
	// loading the raw data
	vAPVInputData.clear();
	std::map<int,int>::iterator iter_strips=Raw_data.begin();
	for(;iter_strips!=Raw_data.end();iter_strips++) {
		vAPVInputData[iter_strips->first]=iter_strips->second;
	}
}

void APVCrossTalkSearch::SetRawAPVData(std::map<int,std::vector<int> > Raw_data){
	for(std::map<int,std::vector<int> > ::iterator iter_stripsID=Raw_data.begin(); iter_stripsID!=Raw_data.end();iter_stripsID++){
		for(unsigned int Tsample_Counter=0;Tsample_Counter<iter_stripsID->second.size();Tsample_Counter++){
			vAPVSingleEventInputData[Tsample_Counter][iter_stripsID->first]=(iter_stripsID->second)[Tsample_Counter];
		}
	}
}

std::map<int,int>APVCrossTalkSearch::APVMapping(std::map<int,int> Mapping_Data) {
	if((Mapping_Data.size()==0 )){ //
		if((cAPVMapping.size()==0)){
			for(int stripNb=0;stripNb<CStripsNumber;stripNb++){
				int RstripNb=32*(stripNb%4)+8*(int)(stripNb/4)-31*(int)(stripNb/16);//channel re-matching for apv25 chip
				RstripNb=RstripNb+1+RstripNb%4-5*(((int)(RstripNb/4))%2);//channel re-matching for INFN type APV front-end card

				cAPVMapping[stripNb]=RstripNb;
				Pos_str_mapping[RstripNb]=stripNb;
			}
		}
	}else{
		// clean the old mapping data, and loading the new data infor
		cAPVMapping.clear();
		Pos_str_mapping.clear();

		cAPVMapping=Mapping_Data;
		std::map<int,int>::iterator iter_strips=Mapping_Data.begin();
		for(;iter_strips!=Mapping_Data.begin();iter_strips++){
			Pos_str_mapping[iter_strips->second]=iter_strips->first;
		};
	}
	return cAPVMapping;
};

// first remove cross talk
int APVCrossTalkSearch::fRemoveCrosstalk(){
	std::vector<std::map<int,int> > AllEvent_buffer; //   find all the condidate crosstalk events, from the raw address, corss talk should near each other
	std::vector<std::map<int,int> > sRemoveCrossTalk_buffer;
	std::vector<std::map<int,int> > CrossTalk_condidate_buffer;

	if(vAPVInputData.size()!=0) {
		// searching for the connectd histograms
		std::map<int, int>::iterator iter_raw = vAPVInputData.begin();
		std::map<int, int> Connected_addr_temp;

		int Pre_addr_buffer = -1;
		//int test = 0;
		while(iter_raw!=vAPVInputData.end()) {

			if(Pre_addr_buffer==(iter_raw->first -1)){
				Connected_addr_temp.insert(std::make_pair(iter_raw->first,iter_raw->second));
				Pre_addr_buffer=iter_raw->first;
			}
			else {
				if(Connected_addr_temp.size()!=0)
				 {
					AllEvent_buffer.push_back(Connected_addr_temp);
					//Connected_addr_temp.clear();
				 }
				Connected_addr_temp.clear();
				Connected_addr_temp.insert(std::make_pair(iter_raw->first,iter_raw->second));
				Pre_addr_buffer=iter_raw->first;
			}
			iter_raw++;
			// if reach the last one, buffer it !!!
			if(iter_raw==vAPVInputData.end())AllEvent_buffer.push_back(Connected_addr_temp);
		}
	}else return -1;
	//+++++ select the crosstalk and the effective events+++++
	// write the cross talk and non-talk in the buffer
	std::vector<std::map<int,int> >::iterator iter_allevents=AllEvent_buffer.begin();  //
	for(;iter_allevents!=AllEvent_buffer.end();iter_allevents++) {
		if((*iter_allevents).size()==1) {	// if no raw strips is connected, it cannot be a cross talk
			sRemoveCrossTalk_buffer.push_back(*iter_allevents);
		}
		else {
			// find the fake maximum for each fake cluster
			std::map<int,int>::iterator itter_cluster=(*iter_allevents).begin();
			int MaxADC_address_temp=-1;
			int MaxADC_temp=-1;
			while(itter_cluster!=(*iter_allevents).end()) {
				if(itter_cluster->second > MaxADC_temp) {
					MaxADC_temp=itter_cluster->second;
					MaxADC_address_temp=itter_cluster->first;
				}
				itter_cluster++;
			}
			// the maximum of the fake cluster would be the reall signal
			std::map<int,int> Cluster_rmct;
			Cluster_rmct.insert(std::make_pair(MaxADC_address_temp,MaxADC_temp));
			sRemoveCrossTalk_buffer.push_back(Cluster_rmct);
			// the strips near the maximum would be the crosstalk condidate
			// write the others in the crosstalk candidate buffer
			std::map<int,int> CrossCandidate_maptemp=(*iter_allevents);
			CrossCandidate_maptemp.erase(MaxADC_address_temp);
			CrossTalk_condidate_buffer.push_back(CrossCandidate_maptemp);
		}
	}
	// save the generated function into the return buffers
	std::vector<std::map<int,int> >::iterator iter_vec_save=sRemoveCrossTalk_buffer.begin();
	for(;iter_vec_save !=sRemoveCrossTalk_buffer.end(); iter_vec_save++) {
		std::map<int,int>::iterator itter_map_save=(*iter_vec_save).begin();
		while(itter_map_save!=(*iter_vec_save).end()){
			vAPVRemoveCrossTalkData.insert(std::make_pair(cAPVMapping[itter_map_save->first],itter_map_save->second));
			itter_map_save++;
		}
	}
	iter_vec_save=CrossTalk_condidate_buffer.begin();
	for(;iter_vec_save!=CrossTalk_condidate_buffer.end();iter_vec_save++) {
		std::map<int,int>::iterator itter_map_save=(*iter_vec_save).begin();
		while(itter_map_save!=(*iter_vec_save).end()) {
			vAPVCrossTalkData.insert(std::make_pair(cAPVMapping[itter_map_save->first],itter_map_save->second));
			itter_map_save++;
		}
	}
	return 1;
}

std::map<int,int> APVCrossTalkSearch::GetSingleSampleCrossTalkData() {
	return vAPVCrossTalkData;
}

std::map<int,int> APVCrossTalkSearch::GetSingleSampleRemoveCrossTalkData() {
	return vAPVRemoveCrossTalkData;
}

// for muti timesample usage
std::map<int, std::map<int,int>> APVCrossTalkSearch::GetSingleEventCrossTalkData() {
	return vAPVSingleEventCrossTalkData;
}
std::map<int, std::map<int,int>> APVCrossTalkSearch::GetSingleEventRemoveCrossTalkData(){
	return vAPVSingleEventRemoveCrossTalkData;
}
int APVCrossTalkSearch::Massage(std::string MSG_TYPE, std::string MSG) {
	return 1;
}
