#include <CLHEP/Random/RandPoisson.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include <Randomize.hh>

#include <cmath>
#include <string>
#include <RAT/DB.hh>
#include <RAT/DS/Frame.hh>
#include <RAT/DS/FrameLight.hh>
#include <RAT/DS/PMTInfo.hh>
#include <RAT/DS/Root.hh>
#include <RAT/DS/Run.hh>
#include <RAT/DS/RunStore.hh>
#include <RAT/FrameLightProcessCodes.hh>
#include <RAT/Log.hh>
#include <RAT/PDFPMTCharge.hh>
#include <RAT/PDFPMTTime.hh>
#include <RAT/AmbientLightProc.hh>

namespace RAT {

AmbientLightProc::AmbientLightProc() : Processor("ambientlight"), rate(0.0), lastFrameIndex(-1) {}

AmbientLightProc::~AmbientLightProc() {
  for (auto *time : fPMTTime) {
    delete time;
  }
  for (auto *charge : fPMTCharge) {
    delete charge;
  }
}

void AmbientLightProc::BeginOfRun(DS::Run *run) {
  if (!run) {
    return;
  }
  UpdatePMTModels(run->GetPMTInfo());
}

void AmbientLightProc::UpdatePMTModels(DS::PMTInfo *pmtinfo) {
  if (!pmtinfo) {
    return;
  }

  for (auto *time : fPMTTime) {
    delete time;
  }
  for (auto *charge : fPMTCharge) {
    delete charge;
  }
  fPMTTime.clear();
  fPMTCharge.clear();

  const size_t numModels = pmtinfo->GetModelCount();
  fPMTTime.resize(numModels, nullptr);
  fPMTCharge.resize(numModels, nullptr);
  for (size_t i = 0; i < numModels; i++) {
    const std::string modelName = pmtinfo->GetModelName(i);
    try {
      fPMTTime[i] = new RAT::PDFPMTTime(modelName);
      info << "AmbientLightProc: Loaded PDFPMTTime for " << modelName << newline;
    } catch (DBNotFoundError &e) {
      Log::Die("AmbientLightProc: Missing PMTTime model for " + modelName);
    }

    try {
      fPMTCharge[i] = new RAT::PDFPMTCharge(modelName);
      info << "AmbientLightProc: Loaded PDFPMTCharge for " << modelName << newline;
    } catch (DBNotFoundError &e) {
      fPMTCharge[i] = new RAT::PDFPMTCharge();
      info << "AmbientLightProc: Loaded PDFPMTCharge DEFAULT for " << modelName << newline;
    }
  }
}

void AmbientLightProc::SetD(std::string param, double value) {
  if (param == "rate") {
    if (value < 0.0) {
      throw ParamInvalid(param, "rate must be non-negative");
    }
    rate = value;
  } else {
    throw ParamUnknown(param);
  }
}

Processor::Result AmbientLightProc::DSEvent(DS::Root *ds) {
  if (!ds || !ds->ExistFrame()) {
    return Processor::OK;
  }

  DS::Frame *frame = ds->GetFrame();
  const int frameIndex = frame->GetFrameIndex();
  if (frameIndex == lastFrameIndex) {
    return Processor::OK;
  }

  DS::Run *run = DS::RunStore::Get()->GetRun(ds);
  if (!run) {
    return Processor::OK;
  }

  DS::PMTInfo *pmtinfo = run->GetPMTInfo();
  if (!pmtinfo) {
    return Processor::OK;
  }

  const double frameWindowLengthNs = frame->GetFrameWindowLengthNs();
  if (frameWindowLengthNs <= 0.0) {
    return Processor::OK;
  }

  const double frameWindowSeconds = frameWindowLengthNs / CLHEP::s;
  const double meanPE = rate * frameWindowSeconds;

  DS::FrameLight *frameLight = ds->GetFrameLight();
  frameLight->Clear();
  frameLight->SetFrameIndex(frameIndex);
  frameLight->SetFrameStartTimeNs(frame->GetFrameStartTimeNs());
  frameLight->SetFrameWindowLengthNs(frameWindowLengthNs);

  std::vector<int> &mcpepmtid = frameLight->GetMCPMTID();
  std::vector<double> &mcpehittime = frameLight->GetMCPEHitTime();
  std::vector<double> &mcpehittimeRel = frameLight->GetMCPEHitTimeRel();
  std::vector<double> &mcpefrontendtime = frameLight->GetMCPEFrontEndTime();
  std::vector<int> &mcpeprocess = frameLight->GetMCPEProcess();
  std::vector<double> &mcpewavelength = frameLight->GetMCPEWavelength();
  std::vector<double> &mcpex = frameLight->GetMCPEX();
  std::vector<double> &mcpey = frameLight->GetMCPEY();
  std::vector<double> &mcpez = frameLight->GetMCPEZ();
  std::vector<double> &mcpecharge = frameLight->GetMCPECharge();

  const int pmtCount = pmtinfo->GetPMTCount();
  for (int pmtid = 0; pmtid < pmtCount; pmtid++) {
    const int numPE = static_cast<int>(std::floor(CLHEP::RandPoisson::shoot(meanPE)));
    if (numPE <= 0) {
      continue;
    }

    const int modelIndex = pmtinfo->GetModel(pmtid);
    if (modelIndex < 0 || static_cast<size_t>(modelIndex) >= fPMTTime.size()) {
      Log::Die("AmbientLightProc: Invalid PMT model index for PMTID " + std::to_string(pmtid));
    }
    RAT::PMTTime *timeModel = fPMTTime[modelIndex];
    if (!timeModel) {
      Log::Die("AmbientLightProc: Missing PMTTime model for PMTID " + std::to_string(pmtid));
    }
    RAT::PMTCharge *chargeModel = nullptr;
    if (modelIndex >= 0 && static_cast<size_t>(modelIndex) < fPMTCharge.size()) {
      chargeModel = fPMTCharge[modelIndex];
    }

    for (int i = 0; i < numPE; i++) {
      const double hitTimeRel = G4UniformRand() * frameWindowLengthNs;
      const double hitTime = frame->GetFrameStartTimeNs() + hitTimeRel;

      mcpepmtid.push_back(pmtid);
      mcpehittime.push_back(hitTime);
      mcpehittimeRel.push_back(hitTimeRel);
      mcpefrontendtime.push_back(timeModel->PickTime(hitTime));
      mcpeprocess.push_back(FrameLightProcess::kFrameAmbientLight);
      mcpewavelength.push_back(0.0);
      mcpex.push_back(0.0);
      mcpey.push_back(0.0);
      mcpez.push_back(0.0);
      mcpecharge.push_back(chargeModel ? chargeModel->PickCharge() : 0.0);
    }
  }

  lastFrameIndex = frameIndex;
  return Processor::OK;
}

}  // namespace RAT
