#include <iostream>
#include "Pythia8/Pythia.h"
#include "TH1.h"
#include "TVirtualPad.h"
#include "TApplication.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TNtuple.h"
using namespace Pythia8;

int main() {
  Pythia pythia;
  pythia.readFile("Heavy-Flavor-MinBias.cmnd");
 // pythia.readString("Beams:eCM = 2760.");
 // pythia.readString("SoftQCD:nonDiffractive = on");
  pythia.init();
  double Events, i, nchar, ET, ETmid;
  Vec4 particle; //4-Vector for each particle
  cout << "Number of Events -> ";
  scanf("%lf", &Events);
  //Possibly add bMPI and nMPI later on
  TNtuple PPTuple("PPTuple", "PPTuple", "NChar:ET:ETmid:bMPI:nMPI");
  for (i = 0; i < Events; i++) {
        if (!pythia.next()) continue;
        ET = 0;
	ETmid = 0;
        nchar = 0;
        for (int k = 0; k < pythia.event.size(); k++) {
                particle = pythia.event[k].p();
                if (pythia.event[k].isFinal() && pythia.event[k].isCharged()) {
                        if(particle.pT() >= .10 && abs(particle.eta()) < 2.4) {
                                nchar++;}}
                if(pythia.event[k].isFinal() && abs(particle.eta()) < 2.4 && particle.pT() >= .10) {
			ETmid = ETmid + particle.eT(); }
		if(pythia.event[k].isFinal() && abs(particle.eta()) > 4 && abs(particle.eta()) < 5.2){
			ET = ET + particle.eT(); }}
	PPTuple.Fill(nchar, ET, ETmid, pythia.info.bMPI(), pythia.info.nMPI());  }
  pythia.stat();
  TFile out("PPCollisionMilWithTune.root", "RECREATE");
  PPTuple.Write();
  out.Close();
  return 0; }
