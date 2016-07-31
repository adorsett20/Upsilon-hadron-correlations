
void TestEnergy() {
  TFile *file = TFile::Open("PPCollisionUps1sMilNoTune.root");
  TFile *file2 = TFile::Open("PPCollisionMilNoTune.root");
  TNtuple* PPTupleUps = (TNtuple*)file->Get("PPTuple");
  TNtuple* PPTupleND = (TNtuple*)file2->Get("PPTuple");
  TH1D* NCharUps = new TH1D("NCharUps", "Sum of Energy at mid rapdity", 30000, 0, 300);
  PPTupleUps->Draw("(ETmid-mu1pT-mu2pT)>>NCharUps", "NChar>2 && abs(deta1) < 1.93 && abs(deta2) < 1.93  && mu1pT > 4 && mu2pT > 4");
//  NCharUps->Scale(3.35e-6);
  double sum, percent = 0, centAr[21];
  int c, centa[21], i;
  sum = 0;
  centAr[0] = 180;
  centAr[20] = 0;
  c = 1;
	//Find Activity Ranges
  double min = 0, average[20], bins;
  i = 0;
  //Activity Range Calculations for Track Count
  TH1D* Tracks = new TH1D("Tracks", "Number of Tracks", 10000, 0, 300);
  PPTupleND->Draw("ETmid>>Tracks", "NChar > 0");
  double MinBiasEvents;
  MinBiasEvents = Tracks->GetEntries();
  NCharUps->Draw();
  Tracks->Draw();
  Tracks->Scale(1./Tracks->GetEntries());
  int TrackCent[21];
  percent = .02;
  TrackCent[0] = 300;
  TrackCent[20] = 0;
  c = 1;
  double Ranges[12];
  int counter = 1;
  Ranges[0] = 0;
	//Loop To Get Activity Range Values
  for (i=0; percent <= .02; percent += .02) {
        sum = 0;
  	cout << c << "		";
        for(i=0; sum < percent && i != 10000; i++) {
                sum += Tracks->GetBinContent(10000-i); }
       	 cout << 100*percent - 2 << "-" << 100*percent << "%" << "      " << MinBiasEvents*sum  << "      " << i << " " << Tracks->GetBinCenter(10000-i) << "\n"; // Use to check Activity Range Values
	Ranges[counter] = sum;
	counter++;		
        TrackCent[c] = Tracks->GetBinCenter(10000-i);
        c++;}
  percent = .08;
  for (i=0; percent <= .08; percent += .02) {
        sum = 0;
        cout << c << "		";
        for(i=0; sum < percent || percent == 1; i++) {
                sum += Tracks->GetBinContent(10000-i); }
                         cout << 100*percent - 2 << "-" << 100*percent << "%" << "      " << MinBiasEvents*sum  << "      " << i << " " << Tracks->GetBinCenter(10000-i) << "\n"; // Use to check Activity Range Values
	Ranges[counter] = sum;
        counter++;
        TrackCent[c] = Tracks->GetBinCenter(10000-i);
        c++;}
  percent = .20;
  for (i=0; percent <= .20; percent += .04) {
        sum = 0;
        cout << c << "		";
        for(i=0; sum < percent || percent == 1; i++) {
                sum += Tracks->GetBinContent(10000-i); }
                         cout << 100*percent - 4 << "-" << 100*percent << "%" << "      " << MinBiasEvents*sum  << "      " << i << " " << Tracks->GetBinCenter(10000-i) << "\n"; // Use to check Activity Range Values
	Ranges[counter] = sum;
        counter++;
        TrackCent[c] = Tracks->GetBinCenter(10000-i);
        c++;}
	percent = .30;
//  for (i=0; percent <= .30; percent += .05) {
  //      sum = 0;
//	cout << c << "\n";
//        for(i=0; sum < percent || percent == 1; i++) {
 //               sum += Tracks->GetBinContent(10000-i); }
  //                       cout << 100*percent - 5 << "-" << 100*percent << "%" << "      " << sum  << "      " << i << " " << Tracks->GetBinCenter(10000-i) << "\n"; // Use to check Activity Range Values
 //       TrackCent[c] = Tracks->GetBinCenter(10000-i) + .5;
   //     c++; }
//	percent = .40;
  for (i=0; percent <= 1; percent += .10) {
        sum = 0;
	cout << c << "		";
        for(i=0; sum <= percent && i != 10000; i++) {
                sum += Tracks->GetBinContent(10000-i); }
                         cout << 100*percent - 10 << "-" << 100*percent << "%" << "      " << MinBiasEvents*sum  << "      " << i << " " << Tracks->GetBinCenter(10000-i) << "\n"; // Use to check Activity Range Values
	Ranges[counter] = sum;
        counter++;
        TrackCent[c] = Tracks->GetBinCenter(10000-i);
        c++; }
  cout << "\n" << "Activity Range Values from Array" << "\n";
  for(c = 0; c < 11; c++) {
    	cout << c << ")   "  << TrackCent[c] << "\n"; }
  min = 0;
  double TrackAverage[20];

  cout << "\n" << NCharUps->GetBinCenter(30000) << "\n";

  i = 0;
  double TotalUps = 0;
  //cout << "\n" << "Activity Bin Averages" << "\n";
	//Loop To Get Average In Each Activity Region
  for(c = 0; NCharUps->GetBinCenter(30000-i) > TrackCent[c+1] && NCharUps->GetBinCenter(30000-i) <= TrackCent[c]; c++) {
        sum = 0;
        bins = 0;
        for(i = min; (NCharUps->GetBinCenter(30000-i) > TrackCent[c+1] && NCharUps->GetBinCenter(30000-i) <= TrackCent[c]) && i != 30000; i++) {
                sum += NCharUps->GetBinContent(30000-i)*(NCharUps->GetBinCenter(30000-i));
                bins += NCharUps->GetBinContent(30000-i); }
        TrackAverage[c] = sum/bins;
	TotalUps += bins;
	cout << "Point " << c << ": " << bins << " +- " << sqrt(bins) << "   Upsilons" << "\n";
        min = i; }
	cout << NCharUps->GetBinCenter(100) << "\n";
       	// cout << 10*c << "-" << 10*(c+1) << "%" << "     " << TrackAverage[c] << "\n";  }
  cout << "Total number of Upsilons: " << TotalUps << "\n";
  cout << "\n" << "Corrected X/Y Track values" << "\n";
  double TrackYpoints[20];
  for(i = 0; i < 12; i++) {
	cout << (Ranges[i+1] - Ranges[i])*MinBiasEvents << "\n"; }

	//Take Average Values and Divide by ND Average to Get X points
 // for(c = 0; c < 4; c++) {
//	TrackAverage[c] = TrackAverage[c]/Tracks->GetMean();
//	TrackYpoints[c] = (50./NCharUps->Integral(0,25000))*NCharUps->Integral(TrackCent[c+1], TrackCent[c]-1 );
//	cout << "(" <<TrackAverage[c] << "," << TrackYpoints[c] <<  ")" <<  "\n"; }
  //for(c = 4; c < 7; c++) {
    //    TrackAverage[c] = TrackAverage[c]/Tracks->GetMean();
  //      TrackYpoints[c] = (25./NCharUps->Integral(0,25000))*NCharUps->Integral(TrackCent[c+1], TrackCent[c]-1 );
    //    cout << "(" <<TrackAverage[c] << "," << TrackYpoints[c] <<  ")" <<  "\n"; }
//  for(c = 7; c < 10; c++) {
 //       TrackAverage[c] = TrackAverage[c]/Tracks->GetMean();
   //     TrackYpoints[c] = (20./NCharUps->Integral(0,25000))*NCharUps->Integral(TrackCent[c+1], TrackCent[c]-1 );
//	cout << "(" <<TrackAverage[c] << "," << TrackYpoints[c] <<  ")" <<  "\n"; }


TrackAverage[0] = TrackAverage[0]/Tracks->GetMean();
        TrackYpoints[0] = (50./NCharUps->Integral(0,30000))*NCharUps->Integral(100*TrackCent[1], 100*(TrackCent[0]-1) );
	cout << "(" <<TrackAverage[0] << "," << TrackYpoints[0] <<  ")" <<  "\n";

TrackAverage[1] = TrackAverage[1]/Tracks->GetMean();
        TrackYpoints[1] = (16.67/NCharUps->Integral(0,30000))*NCharUps->Integral(100*TrackCent[2], 100*TrackCent[1]-100 );
	cout << "(" <<TrackAverage[1] << "," << TrackYpoints[1] <<  ")" <<  "\n";

TrackAverage[2] = TrackAverage[2]/Tracks->GetMean();
        TrackYpoints[2] = (8.33/NCharUps->Integral(0,30000))*NCharUps->Integral(100*TrackCent[3], 100*TrackCent[2]-100 );
	cout << "(" <<TrackAverage[2] << "," << TrackYpoints[2] <<  ")" <<  "\n";

 for(c = 3; c < 14; c++) {
        TrackAverage[c] = TrackAverage[c]/Tracks->GetMean();
        TrackYpoints[c] = (10./NCharUps->Integral(0,30000))*NCharUps->Integral(100*TrackCent[c+1], 100*TrackCent[c]-100 );
        cout << "(" <<TrackAverage[c] << "," << TrackYpoints[c] <<  ")" <<  "\n"; } 
  TF1* line = new TF1("line", "x", 0, 4.5);
  TF1* line2 = new TF1("line2", "1.8*x", 0, 4.5);
  line2->SetLineColor(kWhite);
  sum = 0;
  double scaledSumE = 0, scaleSumT = 0;
  scaleSumT = NCharUps->Integral(0,30000);
  TCanvas* can2 = new TCanvas("can2", "Centrality");
  can2->cd();
  line->SetLineColor(1);
  line2->GetYaxis()->SetTitle("#varUpsilon(1S)/<#varUpsilon(1S)>");
  line2->GetXaxis()->SetTitle("#Sigma E_{T}^{|#eta| < 2.4}/<#Sigma E_{E}^{|#eta| < 2.4}>");
  line2->SetTitle("#varUpsilon(1S) Cross Section vs Track Multiplicity");
  line2->Draw();
  line->SetLineStyle(3);
  line->Draw("same");
   for(c = 0; c < 18; c++) { cout << TrackCent[c] << "           Bin Average: " << 10.32*TrackAverage[c] <<  "\n";}
  double Test, Testaverage, Testsum, Testbin, count = 0, TestY = 0;
  Test = NCharUps->Integral(38,100);
  for(i = 38; i < 100; i++) {
	Testsum += NCharUps->GetBinContent(i)*NCharUps->GetBinCenter(i);
	Testbin += NCharUps->GetBinContent(i); }
  Testaverage = Testsum/Testbin;
  TestY = (10./NCharUps->Integral(0,30000))*NCharUps->Integral(38, 100);
  cout << "\n" << "\n" << TestY << "\n" << "\n";

  TH1D* modelT = new TH1D("modelT", "Pythia model", 5000, 0, 5);
  //Fill Histogram with points from Model
  for(i = 0; i < 20; i++) {
	//Track Values
	modelT->SetBinContent(1000*TrackAverage[i], TrackYpoints[i]);
        modelT->SetBinError(1000*TrackAverage[i], .01); }
  modelT->SetMarkerStyle(20);
  modelT->SetMarkerColor(kBlue);
  modelT->SetLineColor(kBlue);
  modelT->Draw("sameE");
  //Histogram for STAR Data Points
  TH1D* data = new TH1D("data", "CMS Data", 4000, 0, 4);
  data->SetMarkerStyle(20);
  data->SetMarkerColor(kRed);
  data->SetLineColor(kRed);
  data->SetBinContent(3262, 6.67);
  data->SetBinError(3262, .26);
  data->SetBinContent(2010, 3.12);
  data->SetBinError(2010, .15);
  data->SetBinContent(1240, 1.4);
  data->SetBinError(1240, .06);
  data->SetBinContent(630, .240);
  data->SetBinError(630, .01);
  data->Draw("sameE");
  TLegend* leg = new TLegend(.1,.74,.30,.9);
  leg->SetHeader("For |#eta| < 2.4");
  leg->AddEntry(modelT->DrawCopy("same"), "Pythia 8 Model", "l");
  leg->AddEntry(data->DrawCopy("same"), "CMS data", "l");
  leg->Draw("same");
  return; }
