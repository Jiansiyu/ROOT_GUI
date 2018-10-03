/*
 * GEMModule.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: Siyu
 */

#include "GEMModule.h"

GEMModule::GEMModule() {
	// TODO Auto-generated constructor stub

}

GEMModule::~GEMModule() {
	// TODO Auto-generated destructor stub
}

// ! \brief Clsuter matching function
// ! input the cluster array, it will split the vector into x and y dimension \
. And do the x-y cluster matching for this hit
// ! \input the GEMClsuter vector for one Module !!
// ! \output the number of matched cluster pairs
uint32_t GEMModule::MatchCluster(std::vector<GEMCluster> Clusters){

	if(Clusters.size()==0) return 0;

    module=Clusters.front().Module;

	for (auto cluster : Clusters){

		if(module!=cluster.Module){
    		std::cout<<"[ERROR]:: "<<__FUNCTION__<<__LINE__<<" ModuleiD is not unique"<<std::endl;
    		return 0;
    	}

		if(cluster.Plane==0){
    		vxCluster.push_back(cluster);

    	}else if (cluster.Plane==1) {
			vyCluster.push_back(cluster);
		}

    }
	if(vxCluster.size()>0 && vyCluster.size()>0){
	    // sorting  according to the cluster charge
		std::sort(vxCluster.begin(),vxCluster.end(),[](const GEMCluster & a1,const GEMCluster & a2){
    	                                                return a1.Charge > a1.Charge;
                                                      });
		std::sort(vyCluster.begin(),vyCluster.end(),[](const GEMCluster & a1,const GEMCluster & a2){
        	                                                return a1.Charge > a1.Charge;
                                                          });
		// TODO
		// check the sorting
	}else{
		return 0;
	}
    uint32_t matched_number = (vxCluster.size()<vyCluster.size())?vxCluster.size():vyCluster.size();
    for(uint32_t i =0 ; i <matched_number;i++){
    	//MatchedCluster[vxCluster.at(i)]=vyCluster.at(i);  // maybe not a good way, need to change
    	ClusterPair pair(module,vxCluster.at(i),vyCluster.at(i));
    }
    if(vxCluster.size()>vyCluster.size()){
    	gostCluster.insert(gostCluster.begin(),(vxCluster.begin()+matched_number),vxCluster.end());
    }else{
    	gostCluster.insert(gostCluster.begin(),(vyCluster.begin()+matched_number),vyCluster.end());
    }
    // TODO check the position right?
    return matched_number;
}
