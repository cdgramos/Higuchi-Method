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
	int h = 0; // general purpose
	int k = 0; // general purpose
	int acc = 0; // general purpose
	int val = 0; // general purpose
    double aux = 0.0; // general purpose

    int mlarge = 0;

	double *x; // store input / raw data
	double *sequence; // treated data

    double logMin = 0.0; // logarithmic scale purpose
    double logMax = 0.0; // logarithmic scale purpose
    int logBins = 0; // logarithmic scale purpose
    double delta = 0.0; // logarithmic scale purpose
    double accDelta = 0; // logarithmic scale purpose

    double *M; // store unique logarithmic spaced box sizes
    int n = 0; // total number of unique box sizes

    int cutMin = 0;
    int cutMax = 0;

    double *curveLength;

    printf("Opening the raw data file...\n");
    fp = fopen("hurst_050.txt","r");

    printf("Counting number of records...\n");
    N = countRecordsInFile(fp);
    rewind(fp);
    printf( "\tNumber of records: %d\n",N);
    if(N <= minValues){
    	printf( "\t\tError: insufficient data\n");
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

    mlarge = floor(N/5); //5 is an empirical value!
    printf("\tmlarge = %d\n",mlarge);

    printf("Creating a logarithmically scale...\n");
    logMin = log10(1);
    logMax = log10(mlarge);
    logBins = 50; //empirical value
    delta = (logMax - logMin) / logBins;
    accDelta = 0;
    val = 0;
    j = -1;
    acc = 0;

    // Checking how many unique values we have
    for (i = 1; i <= logBins; ++i){
        accDelta += delta;
        val = (int)((pow(10, logMin + accDelta)));
        if(i==logBins) val = mlarge;
        if(j!=val && val>1){
            j = val;
            acc++;
        }
        printf("%d \t %d\n",i, val);
    }
    n = acc;
    printf("\nUnique values = %d\n",acc);

    M = (double *) calloc(n+1, sizeof(double));
    if(M == NULL){
        printf( "\t\tError: allocating memory\n");
        exit(1);
    }


    j = -1;
    accDelta = 0;
    acc = 1;
    // Creating the array of unique box sizes
    for (i = 1; i <= logBins; ++i){
        accDelta += delta;// accDelta = delta * i
        val = (int)((pow(10, logMin + accDelta)));
        if(i==logBins) val = mlarge;

        if(j!=val && val>1){
            j = val;
            M[acc] = val;
            printf("%d \t %d\n",acc, val);
            acc++;
        }
    }


    printf("Defining boundaries and allocating memory...\n");
    cutMin = ceil(n/10);
    cutMax = floor(6*n/10);
    printf("\tMin = %d \t Max = %d\n",cutMin,cutMax);

    curveLength = (double *) calloc(n+1, sizeof(double));
    if(curveLength == NULL){
        printf( "\t\tError: allocating memory\n");
        exit(1);
    }



    printf("Freeing memory...\n");
    free(sequence);
    free(x);
    free(curveLength);
    free(M);
    fclose(fp);

    printf("The end...\n");

	return 0;
}
