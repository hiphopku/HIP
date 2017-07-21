/////////////////////////////////////////////////////////////
//////////////   Execute Program like this.   ///////////////
/////////////////////////////////////////////////////////////
// $ gcc csv_to_beta.C -o csv_to_beta
// $ ./csv_to_beta
/////////////////////////////////////////////////////////////
// Remember you should have a BRoaD3 data file (.csv)
// in the current directory.
/////////////////////////////////////////////////////////////


#include <stdio.h>
#include <string.h>

#define theoreticalValue 6.8 // Theoretical Value for light speed time
#define mean 50 // Mean Value of Gaussian FIT by ROOT
#define threshold 1.0


int check(int);  // Converting TDC to Beta


int main(void)
{
  FILE *fpr;
  FILE *fpw_tdc;
  FILE *fpw_beta;

  int ret = 0; //To skip the first line
  int n1 = 0;
  int events = 0;
  int i = 0;
  float f1, f2;

  int runnum = 0;
  char frun[64];
  char ftdc[64];
  char fbeta[64];

  printf("What is the run number?: ");
  scanf("%d", &runnum);
  printf("Run Number: %04d\n", runnum);

  sprintf(frun, "run%04d.csv", runnum);
  // sprintf(ftdc, "tdc%04d.csv", runnum);
  // sprintf(fbeta, "beta%04d.csv", runnum);


  // Counting the events
  fpr = fopen( frun, "r" );
  if( fpr == NULL ){
    printf("The file, '%s', cannot be found.\n", frun );
    return -1;
  }
  printf("The file, '%s', was found.\n", frun);
  char str[16];
  while( ( ret = fscanf( fpr, "%[^,],%d", str, &n1 ) ) != EOF ){
    events++;
  }
  fclose( fpr );
  events = events - 2;


  // Event Number Check
  int evnum = check(events);


  // Initialization
  int tdc[evnum];
  double  beta[evnum];

  for ( i = 0; i < evnum; i++ ) {
    tdc[i] = 0;
    beta[i] = 0;
  }


  // Read the file's contents
  fpr = fopen( frun, "r" );
  i = 0;
  ret = 0;
  while( ( ( ret = fscanf( fpr, "%[^,],%d", str, &n1 ) ) != EOF ) && ( i <= evnum ) ){
    if ( i != 0 ) tdc[i-1] = n1;
    i++;
  }
  fclose( fpr );


  // Exporting TDC
  sprintf(ftdc, "tdc%04d.csv", runnum);
  fpw_tdc = fopen( ftdc, "w" );
  for ( i = 0; i < evnum; i++ ) {
    fprintf( fpw_tdc, "%d\n", tdc[i] );
  }
  fclose ( fpw_tdc );


  // Converting TDC to Beta and Exporting Beta
  sprintf(fbeta, "beta%04d.csv", runnum);
  fpw_beta = fopen( fbeta, "w" );
  for ( i = 0; i < evnum; i++ ) {
    beta[i] = theoreticalValue / ( theoreticalValue + tdc[i] - mean );
    // theoreticalValue = 6.8, mean = 50
    if ( beta[i] > threshold ) {
      beta[i] = 1.000;
    }
    fprintf( fpw_beta, "%f\n", beta[i] );
  }
  fclose ( fpw_beta );


  printf("\n");
  printf("Conversion has been successfully conducted.\n");
  printf("'%s' has been exported.\n", ftdc);
  printf("'%s' has been exported.\n", fbeta);


  return 0;
}



// Function: Event Number Check
int check(int num) {
  int k = 0;
  char q1, buff;
  if ( num < 1 ) {
    printf("Error: Make sure you have one or more events on the file.\n");
    return 0;
  } else if ( num >= 1 ) {
    if ( num == 1 ) {
      printf("Just one event have been detected.\n");
    } else if ( num >= 1 ) {
      printf("%d events have been detected.\n", num);
    }
    for ( int h = 0; h < 100; h++ ) {
      if ( h != 0 ) printf("Are you sure of it? [y|n]\n");
      // I don't why, but if h = 0, "scanf" doesn't work well.
      scanf("%c", &q1); // cf "%c%*[^ \n]"
      // buff = getchar();
      if ( q1 == 'y' || q1 == 'Y' ) {
        printf("The event number has been confirmed.\n");
        h = 100;
        break;
      } else if ( q1 == 'n' || q1 == 'N' ) {
        k = 0;
        for ( int i = 0; i < 100; i++ ) {
          printf("Input the number of events:");
          scanf("%d", &k);
          if ( 1 <= k && k <= num ) {
            num = k;
            printf("Set the event number for %d\n", num);
            h = 100;
            break;
          } else {
            printf("Error: Make sure a new event number should range from 1 to %d\n", num);
          }
        }
      } else {
        if ( h != 0 ) printf("Input y, Y, n or N.\n");
      }
    }
  }


  return num;
}
