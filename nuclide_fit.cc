#include "TF2.h"
#include "TH2.h"
#include "TROOT.h"
#include <fstream>
#include <iostream>

Double_t gaus2d(Double_t *val, Double_t *par) {
  Double_t x = val[0];
  Double_t y = val[1];
  return par[0] * exp(-(pow((x - par[1]), 2) / (2 * pow(par[2], 2))) -
                      (pow((y - par[3]), 2) / (2 * pow(par[4], 2))));
}

Double_t func2d(Double_t *val, Double_t *par) {
  Double_t *p1 = &par[0];
  Double_t result = gaus2d(val, p1);

  return result;
}

void fit() {
  // histgram range
  Double_t r1 = 1.00000;
  Double_t r2 = 10.00000;
  Double_t r3 = 1.00000;
  Double_t r4 = 10.00000;
  Double_t bin = 40.;
  const Int_t npar = 5; // the number of parameter

  auto c1 = new TCanvas("c1");
  tree->Draw(Form("x:y >> h(%f, %f, %f, %f, %f, %f)", bin, r1, r2, bin, r3, r4),
             "", "colz");

  auto c2 = new TCanvas("c2");
  auto h2 = new TH2F("h2", "", bin, r1, r2, bin, r3, r4);

  tree->Draw(
      Form("x:y >> h2(%f, %f, %f, %f, %f, %f)", bin, r1, r2, bin, r3, r4), "",
      "lego2 0");

  auto f2d = new TF2("f2d", func2d, r1, r2, r3, r4, npar);

  // nuclide parameter
  f2d->SetParameter(0, 1.00000);
  f2d->SetParameter(1, 1.00000);
  f2d->SetParameter(2, 1.00000);
  f2d->SetParameter(3, 1.00000);
  f2d->SetParameter(4, 1.00000);

  f2d->SetNpx(50);
  f2d->SetNpy(50);
  h2->Fit("f2d", "N");
  f2d->Draw("same surf");

  ofstream file;
  file.open("fitting_results.csv", std::ios::app);
  file << "nuclide"
       << ",";

  for (int i = 0; i < 5; i++) {
    file << f2d->GetParameter(i) << "," << f2d->GetParError(i) << ",";
  }

  file << f2d->GetParameter(0) * (f2d->GetParameter(2) / (r2 - r1) * bin) *
              (f2d->GetParameter(4) / (r4 - r3) * bin) * 2 * TMath::Pi()
       << std::endl;
}
