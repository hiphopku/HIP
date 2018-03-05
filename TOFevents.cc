#include<fstream>
#include<TH1.h>
#include<TH2.h>
#include<TFile.h>
#include<TTree.h>
#include<TCanvas.h>
#include<math.h>
#include<stdlib.h>
//#include<iostream>

#include<iostream> using namespace std;


#include<stdio.h>
void TOFevents(){


  //  gStyle->SetOptFit(0001);
  gStyle->SetOptFit(0000);

  char inname[100],outname[100];
  sprintf(inname,"run0186_TDC_201701261.csv");//入力ファイル名
  //  sprintf(inname,"caf2-20111005-21.root");//入力ファイル名
  sprintf(outname,"tof.root");//出力ファイル名




  //making TTree
  TFile ftree(outname,"recreate");
  TTree *tree2= new TTree("h40","Coincidence");

  Float_t Naiadc[1],Naitdc[1];
  tree2->Branch("Naiadc",&Naiadc,"Naiadc[1]/F");
  tree2->Branch("Naitdc",&Naitdc,"Naitdc[1]/F");

  // http://simd.jugem.jp/?eid=49

  FILE *fp;
  fp = fopen( inname, "r" );
  if( fp == NULL ){
    printf( "%sファイルが開けません¥n", fname );
    return -1;
  }
  char f1;
  Int_t f2;

  while( ( ret = fscanf( fp, "%s, %f", &f1, &f2 ) ) != EOF ){
    printf( "%f\n", f2 );
    Naiadc[0]=f2;
    tree2->Fill();
 }


  fclose( fp );

  tree2->Write();
  //  tree2->Print();
  //  tree2->Show(200);
  ftree.Close();


  //ここまででROOTファイルが完成
  //ついでにつくってTTreeからヒストグラムを生成してみる

  //ヒストグラムの準備
  TH1F *adc[1];
  char histname[1][99],histtitle[1][99];
  for (Int_t id=0; id<1; id++){
    sprintf(histname[id],"h10%d",id+1);
    sprintf(histtitle[id],"Geant Simulation for ADC %d",id+1);
    adc[id] = new TH1F(histname[id],histtitle[id],2000,0,4000);
  }


  TFile *file1 = new TFile(outname);

  Float_t Nadc[1];
  h40->SetBranchAddress("Naiadc",&Nadc);

  Int_t nevent =h40->GetEntries();

  for (Int_t nev=1; nev<nevent; nev++){
    printf("%dth loop out of%d events\n",nev,nevent);
    h40->GetEntry(nev);
    for (Int_t id=0;id<1;id++){
      adc[id]->Fill(Nadc[id]);
    }
  }

  TCanvas *c1 = new TCanvas();
  // c1->Divide(2,3);

  c1_1->SetLogy();


  for (Int_t id=0;id<1;id++){
    c1->cd(id+1);
    adc[id]->Draw();
  }
  c1->Print("TOF.root");
}
