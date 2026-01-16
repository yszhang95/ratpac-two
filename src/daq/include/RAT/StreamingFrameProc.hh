#ifndef __RAT_StreamingFrameProc__
#define __RAT_StreamingFrameProc__

#include <RAT/Processor.hh>
#include <string>

namespace RAT {

class StreamingFrameProc : public Processor {
 public:
  StreamingFrameProc();
  virtual Processor::Result DSEvent(DS::Root *ds);
  virtual void SetD(std::string param, double value);

 private:
  double frameWindowLengthMs;
};

}  // namespace RAT

#endif
