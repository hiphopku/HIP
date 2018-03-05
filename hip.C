#define hip_cxx
#include "hip.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <sstream>

#define THRE 30

void hip()
{
  // if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  // Initialize N tuple
  int NT[nenties][3]; // NT for N tuple
  for ( int i = 0; i < nenties; i++ ) {
    for ( int j = 0; j < 3; j++ ) {
      NT[i][j] = 0;
    }
  }

  // 1mm MPPC i.e. NT[nevents][0]
  int nevent; // n-th event
  for ( nevent = 0; nevent < nentires; nevent++ ) {
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
    // Start of Additional Lines

    // Declearation
    int ch = 0;
    double adc3[16];

    // Initialize the array
    for ( ch = 0; ch < 16; ch++ ){
      adc3[ch] = 0;
    }

    // Threshold
    cout << "Showing adc3 if adc2 values more than the threshold" << endl;
    for ( ch = 0; ch < 16; ch++ ) {
      if ( adc2[ch] > THRE ) {
        // Here we use "THRE" defined at the 7th line
        adc3[ch] = adc[ch]; // "adc2[ch] > THRE" OR "adc[ch] > THRE" ?
        cout << "> ch=" << ch << " " << "adc3=" << adc[ch] << endl;
      }
    }

    // Integration
    double SUM[8];
    for ( int j = 0; j < 8; j++ ) {
      SUM[j] = 0;
    }

    int pe = 0; // "pe" for peak
    for ( ch = 0; ch < 16; ch++ ) {
      if ( ch == 0 && adc2[ch] > THRE ) {
        SUM[pe] = SUM[pe] + adc[ch];
      } else if ( 1 <= ch && ch <= 13 && adc2[ch] > THRE ) {
        SUM[pe] = SUM[pe] + adc[ch];
        if ( ( adc3[ch] > adc3[ch+1] && adc3[ch+1] < adc3[ch+2] ) || ( adc2[ch+1] < THRE ) ) {
          // If the next ch is a valley or if adc3[ch] of the next ch is 0, pe should be plused by one

          // Let's go on to the next peak
          pe++;

          // You can get rid of a valley by the following 3 lines
          if ( adc3[ch] > adc3[ch+1] && adc3[ch+1] < adc3[ch+2] ) {
            ch++;
          }
        }
      } else if ( ( ch == 14 || ch == 15 ) &&  adc2[ch] > THRE ) {
        SUM[pe] = SUM[pe] + adc[ch];;
      }
    }

    // Temporarily Turn Off SUM
    /*
    // Display the SUM[pe] i.e. Intergration
    for ( pe = 0; pe < 8; pe++ ) {
      if ( SUM[pe] != 0 ) {
        if ( pe == 0 ) {
          cout << endl;
          cout << "Peak Intergration" << SUM[0] << endl;
          cout << "> 1st Peak Integration = " << SUM[0] << endl;
        } else if ( pe == 1) {
          cout << "> 2nd Peak Integration = " << SUM[1] << endl;
        } else if ( pe == 2 ) {
          cout << "> 3rd Peak Integration = " << SUM[2] << endl;
        } else {
          cout << "> " << pe << "th Peak Integration = " << SUM[pe] << endl;
        }
      }
    }
    cout << endl;
    // End of Additional Lines


    */
    if ( SUM[1] == 0 && SUM[2] == 0 &&  SUM[6] == 0 && SUM[7] == 0 ) {
      NT[nb][0] = SUM [0]; // Put SUM into N tuple
    } else {
      nevent++; // Go on to the next nb
    }


    /*
    int number_of_mppc = 0;
    const double ADC_THRESHOLD = 0;
    double MAXIMUM = 0;
    int MAXIMUMBIN = 0;

    for ( ch = 0 ; ch < 16 ; ++ch ) {
      if(ADC_THRESHOLD < adc2[ch]) {
        h1_hit->Fill(ch, adc[ch]);
      } else {
        adc2[ch] = 0;
      }
    }

    for (int ch2 = 0 ; ch2 < 16 ; ++ch2) {
      if(MAXIMUM < adc2[ch2]) {
        MAXIMUMBIN = ch2;
        MAXIMUM = adc2[ch2];
      }
    }

    cout << MAXIMUMBIN << " " <<  MAXIMUM << endl;
    MaximumBin = h1_hit->GetMaximumBin();

    cout << MaximumBin << endl ;
    cout << adc[1] << " " << adc[2] << " " << adc[3] << " " << adc[4] << " " << adc[5] << " " << adc[6] << " " << adc[7] << " " << adc[8] << " " << adc[9] << " " << adc[10] << " " << adc[11] << " " << adc[12] << " " << adc[13] << " " << adc[14] << " " << adc[15] << " " << adc[16] << endl;

    //      adcSUM_hist->Fill(adcSUM);
    TCanvas* c1 = new TCanvas();
    h1_hit->Draw();
    // hip.C ABOVE

    if ( SUM[1] == 0 && SUM[2] == 0 &&  SUM[6] == 0 && SUM[7] == 0 ) {
      NT[nb][0] = SUM [0]; // Put SUM into N tuple
    } else {
      nb++; // Go on to the next nb
    }
    */

    cout << "NT[" << nevent << "][_1.root] =" << NT[nevent][0] << endl

  } // N tuple THE END LINE of "for ( nevent = 0; nevent < nentires; nevent++ )"

}
