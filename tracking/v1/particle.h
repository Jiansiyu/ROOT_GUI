#ifndef ROOT_PARTICLE
#define ROOT_PARTICLE

#include "TObject.h"
#include "utility.h"

class particle: public TObject{
 public:
  particle(particleId id, double mass, double charge);
  virtual ~particle(){}

  inline void      setParticleId(particleId id) {fParticleId = id;}
  inline particleId  getParticleId() const {return fParticleId;}
  inline void      setParticleMass(Double_t m) {fParticleMass = m;}
  inline Double_t  getParticleMass() const   { return fParticleMass; }
  inline void      setParticleCharge(Double_t q) { fParticleCharge = q;  }
  inline Double_t  getParticleCharge() const   { return fParticleCharge; }

 protected:
  particleId fParticleId;
  Double_t fParticleMass;
  Double_t fParticleCharge;

 private:
  ClassDef(particle,1)      
};

#endif
