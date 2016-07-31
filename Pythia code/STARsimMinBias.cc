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
  pythia.readString("Beams:eCM = 500.");
  pythia.readString("SoftQCD:nonDiffractive = on");
  pythia.init();
  //TH1F *nChar = new TH1F("nChar", "Number of Charged Particles", 100, 0, 100);
  //TH1F *sumE = new TH1F("sumE", "Sum of Transverse Energy", 250, 0, 250);
  double Events, i, nchar, ET;
  Vec4 particle; //4-Vector for each particle
  cout << "Number of Events -> ";
  scanf("%lf", &Events);
  //Possibly add bMPI and nMPI later on
  TNtuple PPTuple("PPTuple", "PPTuple", "NChar:ET:bMPI:nMPI");
  for (i = 0; i < Events; i++) {
        if (!pythia.next()) continue;
        ET = 0;
        nchar = 0;
        for (int k = 0; k < pythia.event.size(); k++) {
                particle = pythia.event[k].p();
                if (pythia.event[k].isFinal() && pythia.event[k].isCharged()) {
                        if(particle.pT() >= .10 && abs(particle.eta()) < 1.) {
                                nchar++;}}
		if(pythia.event[k].isFinal() && (abs(pythia.event[k].id()) == 11 ||  pythia.event[k].id() == 22) && particle.eta() > 2.4 && particle.eta() < 4.0) {
				ET = ET + particle.eT();  }
	PPTuple.Fill(nchar, ET, pythia.info.bMPI(), pythia.info.nMPI()); }
  pythia.stat();
  TFile out("STARMinBias.root", "RECREATE");
  PPTuple.Write();
  out.Close();
  return 0; }
