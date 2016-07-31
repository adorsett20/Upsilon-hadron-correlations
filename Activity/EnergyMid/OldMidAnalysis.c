
void OldMidAnalysis() {
  TFile *file = TFile::Open("PPCollisionUps1sMilWithTune.root");
  TFile *file2 = TFile::Open("PPCollisionMilWithTune.root");
  TNtuple* PPTupleUps = (TNtuple*)file->Get("PPTuple");
  TNtuple* PPTupleND = (TNtuple*)file2->Get("PPTuple");
  TH1D* UpsE = new TH1D("UpsE", "Energy at forward rapidity for events with Trackable Upsilon", 300 ,0, 300);
  PPTupleUps->Draw("(ETmid-mu1pT-mu2pT)>>UpsE", "abs(mu1rap)<1.93 && abs(mu2rap)<1.93 && mu1pT > 4 && mu2pT > 4");
  TH1D* UpsEscale = new TH1D("UpsEscale", "Energy for events with Upsilon scaled",30000,0,300);
  PPTupleUps->Draw("(ETmid-mu1pT-mu2pT)>>UpsEscale", "abs(mu1rap)<1.93 && abs(mu2rap)<1.93 && mu1pT > 4 && mu2pT > 4");
  UpsEscale->Scale(3.34e-6);
  TH1D* NDE = new TH1D("NDE", "Energy for Non-diffractive Events", 300, 0, 300);
  PPTupleND->Draw("ETmid>>NDE", "NChar > 0");
  UpsE->Draw();
  NDE->Draw();
  //Activity Range Calculations For Energy
  TH1D* cutHFE = new TH1D("cutHFE", "Transverse Energy in HFC with cut", 10000, 0, 300);
  PPTupleND->Draw("ETmid>>cutHFE", "NChar > 0");
  cutHFE->Scale(1./cutHFE->GetEntries());
  double sum, percent = 0, centAr[11];
  int c, centa[11], i;
  percent += .1;
  sum = 0;
  centAr[0] = 300;
  centAr[10] = 0;
  c = 1;
  cout << "\n" << cutHFE->GetMean() << "\n";
	//Find Activity Ranges
  for (i=0; percent <= .99; percent += .10) {
        sum = 0;
        for(i=0; sum < percent || percent == 1; i++) {
                sum += cutHFE->GetBinContent(10000-i); }
        	cout << 100*percent - 10 << "-" << 100*percent << "%" << "      " << sum  << "      " << i << " " << cutHFE->GetBinCenter(10000-i) << "\n";
        centAr[c] = cutHFE->GetBinCenter(10000-i);
        c++; }
  //cout << "\n" << "Activity Range Values from Array" << "\n";
  for(c = 0; c < 11; c++) {
        //cout << centAr[c] << "\n"; //Use to check Activity Ranges
	centa[c] = 100000*centAr[c]; }
  for(c = 0; c < 11; c++) {
	centa[c] = (centa[c]-centa[c]%1000)/1000; 
	cout << centa[c] << "\n"; }
  cout << "\n";
  TH1D* UpsEcount = new TH1D("UpsEcount", "Upsilon energy at forward rapidity", 10000, 0, 300);
  PPTupleUps->Draw("(ETmid-mu1pT-mu2pT)>>UpsEcount", "abs(mu1rap)<1.93 && abs(mu2rap)<1.93 && mu1pT > 4 && mu2pT > 4");
  double min = 0, average[10], bins;
  i = 0;
  UpsEcount->Draw();
 // cout << "\n" << "Activity Bin Averages" << "\n";
	//Get Activity Bin Averages
  for(c = 0; UpsEcount->GetBinCenter(10000-i) > centAr[c+1] && UpsEcount->GetBinCenter(10000-i) <= centAr[c]; c++) {
        sum = 0;
        bins = 0;
        for(i = min; UpsEcount->GetBinCenter(10000-i) > centAr[c+1] && UpsEcount->GetBinCenter(10000-i) <= centAr[c]; i++) {
                sum += UpsEcount->GetBinContent(10000 - i)*UpsEcount->GetBinCenter(10000 - i);
                bins += UpsEcount->GetBinContent(10000 - i); }
        average[c] = sum/bins;
	cout << bins << "\n";
        min = i; }
        //cout << 10*c << "-" << 10*(c+1) << "%" << "     " << average[c] << "\n";  } Use to check bin average
  cout << "\n" << "Corrected X/Y Energy values" << "\n";
  double EnergyYpoints[10];
  cout << centa[0] << "\n";
  cout << centa[1] << "\n";
	//Get X/Y Points for Graph
  cout << (10./UpsEscale->Integral(0,30000))*UpsEscale->Integral(9043,30000) << "\n";
  cout << (10./UpsEscale->Integral(0,30000))*UpsEscale->Integral(6289,9043)  << "\n";
  for(c = 0; c < 10; c++) {
	average[c] = average[c]/cutHFE->GetMean();
        cout << "(" << average[c] << "," << (10./UpsEscale->Integral(0,30000))*UpsEscale->Integral(centa[c+1],centa[c]) << ")" <<  "\n";
	EnergyYpoints[c] = (10./UpsEscale->Integral(0,30000))*UpsEscale->Integral(centa[c+1],centa[c]-1); }
  TF1* line = new TF1("line", "x", 0, 4);
  TF1* line2 = new TF1("line2", "1*x", 0, 4);
  line2->SetLineColor(kWhite);
  TCanvas * can = new TCanvas("can", "Energy");
  can->cd();
  sum = 0;
  double scaledSumE = 0;
  scaledSumE = UpsEscale->Integral(0,30000);
  NDE->GetXaxis()->SetTitle("Sum E_{T} (GeV)");
  NDE->SetLineColor(kRed);
  UpsE->SetLineColor(kBlue);
  UpsE->Draw();
  UpsEscale->SetFillColor(kGreen);
  TCanvas* can2 = new TCanvas("can2", "Centrality");
  can2->cd();
  //Set Up Canvas Window
  line->SetLineColor(1);
  line2->GetYaxis()->SetTitle("#varUpsilon(1S)/<#varUpsilon(1S)>");
  line2->GetXaxis()->SetTitle("E_{T}^{|#eta|<2.4}/<E_{T}^{|#eta|<2.4}>");
  line2->SetTitle("#varUpsilon(1S) Cross Section vs Transverse Energy");
  line2->Draw();
  line->SetLineStyle(3);
  line->Draw("same");
  TH1D* modelE = new TH1D("modelE", "Pythia model", 4000, 0, 4);
  modelE->SetMarkerStyle(20);
  modelE->SetMarkerColor(kRed);
 //Fill Histogram with points from Model
  for(i = 0; i < 11; i++) {
	//Energy Values
 	modelE->SetBinContent(1000*average[i], EnergyYpoints[i]);
	modelE->SetBinError(1000*average[i], .01); }
  modelE->SetLineColor(kRed);
  modelE->Draw("sameE");
//  modelE->
  //Histogram for CMS Data Points
  TLegend* leg = new TLegend(.1,.74,.30,.9);
  leg->SetHeader("For |#eta| < 2.4");
  leg->AddEntry(modelE->DrawCopy("sameE"), "Pythia Model", "l");
  leg->Draw("same");
  return; }
