void TuneAnalysisEnergy() {
  //Set up Canvas Area
  TCanvas* can2 = new TCanvas("can2", "Centrality");

  double Xvalues[] = {2.52, 1.72, 1.35, 1.08, .89, .74, .62, .51, .40, .24, 2.47, 1.81, 1.42, 1.12, .88, .71, .58, .47, .37, .23};
  double Yvalues[] = {2.53, 1.81, 1.42, 1.10, .81, .67, .57, .43, .40, .34, 2.74, 2.19, 1.60, 1.14, .73, .50, 39, .27, .26, .22};
  TGraph* model = new TGraph(20, Xvalues, Yvalues);
  model->SetFillStyle(3022);
  model->SetFillColor(kBlue);
  model->Draw("sameLFa4");

  double xAverage[] = {2.50, 1.77, 1.39, 1.1, .89, .73, .6, .49, .39, .23};
  double yAverage[] = {2.64, 2.01, 1.495, 1.12, .77, .59, .48, .35, .33, .28};
  double yError[] = {.105, .185, .105, .02, .04, .085, .09, .08, .07, .06};
  double xError[] = {0,0,0,0,0,0,0,0,0,0};
  TGraphErrors* Emodel = new TGraphErrors(10, xAverage, yAverage, xError, yError);
  Emodel->SetFillStyle(3001);
  Emodel->SetFillColor(kBlue);

  TF1* line = new TF1("line", "x", 0, 4);
  TF1* line2 = new TF1("line2", "1.7*x", 0, 4);
  line2->SetLineColor(kWhite);
  can2->cd();
  line->SetLineColor(1);
  Emodel->GetYaxis()->SetTitle("#varUpsilon(1S)/<#varUpsilon(1S)>");
  Emodel->GetXaxis()->SetTitle("#Sigma E_{T}^{4<|#eta|<5.2}/<#Sigma E_{T}^{4<|#eta|<5.2}>");
  Emodel->SetTitle("#varUpsilon(1S) Cross Section vs #Sigma E_{T}");
  Emodel->Draw("samea3");
  line2->Draw("same");
  line->SetLineStyle(3);
  line->SetLineColor(kGreen);
  line->Draw("same");

  TH1D* mod = new TH1D("mod", "Pythia Model", 400, 0, 4);
  mod->SetLineColor(kBlue);

  TH1D* data = new TH1D("data", "CMS Data", 4000, 0, 4);
  data->SetMarkerStyle(20);
  data->SetMarkerColor(kRed);
  //Manually Entering CMS Data Points with Error
  data->SetBinContent(2590, 2.74);
  data->SetBinError(2590, .14);
  data->SetBinContent(1460, 1.40);
  data->SetBinError(1460, .05);
  data->SetBinContent(700, .52);
  data->SetBinError(700, .02);
  data->SetMarkerColor(kRed);
  data->SetMarkerSize(1);
  data->SetLineColor(kRed);
  data->Draw("sameE");

  TLegend* leg = new TLegend(.1,.74,.30,.9);
  leg->SetHeader("For 4 < |#eta| < 5.2");
  leg->AddEntry(mod->DrawCopy("same"), "Pythia 8 Model", "l");
  leg->AddEntry(data->DrawCopy("same"), "CMS data", "l");
  leg->Draw("same");
  mod->SetLineColor(kBlack);
  mod->Draw("same"); 

  return; }
