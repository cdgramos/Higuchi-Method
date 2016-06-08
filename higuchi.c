#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "auxiliar.h"


#define minValues 16 //mininum records to compute Higuchi Fractal Dimension Algorithm ! Impirical Value



int main(){
    //we start counting values at 1 to avoid divisions by zero in the for loops

	FILE *fp; // general purpose file pointer

	int N = 0; // number of data records

	int i = 0; // general purpose
	int j = 0; // general purpose
    double aux = 0.0; // general purpose

    int mLarge = 0;

	double *x; // store input / raw data
	double *sequence; // treated data

    printf("Opening the raw data file...\n");
    fp = fopen("hurst_050.txt","r");

    printf("Counting number of records...\n");
    N = countRecordsInFile(fp);
    rewind(fp);
    printf( "\tNumber of records: %d\n",N);
    if(N <= minValues){
    	printf( "\t\tError: Insuficient data\n");
    	exit(1);
    }


    printf("Allocating memory...\n");
    sequence = (double *) calloc(N+1, sizeof(double));
    x = (double *) calloc(N+1, sizeof(double));

    if(sequence == NULL || x == NULL){
        printf( "\t\tError: allocating memory\n");
        exit(1);
    }

    printf("Reading raw data and closing the raw data file...\n");
    i = 1;
    while(i <= N){
        fscanf(fp,"%lf\n",&x[i]);
        i++;
    }

    printf("Cumulative sums...\n");
    for(i=1; i<=N; i++){
        aux = 0.0;
        for(j=1; j<=i; j++){
            aux+=x[j];
        }
        sequence[i] = aux;
    }

    mLarge = floor(N/5); //5 is an empirical value!

    printf("Freeing memory...\n");
    free(sequence);
    fclose(fp);

    printf("The end...\n");

	return 0;
}
