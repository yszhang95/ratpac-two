/**
 * @class PMT
 * Data Structure: Output of analysis of digited PMT waveform
 *prepre
 * This represents information about the digitzed PMT waveform in a detector event.
 */

#ifndef __RAT_DS_DiscreteSignalPMT__
#define __RAT_DS_DiscreteSignalPMT__

#include <Rtypes.h>
#include <TObject.h>

#include <RAT/DS/WaveformAnalysisResult.hh>
#include <RAT/Log.hh>
#include <RAT/WaveformUtil.hh>
#include <limits>
#include "DigitPMT.hh"
#include "RAT/DS/DigitPMT.hh"

namespace RAT {
namespace DS {

/** Processed waveform information **/
  using DiscreteSignalPMT = DigitPMT;

}  // namespace DS
}  // namespace RAT

#endif
