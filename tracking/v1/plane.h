#ifndef ROOT_PLANE
#define ROOT_PLANE

#include "utility.h"

class plane: public TObject{
 public:
  plane(planeId id, Double_t position):fPlaneId(id), fPlanePosition(position){}
  virtual ~plane(){}

  inline planeId getPlaneId() const{return fPlaneId;}

  inline void setPlanePosition(Double_t position) { fPlanePosition = position; }
  inline Double_t getPlanePosition() const{ return fPlanePosition;}

 protected:
  planeId fPlaneId;
  Double_t fPlanePosition;

 private:
  ClassDef(plane,1)      

};

#endif
