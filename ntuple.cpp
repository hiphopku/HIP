// source1 for the 1st .dat EASIROC file
// source2 for the 2nd .dat EASIROC file
// source3 for a .csv file which contains beta, ch & cluster.
/////////////////////////////////////////////////////////////
//////////////   Execute Program like this.   ///////////////
/////////////////////////////////////////////////////////////
// (First of all, launch ROOT by the following command.)
// $ root
// (Then ROOT will be opened up.)
// root[0] .L ntuple.cpp
// root[1] ntuple("run0200_1.root", "run0200_2.root", "ntuple_run0200.csv")
// (If everything works out,  you'll see "NTUPLE completed." displayed.)


#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>

#include"TH1.h"
#include"TH2.h"
#include"TTree.h"
#include"TFile.h"

void ntuple(char* source1, char* source2, char* source3){

  char* outfile = "NTUPLE.root";
  Int_t entries = 0;


  // _1.dat
  std::ifstream ifs1(source1, std::ios::binary);

  if(!ifs1.is_open()){
    std::cout << "no file" << std::endl;
    return;
  }

  TH1F *tdc_1[32][16];
  TH1F *tdc_n_1[32][16];
  TH1F *adc_1[32];
  TH1F *adc_l_1[32];

  int dtdc_1[32][16];
  int dtdc_n_1[32][16];
  int dadc_1[32];

  TFile *fout = new TFile(outfile,"RECREATE");
  TTree *tree = new TTree("tree","tree");
  tree->Branch("tdc_1", dtdc_1, "tdc_1[32][16]/I");
  tree->Branch("tdc_n_1", dtdc_n_1, "tdc_n_1[32][16]/I");
  tree->Branch("adc_1", dadc_1, "adc_1[32]/I");

  for(int ch = 0; ch<32; ++ch){
    std::ostringstream _name;
    std::ostringstream _name_l;
    _name << "1-h" << ch;
    _name_l << "1-hl" << ch;

    std::string ahist_name = _name.str();
    adc_1[ch] = new TH1F(ahist_name.c_str(), ahist_name.c_str(),4000,0,4000);

    std::string alhist_name = _name_l.str();
    adc_l_1[ch] = new TH1F(alhist_name.c_str(), alhist_name.c_str(),4000,0,4000);
    for(int i = 0; i<16; ++i){
      std::ostringstream _name_t;
      std::ostringstream _name_t_n;
      _name_t << "1-h" << ch << "_" << i;
      _name_t_n << "1-h" << ch << "_" << i << "_n";
      std::string thist_name = _name_t.str();
      std::string thist_name_n = _name_t_n.str();
      tdc_1[ch][i] = new TH1F(thist_name.c_str(), thist_name.c_str(),2000,0,2000);
      tdc_n_1[ch][i] = new TH1F(thist_name_n.c_str(), thist_name_n.c_str(),2000,0,2000);
    }
  }

  int index[32];
  int index_n[32];
  for ( i = 0; i < 32; i++ ) {
    index[i] = 0;
    index_n[i] = 0;
  }

  while(!ifs1.eof()){
    int data = 0;
    ifs1.read((char*)&data, sizeof(int));

    if((unsigned int)data == 0xffffea0c){
      for(int ch = 0; ch<32; ++ch){
        index[ch] = 0;
        index_n[ch] = 0;
        dadc_1[ch] = -1;
        for(int m = 0; m<16; ++m){
          dtdc_1[ch][m] = -1;
          dtdc_n_1[ch][m] = -1;
        }
      }


      ifs1.read((char*)&data, sizeof(int));
      int NofWord = data & 0xffff;

      // skip event counter
      ifs1.read((char*)&data, sizeof(int));
      for(int i = 0; i<NofWord; ++i){
        ifs1.read((char*)&data, sizeof(int));
        if(0xcc == ((data >> 24) & 0xff)){
          int channel = (data >> 16) & 0xff;
          int val1 = data & 0xfff;
          std::cout << "ch " << channel << " " << val1 << sd::endl;
          if(0 == ((data >> 15) & 0x1)){
            tdc_1[channel][index[channel]]->Fill(val1);
            dtdc_1[channel][index[channel]] = val1;
            index[channel]++;
          }
          if(1 == ((data >> 15) & 0x1)) {
            tdc_n_1[channel][index_n[channel]]->Fill(val1);
	          dtdc_n_1[channel][index_n[channel]] = val1;
	          index_n_1[channel]++;
          }
        } else if(0x81 == ((data >> 24) & 0xff)){
          int channel = (data >> 16) & 0xff;
          int val1 = data & 0xfff;
          adc_1[channel]->Fill(val1);
          dadc_1[channel] = val1;
        } else if(0x60 == ((data >> 24) & 0xff)){
          int channel = (data >> 16) & 0xff;
          int val1 = data & 0xfff;
          adc_l_1[channel]->Fill(val1);
        }
      }
      entries++;
    }
  }
  ifs1.close();
  // End of _1.dat


  // _2.dat
  std::ifstream ifs2(source2, std::ios::binary);


  if(!ifs2.is_open()){
     std::cout << "no file" << std::endl;
     return;
   }

   TH1F *tdc_2[32][16];
   TH1F *tdc_n_2[32][16];
   TH1F *adc_2[32];
   TH1F *adc_l_2[32];

   int dtdc_2[32][16];
   int dtdc_n_2[32][16];
   int dadc_2[32];

   tree->Branch("tdc_2", dtdc_2, "tdc_2[32][16]/I");
   tree->Branch("tdc_n_2", dtdc_n_2, "tdc_n_2[32][16]/I");
   tree->Branch("adc_2", dadc_2, "adc_2[32]/I");

   for(int ch = 0; ch<32; ++ch){
     std::ostringstream _name;
     std::ostringstream _name_l;
     _name << "2-h" << ch;
     _name_l << "2-hl" << ch;

     std::string ahist_name = _name.str();
     adc_2[ch] = new TH1F(ahist_name.c_str(), ahist_name.c_str(),4000,0,4000);

     std::string alhist_name = _name_l.str();
     adc_l_2[ch] = new TH1F(alhist_name.c_str(), alhist_name.c_str(),4000,0,4000);
     for(int i = 0; i<16; ++i){
       std::ostringstream _name_t;
       std::ostringstream _name_t_n;
       _name_t << "2-h" << ch << "_" << i;
       _name_t_n << "2-h" << ch << "_" << i << "_n";
       std::string thist_name = _name_t.str();
       std::string thist_name_n = _name_t_n.str();
       tdc_2[ch][i] = new TH1F(thist_name.c_str(), thist_name.c_str(),2000,0,2000);
       tdc_n_2[ch][i] = new TH1F(thist_name_n.c_str(), thist_name_n.c_str(),2000,0,2000);
     }
   }

   int index[32];
   int index_n[32];
   for ( i = 0; i < 32; i++ ) {
     index[i] = 0;
     index_n[i] = 0;
   }

   while(!ifs2.eof()){
     int data = 0;
     ifs2.read((char*)&data, sizeof(int));

     if((unsigned int)data == 0xffffea0c){
       for(int ch = 0; ch<32; ++ch){
         index[ch] = 0;
         index_n[ch] = 0;
         dadc_2[ch] = -1;
         for(int m = 0; m<16; ++m){
           dtdc_2[ch][m] = -1;
           dtdc_n_2[ch][m] = -1;
         }
       }

       ifs2.read((char*)&data, sizeof(int));
       int NofWord = data & 0xffff;

       // skip event counter
       ifs2.read((char*)&data, sizeof(int));
       for(int i = 0; i<NofWord; ++i){
         ifs2.read((char*)&data, sizeof(int));
         if(0xcc == ((data >> 24) & 0xff)){
           int channel = (data >> 16) & 0xff;
           int val2 = data & 0xfff;
           std::cout << "ch " << channel << " " << val2 << sd::endl;
           if(0 == ((data >> 15) & 0x1)){
             tdc_2[channel][index[channel]]->Fill(val2);
             dtdc_2[channel][index[channel]] = val2;
             index[channel]++;
           }
           if(1 == ((data >> 15) & 0x1)){
             tdc_n_2[channel][index_n[channel]]->Fill(val2);
	           dtdc_n_2[channel][index_n[channel]] = val2;
	           index_n[channel]++;
           }
         }
         else if(0x81 == ((data >> 24) & 0xff)){
           int channel = (data >> 16) & 0xff;
           int val2 = data & 0xfff;
           adc_2[channel]->Fill(val2);
           dadc_2[channel] = val2;
         }
         else if(0x60 == ((data >> 24) & 0xff)){
           int channel = (data >> 16) & 0xff;
           int val2 = data & 0xfff;
           adc_l_2[channel]->Fill(val2);
         }
       }
     }
   }
   ifs2.close();
   // End of _2.dat


   // ntuple_*.csv
   ifstream in;
   in.open(Form(source3));
   if(!in.is_open()){
     std::cout << "no file" << std::endl;
     return;
   }

   Float_t beta, CH, cluster;
   Int_t nlines = 0;

   tree->Branch("beta", &beta, "beta/F");
   tree->Branch("CH", &CH, "CH/F");
   tree->Branch("cluster", &cluster, "cluster/F");

   while ( nlines < entries ) {
     in >> beta >> CH >> cluster;
     if (!in.good()) break;
     tree->Fill();
     nlines++;
     // cout << nlines << " " << beta << " " << CH << " " << cluster << endl;
   }
   in.close();
   // End of ntuple_*.csv


   fout->Write();
   fout->Close();
   delete fout;
   fout = NULL;

   cout << "NTUPLE completed." << endl;

  return;
}
