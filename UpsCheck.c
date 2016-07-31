#include <TH1D.h>
#include <TCanvas.h>
#include "TTree.h"
#include <TBranch.h>
#include <TFile.h>
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include <iostream>
#include <TH2D.h>
#include <stdio.h>
#include <TObject.h>
#include <TMath.h>

void UpsCheck() {
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
  double events=0;
  events = myTree->GetEntries();
  cout << events << endl;
  Int_t QQsize=0;
  Int_t Centrality=0;
  ULong64_t HLTrigger=0;
  ULong64_t Reco_QQ_trig[21];
  Int_t Reco_QQ_sign[21];
  myTree->SetBranchAddress("Centrality",&Centrality);
  myTree->SetBranchAddress("HLTriggers",&HLTrigger);
  myTree->SetBranchAddress("Reco_QQ_4mom",&Reco_QQ_4mom);
  myTree->SetBranchAddress("Reco_QQ_mupl_4mom", &Reco_QQ_mupl_4mom);
  myTree->SetBranchAddress("Reco_QQ_mumi_4mom", &Reco_QQ_mumi_4mom);
  myTree->SetBranchAddress("Reco_QQ_size", &QQsize);
  myTree->SetBranchAddress("Reco_QQ_trig", Reco_QQ_trig);
  myTree->SetBranchAddress("Reco_QQ_sign", Reco_QQ_sign);
//Histogram Initialization
  TH1D* phidiffmid = new TH1D( "phidiffmid", "#Delta#phi for mid mass band (9.0-9.8 GeV)",128,0,3.2);
  TH1D* rapdiffmid = new TH1D( "rapdiffmid", "#Delta#eta for mid mass band (9.0-9.8 GeV)",200,-5, 5);
  TH2D* midmass = new TH2D("midmass","#Delta#phi vs #Delta#eta for mid mass band",128,0,3.2,200,-5,5);
//Event Loop
  int test = 0;
  int mid = 0;
  int index = 0;
  for(int i = 0; i < trackTree->GetEntries(); i++) { 
	trackTree->GetEntry(i);
	myTree->GetEntry(i);
	if(Centrality > 140) {
	if((HLTrigger&128) == 128 || (HLTrigger&256) == 256) {
		for(Int_t j=0; j < QQsize; j++) {
                        dimuon = (TLorentzVector*)Reco_QQ_4mom->At(j);
                        mumi = (TLorentzVector*)Reco_QQ_mumi_4mom->At(j);
                        mupl = (TLorentzVector*)Reco_QQ_mupl_4mom->At(j);
			if(((Reco_QQ_trig[j]&128) == 128 || (Reco_QQ_trig[j]&256) == 256) && Reco_QQ_sign[j] == 0) {
			if(mumi->Pt() > 4 && mupl->Pt() > 4 && TMath::Abs(mumi->Eta()) < 2.4 && TMath::Abs(mupl->Eta()) < 2.4 )  {
				index++;
				if(dimuon->M() > 9.3 && dimuon->M() < 9.6) {
					test++;
					for(Int_t k=0; k < nTrk; k++) {
						if(TMath::Abs(eta[k]) < 2.4 && pT[k] > .1 && TMath::Abs(charge[k]) == 1) {
							if(TMath::Abs(dimuon->Phi() - phi[k]) > 3.1416) { 
								phidiffmid->Fill(6.2832-TMath::Abs(phi[k]-dimuon->Phi()));
								rapdiffmid->Fill(eta[k] - dimuon->Rapidity());
								midmass->Fill(6.2832-TMath::Abs(phi[k]-dimuon->Phi()),eta[k]-dimuon->Rapidity());
							}
							if(TMath::Abs(dimuon->Phi() - phi[k]) <= 3.1416) {
								phidiffmid->Fill(TMath::Abs(phi[k] - dimuon->Phi()));
                                        	        	rapdiffmid->Fill(eta[k]-dimuon->Rapidity());
								midmass->Fill(TMath::Abs(phi[k]-dimuon->Phi()),eta[k]-dimuon->Rapidity());
							} 
						}
					 }
				 } 
                                }
			} } 
		}
	}  }

  TFile out("EtaPhiMidCent.root", "RECREATE");
  phidiffmid->Write();
  rapdiffmid->Write();
  midmass->Write();
  out.Close();
  cout << "Done!   " << test << "	Total Dimuons: " << index <<endl;
  cout << "Mid:	" << mid << endl;
  return; }
