#include <TTimeStamp.h>

#include <RAT/DS/Frame.hh>
#include <RAT/DS/MC.hh>
#include <RAT/DS/Root.hh>
#include <RAT/DS/EV.hh>
#include <RAT/StreamingFrameProc.hh>
#include <cmath>
#include <cstdint>

namespace RAT {

namespace {
constexpr double kMsToNs = 1e6;
constexpr int64_t kNsPerSecond = 1000000000LL;
}  // namespace

StreamingFrameProc::StreamingFrameProc() : Processor("streamingframe"), frameWindowLengthMs(1000.0) {}

void StreamingFrameProc::SetD(std::string param, double value) {
  if (param == "frame_window_length_ms") {
    if (value <= 0.0) {
      throw ParamInvalid(param, "frame_window_length_ms must be positive");
    }
    frameWindowLengthMs = value;
  } else {
    throw ParamUnknown(param);
  }
}

Processor::Result StreamingFrameProc::DSEvent(DS::Root *ds) {
  DS::MC *mc = ds->GetMC();
  const bool hasPrimaries = mc->GetMCParticleCount() > 0;
  const int64_t frameWindowLengthNs = static_cast<int64_t>(frameWindowLengthMs * kMsToNs);

  int frameIndex = 0;
  uint64_t utcSeconds = 0;
  uint64_t utc16NanosecondCycles = 0;
  int64_t frameStartTimeNs = 0;

  if (hasPrimaries) {
    const TTimeStamp utc = mc->GetUTC();
    const uint64_t eventSeconds = static_cast<uint64_t>(utc.GetSec());
    const uint64_t eventNanoseconds = static_cast<uint64_t>(utc.GetNanoSec());
    const uint64_t eventTimeNs = eventSeconds * static_cast<uint64_t>(kNsPerSecond) + eventNanoseconds;

    frameIndex = static_cast<int>(eventTimeNs / static_cast<uint64_t>(frameWindowLengthNs));
    frameStartTimeNs = static_cast<int64_t>((eventTimeNs / static_cast<uint64_t>(frameWindowLengthNs)) *
                                            static_cast<uint64_t>(frameWindowLengthNs));
  } else {
    if (!ds->ExistEV()) {
      return Processor::OK;
    }
    DS::EV *ev = ds->GetEV(0);
    frameIndex = ev->GetID();
    frameStartTimeNs = static_cast<int64_t>(frameIndex) * frameWindowLengthNs;
  }

  utcSeconds = static_cast<uint64_t>(frameStartTimeNs / kNsPerSecond);
  utc16NanosecondCycles = static_cast<uint64_t>((frameStartTimeNs % kNsPerSecond) / 16);

  DS::Frame *frame = ds->GetFrame();
  frame->SetFrameIndex(frameIndex);
  frame->SetUtcSeconds(utcSeconds);
  frame->SetUtc16NanosecondCycles(utc16NanosecondCycles);
  frame->SetFrameStartTimeNs(frameStartTimeNs);
  frame->SetFrameWindowLengthNs(frameWindowLengthNs);

  for (int i = 0; i < ds->GetEVCount(); i++) {
    ds->GetEV(i)->SetFrameIndex(frameIndex);
  }

  return Processor::OK;
}

}  // namespace RAT
