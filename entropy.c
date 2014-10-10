#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <sys/mman.h>
 
#define MAXLEN 20UL*1024*1024 //maximum string length
 
void makehist(unsigned char *S,int *charfreq,int len){
	int i = 0;
	for(i=0;i<256;i++)charfreq[i]=0;
	for(i=0;i<len;i++) charfreq[S[i]]++;
}
 
double entropy(unsigned char* S,int *charfreq,int len){
	int i;
	double H = 0.0;
	for(i=0;i<len;i++){
                double z = ((double)charfreq[S[i]]/len)*(log2((double)charfreq[S[i]]/len));
                if (isnan(z)) z = 0.0;
		H-=z;
	}
	return H;
}
 
int main(void){
	unsigned char* S = (unsigned char*)mmap(NULL, MAXLEN, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_HUGETLB, -1, 0);
        if (S == MAP_FAILED) {
            fprintf(stderr, "Could not allocate!\n");
            return 0;
        }
	int len = 0;
	while (!feof(stdin) && len<MAXLEN) {
            int b = fgetc(stdin);
            if (b == EOF) break;
            S[len] = (unsigned char)b;
            len++;
        }
	int charfreq[256];
	makehist(S,charfreq,len);
	double H=entropy(S,charfreq,len);
	printf("Total: %lf\n",H);
        printf(" Rate: %lf\n",H/len);
	return 0;
}
