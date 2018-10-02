#ifndef ROOT_KAL_MATRIX
#define ROOT_KAL_MATRIX

#include "TMatrixD.h"
#include "TVector3.h"

class matrix : public TMatrixD {
public:
   matrix(Int_t rowdim = 1, Int_t coldim = 1);

   matrix(const matrix &orig);
   matrix(const TMatrixD &orig);

   matrix(TMatrixD::EMatrixCreatorsOp1 op,
              const matrix &prototype);
   matrix(TMatrixD::EMatrixCreatorsOp1 op,
              const TMatrixD &prototype);

   matrix(const matrix &a,
              TMatrixD::EMatrixCreatorsOp2 op,
              const matrix &b) ;
   matrix(const TMatrixD &a,
              TMatrixD::EMatrixCreatorsOp2 op,
              const TMatrixD &b) ;

   matrix(const TVector3 &v);

   virtual ~matrix() {}

   virtual void      DebugPrint(Option_t *opt = "", Int_t nc = 5) const;

   static matrix ToKalMat  (const TVector3 &vec);
   static TVector3   ToThreeVec(const TMatrixD &mat);

   private:
   ClassDef(matrix,1)     

};

#endif
