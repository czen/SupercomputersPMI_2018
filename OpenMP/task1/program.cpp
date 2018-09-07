#include <stdio.h>
int main(){
#ifdef _OPENMP
 printf("OpenMP is supported!\n");
#endif
} 