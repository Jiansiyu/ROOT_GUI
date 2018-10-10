/*
 * GEMSystem.cpp
 *
 *  Created on: Oct 5, 2018
 *      Author: newdriver
 */

#include "GEMSystem.h"
#include "unordered_map"
#include "TObject.h"
#include "algorithm"
#include "GEMLayer.h"
#include "vector"
#include "map"

GEMSystem::GEMSystem() {
	// TODO Auto-generated constructor stub

}

GEMSystem::~GEMSystem() {
	// TODO Auto-generated destructor stub
}

///! Load the hit file
///\input the hit vector
///\out the clusters that find
void GEMSystem::Reconstruct(std::vector<GEMHit> const & hits){
    std::vector<GEMCluster> clusters=clusterSearch(hits);

}

std::vector<GEMCluster> GEMSystem::clusterSearch(std::vector<GEMHit> const & hits){
	std::unordered_map<int16_t,std::unordered_map<int8_t,std::vector<GEMHit>>> hits_buffer;
	for(auto hit : hits){
         hits_buffer[hit.Module][hit.Plane].push_back(hit);
	}
	std::vector<GEMCluster> vclusters;
	//std::unordered_map<int16_t,std::unordered_map<int8_t,std::vector<GEMCluster>>> cluster_group;
	for(auto iter_module=hits_buffer.begin();iter_module!=hits_buffer.end();iter_module++){
		for(auto iter_plane=(iter_module->second).begin();iter_plane!=(iter_module->second).end();iter_plane){
			std::vector<GEMCluster> vclusters_temp=findcluster((iter_plane->second));
 			vclusters.insert(vclusters.end(),vclusters_temp.begin(),vclusters_temp.end());
		};
	}
	return vclusters;
}

std::vector<GEMCluster> GEMSystem::findcluster(std::vector<GEMHit> &vhits){
     std::vector<GEMCluster> Clusters_buffer;
    // sort the position, in case the vector position is not sorted
	std::sort(vhits.begin(),vhits.end(),[](const GEMHit & a1,const GEMHit & a2){
                                            return a1.Strip<a2.Strip;});
	if( vhits.size()!=0){

		int16_t module=vhits.front().Module;
		int8_t plane=vhits.front().Plane;

		TObjArray hitindex;
		hitindex.SetOwner();
		vector<GEMHit>::iterator next = vhits.begin();
		vector<GEMHit>::iterator cur = next;
		vector<GEMHit>::iterator end = vhits.end();
		Int_t cluster_size = 0;
		while(next!=end){
			GEMHit *gem_hit=new GEMHit();
			cur->Clone(*gem_hit);
			hitindex.Add(gem_hit);
			++cluster_size;
			++next;
			if((next->Strip)-(cur->Strip)==1){
				cur=next;
			}else{
				// need to change the coded to configuration files
				//TODO
				if(cluster_size>=2){
					GEMCluster cluster(module,plane,cluster_size,hitindex);
					cluster.SetTiming();
					Clusters_buffer.push_back(cluster);
				}else{
					cluster_size=0;
					hitindex.Clear();
					cur=next;
				}
			}

		}
	}
	return Clusters_buffer;
}

std::vector<GEMLayer> GEMSystem::clusterMatch(std::unordered_map<int16_t,std::unordered_map<int8_t,std::vector<GEMCluster>>> const &clusters){

}


std::vector<GEMLayer> GEMSystem::clusterMatch(std::vector<GEMCluster> const &){

}
