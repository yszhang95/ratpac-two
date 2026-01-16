#ifndef __RATOutFrameMetaProc___
#define __RATOutFrameMetaProc___

#include <RAT/Processor.hh>
#include <cstdint>
#include <string>

class TFile;
class TTree;

namespace RAT {

class OutFrameMetaProc : public Processor {
 public:
  OutFrameMetaProc();
  virtual ~OutFrameMetaProc();

  virtual Processor::Result DSEvent(DS::Root *ds);
  virtual void SetS(std::string param, std::string value);

 private:
  bool OpenFile(const std::string &filename, bool update);

  std::string defaultFilename;
  std::string filename;
  TFile *outputFile;
  TTree *frameTree;
  int lastFrameIndex;

  int frameIndex;
  uint64_t utcSeconds;
  uint64_t utc16NanosecondCycles;
};

}  // namespace RAT

#endif
