#ifndef ROOT_PLANEHit
#define ROOT_PLANEHit

#include "TObject.h"
#include "TVector3.h"

#include "plane.h"
#include "utility.h"

class planeHit: public TObject{
public:
  planeHit(){}
  planeHit(planeId id, Double_t position):fPlaneId(id), fPlanePosition(position){}
 planeHit(planeId id, Double_t position, Int_t hitLocalXId, Int_t hitLocalYId, Double_t x, Double_t y, Double_t xErr=1, Double_t yErr=1):fPlaneId(id), fPlanePosition(position), fHitLocalXId(hitLocalXId), fHitLocalYId(hitLocalYId), fX(x), fY(y), fXErr(xErr), fYErr(yErr){}

  inline planeId getPlaneId() const{return fPlaneId;}

  inline void setPlanePosition(Double_t position) { fPlanePosition = position; }
  inline Double_t getPlanePosition() const{ return fPlanePosition;}

  inline int getLocalXHitId () const {return fHitLocalXId;}
  inline int getLocalYHitId () const {return fHitLocalYId;}
  inline TVector3 getPosition3D () const{ 
    TVector3 pos(fX,fY,fPlanePosition);
    return pos;}
  inline TVector3 getPosition3DError () const{
    TVector3 posErr(fXErr,fYErr,0);
    return posErr;
  }

  inline void setX (Double_t x){ fX = x; }
  inline void setY (Double_t y){ fY = y; }

  inline void setXError (Double_t xErr){ fXErr = xErr; }
  inline void setYError (Double_t yErr){ fYErr = yErr; }

  inline Double_t getX () const{ return fX; }
  inline Double_t getY () const{ return fY; }

  inline Double_t getXError () const{ return fXErr; }
  inline Double_t getYError () const{ return fYErr; }

  virtual ~planeHit() {}

private:
  planeId fPlaneId;
  Double_t fPlanePosition;

  int fHitLocalXId; // hit # in local X direction; start from 0
  int fHitLocalYId; // hit # in local Y direction; start from 0
  Double_t fX;
  Double_t fY;
  Double_t fXErr;
  Double_t fYErr;

  ClassDef(planeHit,1)    

};

#endif
