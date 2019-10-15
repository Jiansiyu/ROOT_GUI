/*
 * HRSOpticsInfor.h
 *
 *  Created on: Jul 4, 2019
 *      Author: newdriver
 *
 *      Interface with libconfig
 *      Read and Write all the informations for the Optics
 */

#ifndef HRSOPTICSINFOR_H_
#define HRSOPTICSINFOR_H_

#include <string>
#include <math.h>
#include <TVector3.h>
#include <TVector2.h>
#include <map>
#include <vector>
#include <TCutG.h>
#include <TCut.h>
#include <TVector.h>

//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo
//
//
//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo
Int_t GetSieveUID(Int_t TargetID, Int_t SieveRow=0, Int_t SieveCollum=0){

	return 1;
}

//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo
//
//
//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo
enum optCutItem{
	Vz,
	Sieve,
	Dp,
	Others
};


//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo
//
//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo
struct optCut{
public:
	optCut();
	~ optCut();

	std::string GetVasexp();
	TCut *GetCut();
	void LoadConfig(std::string fname);  // load the predefined config

	template <typename T>
	Bool_t IsKeyExist(std::map<T,T> a, T key){
		if (a.find(key)!=a.end()){
			return kTRUE;
		}else{
			return kFALSE;
		}
	}

	TCut *AddSelections(TCut *cut){
		*selection=*selection+*cut;
		return selection;
	};
	TCut *GetSelection(){
		return selection;
	}

protected:

	std::string cutIterm;
	std::string varexp;  // the histogram plot expressions
	TCut *selection;     // the original cut used for the plot
};


//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo
// Ready for Multiple-target usage
// TODO multiple folder usage
//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo

struct optTargetCut : public optCut{
public:
	optTargetCut(){};
	~optTargetCut(){};

	TCut *AddTargetCut(Int_t foildID, TCut *cut, Bool_t overWrite=kFALSE);
protected:
	std::map<Int_t, TCut*> targetCut;   // for multiple-target usage
};



//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo
//
//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo
struct optSieveCut : public optCut{

public:
	optSieveCut(){};
	~optSieveCut(){};
	TCut *AddSieveCut(Int_t Row, Int_t Collum, Int_t FoildID, TCut * cut, Bool_t overWrite=kFALSE);

protected:
	std::map<Int_t, TCut*> SieveCut;   // for multiple-target usage

};


//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo
//
//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo
struct optDpCut : public optCut{
public:
	optDpCut(){};
	~optDpCut(){};
	TCut *AddDpCut(Int_t DpID, TCut *cut, Bool_t overWrite=kFALSE);

protected:
	std::map<Int_t, TCut *> DpCut;    // for multiple Dp optimization usage
};


//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo
//
//
//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo
struct optBeamInfor{
public:
	optBeamInfor(){};
	~optBeamInfor(){};

	Bool_t ReadFromDat();
	void SetBeamPosition(double_t X, double_t Y);
	void SetBeamEnergy();
	void SetBeamEnergyLoss();
	double_t GetEnergyOnTarget();
protected:
	TVector3 BeamPosition;
	double_t BeamEnergy;

};

//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo
//
//
//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo
struct optTargerInfor{
public:
	optTargerInfor(){};
	~optTargerInfor(){};

	void LoadTargetDB(){
		// load the target mass etc from database
	};

protected:
	std::map<Int_t,TVector3> TargetPos;
	std::map<Int_t, std::string> TargetType; // used for calculate the elastic E

};


//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo
//
//ooooooooooooooooOOOOOOOOOO000000000000OOOOOOOoooooooooooo
class HRSOpticsInfor {
public:
	HRSOpticsInfor();
	virtual ~HRSOpticsInfor();

public:
	void LoadConfiguration(std::string, bool overWriteCurrentSetting=false);
	void SaveConfiguration(std::string fname);

private:
	inline Int_t getSieveHoleRaw(Int_t SieveHoleID);
	inline Int_t getSieveHoleColumn(Int_t SieveHoleID);
	inline Int_t getSieveHoleUID(Int_t SieveRow, Int_t SieveColumn);

protected:

	// structure used for save the configuration informations
	std::string optExperimentName;
	std::vector<std::string> optRootFileList;
	std::string optSettingTyple="template"; // 'template' / 'production'

	// configuration for the Beam
	double_t optBeamEnergy;
	TVector3 optBeamPosition;

	// Configuration for the Target
	std::map<Int_t, std::string> optTargetType;
	std::map<Int_t,    TVector3> optTargetPosition;

	//Configuration for the Sieve Pattern
	TVector2 optSieveSize;
	std::map<Int_t, TVector3> optSieveHolePosition;

	//cut informations
	bool optDpCutEnable=false;
	std::string optDpCut;

	bool optSieveCut=true;

	//Sieve pattern Input
	std::map<Int_t,TCutG *> optSievePatternCut;

public:
	std::string GetExperimentName();
	std::vector<std::string> GetDataFileName();

	bool IstempplateConfigure();
	double_t GetBeamEnergy();
	TVector3 GetBeamPosition();

	Int_t GetNumberofTarget();
	std::vector<Int_t> GetTargetList();
	TVector3 GetTargetPosition(Int_t);


	TCutG GetSieveHoleCut(Int_t SieveRow, Int_t SieveColumn);
	TCutG GetSieveHoleCut(Int_t SieveUID);
	std::vector<Int_t> GetSieveUIDList();

	// The interface with the libconfig
private:
	Bool_t LoadConfig(std::string ConfigFname);
	Bool_t SaveConfig(std::string ConfigFname);
	Bool_t ConfigCheck();    // check the current setting in the GUI whether it is the same setting in the config


protected:

	inline bool SetExperimentName(std::string experimentName);
	inline bool AddRootFiles(std::string rootFilename);

	inline bool SetoptBeamEnergy(double_t beamEnergy);
	inline bool SetoptBeamPosition(double_t x, double_t y, double_t z);
	inline bool SetoptBeamPosition(TVector3 position);

	inline bool SetoptTargetLayers(Int_t TargerNum);
	inline bool SetoptTargetType(std::string TarggetTyple, Int_t TargetID);
	inline bool SetoptTargetPosition(double_t x, double_t y, double_t z, Int_t TargetID);

	// Sieve Setting
	inline bool SetoptSieveSize(double_t width, double_t hight);
	inline bool AddoptSieveHole(Int_t, Int_t, double_t, double_t);

	// Sieve Cut
	inline bool SetoptSieveHoleCut(Int_t SieveRow, Int_t Sieve_Column, TCutG *);

	// customize Cut
	inline bool SetoptCustomizeCut(std::string cut);


};

#endif /* HRSOPTICSINFOR_H_ */
