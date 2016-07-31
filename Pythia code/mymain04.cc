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
  UserHooks *oniumUserHook = new SuppressSmallPT();
  pythia.setUserHooksPtr(oniumUserHook);
  pythia.readFile("Heavy-Flavor-Ups.cmnd");
 // pythia.readString("Beams:eCM = 2760.");
 // pythia.readString("Bottomonium:gg2bbbar(3S1)[3S1(1)]g = on,off,off");
//  pythia.readString("Bottomonium:gg2bbbar(3S1)[3S1(8)]g = on,off,off");
//  pythia.readString("Bottomonium:qg2bbbar(3S1)[3S1(8)]q = on,off,off");
//  pythia.readString("Bottomonium:qqbar2bbbar(3S1)[3S1(8)]g = on,off,off");
//  pythia.readString("553:onMode = off"); //Turns off all Upsilond decay channels
//  pythia.readString("553:onIfMatch = 13 -13"); //Turns on only Ups->mu+mu- (Not Ups->mumu+stuff)
  pythia.init();
  double Events, i, nchar, ET, ETmid;
  double Ups = 0;
  double pT, rapidity, phi, eta, energyT, deta1, deta2, d1 = 0, d2 = 0;
  double mu1pT, mu1rap, mu2pT, mu2rap; //daughter properties
  int Bot = 0; //Bottom index for single particles, used to check for multiple Ups
  Vec4 particle; //4-Vector for each particle
  cout << "Number of Events -> ";
  scanf("%lf", &Events);
  TNtuple PPTuple("PPTuple", "PPTuple", "NChar:ET:ETmid:pT:rapidity:phi:eta:energyT:deta1:mu1pT:mu1rap:deta2:mu2pT:mu2rap");
  for (i = 0; i < Events; i++) {
        if (!pythia.next()) continue;
        ET = 0;
	ETmid = 0;
        nchar = 0;
	Ups = 0;
	Bot = -1;
	d1 = -1;
	d2 = -1;
        for (int k = 0; k < pythia.event.size(); k++) {
                particle = pythia.event[k].p();
		if (pythia.event[k].isFinal() && pythia.event[k].isCharged()) {
                        if(particle.pT() >= .10 && abs(particle.eta()) < 2.4) {
                                nchar++;}}
                if(pythia.event[k].isFinal() && abs(particle.eta()) < 2.4 && particle.pT() >= .10) {
				ETmid = ETmid + particle.eT(); }
		 if(pythia.event[k].isFinal() && abs(particle.eta()) > 4 && abs(particle.eta()) < 5.2){
                        ET = ET + particle.eT(); }
		if(pythia.event[k].id() == 553 && Ups == 0) {
				Ups = 1;
				Bot = pythia.event[k].iBotCopy(); }
		if(pythia.event[k].id() == 553 && Ups == 1) {
			if(pythia.event[k].iBotCopy() != Bot) {
				//Check for multiple Upsilons
				Ups += 1; } }
		if(k == Bot) {
			//Stats for final Upsilon
			d1 = pythia.event[k].daughter1();
			d2 = pythia.event[k].daughter2();
			pT = particle.pT();
                        rapidity = particle.rap();
                        eta = particle.eta();
                        phi = particle.phi();
                        energyT = particle.eT(); }
		if(k == d1) {
			//Stats for first daughter(muon)
			deta1 = particle.eta();
			mu1pT = particle.pT();
			mu1rap = particle.rap(); }
		if(k == d2) {
			//Stats for second daughter(muon)
			deta2 = particle.eta();
			mu2pT = particle.pT();
			mu2rap = particle.rap(); } }
	PPTuple.Fill(nchar,ET,ETmid,pT,rapidity,phi,eta,energyT,deta1,mu1pT,mu1rap,deta2,mu2pT,mu2rap); }
  pythia.stat();
  TFile out("PPCollisionUps1sTuneTest.root", "RECREATE");
  PPTuple.Write();
  out.Close();
  return 0; }
