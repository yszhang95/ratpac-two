#include <TFile.h>
#include <TTree.h>

#include <RAT/DS/FrameLight.hh>
#include <RAT/DS/Root.hh>
#include <RAT/Log.hh>
#include <RAT/OutFrameAmbientLightProc.hh>

namespace RAT {

OutFrameAmbientLightProc::OutFrameAmbientLightProc()
    : Processor("outframeambientlight"),
      defaultFilename("frame_ambient_light.root"),
      outputFile(nullptr),
      ambientTree(nullptr),
      lastFrameIndex(-1),
      frameIndex(0),
      frameStartTimeNs(0.0),
      frameWindowLengthNs(0.0) {}

OutFrameAmbientLightProc::~OutFrameAmbientLightProc() {
  if (!outputFile) {
    return;
  }
  outputFile->cd();
  if (ambientTree) {
    ambientTree->Write();
  }
  outputFile->Close();
  delete outputFile;
}

void OutFrameAmbientLightProc::SetS(std::string param, std::string value) {
  if (param == "file") {
    if (!OpenFile(value, false)) {
      Log::Die("outframeambientlight: Cannot open file " + value);
    }
  } else if (param == "updatefile") {
    if (!OpenFile(value, true)) {
      Log::Die("outframeambientlight: Cannot open file " + value);
    }
  }
}

bool OutFrameAmbientLightProc::OpenFile(const std::string &outputFilename, bool update) {
  filename = outputFilename;
  outputFile = TFile::Open(outputFilename.c_str(), update ? "UPDATE" : "RECREATE");
  if (!outputFile) {
    return false;
  }

  if (update) {
    ambientTree = static_cast<TTree *>(outputFile->Get("frame_ambient_light"));
  }

  if (!ambientTree) {
    ambientTree = new TTree("frame_ambient_light", "frame_ambient_light");
    ambientTree->Branch("frame_index", &frameIndex);
    ambientTree->Branch("frame_start_time_ns", &frameStartTimeNs);
    ambientTree->Branch("frame_window_length_ns", &frameWindowLengthNs);
    ambientTree->Branch("mcpepmtid", &mcpepmtid);
    ambientTree->Branch("mcpehittime", &mcpehittime);
    ambientTree->Branch("mcpehittime_rel", &mcpehittimeRel);
    ambientTree->Branch("mcpefrontendtime", &mcpefrontendtime);
    ambientTree->Branch("mcpeprocess", &mcpeprocess);
    ambientTree->Branch("mcpewavelength", &mcpewavelength);
    ambientTree->Branch("mcpex", &mcpex);
    ambientTree->Branch("mcpey", &mcpey);
    ambientTree->Branch("mcpez", &mcpez);
    ambientTree->Branch("mcpecharge", &mcpecharge);
  } else {
    ambientTree->SetBranchAddress("frame_index", &frameIndex);
    ambientTree->SetBranchAddress("frame_start_time_ns", &frameStartTimeNs);
    ambientTree->SetBranchAddress("frame_window_length_ns", &frameWindowLengthNs);
    ambientTree->SetBranchAddress("mcpepmtid", &mcpepmtid);
    ambientTree->SetBranchAddress("mcpehittime", &mcpehittime);
    ambientTree->SetBranchAddress("mcpehittime_rel", &mcpehittimeRel);
    ambientTree->SetBranchAddress("mcpefrontendtime", &mcpefrontendtime);
    ambientTree->SetBranchAddress("mcpeprocess", &mcpeprocess);
    ambientTree->SetBranchAddress("mcpewavelength", &mcpewavelength);
    ambientTree->SetBranchAddress("mcpex", &mcpex);
    ambientTree->SetBranchAddress("mcpey", &mcpey);
    ambientTree->SetBranchAddress("mcpez", &mcpez);
    ambientTree->SetBranchAddress("mcpecharge", &mcpecharge);
  }

  return true;
}

Processor::Result OutFrameAmbientLightProc::DSEvent(DS::Root *ds) {
  if (!outputFile) {
    if (!OpenFile(defaultFilename, false)) {
      Log::Die("outframeambientlight: Cannot open default file " + defaultFilename);
    }
  }

  if (!ds->ExistFrameLight()) {
    return Processor::OK;
  }

  DS::FrameLight *frameLight = ds->GetFrameLight();
  const int currentIndex = frameLight->GetFrameIndex();
  if (currentIndex == lastFrameIndex) {
    return Processor::OK;
  }

  frameIndex = currentIndex;
  frameStartTimeNs = frameLight->GetFrameStartTimeNs();
  frameWindowLengthNs = frameLight->GetFrameWindowLengthNs();
  mcpepmtid = frameLight->GetMCPMTID();
  mcpehittime = frameLight->GetMCPEHitTime();
  mcpehittimeRel = frameLight->GetMCPEHitTimeRel();
  mcpefrontendtime = frameLight->GetMCPEFrontEndTime();
  mcpeprocess = frameLight->GetMCPEProcess();
  mcpewavelength = frameLight->GetMCPEWavelength();
  mcpex = frameLight->GetMCPEX();
  mcpey = frameLight->GetMCPEY();
  mcpez = frameLight->GetMCPEZ();
  mcpecharge = frameLight->GetMCPECharge();

  ambientTree->Fill();
  lastFrameIndex = currentIndex;

  return Processor::OK;
}

}  // namespace RAT
