/*
*
* Matrix Correction Calculation 
* Author: Siyu Jian
*         jiansiyu@gmail.com
*         Physics Dept of
*         University of Virginia
*/
#include <TROOT.h>
#include <TMath.h>
#include <TFile.h>
#include <TH1F.h>
#include <TMinuit.h>
#include <Riostream.h>
#include <TMatrix.h>
#include <TMatrixD.h>
#include <TMatrixDEigen.h>
#include <TGraphErrors.h>
#include <TDecompSVD.h>
#include <TApplication.h>
#include <TArray.h>
#include <TArrayD.h>
#include <TVectorD.h>
#include <TVector3.h>

//machine learning interface
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TString.h"
#include "map"
#include <sstream>
#include <fstream>

class Correctionsolver{
public:
	Correctionsolver(){};
    virtual ~Correctionsolver(){};
    TMatrixD GetAverageMatrixD(TMatrixD);
    TMatrixD GetEigenMatrix(TMatrixD);
    TMatrixD GetEigenValueMatrix(TMatrixD);
    TMatrixD GetEigenVectorMatrix(TMatrixD);
    double_t GetEigenValueMax(TMatrixD);
    std::vector<TMatrixD> ReadCorrectList(uint64_t);
    void Open(std::vector<std::string> fnames);
    void Open(std::string fname);
    void GridSolver(TMatrixD M,std::vector<TMatrixD> correctList);
    void GridsSolver(std::vector<TMatrixD> Ms,std::vector<TMatrixD> correctList);

private:
    const std::vector<TMatrixD> Read(TFile *file);
    std::vector<TMatrixD> cTrainingData;   // used for buffer correct data point from the file
    std::vector<TMatrixD> CorrectList;     // calculate the correctList
    std::map<double_t,TMatrixD> correctResult;   // used for buffer the candidate list
    uint64_t CorrectListPosition;
};

void Correctionsolver::Open(std::string fname){
	std::ifstream file(fname.c_str(),std::ifstream::in);
	std::vector<std::string> lines;
	if(file.good()){
		std::string line;
		while(getline(file,line)){
			if(line.size==0) continue;

			std::stringstream ss(line);
			std::vector<std::string> result;
			while(ss.good()){
				std::string substr;
				getline(ss,substr,' ');
				result.push_back(substr);
			}
		}
	}

}



TMatrixD Correctionsolver::GetEigenValueMatrix(TMatrixD matrix){
	if(matrix.GetNcols()!=matrix.GetNrows()){
		std::cout<<"[ERROR]Matrix rol != col"<<std::endl;
		exit(-1);
	}
	TMatrixDEigen eig(matrix);
	const TMatrixD &ev=eig.GetEigenValues();
	return ev;
}


double_t Correctionsolver::GetEigenValueMax(TMatrixD matrix){
	if(matrix.GetNcols()!=matrix.GetNrows()){
		std::cout<<"[ERROR]Matrix rol != col"<<std::endl;
		exit(-1);
	}
	TMatrixDEigen eig(matrix);
	const TMatrixD &ev=eig.GetEigenValues();
	double max_evalue=ev(0,0);
	for(int i = 0 ; i < matrix.GetNcols();i++){
		if(ev(i,i)>max_evalue) max_evalue=ev(i,i);
		//std::cout<<"["<<i<<"]: "<< ev(i,i)<<std::endl;
	}
	return max_evalue;
}



TMatrixD Correctionsolver::GetEigenVectorMatrix(TMatrixD matrix){
	if(matrix.GetNcols()!=matrix.GetNrows()){
		std::cout<<"[ERROR]Matrix rol != col"<<std::endl;
		exit(-1);
	}
	TMatrixDEigen eig(matrix);
	const TMatrixD &evec= eig.GetEigenVectors();
	return evec;
}

TMatrixD Correctionsolver::GetAverageMatrixD(TMatrixD matrix){
	TMatrixD matrix_temp(1,matrix.GetNrows());
	TArrayD udata(matrix.GetNrows());
	for(uint i =0; i < matrix.GetNrows(); i ++){
		udata[i]=1.0/matrix.GetNrows();
	}
	matrix_temp.Use(matrix_temp.GetNrows(),matrix_temp.GetNcols(),udata.GetArray());
	TMatrixD sumMatrix =matrix_temp*matrix;
	sumMatrix.ResizeTo(matrix);
	uint nelement=matrix.GetNrows()*matrix.GetNcols();
	TArrayD avgdata(nelement);
	for(uint i = 0; i < nelement; i ++){
		avgdata[i]=sumMatrix(0,i%matrix.GetNcols());
	}
	TMatrixD uMatrix;
	uMatrix.ResizeTo(matrix);
	uMatrix.Use(uMatrix.GetNrows(),uMatrix.GetNcols(),avgdata.GetArray());
	const TMatrixD retur=uMatrix;
	std::cout<<"Average::"<<std::endl;
	retur.Print();
	return retur;
}
void positioncorrection(){
	// machine learning training interface

}


void Test(){

    gROOT->Time();
    TMatrixD matrix(3,3); // a test matrix  used for solve the aigenvector and aigenvalue
    uint nelement=(matrix.GetNcols())*(matrix.GetNrows());
    TArrayD data(nelement);
    for(int i = 0; i<nelement; i ++){
        data[i]=i;
    }
    matrix.Use(matrix.GetNrows(),matrix.GetNcols(),data.GetArray());
    matrix.Print();
    Correctionsolver *test = new Correctionsolver();
    const TMatrixD &ev=test->GetAverageMatrixD(matrix);
    test->GetEigenMatrix(matrix);
    (test->GetEigenValueMatrix(matrix)).Print();
    std::cout<<"max eigen value : "<< (test->GetEigenValueMax(matrix))<<std::endl;
    ev.Print();

}
int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
