#include <iostream>
using namespace std;

#include <iomanip>
#include "TString.h"

#include "matrix.h"


//___________________________________________________________
matrix::matrix(Int_t rowdim, Int_t coldim) : TMatrixD(rowdim, coldim) {}
//___________________________________________________________
matrix::matrix(const matrix &orig) : TMatrixD(orig){}
//___________________________________________________________
matrix::matrix(const TMatrixD &orig) : TMatrixD(orig){}
//___________________________________________________________
matrix::matrix(TMatrixD::EMatrixCreatorsOp1 op, const matrix &prototype) : TMatrixD(op, prototype){}
//___________________________________________________________
matrix::matrix(TMatrixD::EMatrixCreatorsOp1 op, const TMatrixD &prototype) : TMatrixD(op, prototype){}
//___________________________________________________________
matrix::matrix(const matrix &a, TMatrixD::EMatrixCreatorsOp2 op, const matrix &b) : TMatrixD(a, op, b){}
//___________________________________________________________
matrix::matrix(const TMatrixD &a, TMatrixD::EMatrixCreatorsOp2 op, const TMatrixD &b) : TMatrixD(a, op, b){}
//___________________________________________________________
matrix::matrix(const TVector3 &v) : TMatrixD(matrix::ToKalMat(v)){}
//___________________________________________________________
void matrix::DebugPrint(Option_t *opt, Int_t ncolsps) const{
  Int_t ncols   = GetNcols();
  Int_t nrows   = GetNrows();
  Int_t nsheets = (ncols-1)/ncolsps + 1;
  Int_t lastcw  = ncols - ncolsps*(nsheets-1);
  Int_t ns      = 0;

  TString title(opt);
  TString off(title.Length());
  for (Int_t c=0; c<title.Length(); c++) off += " ";

  for (Int_t sc = 1; sc <= ncols; sc += ncolsps) {
    ns++;
    if (ns == 1) cerr << title << "+-";
    if (ns == nsheets) {
      for (Int_t ib = 1; ib <= 11*lastcw; ib++) cerr << " ";
      cerr << "-+" << endl;
    } else {
      cerr << endl;
    }
    for (Int_t i = 1; i <= nrows; i++) {
      if (ns == 1) cerr << off << "| ";
      for (Int_t j = sc; j < sc+ncolsps && j <= ncols; j++) {
	cerr // << setiosflags(ios::scientific) 
	  << setw(11) << setprecision(4)
	  << (*this)(i-1,j-1);
	if (j == ncols) cerr << " |";
      }
      cerr << endl;
    }
    if (ns == 1) cerr << off << "+-";
    if (ns == nsheets) {
      for (Int_t ib = 1; ib <= 11*lastcw; ib++) cerr << " ";
      cerr << "-+" << endl;
    } else {
      cerr << endl;
    }
  }
}
//___________________________________________________________
matrix matrix::ToKalMat(const TVector3 &a){
  matrix md(3,1);
  md(0,0) = a.x();
  md(1,0) = a.y();
  md(2,0) = a.z();
  return md;
}
//___________________________________________________________
TVector3 matrix::ToThreeVec(const TMatrixD &a){
  TVector3 v;
  v.SetXYZ(a(0,0), a(1,0),a(2,0));
  return v;
}
//___________________________________________________________

ClassImp(matrix)








