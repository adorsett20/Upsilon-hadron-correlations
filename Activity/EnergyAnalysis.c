
void EnergyAnalysis() {
  TFile *file = TFile::Open("PPCollisionUps1sWithTune.root");
  TFile *file2 = TFile::Open("PPCollisionWithTune.root");
  TNtuple* PPTupleUps = (TNtuple*)file->Get("PPTuple");
  TNtuple* PPTupleND = (TNtuple*)file2->Get("PPTuple");
  TH1D* UpsE = new TH1D("UpsE", "Energy at forward rapidity for events with Trackable Upsilon", 40 ,0, 40);
  PPTupleUps->Draw("ET>>UpsE", "abs(mu1rap)<1.93 && abs(mu2rap)<1.93 && mu1pT > 4 && mu2pT > 4");
  TH1D* UpsEscale = new TH1D("UpsEscale", "Energy for events with Upsilon scaled",4000,0,40);
  PPTupleUps->Draw("ET>>UpsEscale", " abs(mu1rap)<1.93 && abs(mu2rap)<1.93 && mu1pT > 4 && mu2pT > 4");
  UpsEscale->Scale(3.34e-6);
  TH1D* NDE = new TH1D("NDE", "Energy for Non-diffractive Events", 40, 0, 40);
  PPTupleND->Draw("ET>>NDE", "NChar > 0");
  UpsE->Draw();
  NDE->Draw();
  //Activity Range Calculations For Energy
  TH1D* cutHFE = new TH1D("cutHFE", "Transverse Energy in HFC with cut", 10000, 0, 40);
  PPTupleND->Draw("ET>>cutHFE", "NChar > 0");
  cutHFE->Scale(1./cutHFE->GetEntries());
  double sum, percent = 0, centAr[11];
  int c, centa[11], i;
  percent += .1;
  sum = 0;
  centAr[0] = 40;
  centAr[10] = 0;
  c = 1;
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
  TH1D* UpsEcount = new TH1D("UpsEcount", "Upsilon energy at forward rapidity", 10000, 0, 40);
  PPTupleUps->Draw("ET>>UpsEcount", "abs(mu1rap)<1.93 && abs(mu2rap)<1.93 && mu1pT > 4 && mu2pT > 4");
  double min = 0, average[10], bins;
  i = 0;
 // cout << "\n" << "Activity Bin Averages" << "\n";
	//Get Activity Bin Averages
  for(c = 0; UpsEcount->GetBinCenter(10000-i) > centAr[c+1] && UpsEcount->GetBinCenter(10000-i) <= centAr[c]; c++) {
        sum = 0;
        bins = 0;
        for(i = min; UpsEcount->GetBinCenter(10000-i) > centAr[c+1] && UpsEcount->GetBinCenter(10000-i) <= centAr[c]; i++) {
                sum += UpsEcount->GetBinContent(10000 - i)*UpsEcount->GetBinCenter(10000 - i);
                bins += UpsEcount->GetBinContent(10000 - i); }
        average[c] = sum/bins;
        min = i; }
  cout << average[0] << "\n";
        //cout << 10*c << "-" << 10*(c+1) << "%" << "     " << average[c] << "\n";  } Use to check bin average
  cout << "\n" << "Corrected X/Y Energy values" << "\n";
  double EnergyYpoints[10];
	//Get X/Y Points for Graph
  for(c = 0; c < 10; c++) {
	average[c] = average[c]/cutHFE->GetMean();
        cout << "(" << average[c] << "," << (10./UpsEscale->Integral(0,4000))*UpsEscale->Integral(centa[c+1],centa[c]) << ")" <<  "\n";
	EnergyYpoints[c] = (10./UpsEscale->Integral(0,4000))*UpsEscale->Integral(centa[c+1],centa[c]); }
  cout << average[0] << "   -    " << EnergyYpoints[0] << "\n";
  TF1* line = new TF1("line", "x", 0, 4);
  TF1* line2 = new TF1("line2", "x", 0, 4);
  line2->SetLineColor(kWhite);
  TCanvas * can = new TCanvas("can", "Energy");
  can->cd();
  sum = 0;
  double scaledSumE = 0;
  scaledSumE = UpsEscale->Integral(0,4000);
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
  line2->GetXaxis()->SetTitle("E_{T}^{4<|#eta|<5.2}/<E_{T}^{4<|#eta|<5.2}>");
  line2->SetTitle("#varUpsilon(1S) Cross Section vs Transverse Energy");
  line2->Draw();
  line->SetLineStyle(3);
  line->Draw("same");
  TH1D* modelE = new TH1D("modelE", "Pythia model", 4000, 0, 4);
  modelE->SetMarkerStyle(20);
  modelE->SetMarkerColor(kRed);
  cout << 1000*average[0] << "   -    " << EnergyYpoints[0] << "\n";
 //Fill Histogram with points from Model
  for(i = 0; i < 11; i++) {
	//Energy Values
 	modelE->SetBinContent(1000*average[i], EnergyYpoints[i]);
	modelE->SetBinError(1000*average[i], .01); }
  modelE->SetLineColor(kRed);
  modelE->Draw("sameE");
//  modelE->
  //Histogram for CMS Data Points
  TH1D* data = new TH1D("data", "CMS Data", 4000, 0, 4);
  data->SetMarkerStyle(20);
  data->SetMarkerColor(62);
  //Manually Entering CMS Data Points with Error
  data->SetBinContent(2590, 2.74);
  data->SetBinError(2590, .14);
  data->SetBinContent(1460, 1.40);
  data->SetBinError(1460, .05);
  data->SetBinContent(700, .52);
  data->SetBinError(700, .02);
  data->SetMarkerStyle(20);
  data->SetMarkerColor(62);
  data->SetMarkerSize(1);
  data->SetLineColor(62);
  data->Draw("sameE");
  TLegend* leg = new TLegend(.1,.74,.30,.9);
  leg->SetHeader("For 4 < |#eta| < 5.2");
  leg->AddEntry(modelE->DrawCopy("sameE"), "Pythia Model", "l");
  leg->AddEntry(data->DrawCopy("same"), "CMS", "l");
  leg->Draw("same");
  return; }
