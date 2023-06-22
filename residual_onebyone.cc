#include "TF2.h"
#include "TH2.h"
#include "TROOT.h"
#include <iostream>
#include <string>

Double_t gaus2d(Double_t *val, Double_t *par) {
  Double_t x = val[0];
  Double_t y = val[1];
  return par[0] * exp(-(pow((x - par[1]), 2) / (2 * pow(par[2], 2))) -
                      (pow((y - par[3]), 2) / (2 * pow(par[4], 2))));
}

Double_t func2d(Double_t *val, Double_t *par) {
  Double_t *p1 = &par[0];
  Double_t result = gaus2d(val, p1); // + gaus2d(val,p2) + gaus2d(val,p3);

  return result;
}

class fit_residual {
public:
  Double_t r1 = 1.00000;
  Double_t r2 = 10.00000;
  Double_t r3 = 1.00000;
  Double_t r4 = 10.00000;
  Double_t bin = 60.;
  const Int_t npar = 5;
  double prm[5];

  void setprm(double_t x1, double_t x2, double_t x3, double_t x4, double_t x5) {
    prm[0] = x1;
    prm[1] = x2;
    prm[2] = x3;
    prm[3] = x4;
    prm[4] = x5;
  }

  TH2F *residual(TH2 *h, TCanvas *c1, string name) {
    c1->cd();
    auto f = new TF2("f", func2d, r1, r2, r3, r4, npar);

    for (int i = 0; i < 5; i++) {
      if (i == 0) {
        f->SetParameter(i, prm[i]);
      } else
        f->FixParameter(i, prm[i]);
    }

    f->SetNpx(50);
    f->SetNpy(50);
    h->Draw("lego2 0");
    h->Fit("f");
    f->Draw("same surf"); //

    std::cout << " hoge " << std::endl;
    auto c2 = new TCanvas(Form("c_resi%s_%d", name.c_str(), 1));
    TH2F *hist =
        new TH2F(Form("h_resi%s", name.c_str()),
                 Form("%s residual", name.c_str()), bin, r1, r2, bin, r3, r4);
    Double_t val[2];
    Double_t a;
    Double_t *par = f->GetParameters();

    for (Int_t i = 0; i < h->GetNbinsX() + 1; i++) {
      val[0] = (r2 - r1) / bin * i + r1;
      for (Int_t j = 0; j < h->GetNbinsY() + 1; j++) {
        val[1] = (r4 - r3) / bin * j + r3;
        a = func2d(val, par);
        Double_t diff = h->GetBinContent(i, j) - a;
        hist->SetBinContent(i, j, diff);
      }
    }
    hist->Draw("lego2 0");
    gPad->SetLogz(0);

    auto c3 = new TCanvas(Form("c_resi%s_%d", name.c_str(), 2));
    hist->Draw("colz");
    gPad->SetLogz(0);

    return hist;
  }
};

void makehist() {
  Double_t r1 = 1.00000;
  Double_t r2 = 10.00000;
  Double_t r3 = 1.00000;
  Double_t r4 = 10.00000;
  Double_t bin = 60.;

  auto c1 = new TCanvas("c1");
  tree->Draw(
      Form("y:x >> h1(%f, %f, %f, %f, %f, %f)", bin, r1, r2, bin, r3, r4), "",
      "colz");

  auto c2 = new TCanvas("c2");
  auto h2 = new TH2F("h2", "238Np", bin, r1, r2, bin, r3, r4);

  tree->Draw(
      Form("y:x >> h2(%f, %f, %f, %f, %f, %f)", bin, r1, r2, bin, r3, r4), "",
      "lego2 0");

  fit_residual nuclide1;
  nuclide1.setprm(1.0, 1.0, 1.0, 1.0, 1.0);
  auto h3 = nuclide1.residual(h2, c2, "nuclide1");
  TCanvas *c8 = new TCanvas("c8");
  h3->Draw("lego2 0");

  fit_residual nuclide2;
  nuclide2.setprm(1.0, 1.0, 1.0, 1.0, 1.0);
  auto h4 = nuclide2.residual(h3, c8, "nuclide2");
  TCanvas *c9 = new TCanvas("c9");
  h4->Draw("lego2 0");

  fit_residual nuclide3;
  nuclide3.setprm(1.0, 1.0, 1.0, 1.0, 1.0);
  auto h5 = nuclide3.residual(h4, c9, "nuclide3");
  TCanvas *c10 = new TCanvas("c10");
  h5->Draw("lego2 0");
}
