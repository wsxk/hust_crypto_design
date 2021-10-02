#include <stdio.h>
#include<string.h>
#include<map>
#include<algorithm>
#include<iterator>

#define SPNBYTES (8)
#define INPUTBYTES (1<<24)

using namespace std;

unsigned long long substitution[16] = { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 } ;
unsigned long long permutationArr[64];
int permutationPos[64] = {0,16,32,48,1,17,33,49,2,18,34,50,3,19,35,51,4,20,36,52,5,21,37,53,6,22,38,54,7,23,39,55,8,24,40,56,9,25,41,57,10,26,42,58,11,27,43,59,12,28,44,60,13,29,45,61,14,30,46,62,15,31,47,63};

unsigned long long init_key[2];
unsigned long long round_key[5];
unsigned long long IV = 20001009;
unsigned long long plain_text[1];
unsigned long long cipher_text[1];

void key_gen(){
    round_key[0]  = init_key[0];
    round_key[1]  = ((init_key[0]<<16)&0xffffffffffff0000)|((init_key[1]>>48)&0x000000000000ffff);
    round_key[2]  = ((init_key[0]<<32)&0xffffffff00000000)|((init_key[1]>>32)&0x00000000ffffffff);
    round_key[3]  = ((init_key[0]<<48)&0xffff000000000000)|((init_key[1]>>16)&0x0000ffffffffffff);
    round_key[4]  = init_key[1];
}

unsigned long long permutation(unsigned long long plaintext) {
	unsigned long long mask = 1;
	for (int i = 0; i < 64; i++) {
		permutationArr[i] = ((plaintext & mask) == 0? 0 : 1);
		mask <<= 1;
	}
	plaintext = 0;
	for (int i = 0; i < 64; i++) {
		plaintext ^= (permutationArr[permutationPos[i]]<<i);
	}
	return plaintext;
}

unsigned long long SPN(unsigned long long plaintext){
    plaintext = plaintext^IV;
    for(int i=0;i<3;i++){
        plaintext ^= round_key[i];
		plaintext = substitution[plaintext & 0xf] |
			((substitution[(plaintext & 0xf0) >> 4]) << 4) |
			((substitution[(plaintext & 0xf00) >> 8]) << 8) |
			((substitution[(plaintext & 0xf000) >> 12]) << 12) |
			((substitution[(plaintext & 0xf0000) >> 16]) << 16) |
			((substitution[(plaintext & 0xf00000) >> 20]) << 20) |
			((substitution[(plaintext & 0xf000000) >> 24]) << 24) |
			((substitution[(plaintext & 0xf0000000) >> 28]) << 28) |
			((substitution[(plaintext & 0xf00000000) >> 32]) << 32) |
			((substitution[(plaintext & 0xf000000000) >> 36]) << 36) |
			((substitution[(plaintext & 0xf0000000000) >> 40]) << 40) |
			((substitution[(plaintext & 0xf00000000000) >> 44]) << 44) |
			((substitution[(plaintext & 0xf000000000000) >> 48]) << 48) |
			((substitution[(plaintext & 0xf0000000000000) >> 52]) << 52) |
			((substitution[(plaintext & 0xf00000000000000) >> 56]) << 56) |
			((substitution[(plaintext & 0xf000000000000000) >> 60]) << 60);
        plaintext=permutation(plaintext);
    }
    plaintext ^=round_key[3];
    plaintext = substitution[plaintext & 0xf] |
        ((substitution[(plaintext & 0xf0) >> 4]) << 4) |
        ((substitution[(plaintext & 0xf00) >> 8]) << 8) |
        ((substitution[(plaintext & 0xf000) >> 12]) << 12) |
        ((substitution[(plaintext & 0xf0000) >> 16]) << 16) |
        ((substitution[(plaintext & 0xf00000) >> 20]) << 20) |
        ((substitution[(plaintext & 0xf000000) >> 24]) << 24) |
        ((substitution[(plaintext & 0xf0000000) >> 28]) << 28) |
        ((substitution[(plaintext & 0xf00000000) >> 32]) << 32) |
        ((substitution[(plaintext & 0xf000000000) >> 36]) << 36) |
        ((substitution[(plaintext & 0xf0000000000) >> 40]) << 40) |
        ((substitution[(plaintext & 0xf00000000000) >> 44]) << 44) |
        ((substitution[(plaintext & 0xf000000000000) >> 48]) << 48) |
        ((substitution[(plaintext & 0xf0000000000000) >> 52]) << 52) |
        ((substitution[(plaintext & 0xf00000000000000) >> 56]) << 56) |
        ((substitution[(plaintext & 0xf000000000000000) >> 60]) << 60); 
    //plaintext ^=round_key[4];
    IV = plaintext;
    return plaintext;
}

int main(){
    freopen("F:\\vs_workplace\\c++_code\\1.in", "rb", stdin);
    fread(init_key,SPNBYTES,1,stdin);
    fread(init_key+1,SPNBYTES,1,stdin);
    key_gen();

    for(int i=0;i<INPUTBYTES/SPNBYTES;i++){
        fread(plain_text,SPNBYTES,1,stdin);
        cipher_text[0]=SPN(plain_text[0]);
        fwrite(cipher_text,8,1,stdout);
    }
    
    return 0;
}