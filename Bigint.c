#include "Bigint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//header funcoes:
BigInt neg( BigInt a );
BigInt add( BigInt a, BigInt b );
BigInt sub( BigInt a, BigInt b );
BigInt init( char * str );
void print( BigInt a );
BigInt mul( BigInt a, BigInt b );
BigInt shl( BigInt a, int n);
BigInt shr( BigInt a, int n);

//funcoes de auxilio:
//retorna a posicao do ultimo caracter nao nulo
int len_Big(BigInt a);
// multiplica numero (num) com vetor(pot) base 10 
//o resultado e armazenado na array como vetor base 10
void multiplica(char *pot,char *array,int num);
//soma dois vetores
void soma_str(char *str1,char *str2,int n);


BigInt neg( BigInt a ){
	BigInt neg={{}};
	//inversão dos bits byte a byte
	for(int i=0;i<128;i++)
		neg.x[i]=~a.x[i];
		
	int j=0,carry;
	//soma 1 ao byte com bits invertido 
	do{
		carry=0;
		if(++neg.x[j++]>255) carry=1;	
	}while(carry && j<128);
	
	return neg;
}

BigInt add( BigInt a, BigInt b ){
	BigInt soma={{}};

	int carry=0;
	//soma byte a byte caso o valor ultrapasse o limite
	//soma 1 pra proxima soma entre bytes
	for(int i=0;i<128;i++){
		soma.x[i]= a.x[i] + b.x[i] + carry;
		
		if(a.x[i] + b.x[i] + carry>255) carry=1;
		else carry=0;
	}
	return soma;
}

BigInt sub( BigInt a, BigInt b){
	return add(a,neg(b));
}

BigInt init( char *str ){
	//o dividendo assume o valor do numero dado
	char dividendo[310]={'0'};
	strcpy(dividendo,str);
	
	//declaracao do big int que recebera os valores 
	//convertidos para a base 256
	BigInt bigint={{}};
	
    int i=0;

	//loop de conversao
	for(;;){
		//armazena o quociente em uma string	
		char quociente[310]={'0'};
		
		//caso o numero n seja divisivel por 256
		//concluimos que ele e o resto que faltava add no 
		//vetor bigint de base 256 e finaliza o loop
		if(strlen(dividendo)<4){
			if(atoi(dividendo)<256){
				bigint.x[i]=atoi(dividendo);
				break;
			}
		}
		
		//indice para o dividendo
		int indx=0;
		//temp == valor temporario
		int temp= dividendo[indx++]-'0';
		
		// encontra o prefixo do numero que 
		// e maior que 256
		while(temp<256){
			temp = (temp*10) + (dividendo[indx++]-'0');
		}
		
		//indice para o quociente
		int y=0;
	
		while(strlen(dividendo)>indx){
			//armazena o resultado no vetor resposta
			quociente[y++]= temp/256 + '0';
			//pega o proximo digito do numero
			temp=(temp % 256)*10 + (dividendo[indx++] -'0'); 
		}
		quociente[y] = (temp / 256) + '0'; 
		
		//armazena o resto da divisao
		bigint.x[i++] = temp % 256;
		
		//o quociente passa a ser o novo dividendo
		strcpy(dividendo,quociente);
	}
	
	return bigint;
}

int len_Big(BigInt a){
    int tam=0;
    //o caractere diferente de '\0' marca o fim 
    for(int i=127;i>=0;--i){
		if(a.x[i]!=0){
				tam=i;
				break;
		}
	}
	//retorna a posicao do ultimo caracter nao nulo
	return tam;
}

void multiplica(char *pot,char *array,int num){
    int carry=0;
    char copia[310]={'0'};
    
    int y;
    //multiplicacao basica
	//num é pequeno,logo multiplicamos ele com cd casa da array
	//armazenamos o resto do resultado por 10 de cada multiplicação
	//e damos ao carry o resultado/10
    for(y=0;y<strlen(pot);++y){
        
        int val=num*(pot[y]-'0')+carry;
        copia[y]=(val%10)+'0';
        carry=val/10;
    }
    //add oq restou do carry
    if(carry!=0){
        do{
            copia[y++]=(carry%10)+'0';
            carry = carry/10;
        }while(carry);
    }
    
    //array pega o valor da copia
    strcpy(array,copia);
}

void soma_str(char *str1,char *str2,int n){
    char soma[310]={'0'};
    
    int len1=strlen(str1),len2=strlen(str2);
    
    //soma basica, soma dois numeros na mesma casa decimal
    //anotamos o resto da soma por 10
    //guardamos um carry com soma/10
    int carry=0;
    int sum;
    int x;
    for(x=0;x<len1;++x){
        sum=(str1[x]-'0')+(str2[x]-'0')+carry;
        soma[x]=(sum%10)+'0';
        carry=(sum/10);
    }
    //add os digitos restantes
    for(;x<len2;++x){
        sum=(str2[x]-'0')+carry;
        soma[x]=(sum%10)+'0';
        carry=(sum/10);
    }
    //add o restante do carry
    if(carry>0)
        soma[x]=carry+'0';
       
    //copia o valor da soma    
    if(n==1)
        strcpy(str2,soma);
    else
        strcpy(str1,soma);            
}


void print( BigInt a ){	
    //declaro array para as potencias de 256 e para o numero na base 10
    char pot_256[310]={'1'}; 
    char base_10[310]={'0'};
	
    //iniciamos base_10 
    if(a.x[0]<100){
        base_10[1]=(a.x[0]/10)+'0';
        base_10[0]=(a.x[0]%10)+'0';
    }
    else{
        base_10[2]=(a.x[0]/100)+'0';
        base_10[1]=((a.x[0]%100)/10)+'0';
        base_10[0]=((a.x[0]%100)%10)+'0';   
    }
    
    //len
    int tam = len_Big(a); 
    
    for(int indx=1;indx<=tam;++indx){
		
		char copia[310]={'0'};

		//gera 256^(indx)
		multiplica(pot_256,pot_256,256);
		
		//multiplica a potencia pelo numero a.x[indx]
		multiplica(pot_256,copia,a.x[indx]);
		
		//a copia e somado ao resultado da base_10
		//para o exito da funcao soma_str
		//devemos colocar o menor vetor na primeira posicao
		int val=strlen(copia)<strlen(base_10)?1:-1;
		
		if(val==1)
			soma_str(copia,base_10,val);
		else
			soma_str(base_10,copia,val);
				
    }
	for(int x=strlen(base_10)-1;x>=0;--x)
		printf("%c",base_10[x]);
	printf("\n");
}

BigInt mul( BigInt a, BigInt b ){
	BigInt mult={{}};

	int tam1=len_Big(a),tam2=len_Big(b);
	//carry e index da copia
	int carry,z;
	
	for(int x=0;x<=tam1;++x){
		BigInt copia={{}};
		//z comeca na posicao x	
		z=x;
		carry=0;
		for(int y=0;y<=tam2;++y){
			//pegamos o resto
			copia.x[z++] = a.x[x]*b.x[y]+carry;
			//atribuimos ao carry o quociente
			carry=(a.x[x]*b.x[y]+carry)/256;	
		}
		copia.x[z]=carry;
		
		//mult recebe a soma 
		mult = add(copia,mult);
	}
	return mult;
}

//todas abaixo erradas
BigInt shl( BigInt a, int n){
	BigInt left=a;
	
	int carry,val;
	for(int y=0;y<n;++y){
		carry=0;
		for (int z=0;z < 128; z++){
			val = left.x[z];

			left.x[z] = (left.x[z] << 1) + carry;
					
			//se numero for maior q 127,carrega 1 para o proximo byte
			//pois e da forma 1XXXXXXX, sendo X igual a 0 ou 1	
			if(val>127) carry=1;
			
			else carry = 0;
		}
	}
	return left;
}

// Shift Right, ou seja calcular (a >> n)
BigInt shr( BigInt a, int n){
	BigInt right=a;
	
	int carry,val;
	for(int y=0;y<n;++y){
		carry=0;
		for (int z=127;z>=0; z--){
			val = right.x[z];
			
			right.x[z] = (right.x[z] >> 1) + carry;
			
			//se numero for impar, carrega 128 para o proximo byte
			//pois e da forma XXXXXXX1, sendo X igual a 0 ou 1		
			if((val&1)==1) carry=128;
			
			else carry = 0;
		}
	}
	return right;
}
