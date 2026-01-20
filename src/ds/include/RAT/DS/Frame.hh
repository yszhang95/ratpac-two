#ifndef __RAT_DS_Frame__
#define __RAT_DS_Frame__

#include <TObject.h>
#include <cstdint>

namespace RAT {
namespace DS {

class Frame : public TObject {
 public:
  Frame() : TObject() {}
  virtual ~Frame() {}

  virtual int GetFrameIndex() const { return frameIndex; }
  virtual void SetFrameIndex(int _frameIndex) { frameIndex = _frameIndex; }

  virtual uint64_t GetUtcSeconds() const { return utcSeconds; }
  virtual void SetUtcSeconds(uint64_t _utcSeconds) { utcSeconds = _utcSeconds; }

  virtual uint64_t GetUtc16NanosecondCycles() const { return utc16NanosecondCycles; }
  virtual void SetUtc16NanosecondCycles(uint64_t _utc16NanosecondCycles) {
    utc16NanosecondCycles = _utc16NanosecondCycles;
  }

  virtual double GetFrameStartTimeNs() const { return frameStartTimeNs; }
  virtual void SetFrameStartTimeNs(double _frameStartTimeNs) { frameStartTimeNs = _frameStartTimeNs; }

  virtual double GetFrameWindowLengthNs() const { return frameWindowLengthNs; }
  virtual void SetFrameWindowLengthNs(double _frameWindowLengthNs) { frameWindowLengthNs = _frameWindowLengthNs; }

  ClassDef(Frame, 1);

 protected:
  int frameIndex = 0;
  uint64_t utcSeconds = 0;
  uint64_t utc16NanosecondCycles = 0;
  double frameStartTimeNs = 0;
  double frameWindowLengthNs = 0;
};

}  // namespace DS
}  // namespace RAT

#endif
