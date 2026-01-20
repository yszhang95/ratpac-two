#include <RAT/DS/MC.hh>
#include <RAT/DS/Root.hh>
#include <RAT/DS/EV.hh>
#include <RAT/StreamingDAQProc.hh>

namespace RAT {

StreamingDAQProc::StreamingDAQProc() : Processor("streamingdaq"), eventIndex(0) {}

Processor::Result StreamingDAQProc::DSEvent(DS::Root *ds) {
  DS::MC *mc = ds->GetMC();
  if (mc->GetMCParticleCount() > 0) {
    return Processor::OK;
  }

  if (ds->ExistEV()) {
    ds->PruneEV();
  }

  DS::EV *ev = ds->AddNewEV();
  ev->SetID(eventIndex);
  eventIndex++;

  return Processor::OK;
}

}  // namespace RAT
