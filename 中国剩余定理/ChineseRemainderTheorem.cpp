#include <iostream>
#include <gmp.h>

#define window_size 5
/*
inline void expmod(mpz_t a, mpz_t paraE, mpz_t n) {
	// 平方乘算法
	int i = 0;
    char str[2000];
	mpz_t aBack;
	mpz_init_set(aBack, a);
	mpz_get_str(str, 2, paraE);		// 将其转化为二进制字符串，加速运算
	mpz_set_ui(a, 1);
	while (str[i] != '\0') {		// 使用平方乘算法
		mpz_mul(a, a, a);
		mpz_mod(a, a, n);
		if (str[i] == '1') {
			mpz_mul(a, a, aBack);
			mpz_mod(a, a, n);
		}
		i++;    
	}   
}*/
void expmod(mpz_t a, mpz_t paraE, mpz_t n){
    mpz_t ret,tmp;
    mpz_init(ret);
    mpz_init(tmp);
    mpz_set_ui(ret,1);
    mpz_t pre[1<<window_size];
    for(int i=0;i<(1<<window_size);i++){
        mpz_init(pre[i]);
    }
    mpz_set_ui(pre[0],1);
    mpz_mod(pre[1],a,n);
    mpz_mul(tmp,pre[1],a);
    mpz_mod(tmp,tmp,n);
    for(int i=3;i<(1<<window_size);i+=2){
        mpz_mul(pre[i],pre[i-2],tmp);
        mpz_mod(pre[i],pre[i],n);
    }
    unsigned long l = paraE->_mp_size*GMP_LIMB_BITS;
    long i=l-1;
    long s,ni;
    while (i>=0)
    {
        if(mpz_tstbit(paraE,i)==0){
            mpz_mul(ret,ret,ret);
            mpz_mod(ret,ret,n);
            i--;
        }else{
            s = (i + 1 - window_size) >= 0 ? (i + 1 - window_size) : 0;
            while (mpz_tstbit(paraE,s)==0)
            {
               s++;
            }
            for(int j=1;j<=i-s+1;j++){
                mpz_mul(ret,ret,ret);
                mpz_mod(ret,ret,n);
            }
            //mpz_set_ui(tmp,s);
            mpz_div_2exp(tmp,paraE,s);
            unsigned long long temp = mpz_get_ui(tmp)&((1 << (i - s + 1)) - 1);
            mpz_mul(ret,ret,pre[temp]);
            mpz_mod(ret,ret,n);
            i=s-1;
        }
    }
    mpz_set(a,ret);
}

int main(){
    int n;
    mpz_t e,p,q,d,c,p_inverse,q_inverse,N,phi_N,phi_q,phi_p,d1,d2,answer;
    mpz_init(e);
    mpz_init(p);
    mpz_init(q);
    mpz_init(d);
    mpz_init(c);
    mpz_init(p_inverse);
    mpz_init(q_inverse);
    mpz_init(N);
    mpz_init(phi_N);
    mpz_init(phi_q);
    mpz_init(phi_p);
    mpz_init(d1);
    mpz_init(d2);
    mpz_init(answer);

    //freopen("1.in","r",stdin);
    scanf("%d",&n);
    gmp_scanf("%Zd%Zd%Zd",p,q,e);
    //求各自的逆
    //inverse(p_inverse,p,q);
    //inverse(q_inverse,q,p);
    mpz_invert(p_inverse,p,q);
    mpz_invert(q_inverse,q,p);
    //N=p*q p-1 q-1
    mpz_mul(N,p,q);
    mpz_sub_ui(phi_q,q,1);
    mpz_sub_ui(phi_p,p,1);
    //inverse(d,e,phi_N);
    mpz_mul(phi_N,phi_q,phi_p);
    mpz_invert(d,e,phi_N);
    //求相对运算的模
    mpz_mod(d1,d,phi_p);
    mpz_mod(d2,d,phi_q);

    mpz_t b1,b2;
    mpz_init(b1);
    mpz_init(b2);

    for(int i=0;i<n;i++){
        gmp_scanf("%Zd",c);
        //mpz_powm(answer,c,d,N);
        
        mpz_set(b1,c);
        mpz_set(b2,c);
        expmod(b1,d1,p);
        expmod(b2,d2,q);

        mpz_mul(b1, b1, q);		// 运用中国剩余定理计算
		mpz_mod(b1, b1, N);
		mpz_mul(b1, b1, q_inverse);
		mpz_mod(b1, b1, N);

        mpz_mul(b2, b2, p);
		mpz_mod(b2, b2, N);
		mpz_mul(b2, b2, p_inverse);
		mpz_mod(b2, b2, N);

		mpz_add(b1, b1, b2);
		mpz_mod(answer, b1, N);

        gmp_printf("%Zd\n",answer);
    }
    return 0;
}