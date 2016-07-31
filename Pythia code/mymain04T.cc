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
 // UserHooks *oniumUserHook = new SuppressSmallPT();
 // pythia.setUserHooksPtr(oniumUserHook);
//  pythia.readFile("Heavy-Flavor-Ups.cmnd");
  pythia.readString("Beams:eCM = 5020.");
  pythia.readString("Bottomonium:gg2bbbar(3S1)[3S1(1)]g = on,off,off");
  pythia.readString("Bottomonium:gg2bbbar(3S1)[3S1(8)]g = on,off,off");
  pythia.readString("Bottomonium:qg2bbbar(3S1)[3S1(8)]q = on,off,off");
  pythia.readString("Bottomonium:qqbar2bbbar(3S1)[3S1(8)]g = on,off,off");
  pythia.readString("553:onMode = off");
  pythia.readString("553:onIfMatch = 13 -13");
  pythia.init();
  double Events, i, nchar, ET, ETmid;
  double Ups = 0;
  double d1, d2;
  double mu1pT, mu1rap, mu2pT, mu2rap; //daughter properties
  int Bot = 0; //Bottom index for single particles, used to check for multiple Ups
  Vec4 particle; //4-Vector for each particle
  double Event, ID, pT, pTot, eta, phi, charge, mass, rapidity, daughter;
  TTree *Tree = new TTree("Tree", "pp Tree");
  Tree->Branch("Event", &Event);
  Tree->Branch("ID", &ID);
  Tree->Branch("pT", &pT);
  Tree->Branch("pTot", &pTot);
  Tree->Branch("eta", &eta);
  Tree->Branch("phi", &phi);
  Tree->Branch("charge", &charge);
  Tree->Branch("mass", &mass);
  Tree->Branch("rapidity", &rapidity);
  Tree->Branch("daughter", &daughter);
  cout << "Number of Events -> ";
  scanf("%lf", &Events);
  //TNtuple PPTuple("PPTuple", "PPTuple", "NChar:ET:ETmid:pT:rapidity:phi:eta:energyT:deta1:mu1pT:mu1rap:deta2:mu2pT:mu2rap"); // Need tree Instead
  for (i = 0; i < Events; i++) {
        if (!pythia.next()) continue;
        ET = 0;
	ETmid = 0;
        nchar = 0;
	Ups = 0;
	Bot = -1;
	d1 = -1;
	d2 = -1;
	Event = i;
        for (int k = 0; k < pythia.event.size(); k++) {
                particle = pythia.event[k].p();
		daughter = 0;
		if (pythia.event[k].isFinal()) {
                        if(particle.pT() >= 0 && abs(particle.eta()) < 2.4 && k != d1 && k != d2) {
				ID = pythia.event[k].id();
				pT = pythia.event[k].pT();
				pTot = pythia.event[k].pAbs();			
				eta = pythia.event[k].eta();
				phi = pythia.event[k].phi();
				charge = pythia.event[k].charge();
				mass = pythia.event[k].m();
				rapidity =  pythia.event[k].y();
				Tree->Fill();
                                nchar++;}} //Need to add particle (Vec4) to Tree
               // if(pythia.event[k].isFinal() && abs(particle.eta()) < 2.4 && particle.pT() >= .10) {
		//		ETmid = ETmid + particle.eT(); }
		// if(pythia.event[k].isFinal() && abs(particle.eta()) > 4 && abs(particle.eta()) < 5.2){
                  //      ET = ET + particle.eT(); }
		if(pythia.event[k].id() == 553 && Ups == 0) {
				Ups = 1;
				Bot = pythia.event[k].iBotCopy(); }
		if(pythia.event[k].id() == 553 && Ups == 1) {
			if(pythia.event[k].iBotCopy() != Bot) {
				//Check for multiple Upsilons
				Ups += 1; } }
		if(k == Bot) {
			d1 = pythia.event[k].daughter1();
                        d2 = pythia.event[k].daughter2();
			//Stats for final Upsilon
                        ID = pythia.event[k].id();
                        pT = pythia.event[k].pT();
                        pTot = pythia.event[k].pAbs();
                        eta = pythia.event[k].eta();
                        phi = pythia.event[k].phi();
                        charge = pythia.event[k].charge();
                        mass = pythia.event[k].m();
                        rapidity =  pythia.event[k].y();
                        Tree->Fill(); }
		if(k == d1) {
			//Stats for first daughter(muon)
			ID = pythia.event[k].id();
                	pT = pythia.event[k].pT();
                        pTot = pythia.event[k].pAbs();
                        eta = pythia.event[k].eta();
                        phi = pythia.event[k].phi();
                        charge = pythia.event[k].charge();
                        mass = pythia.event[k].m();
                        rapidity =  pythia.event[k].y();
                        daughter = 1;
			Tree->Fill(); } 
		if(k == d2) {
			//Stats for second daughter(muon)
			ID = pythia.event[k].id();
                        pT = pythia.event[k].pT();
                        pTot = pythia.event[k].pAbs();
                        eta = pythia.event[k].eta();
                        phi = pythia.event[k].phi();
                        charge = pythia.event[k].charge();
                        mass = pythia.event[k].m();
                        rapidity =  pythia.event[k].y();
			daughter = 1;
                        Tree->Fill();  } } }
//	PPTuple.Fill(nchar,ET,ETmid,pT,rapidity,phi,eta,energyT,deta1,mu1pT,mu1rap,deta2,mu2pT,mu2rap); }
 
 pythia.stat();
  TFile out("TreeTest.root", "RECREATE");
  Tree->Write();
  out.Close();
  return 0; }
