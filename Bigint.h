#ifndef BIGINT_H
#define BIGINT_H

typedef struct _BigInt {
	unsigned char x[128];
} BigInt;

// returna '-a' (inverte todos os bits e soma 1)
BigInt neg( BigInt a );

// Soma dois BigInt
BigInt add( BigInt a, BigInt b );

// Subtrai dois BigInt.
//para o exito da funcao a>=b
BigInt sub( BigInt a, BigInt b );

//// Converte uma string em base 10 para BigInt
BigInt init( char * str );

// Imprime em base 10
void print( BigInt a );

// Multiplica dois BigInt.
BigInt mul( BigInt a, BigInt b );

// Shift Left, ou seja calcula (a << n)
BigInt shl( BigInt a, int n);

// Shift Right, ou seja calcula (a >> n)
BigInt shr( BigInt a, int n);

#endif

