#ifndef __RATOutFrameAmbientLightProc___
#define __RATOutFrameAmbientLightProc___

#include <RAT/Processor.hh>
#include <cstdint>
#include <string>
#include <vector>

class TFile;
class TTree;

namespace RAT {

class OutFrameAmbientLightProc : public Processor {
 public:
  OutFrameAmbientLightProc();
  virtual ~OutFrameAmbientLightProc();

  virtual Processor::Result DSEvent(DS::Root *ds);
  virtual void SetS(std::string param, std::string value);

 private:
  bool OpenFile(const std::string &filename, bool update);

  std::string defaultFilename;
  std::string filename;
  TFile *outputFile;
  TTree *ambientTree;
  int lastFrameIndex;

  int frameIndex;
  double frameStartTimeNs;
  double frameWindowLengthNs;
  std::vector<int> mcpepmtid;
  std::vector<double> mcpehittime;
  std::vector<double> mcpehittimeRel;
  std::vector<double> mcpefrontendtime;
  std::vector<int> mcpeprocess;
  std::vector<double> mcpewavelength;
  std::vector<double> mcpex;
  std::vector<double> mcpey;
  std::vector<double> mcpez;
  std::vector<double> mcpecharge;
};

}  // namespace RAT

#endif
