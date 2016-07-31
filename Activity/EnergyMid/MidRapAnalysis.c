void MidRapAnalysis() {
  //Set up Canvas Area
  TCanvas* can2 = new TCanvas("can2", "Centrality");

  double xAverage[] = {3.42, 2.62, 1.96, 1.45, 1.12, .87, .67, .49, .39, .28, .23};
  double yAverage[] = {5.22, 3.33, 2.21, 1.40, 1.06, .72, .49, .22, .16, .06, .03};
  double yError[] = {.36, .25, .12, .06, .01, .12, .04, .03, .01, .01, 0};
  double xError[] = {0,0,0,0,0,0,0,0,0,0};
  TGraphErrors* Emodel = new TGraphErrors(10, xAverage, yAverage, xError, yError);
  Emodel->SetFillStyle(3001);
  Emodel->SetFillColor(kBlue);

  double xAverageE[] = {4.19, 2.92, 2.03, 1.40, 1.04, .77, .57, .42, .32, .24};
  double yAverageE[] = {5.39, 3.36, 2.14, 1.33, .90, .56, .37, .16, .14, .01};
  double yErrorE[] = {.31, .24, .08, .07, .01, .01, .02, .06, .07, .01};
  double xErrorE[] = {0,0,0,0,0,0,0,0,0,0};
  TGraphErrors* EmodelE = new TGraphErrors(10, xAverageE, yAverageE, xErrorE, yErrorE);
  EmodelE->SetFillStyle(3001);
  EmodelE->SetFillColor(kGreen);

  can2->cd();
  TH1F* frame = can2->DrawFrame(0, 0, 3.5, 7.2);
  TF1* line = new TF1("line", "x", 0, 4.3);
  TF1* line2 = new TF1("line2", "1.6*x", 0, 4.3);
  line2->SetLineColor(kWhite);
  line->SetLineColor(1);
  line2->GetYaxis()->SetTitle("#varUpsilon(1S)/<#varUpsilon(1S)>");
//  line2->GetXaxis()->SetTitle("N_{Ch}^{|#eta|<2.4}/<N_{Ch}^{|#eta|<2.4}>");
  line2->SetTitle("#varUpsilon(1S) Cross Section vs Event Activity");
  line2->Draw();
  Emodel->Draw("P3");
  EmodelE->Draw("3");
 //line2->Draw("same");
  line->SetLineStyle(3);
  line->SetLineColor(kBlack);
  line->Draw("same");
  
  TH1D* mod = new TH1D("mod", "Pythia Model", 400, 0, 4);
  mod->SetLineColor(kBlue);
  TH1D* modEnergy = new TH1D("modEnergy", "Pythia Energy Model", 400, 0, 4);
  modEnergy->SetLineColor(kGreen);

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

  TLegend* leg = new TLegend(.1,.7,.38,.9);
  leg->SetHeader("For |#eta| < 2.4");
  leg->AddEntry(modEnergy->DrawCopy("same"), "Pythia 8 - #Sigma E_{T}/<#Sigma E_{T}>", "l");
  leg->AddEntry(mod->DrawCopy("same"), "Pythia 8 - N_{Ch}/<N_{Ch}>", "l");
  leg->AddEntry(data->DrawCopy("sameE"), "CMS data - N_{Ch}/<N_{Ch}>", "l");
  leg->Draw("same");
  mod->SetLineColor(kBlack);
  mod->Draw("same");
  return; }
