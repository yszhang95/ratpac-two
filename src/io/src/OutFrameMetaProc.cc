#include <TFile.h>
#include <TTree.h>

#include <RAT/DS/Frame.hh>
#include <RAT/DS/Root.hh>
#include <RAT/Log.hh>
#include <RAT/OutFrameMetaProc.hh>

namespace RAT {

OutFrameMetaProc::OutFrameMetaProc()
    : Processor("outframemeta"),
      defaultFilename("frame_meta.root"),
      outputFile(nullptr),
      frameTree(nullptr),
      lastFrameIndex(-1),
      frameIndex(0),
      utcSeconds(0),
      utc16NanosecondCycles(0) {}

OutFrameMetaProc::~OutFrameMetaProc() {
  if (!outputFile) {
    return;
  }
  outputFile->cd();
  if (frameTree) {
    frameTree->Write();
  }
  outputFile->Close();
  delete outputFile;
}

void OutFrameMetaProc::SetS(std::string param, std::string value) {
  if (param == "file") {
    if (!OpenFile(value, false)) {
      Log::Die("outframemeta: Cannot open file " + value);
    }
  } else if (param == "updatefile") {
    if (!OpenFile(value, true)) {
      Log::Die("outframemeta: Cannot open file " + value);
    }
  }
}

bool OutFrameMetaProc::OpenFile(const std::string &outputFilename, bool update) {
  filename = outputFilename;
  outputFile = TFile::Open(outputFilename.c_str(), update ? "UPDATE" : "RECREATE");
  if (!outputFile) {
    return false;
  }

  if (update) {
    frameTree = static_cast<TTree *>(outputFile->Get("frame_meta"));
  }

  if (!frameTree) {
    frameTree = new TTree("frame_meta", "frame_meta");
    frameTree->Branch("frame_index", &frameIndex);
    frameTree->Branch("utc_seconds", &utcSeconds);
    frameTree->Branch("utc_16nanosecondcycles", &utc16NanosecondCycles);
  } else {
    frameTree->SetBranchAddress("frame_index", &frameIndex);
    frameTree->SetBranchAddress("utc_seconds", &utcSeconds);
    frameTree->SetBranchAddress("utc_16nanosecondcycles", &utc16NanosecondCycles);
  }

  return true;
}

Processor::Result OutFrameMetaProc::DSEvent(DS::Root *ds) {
  if (!outputFile) {
    if (!OpenFile(defaultFilename, false)) {
      Log::Die("outframemeta: Cannot open default file " + defaultFilename);
    }
  }

  if (!ds->ExistFrame()) {
    return Processor::OK;
  }

  DS::Frame *frame = ds->GetFrame();
  const int currentIndex = frame->GetFrameIndex();
  if (currentIndex == lastFrameIndex) {
    return Processor::OK;
  }

  frameIndex = currentIndex;
  utcSeconds = frame->GetUtcSeconds();
  utc16NanosecondCycles = frame->GetUtc16NanosecondCycles();
  frameTree->Fill();
  lastFrameIndex = currentIndex;

  return Processor::OK;
}

}  // namespace RAT
