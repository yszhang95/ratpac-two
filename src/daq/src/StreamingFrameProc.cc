#include <TTimeStamp.h>

#include <CLHEP/Units/SystemOfUnits.h>
#include <TTimeStamp.h>

#include <RAT/DS/EV.hh>
#include <RAT/DS/Frame.hh>
#include <RAT/DS/MC.hh>
#include <RAT/DS/Root.hh>
#include <RAT/StreamingFrameProc.hh>
#include <cmath>
#include <cstdint>

namespace RAT {

StreamingFrameProc::StreamingFrameProc() : Processor("streamingframe"), frameWindowLengthMs(100.0) {}

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
  const double frameWindowLengthNs = frameWindowLengthMs * CLHEP::ms;

  int frameIndex = 0;
  uint64_t utcSeconds = 0;
  uint64_t utc16NanosecondCycles = 0;
  double frameStartTimeNs = 0.0;

  if (hasPrimaries) {
    const TTimeStamp utc = mc->GetUTC();
    const double eventTimeNs = static_cast<double>(utc.GetSec()) * CLHEP::s +
                               static_cast<double>(utc.GetNanoSec()) * CLHEP::ns;
    const double eventTimeMs = eventTimeNs / CLHEP::ms;

    frameIndex = static_cast<int>(std::floor(eventTimeMs / frameWindowLengthMs));
    frameStartTimeNs = static_cast<double>(frameIndex) * frameWindowLengthNs;
  } else {
    if (!ds->ExistEV()) {
      return Processor::OK;
    }
    DS::EV *ev = ds->GetEV(0);
    frameIndex = ev->GetID();
    frameStartTimeNs = static_cast<double>(frameIndex) * frameWindowLengthNs;
  }

  utcSeconds = static_cast<uint64_t>(std::floor(frameStartTimeNs / CLHEP::s));
  utc16NanosecondCycles = static_cast<uint64_t>(
      std::floor(std::fmod(frameStartTimeNs, CLHEP::s) / (16.0 * CLHEP::ns)));

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
