#include <TH1D.h>
#include <TCanvas.h>
#include "TTree.h"
#include <TFile.h>
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include <iostream>
#include <TH2D.h>
#include <stdio.h>
#include <TObject.h>
#include <TRandom3.h>

//Event-Mixing to approximate background signal
//Calculates dPhi and dEta between Upsilons and hadrons from different events
//The events are mixed with other events within the mass region being examined
//This is done for all 3 mass regions

void MixingTest() {
  TFile *file = TFile::Open("/scratch_rigel/CMSTrees/PbPb_Data/MegaTree/OniaTree_MEGA_Peripheral30100_PromptReco_262548_263757.root");
//Track Tree Set Up
  TTree *trackTree = (TTree*)file->Get("anaTrack/trackTree");
  trackTree->SetBranchStatus("*",0);
  trackTree->SetBranchStatus("nLumi", 1);
  trackTree->SetBranchStatus("nTrk", 1);
  trackTree->SetBranchStatus("trkPt", 1);
  trackTree->SetBranchStatus("trkEta", 1);
  trackTree->SetBranchStatus("trkPhi", 1);
  trackTree->SetBranchStatus("trkCharge", 1);
  trackTree->SetBranchStatus("nEv", 1);
  trackTree->SetBranchStatus("highPurity",1);
  Bool_t highPurity[9804];
  trackTree->SetBranchAddress("highPurity",highPurity);
  Int_t Lumi, nTrk, event;
  Float_t eta[9804];
  Float_t phi[9804];
  Int_t charge[9804];
  Float_t pT[9804];
  trackTree->SetBranchAddress("nLumi", &Lumi);
  trackTree->SetBranchAddress("nTrk", &nTrk);
  trackTree->SetBranchAddress("trkPt", pT);
  trackTree->SetBranchAddress("trkEta", eta);
  trackTree->SetBranchAddress("trkPhi", phi);
  trackTree->SetBranchAddress("trkCharge", charge);
  trackTree->SetBranchAddress("nEv", &event);
//Dimuon Tree Set Up
  TTree *myTree = (TTree*)file->Get("hionia/myTree");
  myTree->SetBranchStatus("*",0);
  myTree->SetBranchStatus("Reco_QQ_4mom",1);
  myTree->SetBranchStatus("Reco_QQ_mupl_4mom",1);
  myTree->SetBranchStatus("Reco_QQ_mumi_4mom",1);
  myTree->SetBranchStatus("Reco_QQ_size",1);
  myTree->SetBranchStatus("Centrality",1);
  myTree->SetBranchStatus("HLTriggers",1);
  myTree->SetBranchStatus("Reco_QQ_trig",1);
  myTree->SetBranchStatus("Reco_QQ_sign",1);
  TClonesArray *Reco_QQ_4mom=0;
  TClonesArray *Reco_QQ_mupl_4mom=0;
  TClonesArray *Reco_QQ_mumi_4mom=0;
  TLorentzVector *dimuon;
  TLorentzVector *mumi;
  TLorentzVector *mupl;
  Int_t Reco_QQ_sign[21];
  double events=0;
  events = myTree->GetEntries();
  cout << events << endl;
  Int_t QQsize=0;
  Int_t Centrality=0;
  ULong64_t HLTrigger=0;
  ULong64_t Reco_QQ_trig[36];
  myTree->SetBranchAddress("Centrality",&Centrality);
  myTree->SetBranchAddress("HLTriggers",&HLTrigger);
  myTree->SetBranchAddress("Reco_QQ_4mom",&Reco_QQ_4mom);
  myTree->SetBranchAddress("Reco_QQ_mupl_4mom", &Reco_QQ_mupl_4mom);
  myTree->SetBranchAddress("Reco_QQ_mumi_4mom", &Reco_QQ_mumi_4mom);
  myTree->SetBranchAddress("Reco_QQ_size", &QQsize);
  myTree->SetBranchAddress("Reco_QQ_trig", Reco_QQ_trig);
  myTree->SetBranchAddress("Reco_QQ_sign", Reco_QQ_sign);
//Histogram Initialization
  TH1D* phidifflow = new TH1D( "phidifflow",  "#Delta#phi for low mass band (7.5-8.5 GeV)", 128, 0, 3.2);
  TH1D* rapdifflow = new TH1D( "rapdifflow",  "#Delta#eta for low mass band (7.5-8.5 GeV)", 200, -5, 5);
  TH1D* phidiffhigh = new TH1D("phidiffhigh", "#Delta#phi for high mass band (12-13 GeV)",  128, 0, 3.2);
  TH1D* rapdiffhigh = new TH1D("rapdiffhigh", "#Delta#eta for high mass band (12-13 GeV)",  200, -5, 5);
  TH1D* phidiffmid = new TH1D( "phidiffmid",  "#Delta#phi for mid mass band (9.0-9.8 GeV)", 128, 0, 3.2);
  TH1D* rapdiffmid = new TH1D( "rapdiffmid",  "#Delta#eta for mid mass band (9.0-9.8 GeV)", 200, -5, 5);
  TH2D* lowmass = new TH2D("lowmass","#Delta#phi vs #Delta#eta for low mass band",128,0,3.2,200,-5,5);
  TH2D* midmass = new TH2D("midmass","#Delta#phi vs #Delta#eta for mid mass band",128,0,3.2,200,-5,5);
  TH2D* highmass = new TH2D("highmass","#Delta#phi vs #Delta#eta for high mass band",128,0,3.2,200,-5,5);
//Full mass region
  TH1D* phidiffall = new TH1D( "phidiffall", "#Delta#phi for entire mass band (7-14 GeV)",128,0,3.2);
  TH1D* rapdiffall = new TH1D( "rapdiffall", "#Deltay for entire mass band (7-14 GeV)",200,-5, 5);
  TH2D* allmass = new TH2D("allmass","#Delta#phi vs #Deltay for entire mass band",128,0,3.2,200,-5,5);
//Event Loop - Determine which events have dimuon pairs that pass all the cuts
  int test = 0, temp=0, index=0;
  int low=0,high=0,mid=0;
  int UpsEvents[40000];
  int dimuons[100000];
  for(int i = 0; i < myTree->GetEntries(); i++) { 
	myTree->GetEntry(i);
	if( (HLTrigger&128)==128 || (HLTrigger&256)==256) {
		for(int j=0; j < QQsize; j++) {
                        dimuon = (TLorentzVector*) Reco_QQ_4mom->At(j);
                        mumi = (TLorentzVector*) Reco_QQ_mumi_4mom->At(j);
                        mupl = (TLorentzVector*) Reco_QQ_mupl_4mom->At(j);
			if( ( (Reco_QQ_trig[j]&128)==128 || (Reco_QQ_trig[j]&256)==256) && Reco_QQ_sign[j] == 0) {
			if(mumi->Pt() > 4 && mupl->Pt() > 4 && TMath::Abs(mumi->Eta()) < 2.4 && TMath::Abs(mupl->Eta()) < 2.4)  {
				if(dimuon->M() > 7 && dimuon->M() < 14 && dimuon->Pt() < 30) {
					if(dimuon->M() > 7.5 && dimuon->M() < 8.5) {
						if(temp != i) {
                                        	UpsEvents[test] = i;
                                        	test++;
						dimuons[index] = i;
						index++;
                                        	temp = i;}
						low++;  } 
					else if(dimuon->M() > 9.31 && dimuon->M() < 9.61) {
						if(temp != i) {
                                        	UpsEvents[test] = i;
                                        	test++;
						dimuons[index] = i;
						index++;
                                       		temp = i;}
						mid++;  }
					else if(dimuon->M() > 12 && dimuon->M() < 13) {
						if(temp != i) {
						dimuons[index] = i;
						index++;
						UpsEvents[test] = i;
						test++;
						temp = i;}
						high++;  }
					else{   if(temp != i) {
                                        	dimuons[index] = i;
                                        	index++;}  }
				}
                                }
			} } 
		}
	}

  cout << "Index Scan complete. test = " << test << endl;
  cout << "Low: " << low << endl << "Mid: " << mid << endl << "High: " << high << endl;
//Event Loop 2 - Mixing
  TRandom3 ran;
  ran.SetSeed(0);
  double rando = ran.Rndm();
  int test2=0;
  low = 0;
  mid = 0;
  high = 0;
  for(int i = 0; i < test; i++) {
  for(int n = 0; n < 3; n++) {	
        rando = index*ran.Rndm();
	while(dimuons[(int)TMath::Nint(rando)] == UpsEvents[i]) { 
		rando = index*ran.Rndm(); } 
        trackTree->GetEntry( dimuons[(int)TMath::Nint(rando)] );
        myTree->GetEntry(UpsEvents[i]);
        if( (HLTrigger&128) == 128 || (HLTrigger&256) == 256) {
                for(int j=0; j < QQsize; j++) {
                        dimuon = (TLorentzVector*) Reco_QQ_4mom->At(j);
                        mumi = (TLorentzVector*) Reco_QQ_mumi_4mom->At(j);
                        mupl = (TLorentzVector*) Reco_QQ_mupl_4mom->At(j);
                        if(((Reco_QQ_trig[j]&128) == 128 || (Reco_QQ_trig[j]&256) == 256) && Reco_QQ_sign[j] == 0) {
                        if(mumi->Pt() > 4 && mupl->Pt() > 4 && TMath::Abs(mumi->Eta()) < 2.4 && TMath::Abs(mupl->Eta()) < 2.4)  {
			if(dimuon->Pt() < 30) {
                                if(dimuon->M() > 7.5 && dimuon->M() < 8.5) {
                                        test2++;
					low++;
                                        for(int k=0; k < nTrk; k++) {
                                        if(TMath::Abs(eta[k]) < 2.4 && pT[k] > .1  && TMath::Abs(charge[k]) == 1 && highPurity[k]==0) {
                                                if(TMath::Abs(dimuon->Phi() - phi[k]) > 3.1416) {
                                                        phidifflow->Fill(6.2832-TMath::Abs(phi[k]-dimuon->Phi()));
                                                        rapdifflow->Fill(eta[k] - dimuon->Eta());
							lowmass->Fill(6.2832-TMath::Abs(phi[k]-dimuon->Phi()),eta[k]-dimuon->Eta());
                                                }
                                                if(TMath::Abs(dimuon->Phi() - phi[k]) <= 3.1416) {
                                                        phidifflow->Fill(TMath::Abs(phi[k]-dimuon->Phi()));
                                                        rapdifflow->Fill(eta[k] - dimuon->Eta());
							lowmass->Fill(TMath::Abs(phi[k]-dimuon->Phi()),eta[k]-dimuon->Eta());
                                                } }
                                         }
                                }
				if(dimuon->M() > 9.31 && dimuon->M() < 9.61) {
                                        test2++;
					mid++;
                                        for(int k=0; k < nTrk; k++) {
                                        if(TMath::Abs(eta[k]) < 2.4 && pT[k] > .1 && TMath::Abs(charge[k]) == 1 && highPurity[k]==0) {
                                                if(TMath::Abs(dimuon->Phi() - phi[k]) > 3.1416) {
                                                        phidiffmid->Fill(6.2832-TMath::Abs(phi[k]-dimuon->Phi()));
                                                        rapdiffmid->Fill(eta[k] - dimuon->Eta());
							midmass->Fill(6.2832-TMath::Abs(phi[k]-dimuon->Phi()),eta[k]-dimuon->Eta());
                                               		}
                                                if(TMath::Abs(dimuon->Phi() - phi[k]) <= 3.1416) {
                                                        phidiffmid->Fill(TMath::Abs(phi[k] - dimuon->Phi()));
                                                        rapdiffmid->Fill(eta[k] - dimuon->Eta());
							midmass->Fill(TMath::Abs(phi[k]-dimuon->Phi()),eta[k]-dimuon->Eta());
                                                	}
						 }}
                               		 }	
                                if(dimuon->M() > 12 && dimuon->M() < 13) {
                                        test2++;
					high++;
                                        for(int k=0; k < nTrk; k++) {
                                        if(TMath::Abs(eta[k]) < 2.4 && pT[k] > .1 && TMath::Abs(charge[k]) == 1 && highPurity[k]==0) {
                                                if(TMath::Abs(dimuon->Phi() - phi[k]) > 3.1416) {
                                                        phidiffhigh->Fill(6.2832-TMath::Abs(phi[k]-dimuon->Phi()));
                                                        rapdiffhigh->Fill(eta[k] - dimuon->Eta());
							highmass->Fill(6.2832-TMath::Abs(phi[k]-dimuon->Phi()),eta[k]-dimuon->Eta());
                                                        }
                                                if(TMath::Abs(dimuon->Phi() - phi[k]) <= 3.1416) {
                                                        phidiffhigh->Fill(TMath::Abs(phi[k] - dimuon->Phi()));
                                                        rapdiffhigh->Fill(eta[k] - dimuon->Eta());
							highmass->Fill(TMath::Abs(phi[k]-dimuon->Phi()),eta[k]-dimuon->Eta());
                                                        }
                                                } }
                                         }
                                } }
                        } }
                }
		}
	} 

  TFile out("MixingTestPure0.root", "RECREATE");
  phidifflow->Write();
  rapdifflow->Write();
  lowmass->Write();
  phidiffhigh->Write();
  rapdiffhigh->Write();
  highmass->Write();
  phidiffmid->Write();
  rapdiffmid->Write();
  midmass->Write();
  out.Close();

  cout << "Done!  " << test2 << endl;
  cout << "Low: " << low << endl << "Mid: " << mid << endl << "High: " << high << endl;
  return; }
