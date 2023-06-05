#include "TF2.h"
#include "TH2.h"
#include "TROOT.h"
#include <iostream>
#include <stdio.h>

Double_t gaus2d(Double_t *val, Double_t *par) {
  Double_t x = val[0];
  Double_t y = val[1];
  return par[0] * exp(-(pow((x - par[1]), 2) / (2 * pow(par[2], 2))) -
                      (pow((y - par[3]), 2) / (2 * pow(par[4], 2))));
}

Double_t func2d(Double_t *val, Double_t *par) {
  Double_t *p1 = &par[0];
  Double_t *p2 = &par[5];
  Double_t *p3 = &par[10];
  Double_t result = gaus2d(val, p1) + gaus2d(val, p2) + gaus2d(val, p3);

  return result;
}

void fit() {
  Double_t r1 = 1.000;
  Double_t r2 = 10.000;
  Double_t r3 = 1.000;
  Double_t r4 = 10.000;
  Double_t bin = 40.;
  const Int_t npar = 5;

  auto c1 = new TCanvas("c1");
  tree->Draw(
      Form("y:x >> h1(%f, %f, %f, %f, %f, %f)", bin, r1, r2, bin, r3, r4), "",
      "colz");

  auto c2 = new TCanvas("c2");
  auto h2 = new TH2F("h2", "", bin, r1, r2, bin, r3, r4);
  tree->Draw(
      Form("y:x >> h2(%f, %f, %f, %f, %f, %f)", bin, r1, r2, bin, r3, r4), "",
      "lego2 0");
  auto f2d = new TF2("f2d", func2d, r1, r2, r3, r4, npar);

  // nuclide 1
  f2d->SetParameter(0, 1.000);
  f2d->SetParameter(1, 1.000);
  f2d->SetParameter(2, 1.000);
  f2d->SetParameter(3, 1.000);
  f2d->SetParameter(4, 1.000);

  // nuclide 2
  f2d->SetParameter(5, 1.000);
  f2d->SetParameter(6, 1.000);
  f2d->SetParameter(7, 1.000);
  f2d->SetParameter(8, 1.000);
  f2d->SetParameter(9, 1.000);

  // nuclide 3
  f2d->SetParameter(10, 1.000);
  f2d->SetParameter(11, 1.000);
  f2d->SetParameter(12, 1.000);
  f2d->SetParameter(13, 1.000);
  f2d->SetParameter(14, 1.000);

  f2d->SetNpx(50);
  f2d->SetNpy(50);
  h2->Fit("f2d", "N");
  f2d->Draw("same surf");

  TCanvas *c3 = new TCanvas("c3");
  TH2F *h3 = new TH2F("h3", "residual", bin, r1, r2, bin, r3, r4);
  Double_t val[2];
  Double_t a;
  Double_t *par = f2d->GetParameters();

  for (Int_t i = 0; i < h2->GetNbinsX() + 1; i++) {
    val[0] = (r2 - r1) / bin * i + r1;
    for (Int_t j = 0; j < h2->GetNbinsY() + 1; j++) {
      val[1] = (r4 - r3) / bin * j + r3;
      a = func2d(val, par);
      Double_t diff = h2->GetBinContent(i, j) - a;
      h3->SetBinContent(i, j, diff);
    }
  }
  h3->Draw("lego2 0");
  gPad->SetLogz(0);

  TCanvas *c4 = new TCanvas("c4");
  h3->Draw("colz");
  gPad->SetLogz(0);
}
