void TuneAnalysis() {
  //Set up Canvas Area
  TCanvas* can2 = new TCanvas("can2", "Centrality");

  double Xvalues[] = {2.64, 1.79, 1.38, 1.1, .87, .68, .55, .45, .37, .24, 2.54, 1.88, 1.43, 1.08, .81, .61, .48, .38, .29, .20};
  double Yvalues[] = {3.38, 1.98, 1.48, .90, .89, .56, .39, .19, .16, .05, 3.69, 2.12, 1.51, 1.09, .64, .40, .26, .12, .05, .01};
  TGraph* model = new TGraph(20, Xvalues, Yvalues);
  model->SetFillStyle(3022);
  model->SetFillColor(kBlue);
  model->Draw("sameLFa4");

  double xAverage[] = {3.42, 2.62, 1.96, 1.45, 1.12, .87, .67, .49, .39, .28, .23};
  double yAverage[] = {5.22, 3.33, 2.21, 1.40, 1.06, .72, .49, .22, .16, .06, .03};
  double yError[] = {.36, .25, .12, .06, .05, .12, .04, .03, .01, .01, 0};
  double xError[] = {0,0,0,0,0,0,0,0,0,0};
  TGraphErrors* Emodel = new TGraphErrors(10, xAverage, yAverage, xError, yError);
  Emodel->SetFillStyle(3001);
  Emodel->SetFillColor(kBlue);

  double xAverageEnergy[] = {3.24, 1.96, 1.42, 1.045, .78, .58, .43, .34, .265, .185};
  double yAverageEnergy[] = {3.605, 2.14, 1.475, 1.005, .68, .45, .295, .175, .115, .06};
  double yErrorEnergy[] = {.23, .065, .035, .035, .03, .06, .065, .055, .055, .03};
  double xErrorEnergy[] = {0,0,0,0,0,0,0,0,0,0};
  TGraphErrors* EmodelEnergy = new TGraphErrors(10, xAverageEnergy, yAverageEnergy, xErrorEnergy, yErrorEnergy);
  EmodelEnergy->SetFillStyle(3001);
  EmodelEnergy->SetFillColor(kRed);
  can2->cd();
  TH1F* frame = can2->DrawFrame(0, 0, 3.5, 7.2);
  TF1* line = new TF1("line", "x", 0, 4);
  TF1* line2 = new TF1("line2", "2*x", 0, 3.5);
  line2->SetLineColor(kWhite);
  line->SetLineColor(1);
  line2->GetYaxis()->SetTitle("#varUpsilon(1S)/<#varUpsilon(1S)>");
  line2->GetXaxis()->SetTitle("N_{Ch}^{|#eta|<2.4}/<N_{Ch}^{|#eta|<2.4}>");
  line2->SetTitle("#varUpsilon(1S) Cross Section vs Track Multiplicty");
  line2->Draw();
  Emodel->Draw("P3");
 // EmodelEnergy->Draw("3");
 //line2->Draw("same");
  line->SetLineStyle(3);
  line->SetLineColor(kGreen);
  line->Draw("same");
  
  TH1D* mod = new TH1D("mod", "Pythia Model", 400, 0, 4);
  mod->SetLineColor(kBlue);
  //TH1D* modEnergy = new TH1D("modEnergy", "Pythia Energy Model", 400, 0, 4);
  //modEnergy->SetLineColor(kRed);

  TH1D* data = new TH1D("data", "CMS Data", 4000, 0, 4);
  data->SetMarkerStyle(20);
  data->SetMarkerColor(kRed);
  //Manually Entering CMS Data Points with Error
  data->SetBinContent(3262, 6.67);
  data->SetBinError(3262, .26);
  data->SetBinContent(2010, 3.12);
  data->SetBinError(2010, .15);
  data->SetBinContent(1240, 1.4);
  data->SetBinError(1240, .06);
  data->SetBinContent(630, .240);
  data->SetBinError(630, .01);
  //data->SetMarkerColor(kBlue);
  data->SetMarkerSize(1);
  data->SetLineColor(kRed);
  data->Draw("sameE");

  TLegend* leg = new TLegend(.1,.74,.30,.9);
  leg->SetHeader("For |#eta| < 2.4");
 // leg->AddEntry(modEnergy->DrawCopy("same"), "Pythia 8 model - #Sigma E_{T}/<#Sigma E_{T}>", "l");
  leg->AddEntry(mod->DrawCopy("same"), "Pythia 8 Model", "l");
  leg->AddEntry(data->DrawCopy("sameE"), "CMS data", "l");
  leg->Draw("same");
  mod->SetLineColor(kBlack);
  mod->Draw("same");
  return; }
