#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <sys/time.h>	// para gettimeofday(), struct timeval

#define TEST					    0
#define COPY_PASTE_CALC 	1

#if ! TEST
	#define NBITS 20
	#define SIZE (1<<NBITS)	// tamaño suficiente para tiempo apreciable
	unsigned lista[SIZE];
	#define RESULT (1<<NBITS-1)
#else
/*-----------------------------------------------------------------------------------------------------------------------* /
	#define SIZE 4
	unsigned lista[SIZE] = {0x80000000, 0x00100000, 0x00000800, 0x00000001};
	#define RESULT 4
/*-----------------------------------------------------------------------------------------------------------------------* /
	#define SIZE 8
	unsigned lista[SIZE] = {0x7fffffff, 0xffefffff, 0xfffff7ff, 0xfffffffe, 0x01000024, 0x00356700, 0x8900ac00, 0x00bd00ef};
	#define RESULT 8
/*-----------------------------------------------------------------------------------------------------------------------* /
	#define SIZE 8
	unsigned lista[SIZE] = {0x0, 0x10204080, 0x3590ac06, 0x70b0d0e0, 0xffffffff, 0x12345678, 0x9abcdef0, 0xcafebeef};
	#define RESULT 2
/*-----------------------------------------------------------------------------------------------------------------------*/
#endif
int resultado = 0;

int parity1(unsigned* array, int len){
  int  i, j;
	unsigned x;
	int val, result = 0;

	for (i=0; i<len; i++){
	 	x = array[i];
		val = 0;

		for(j = 0; j < 8 * sizeof(unsigned); j++){
			val ^= x & 0x1;
			x >>= 1;
		}
		result += val;
	}
	return result;

}

int parity2(unsigned* array, int len){
  int  i;
	unsigned x;
	int val, result = 0;

	for (i=0; i<len; i++){
	 	x = array[i];
		val = 0;

		do {
			val ^= x & 0x1;
			x >>= 1;
		} while (x);

		result += val;
	}
	return result;

}

int parity3(unsigned* array, int len){
	int  i;
	unsigned x;
	int val, result = 0;

	for (i=0; i<len; i++){
	 	x = array[i];
		val = 0;

		while(x){
			val ^= x;
			x >>= 1;
		}

		result += val & 0x1;
	}

	return result;
}

int parity4(unsigned* array, int len) {
	int i, k;
	unsigned x;
	int val, result = 0;

	for (i=0; i<len; i++) {
   	val = 0;
   	x = array[i];

		asm(                       "\n"
       "ini4:                 \n\t"
          "xor %[x], %[v]     \n\t"
			    "shr $0x1, %[x]     \n\t"
          "test %[x], %[x]    \n\t"
			    "jnz ini4"
	        : [v] "+r" (val)
          : [x] "r" (x) );

		result += val & 0x1;
	}
	return result;
}

int parity5(unsigned* array, int len) {
	int i, j;
	unsigned x;
	int val, result = 0;

	for (i = 0; i < len; i++) {
   	val = 0;
    	x = array[i];

   	for (j = 16; j >= 1; j /= 2)
          x ^= x >> j;

	  	result += (x & 0x01);
	}
	return result;
}

int parity6(unsigned* array, int len) {
   int i;
   unsigned x;
  	int result = 0;

	for (i = 0; i < len; i++) {
   	x = array[i];

		asm(     "mov    %[x],   %%edx       \n\t"
             "shr    $16,    %%edx       \n\t"
             "xor    %[x],   %%edx       \n\t"
             "xor    %%dh,   %%dl        \n\t"
             "setpo  %%dl                \n\t"
             "movzx  %%dl,   %[x]        \n\t"
             : [x] "+r" (x)
             :
             : "edx");
      result += x;
  }

return result;

}


void crono(int (*func)(), char* msg){
    struct timeval tv1,tv2;	// gettimeofday() secs-usecs
    long           tv_usecs;	// y sus cuentas

    gettimeofday(&tv1,NULL);
    resultado = func(lista, SIZE);
    gettimeofday(&tv2,NULL);

    tv_usecs=(tv2.tv_sec -tv1.tv_sec )*1E6+
             (tv2.tv_usec-tv1.tv_usec);

#if COPY_PASTE_CALC
	printf("%ld" "\n", tv_usecs);
#else
	printf("resultado = %d\t", resultado);
  printf("%s:%9ld us\n", msg, tv_usecs);
#endif

}

int main(){
#if ! TEST
    int i;
    for (i=0; i<SIZE; i++)
	 	lista[i]=i;
#endif

   crono(parity1, "parity1  (lenguaje C -            for)");
   crono(parity2, "parity2  (lenguaje C -     do - while)");
	 crono(parity3, "parity3  (lenguage C -          while)");
	 crono(parity4, "parity4  (Máquina    -          while)");
	 crono(parity5, "parity5  (lenguage C - deplazamientos)");
	 crono(parity6, "parity6  (Máquina    -            for)");

#if ! COPY_PASTE_CALC
    printf("calculado = %d\n", RESULT);
#endif

	exit(0);
}
