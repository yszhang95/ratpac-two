#ifndef __RAT_StreamingDAQProc__
#define __RAT_StreamingDAQProc__

#include <RAT/Processor.hh>

namespace RAT {

class StreamingDAQProc : public Processor {
 public:
  StreamingDAQProc();
  virtual Processor::Result DSEvent(DS::Root *ds);

 private:
  int eventIndex;
};

}  // namespace RAT

#endif
