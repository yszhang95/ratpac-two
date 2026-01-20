#ifndef __RAT_DS_FrameLight__
#define __RAT_DS_FrameLight__

#include <TObject.h>
#include <vector>

namespace RAT {
namespace DS {

class FrameLight : public TObject {
 public:
  FrameLight() : TObject() {}
  virtual ~FrameLight() {}

  virtual int GetFrameIndex() const { return frameIndex; }
  virtual void SetFrameIndex(int _frameIndex) { frameIndex = _frameIndex; }

  virtual double GetFrameStartTimeNs() const { return frameStartTimeNs; }
  virtual void SetFrameStartTimeNs(double _frameStartTimeNs) { frameStartTimeNs = _frameStartTimeNs; }

  virtual double GetFrameWindowLengthNs() const { return frameWindowLengthNs; }
  virtual void SetFrameWindowLengthNs(double _frameWindowLengthNs) { frameWindowLengthNs = _frameWindowLengthNs; }

  std::vector<int> &GetMCPMTID() { return mcpepmtid; }
  const std::vector<int> &GetMCPMTID() const { return mcpepmtid; }

  std::vector<double> &GetMCPEHitTime() { return mcpehittime; }
  const std::vector<double> &GetMCPEHitTime() const { return mcpehittime; }

  std::vector<double> &GetMCPEHitTimeRel() { return mcpehittimeRel; }
  const std::vector<double> &GetMCPEHitTimeRel() const { return mcpehittimeRel; }

  std::vector<double> &GetMCPEFrontEndTime() { return mcpefrontendtime; }
  const std::vector<double> &GetMCPEFrontEndTime() const { return mcpefrontendtime; }

  std::vector<int> &GetMCPEProcess() { return mcpeprocess; }
  const std::vector<int> &GetMCPEProcess() const { return mcpeprocess; }

  std::vector<double> &GetMCPEWavelength() { return mcpewavelength; }
  const std::vector<double> &GetMCPEWavelength() const { return mcpewavelength; }

  std::vector<double> &GetMCPEX() { return mcpex; }
  const std::vector<double> &GetMCPEX() const { return mcpex; }

  std::vector<double> &GetMCPEY() { return mcpey; }
  const std::vector<double> &GetMCPEY() const { return mcpey; }

  std::vector<double> &GetMCPEZ() { return mcpez; }
  const std::vector<double> &GetMCPEZ() const { return mcpez; }

  std::vector<double> &GetMCPECharge() { return mcpecharge; }
  const std::vector<double> &GetMCPECharge() const { return mcpecharge; }

  void Clear() {
    mcpepmtid.clear();
    mcpehittime.clear();
    mcpehittimeRel.clear();
    mcpefrontendtime.clear();
    mcpeprocess.clear();
    mcpewavelength.clear();
    mcpex.clear();
    mcpey.clear();
    mcpez.clear();
    mcpecharge.clear();
  }

  ClassDef(FrameLight, 1);

 protected:
  int frameIndex = 0;
  double frameStartTimeNs = 0.0;
  double frameWindowLengthNs = 0.0;
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

}  // namespace DS
}  // namespace RAT

#endif
