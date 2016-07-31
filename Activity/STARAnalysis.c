void STARAnalysis() {
  //Set up Canvas Area
  TCanvas* can2 = new TCanvas("can2", "Centrality");

  double Xvalues[] = {2.64, 1.79, 1.38, 1.1, .87, .68, .55, .45, .37, .24, 2.54, 1.88, 1.43, 1.08, .81, .61, .48, .38, .29, .20};
  double Yvalues[] = {3.38, 1.98, 1.48, .90, .89, .56, .39, .19, .16, .05, 3.69, 2.12, 1.51, 1.09, .64, .40, .26, .12, .05, .01};
  TGraph* model = new TGraph(20, Xvalues, Yvalues);
  model->SetFillStyle(3022);
  model->SetFillColor(kBlue);
  model->Draw("sameLFa4");

  double xAverage[] = {3.69, 2.60, 1.88, 1.41, 1.14, .92, .75, .53, .50, .39, .26};
  double yAverage[] = {6.47, 4.14, 2.23, 1.13, 1.08, .52, .41, .22, .13, .05, .02};
  double yError[] = {.16, .20, .12, .1, .1, .14, .08, .14, .12, .04, .08};
  double xError[] = {.01,.05,.06,.04,.03,.02,.03,.12,.02,0};
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
  TH1F* frame = can2->DrawFrame(0, 0, 4, 7.2);
  TF1* line = new TF1("line", "x", 0, 4);
  TF1* line2 = new TF1("line2", "2*x", 0, 4.1);
  line2->SetLineColor(kWhite);
  line->SetLineColor(1);
  line2->GetYaxis()->SetTitle("#varUpsilon(1S)/<#varUpsilon(1S)>");
  line2->GetXaxis()->SetTitle("TofMult/<TofMult>");
  line2->SetTitle("#varUpsilon(1S) Cross Section vs Time of Flight Multiplicity");
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

  TH1D* data = new TH1D("data", "STAR Data", 4000, 0, 4);
  data->SetMarkerStyle(20);
  data->SetMarkerColor(kRed);
  //Manually Entering CMS Data Points with Error
  data->SetBinContent(3790, 7.10);
  data->SetBinError(3790, 1.27);
  data->SetBinContent(1888, 2.44);
  data->SetBinError(1888, .32);
  data->SetBinContent(630, .386);
  data->SetBinError(630, .05);
  //data->SetMarkerColor(kBlue);
  data->SetMarkerSize(1);
  data->SetLineColor(kRed);
  data->Draw("sameE");

  TLegend* leg = new TLegend(.1,.74,.30,.9);
  leg->SetHeader("For |#eta| < 1.0");
 // leg->AddEntry(modEnergy->DrawCopy("same"), "Pythia 8 model - #Sigma E_{T}/<#Sigma E_{T}>", "l");
  leg->AddEntry(mod->DrawCopy("same"), "Pythia 8 Model", "l");
  leg->AddEntry(data->DrawCopy("sameE"), "STAR data", "l");
  leg->Draw("same");
  mod->SetLineColor(kBlack);
  mod->Draw("same");
  return; }
