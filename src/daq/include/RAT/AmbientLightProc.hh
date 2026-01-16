#ifndef __RAT_AmbientLightProc__
#define __RAT_AmbientLightProc__

#include <RAT/PMTCharge.hh>
#include <RAT/Processor.hh>
#include <string>
#include <vector>

namespace RAT {

class AmbientLightProc : public Processor {
 public:
  AmbientLightProc();
  virtual ~AmbientLightProc();
  virtual void BeginOfRun(DS::Run *run);
  virtual Processor::Result DSEvent(DS::Root *ds);
  virtual void SetD(std::string param, double value);

 private:
  void UpdatePMTModels(DS::PMTInfo *pmtinfo);

  double rate;
  int lastFrameIndex;
  std::vector<RAT::PMTCharge *> fPMTCharge;
};

}  // namespace RAT

#endif
