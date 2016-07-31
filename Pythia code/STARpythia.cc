#include <iostream>
#include "Pythia8/Pythia.h"
#include "TH1.h"
#include "TVirtualPad.h"
#include "TApplication.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TNtuple.h"
#include "TF1.h"
using namespace Pythia8;

int main() {
  Pythia pythia;
  pythia.readFile("star_hf_tune_v1.1.cmnd");
  //pythia.readString("Beams:eCM = 500.");
  //pythia.readString("Bottomonium:gg2bbbar(3S1)[3S1(1)]g = on,off,off");
  //pythia.readString("Bottomonium:gg2bbbar(3S1)[3S1(8)]g = on,off,off");
  //pythia.readString("Bottomonium:qg2bbbar(3S1)[3S1(8)]q = on,off,off");
  //pythia.readString("Bottomonium:qqbar2bbbar(3S1)[3S1(8)]g = on,off,off");
  //pythia.readString("553:onMode = off");
  //pythia.readString("553:onIfMatch = 13 -13");
  pythia.init();
  double Events, i, nchar, ET, ETmult;
  double Ups = 0, BBC1, BBC2, BBC;
  double pT, rapidity, phi, eta, energyT, deta1, deta2, d1 = 0, d2 = 0;
  double mu1pT, mu1rap, mu2pT, mu2rap; //daughter properties
  int Bot = -1; //Bottom index for single particles, used to check for multiple Ups
  Vec4 particle; //4-Vector for each particle
  cout << "Number of Events -> ";
  scanf("%lf", &Events);
// Create NTuple
  TNtuple PPTuple("PPTuple", "PPTuple", "NChar:ET:BBC:ETmult:pT:rapidity:phi:eta:energyT:deta1:mu1pT:mu1rap:deta2:mu2pT:mu2rap");
// Create MIP distribution (mean = .3, std = .06)
  TF1* MIPs = new TF1("MIPs", "exp((x - .3)**2/(-.12*.06))", 0, .6);
  for (i = 0; i < Events; i++) {
        if (!pythia.next()) continue;
        ET = 0;
	ETmult = 0;
        nchar = 0;
	Ups = 0;
	BBC1 = 0;
	BBC2 = 0;
	BBC = 0;
	Bot = -1; // If initialize as 0, will trigger on first loop
	d1 = -1;
	d2 = -1;
	energyT = 0;
        for (int k = 0; k < pythia.event.size(); k++) {
                particle = pythia.event[k].p();
		if (pythia.event[k].isFinal() && pythia.event[k].isCharged()) {
                        if(particle.pT() >= .10 && abs(particle.eta()) < 1.0) {
                                nchar++;}}
                if(pythia.event[k].isFinal() && particle.eta() > 2.5 && particle.eta() < 4) {
			if((abs(pythia.event[k].id()) == 11 || pythia.event[k].id() == 22)) {
				ET = ET + particle.eT(); // ET for photons and electrons
				ETmult++;  }
		 	if(pythia.event[k].isHadron()) {
				ET = ET + MIPs->GetRandom(); // ET for Hadrons, uses MIP distribution
				ETmult++;  } }
		if(pythia.event[k].id() == 553) { //Records index for final Upsilon in event record
				Bot = pythia.event[k].iBotCopy(); }
		if(pythia.event[k].isFinal() && particle.eta() < 5 && particle.eta() > 3.3) {
			BBC1 = 1; }
		if(pythia.event[k].isFinal() && particle.eta() > -5 && particle.eta() < -3.3) {
			BBC2 = 1; }
		if(BBC1 == 1 && BBC2 == 1) {
			BBC = 1;  } //Check for BBC coincidence
		if(k == Bot) {
			//Stats for final Upsilon in Event Record
			d1 = pythia.event[k].daughter1();
			d2 = pythia.event[k].daughter2();
			pT = particle.pT();
                        rapidity = particle.rap();
                        eta = particle.eta();
                        phi = particle.phi();
                        energyT = particle.eT(); }
		if(k == d1) {
			//Stats for first daughter
			deta1 = particle.eta();
			mu1pT = particle.pT();
			mu1rap = particle.rap(); }
		if(k == d2) {
			//Stats for second daughter
			deta2 = particle.eta();
			mu2pT = particle.pT();
			mu2rap = particle.rap(); } }
	PPTuple.Fill(nchar,ET,BBC,ETmult,pT,rapidity,phi,eta,energyT,deta1,mu1pT,mu1rap,deta2,mu2pT,mu2rap); }
  pythia.stat();
  TFile out("STARwithTune.root", "RECREATE");
  PPTuple.Write();
  out.Close();
  return 0; }
