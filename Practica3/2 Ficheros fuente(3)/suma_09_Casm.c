//  según la versión de gcc y opciones de optimización usadas, tal vez haga falta
//  usar gcc –fno-omit-frame-pointer si gcc quitara el marco pila (%ebp)

#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <sys/time.h>	// para gettimeofday(), struct timeval

#define TEST 0
#define COPY_PASTE_CALC 1

#if ! TEST
	#define NBITS 20
	#define SIZE (1<<NBITS)
	unsigned lista[SIZE];
	#define RESULT 10495760
#else
	#define SIZE 8
	unsigned lista[SIZE]={0x7fffffff, 0xffefffff, 0xfffff7ff, 0xfffffffe, 0x01000024, 0x00356700, 0x8900ac00, 0x00bd00ef}; 
	
#endif
#define WSIZE 8*sizeof(int)
int resultado=0;

/*
int suma1(int* array, int len)
{
    int  i,   res=0;
    for (i=0; i<len; i++)
	  res += array[i];
    return res;
}
*/


//----------------------------------------------------------

//Version 1
int v1(unsigned* x) {
	int i, j;
	unsigned result;
	int result_total=0;
	for (i = 0; i < SIZE; i++) {
		result= x[i];
		for (j = 0; j < WSIZE; j++) {
			result_total+= result & 0x1;
			result>>=1;
		}
	}
	return result_total;
}

//----------------------------------------------------------


//Version 2

int v2(unsigned* x) {
	int i, j;
	unsigned result;
	int result_total=0;
	for (i = 0; i < SIZE; i++) {
		result= x[i];
		do{
			result_total+= result & 0x1;
			result>>=1;
		}while(result);
	}
	return result_total;
}

//----------------------------------------------------------

//Version 3
int v3(int* array, int len)
{
    int  i;
	unsigned result;
	int result_total=0;
    for (i=0; i<len; i++){
		result= array[i];
		asm("\n"
			"bucle1:	\n\t"
			"shr %[x] \n\t"
			"adc $0, %[r] \n\t"
			"test %[x], %[x]\n\t"
			"jne bucle1 \n\t"
			: [r] "+r" (result_total)
			: [x] "r" (result)
		);
	}
    return result_total;
}

//----------------------------------------------------------
//Version 4
int v4(unsigned* x, int len) {
	int val1,val2;
	int i,j;
	int total=0;
	for(j=0; j<len; j++){
		val2=x[j];
		val1=0;
		for (i = 0; i < 8; i++) {
			val1 += val2 & 0x01010101;
			val2 >>= 1;
		}
		val1 += (val1 >> 16);
		val1 += (val1 >> 8);
		total+= (val1 & 0xFF);
	}
	return total;
}
//----------------------------------------------------------
//Version 5

int v5(unsigned* array, int len){
    int i;
    int val, result=0;
    int SSE_mask[] = {0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f};
    int SSE_LUTb[] = {0x02010100, 0x03020201, 0x03020201, 0x04030302};
    if(len & 0x3) printf("leyendo 128b pero len no multiplo de 4?\n");
    for(i=0; i<len; i+=4){
        asm("movdqu		%[x],		%%xmm0    \n\t"   
            "movdqa 	%%xmm0,		%%xmm1    \n\t"
            "movdqu     %[m],		%%xmm6    \n\t"
            "psrlw      $4,    		%%xmm1    \n\t"
            "pand    	%%xmm6,    	%%xmm0    \n\t"
            "pand   	%%xmm6, 	%%xmm1    \n\t"
           
            "movdqu     %[l],		%%xmm2    \n\t"
        	"movdqa 	%%xmm2, 	%%xmm3    \n\t"
            "pshufb 	%%xmm0, 	%%xmm2    \n\t"
            "pshufb 	%%xmm1, 	%%xmm3    \n\t"
           
            "paddb    	%%xmm2, 	%%xmm3    \n\t"
            "pxor    	%%xmm0, 	%%xmm0    \n\t"
            "psadbw    	%%xmm0, 	%%xmm3    \n\t"
            "movhlps 	%%xmm3,		%%xmm0    \n\t"
            "paddd    	%%xmm3,		%%xmm0    \n\t"
            "movd    	%%xmm0, 	%[val]    \n\t"
            : [val]"=r" (val)
            :    [x] "m" (array[i]),
                [m] "m" (SSE_mask[0]),
                [l] "m" (SSE_LUTb[0])
        );
    result+= val;
	}
	return result; }

//----------------------------------------------------------
//Ejercicio 4.2
/*
//#define SIZE 4
#define WSIZE 8*sizeof(int)
//unsigned lista[SIZE]={0x80000000, 0x00100000, 0x00000800, 0x00000001};
#define SIZE 8
unsigned int lista[SIZE]={0x7fffffff,0xffefffff,0xfffff7ff,0xfffffffe,0x01000024,0x00356700,0x8900ac00,0x00bd00ef};
#define SIZE 8
unsigned lista[SIZE]={0x0,0x10204080,0x3590ac06,0x70b0d0e0,0xffffffff,0x12345678,0x9abcdef0,0xcafebeef};
*/
/*
#define TEST 0
#define COPY_PASTE_CALC 1

#if ! TEST
	#define NBITS 20
	#define SIZE (1<<NBITS)
	unsigned lista[SIZE];
	#define RESULT 10495760
#else
	#define SIZE 8
	unsigned lista[SIZE]={0x0,0x10204080,0x3590ac06,0x70b0d0e0,0xffffffff,0x12345678,0x9abcdef0,0xcafebeef}; 
	
#endif
#define WSIZE 8*sizeof(int)
int resultado=0;

int version1(unsigned* x) {
	int i, j;
	unsigned elem;
	int xor;
	int result_total=0;
	for (i = 0; i < SIZE; i++) {
		elem= x[i];
		xor=0;
		for (j = 0; j < WSIZE; j++) {
			xor^= (elem & 0x1);
			elem>>=1;
		} 
        result_total+= xor;

	}
	return result_total;
}

int version2(unsigned* array){
	int i;
	unsigned elemento;
	int result_total=0, xor;
	for (i = 0; i < SIZE; i++) {
		elemento= array[i];
		xor=0;
		do{
			xor^= elemento & 0x1;
			elemento>>=1;
		}while(elemento);
		result_total+= xor;
	}
	return result_total;
}

int version3(unsigned *array){
	int i;
	unsigned elemento;
	int result_total=0, xor;
	
	for(i=0; i<SIZE;i++){
		elemento=array[i];
		xor = 0;
		while (elemento) {
			xor ^= elemento;
			elemento >>= 1;
		}
		result_total+=(xor & 0x1);
	}
	return result_total;
}

int version4(unsigned *array){
	int i;
	unsigned elemento;
	int result_total=0, val;

	for(i=0; i<SIZE; i++){
		elemento=array[i];
		val=0;

		asm("\n"
		"ini3:			\n\t"
			"XOR %[x], %[v] \n\t"
			"SHR %[x] \n\t"
			"JNZ ini3 \n\t"
			"AND $1, %[v] \n\t"
			: [v] "+r" (val)
			: [x] "r" (elemento)
		);
		result_total+=val;
	}
	return result_total;
}

int version5(unsigned *array){
	int i,j,resultado=0;
	unsigned elemento;
	for(i=0; i<SIZE; i++){
		elemento= array[i];
		for(j=16; j==1; j/=2){
			elemento^=(elemento>>j);
		}
		resultado+=(elemento & 0x01);
	}
	return resultado;
}
*/
/*
int suma3(int* array, int len)
{
    asm("mov 8(%%ebp), %%ebx	\n"  // array
"	mov 12(%%ebp), %%ecx	\n"  // len
"				\n"
"	mov $0, %%eax		\n"  // "bucle:				\n"

"	mov $0, %%edx		\n"  // index
"	add (%%ebx,%%edx,4),%%eax\n"
"	inc       %%edx		\n"
"	cmp %%edx,%%ecx		\n"
"	jne bucle		\n"
     : 				// output
     : 				// input
     :	"ebx"			// clobber
    );
}
*/
void crono(int (*func)(), char* msg){
    struct timeval tv1,tv2;	// gettimeofday() secs-usecs
    long           tv_usecs;	// y sus cuentas

    gettimeofday(&tv1,NULL);
    resultado = func(lista, SIZE);
    gettimeofday(&tv2,NULL);

    tv_usecs=(tv2.tv_sec -tv1.tv_sec )*1E6+
             (tv2.tv_usec-tv1.tv_usec);
	printf("%9ld\t", tv_usecs);
    printf("%s:%d re\n",msg, resultado);
	
    
}

int main()
{
#if ! TEST
    int i;			// inicializar array
    for (i=0; i<SIZE; i++)	// se queda en cache
		lista[i]=i;
#endif
	
	//-------------------------Ejercicio 1---------------------------------------
    crono(v1, "version1 (en lenguaje C    )");
	crono(v2, "version2 ");
    crono(v3, "version3 (1 instrucción asm)");
	crono(v4, "version4 ");
	crono(v5, "version5 ");

	#if !COPY_PASTE_CALC
		printf("calculado= %d\n", RESULT);
	#endif
		exit(0);

	//-------------------------Ejercicio 2---------------------------------------
/*
	crono(version1, "version 1");
	crono(version2, "version 2");
	crono(version3, "version 3");
	crono(version4, "version 4");
	crono(version5, "version 5");
*/
  //  crono(suma3, "suma3 (bloque asm entero)");
    printf("N*(N+1)/2 = %d\n", (SIZE-1)*(SIZE/2)); /*OF*/

    exit(0);
}
