#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "auxiliar.h"

#define minValues 16 //mininum records to compute Higuchi Fractal Dimension Algorithm ! Empirical Value

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
    double aux1 = 0.0; // general purpose

    int mlarge = 0;

    double *x; // store input / raw data
    double *sequence; // treated data

    double logMin = 0.0; // logarithmic scale purpose
    double logMax = 0.0; // logarithmic scale purpose
    int logBins = 0; // logarithmic scale purpose
    double delta = 0.0; // logarithmic scale purpose
    double accDelta = 0; // logarithmic scale purpose

    int *M; // store unique logarithmic spaced box sizes
    double *MM;
    int n = 0; // total number of unique box sizes
    int m = 0;

    int cutMin = 0;
    int cutMax = 0;

    double *curveLength;
    double *CL;
    double **temp_lenght;

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

    M = (int *) calloc(n+1, sizeof(int));
    MM = (int *) calloc(n+1, sizeof(int));
    if(M == NULL || MM == NULL){
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
            printf("%d \t %.4lf\n",acc, M[acc]);
            acc++;
        }
    }

    printf("Defining boundaries and allocating memory...\n");
    cutMin = ceil(n/2);
    cutMax = floor(6*n/10);
    printf("\tMin = %d \t Max = %d\n",cutMin,cutMax);

    curveLength = (double *) calloc(n+1, sizeof(double));
    CL = (double *) calloc(n+1, sizeof(double));

    if(curveLength == NULL || CL == NULL){
        printf( "\t\tError: allocating memory\n");
        exit(1);
    }

    printf("Starting the core calculations...\n");
    m = M[1];
    k = floor((N-m)/m);
    m = M[n];

    // Creating the biggest matrix possible
    temp_lenght = malloc( (m+2) * sizeof(double *));

    for(i=0; i <= m+1; i++){
        temp_lenght[i] = malloc( (k+2) * sizeof(double));
    }

    for(h=1; h<=n; h++){
        m = M[h];
        k = floor((N-m)/m);
        aux = 0;
        for(i=1; i<=m; i++){
            aux1 = 0;
            for(j=1; j<=k; j++){
                temp_lenght[i][j] = abs(sequence[i+j*m] - sequence[i+(j-1)*m]);
                aux1 += temp_lenght[i][j];
            }

            aux1 /= k;
            aux += aux1;

        }

        curveLength[h] = aux * ((N-1)/pow(m,3));
    }
    printf("step...4\n");
    printf("Log x Log Plot...\n");
    curveLength[1] = 6.5;
    for(i=1; i<=n; i++){
        MM[i] = log(M[i]);
        CL[i] = log(curveLength[i]);
        if(i>1) curveLength[i] = curveLength[i-1]-0.3;
    }


    printf("Linear Regression ...\n");
    reg_Linear r;
    r = reg_LeastSquareMeans(MM,CL,cutMin,cutMax);

    printf("\n_______________________________\n\n");
    printf( "\tHurst = %.4lf\n_______________________________\n\n",2+r.b);

    printf("Freeing memory...\n");
    free(temp_lenght);
    free(sequence);
    free(x);
    free(curveLength);
    free(M);
    fclose(fp);

    printf("The end...\n");

	return 0;
}
