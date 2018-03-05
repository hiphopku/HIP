// NTupleConverter.c
// Here's how to use this file.
// After you edit it, you have to run the following commands on a terminal.
//////////////////////////////////////////////////
//  $ gcc NTupleConverter.c -o NTupleConverter  //
//  $ ./NTupleConverter                         //
//////////////////////////////////////////////////
// Then you will find an output file you give a name.

// beta - ADC Graph Converter


#include <stdio.h>

int main(void)
{
  // 1 for Delta Easiroc: _1
  // 2 for Gamma Easiroc: _2
  // 3 for TDC BRoaD3

  FILE *fpr1, *fpr2, *fpr3; // For Reading Function
  FILE *fpw; // For Writing Function

  char filename1[256] = "run0196_1.csv"; // bin - ADC .csv File
  // char filename2[256] = "run0196_2.csv";
  char filename3[256] = "run0192.csv"; // TDC Data File
  char filenamew[256] = "ntuple_run0196.csv"; // Output File Name

  int ret1, ret2, ret3;
  int n0;
  double n1; // For Delta Integration
  // int m0, m1, m2, m3, m4, m5, m6, m7; // For Gamma Integration
  int nevents1 = 0, nv = 0; //nv for total events
  int i = 0, j = 0;




  // Start Scanning Delta Data: _1.csv
  fpr1 = fopen( filename1, "r" );
  if( fpr1 == NULL ){
    printf( "%s ファイルが開けません¥n", filename1 );
    return -1;
  }
  while( ( ret1 = fscanf( fpr1, "%d,%lf", &n0, &n1 ) ) != EOF ){
    nevents1++;
  }
  fclose( fpr1 );

  // Declare & Initialize Delta Integraton Array
  double eas1[nevents1][2];
  for ( i = 0; i < nevents1; i++ ) {
    for ( j = 0; j < 2; j++ )
    eas1[i][j] = 0.00;
  }

  // Read the file's contents
  fpr1 = fopen( filename1, "r" );
  i = 0;
  ret1 = 0;
  while( ( ( ret1 = fscanf( fpr1, "%lf,%lf", &eas1[i][0], &eas1[i][1] ) ) != EOF ) && ( i < nevents1 ) ) {
    i++;
  }
  fclose( fpr1 );
  // Finish Scanning Gamma Data: _1.csv
  fpr1 = NULL;


  nv = nevents1;


  // Start Scanning TDC Data: .csv
  int tdc[nv];
  double  bt[nv];
  // Initialize tdc & beta Arrays
  for ( i = 0; i < nv; i++ ) {
    tdc[i] = 0;
    bt[i] = 0.00;
  }

  fpr3 = fopen( filename3, "r" );
  if( fpr3 == NULL ){
    printf( "%s ファイルが開けません¥n", filename3 );
    return -1;
  }
  i = 0;
  while( ( ( ret3 = fscanf( fpr3, "%d", &tdc[i] ) ) != EOF ) && ( i < nv - 1 )){
    i++;
  }
  fclose( fpr3 );

  i = 0;
  for ( i = 0; i < nv; i++ ) {
    fscanf( fpr3, "%d\n", &tdc[i] );
    bt[i] = 6.8 / ( 6.8 + tdc[i] - 50 );
    if ( bt[i] > 1.86 ) {
      bt[i] = -1.00;
    }
  }
  // Finish Scanning TDC Data: .csv




  // Display the scanned data
  // Display _1.csv
  printf("Here's the Scanning Result of Delta Easiroc: %s\n\n", filename1 );
  printf("Event\t bin\t ADC\n");
  // 1st to 3rd
  if ( nv >= 0 ) {
    i = 0;
    printf("1st\t %f\t %f\n", eas1[i][0], eas1[i][1] );
  }
  if ( nv >= 1 ) {
    i = 1;
    printf("2nd\t %f\t %f\n", eas1[i][0], eas1[i][1] );
  }
  if ( nv >= 2 ) {
    i = 2;
    printf("3rd\t %f\t %f\n", eas1[i][0], eas1[i][1] );
  }
  // From 4th
  if (nv >= 3) {
    for ( i = 3; i < nv; i++ ) {
      printf("%dth\t %f\t %f\n", i + 1, eas1[i][0], eas1[i][1] );
    }
  }
  printf("\n\n");

  // Display TDC
  printf("Here's the Scanning Result of TDC file: %s\n\n", filename3 );
  printf("Event\t bin\t beta\n");
  // 1st to 3rd
  if ( nv >= 0 ) {
    i = 0;
    printf("1st\t %d\t %f\n", tdc[i], bt[i] );
  }
  if ( nv >= 1 ) {
    i = 1;
    printf("2nd\t %d\t %f\n", tdc[i], bt[i] );
  }
  if ( nv >= 2 ) {
    i = 2;
    printf("3rd\t %d\t %f\n", tdc[i], bt[i] );
  }
  // From 4th
  if (nv >= 3) {
    for ( i = 3; i < nv; i++ ) {
      printf("%dth\t %d\t %f\n", i + 1, tdc[i], bt[i] );
    }
  }
  printf("\n\n");
  // Display End




  // Converting N Tuple file
  fpw = fopen( filenamew, "w" );
  for ( i = 0; i < nv; i++ ) {
    fprintf( fpw, "%f %d %f\n", bt[i], (int)eas1[i][0], eas1[i][1] );
  }
  fclose ( fpw );




  // 8 SUMs of Delta & 8 SUMs of Gamma & TDC[ns] & beta
  printf("Conversion has been successfully conducted.\n");
  printf("Check out the output file: %s\n\n", filenamew );
  return 0;
}
