#include <gmp.h>
#include <iostream>
#include <string.h>
using namespace std;

void expmod(mpz_t e,mpz_t m,mpz_t p,mpz_t q){
    char exp[8182+10];
    mpz_get_str(exp,2,e);

    mpz_t pq,temp1,temp2,temp3;
    mpz_init(pq);
    mpz_init(temp1);
    mpz_init(temp2);
    mpz_init(temp3);
    mpz_mul(pq,p,q);
    mpz_set_ui(temp1,1);//temp1 放最终结果
    mpz_mod(temp2,m,pq);

    for(int i=strlen(exp)-1;i>=0;i--){
        if(exp[i]=='1'){
            mpz_mul(temp1,temp1,temp2);
            mpz_mod(temp1,temp1,pq);
        }
        mpz_mul(temp2,temp2,temp2);
        mpz_mod(temp2,temp2,pq);
    }

    gmp_printf("%Zd\n",temp1);
}

int main(){
    int n;
    mpz_t e,m,p,q;
    mpz_init(e);
    mpz_init(m);
    mpz_init(p);
    mpz_init(q);

    scanf("%d",&n);
    for(int i=0;i<n;i++){
        gmp_scanf("%Zd%Zd%Zd%Zd",e,m,p,q);
        expmod(e,m,p,q);
    }

    return 0;
}