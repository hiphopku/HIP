#define ClusterAlgorism_cxx
#include "ClusterAlgorism.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <sstream>

#define THRE 1560

void ClusterAlgorism::Loop()
{
   if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  // Initialize N tuple
  int NT[10000][3]; // NT for N tuple
  for ( int i = 0; i < nentries; i++ ) {
    for ( int j = 0; j < 3; j++ ) {
      NT[i][j] = 0;
    }
  }

  // 3mm MPPC i.e. NT[nevents][0]
  int nevent; // n-th event
  for ( nevent = 0; nevent < 10000; nevent++ ) {
    // TH1F* h1_hit = new TH1F("h1_hit", "h1_hit", 16, -0.5, 15.5);
    Long64_t nbytes = 0, nb = 0;
    Long64_t ientry = LoadTree(nevent);
    if (ientry < 0) break;
    nb = fChain->GetEntry(nevent);
    nbytes += nb;

    float adcSUM = 0;

    adcSUM = adc[0]-802
 	  + adc[1]-815
 	  + adc[2]-810
 	  + adc[3]-807
 	  + adc[4]-815
 	  + adc[5]-810
 	  + adc[6]-815
 	  + adc[7]-820
 	  + adc[8]-815
 	  + adc[9]-807
 	  + adc[10]-806
 	  + adc[11]-817
    + adc[12]-810
    + adc[13]-813
    + adc[14]-823
    + adc[15]-810;


    // new valiable adc count //
    double adc2[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    adc2[0] = adc[0]-802;
    adc2[1] = adc[1]-815;
    adc2[2] = adc[2]-810;
    adc2[3] = adc[3]-807;
    adc2[4] = adc[4]-815;
 	  adc2[5] = adc[5]-810;
 	  adc2[6] = adc[6]-815;
 	  adc2[7] = adc[7]-820;
 	  adc2[8] = adc[8]-815;
 	  adc2[9] = adc[9]-807;
 	  adc2[10] = adc[10]-806;
 	  adc2[11] = adc[11]-817;
 	  adc2[12] = adc[12]-810;
 	  adc2[13] = adc[13]-813;
 	  adc2[14] = adc[14]-823;
 	  adc2[15] = adc[15]-810;



    // Cluster Algorithm
    // These lines below are appended.
    // Declearation
    int ch = 0;
    double adc3[16];

    // Initialize the array
    for ( ch = 0; ch < 16; ch++ ){
      adc3[ch] = 0;
    }

    // Threshold
    // cout << "Showing adc3 if adc2 values more than the threshold" << endl;
    for ( ch = 0; ch < 16; ch++ ) {
      if ( adc[ch] > THRE ) {
        // Here we use "THRE" defined at the 7th line
        adc3[ch] = adc[ch]; // "adc2[ch] > THRE" OR "adc[ch] > THRE" ?
        // cout << "> ch=" << ch << " " << "adc3=" << adc[ch] << endl;
      }
    }

    // Integration
    double SUM[8];
    for ( int j = 0; j < 8; j++ ) {
      SUM[j] = 0;
    }

    int peak = 0;
    for ( ch = 0; ch < 16; ch++ ) {
      if ( ch == 0 && adc[ch] > THRE ) {
        SUM[peak] = SUM[peak] + adc[ch];
      } else if ( 1 <= ch && ch <= 13 && adc[ch] > THRE ) {
        SUM[peak] = SUM[peak] + adc[ch];
        if ( ( adc3[ch] > adc3[ch+1] && adc3[ch+1] < adc3[ch+2] ) || ( adc[ch+1] < THRE ) ) {
          // If the next ch is a valley or if adc3[ch] of the next ch is 0, peak should be plused by one

          // Let's go on to the next peak
          peak++;

          // You can get rid of a valley by the following 3 lines
          if ( adc3[ch] > adc3[ch+1] && adc3[ch+1] < adc3[ch+2] ) {
            ch++;
          }
        }
      } else if ( ( ch == 14 || ch == 15 ) &&  adc[ch] > THRE ) {
        SUM[peak] = SUM[peak] + adc[ch];
      }
    }
    // Appended lines are over.


    if ( SUM[1] == 0 && SUM[2] == 0 &&  SUM[6] == 0 && SUM[7] == 0 ) {
      NT[nevent][0] = SUM [0]; // Put SUM into N tuple
    } else {
      nevent++; // Go on to the next nb
    }
    cout << "> " << nevent+1 << "th  SUM[0]=" << SUM[0] << " SUM[1]=" << SUM[1] << " NT=" << NT[nevent][0] << endl;

  }

  ofstream outputfile("Ntuple2.csv");
  for ( nevent = 0; nevent < nentries; nevent++ ) {
    outputfile << NT[nevent][0] << endl;
  }

  outputfile.close();

}
