#include <TSystem>
#include "TStopwatch.h"

class StMaker;
class StChain;
class StPicoDstMaker;
class StPicoDst;
class StPicoEvent;

StChain *chain;

void VecMesonTree(const Char_t *inputFile="../FileList/54GeV_2017/pico_xrootd_local.list", const string jobId = "14", const Int_t Mode = 1, const Int_t energy = 1, const Int_t flag_ME = 0)
// void VecMesonTree(const Char_t *inputFile="../FileList/27GeV_2018/pico_xrootd_local.list", const string jobId = "14", const Int_t Mode = 1, const Int_t energy = 2, const Int_t flag_ME = 0)
{
  // mBeamEnergy[NumBeamEnergy] = {"200GeV","54GeV","27GeV"};
  // Mode: 0 for QA, 1 for re-center correction, 2 for shift correction, 3 for resolution calculation, 4 for phi meson
  // flag_ME: 0 for Same Event, 1 for Mixed Event

  TStopwatch *stopWatch = new TStopwatch();
  stopWatch->Start();

  string SL_version = "pro";
  if(energy == 0) SL_version = "SL18h"; // 200GeV_2014
  if(energy == 1) SL_version = "SL18c"; // 54GeV_2017
  if(energy == 2) SL_version = "SL19b"; // 27GeV_2018
  string env_SL = getenv ("STAR");
  if (env_SL.find(SL_version)==string::npos) 
  {
    cout<<"Environment Star Library does not match the requested library in runPicoMixedEventMaker.C. Exiting..."<<endl;
    exit(1);
  }

  // Int_t nEvents = 10000000;
  Int_t nEvents = 10000;

  gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
  loadSharedLibraries();

  gSystem->Load("StPicoEvent");
  gSystem->Load("StPicoDstMaker");
  gSystem->Load("StRefMultCorr");
  gSystem->Load("StAlexPhiMesonEvent");
  gSystem->Load("StVecMesonMaker");

  chain = new StChain();
  StPicoDstMaker *picoMaker = new StPicoDstMaker(2,inputFile,"picoDst");

  StVecMesonMaker *VecMesonMaker = new StVecMesonMaker("VecMeson",picoMaker,jobId,Mode,energy,flag_ME);

  if( chain->Init()==kStErr ){ 
    cout<<"chain->Init();"<<endl;
    return;
  }

  int total = picoMaker->chain()->GetEntries();
  cout << " Total entries = " << total << endl;
  if(nEvents>total) nEvents = total;

  for (Int_t i=0; i<nEvents; i++)
  {
    if(i != 0 && i%50 == 0)
    {
      cout << "." << flush;
    }
    if(i != 0 && i%500 == 0)
    {
      Float_t event_percent = 100.0*i/nEvents;
      cout << " " << i << "(" << event_percent << "%)" << "\n" << " ==> Processing data " << flush;
    }

    chain->Clear();
    int iret = chain->Make(i);

    if (iret)
    { 
      cout << "Bad return code!" << iret << endl;
      break;
    }

    total++;
  }

  cout << "." << flush;
  cout << " " << nEvents << "(" << 100 << "%)";
  cout << endl;
  cout << "****************************************** " << endl;
  cout << "Work done... now its time to close up shop!"<< endl;
  cout << "****************************************** " << endl;
  chain->Finish();
  cout << "****************************************** " << endl;
  cout << "total number of events  " << nEvents << endl;
  cout << "****************************************** " << endl;

  stopWatch->Stop();
  stopWatch->Print();

  delete VecMesonMaker;
  delete picoMaker;
  delete chain;
}
