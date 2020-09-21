#ifndef analysis_sm_test_analysis_h
#define analysis_sm_test_analysis_h

#include <math.h>
#include <TH1F.h>
#include <TFile.h>
#include "SampleAnalyzer/Process/Analyzer/AnalyzerBase.h"

TH1F* plot_mass_check;  
TH1F* plot_invar_mass;  

namespace MA5{

  class sm_test_analysis : public AnalyzerBase
  {
    INIT_ANALYSIS(sm_test_analysis,"sm_test_analysis")
    
    public:
    virtual bool Initialize(const MA5::Configuration& cfg, const std::map<std::string,std::string>& parameters);
    virtual void Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files);
    virtual bool Execute(SampleFormat& sample, const EventFormat& event);
  private:
  };
}

#endif
