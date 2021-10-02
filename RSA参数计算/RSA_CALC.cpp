#include <gmp.h>
#include <stdio.h>
using namespace std;

#define min_e 65536
#define flat 20         //表示p,q平滑程度

bool is_prime(mpz_t paraN)
{
    //拉宾米勒测试
    int count = 0;
    gmp_randstate_t state;
    gmp_randinit_mt(state);

    mpz_t b, t, temp, a, n;
    mpz_init(b);
    mpz_init(n);
    mpz_init(a);
    mpz_init(t);
    mpz_init(temp);

    mpz_init_set(n, paraN);
    mpz_set(t, n);
    mpz_sub_ui(t, t, 1);
    mpz_mod_ui(b, t, 2);
    while (mpz_cmp_ui(b, 0) == 0)
    { //  求解t
        mpz_div_ui(t, t, 2);
        mpz_mod_ui(b, t, 2);
        count++;
    }
    // 偶数直接排除
    mpz_sub_ui(t, n, 1);
    if (count == 0)
    {
        return false; 
    }
    for (int i = 0; i < 300; i++)
    {
        mpz_urandomm(a, state, n); // 随机选择a
        if (mpz_cmp_ui(a, 0) != 0 && mpz_cmp_ui(a, 1) != 0)
        {
            mpz_powm(temp, a, t, n);
            if (mpz_cmp_ui(temp, 1) == 0)
            { //  是素数
                return true;
            }
            for (int j = 0; j < count; j++)
            {
                if (mpz_cmp(temp, t) == 0)
                { // 是素数
                    return true;
                }
                mpz_powm_ui(temp, temp, 2, n);
            }
        }
    }
    return false; 
}

bool gcd(mpz_t paraA, mpz_t paraB,int mode)
{
    mpz_t temp, a, b;
    mpz_init(temp);
    mpz_init(a);
    mpz_init(b);
    mpz_init_set(a, paraA);
    mpz_init_set(b, paraB);
    mpz_mod(temp, a, b);
    while (mpz_cmp_ui(temp, 0) != 0)
    {
        mpz_init_set(a, b);
        mpz_init_set(b, temp);
        mpz_mod(temp, a, b);
    }
    if(mode==0){
        if (mpz_cmp_ui(b, 1) == 0)
        {
            return true;
        }
    }else{
        if (mpz_cmp_ui(b, flat) > 0)
        {
            return true;
        }      
    }
    return false;
}

void calc(mpz_t e, mpz_t p, mpz_t q)
{
    mpz_t st[5000];
    int count = -1;
    for (int i = 0; i < 5000; i++)
    {
        mpz_init(st[i]);
    }
    mpz_t phi, temp, temp1, temp2;
    mpz_init(phi);
    mpz_init(temp);
    mpz_init(temp1);
    mpz_init(temp2);

    //e过小
    if (mpz_cmp_ui(e, min_e) < 0)
    { 
        printf("ERROR\n");
        return;
    }

    //p,q不是素数 
    if (!(is_prime(p) && is_prime(q)))
    { 
        printf("ERROR\n");
        return;
    }

    // p,q差值过小
    mpz_sub(temp, p, q);
    mpz_div_ui(temp1, p, 10);
    mpz_abs(temp, temp);
    if (mpz_cmp(temp, temp1) < 0)
    { 
        printf("ERROR\n");
        return;
    }

    //太平滑(指p-1和q-1的最大公因数超过20)
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    if (gcd(p, q,1))
    {
        printf("ERROR\n");
        return;
    }
    //e和phi不互素
    mpz_mul(phi, p, q);
    mpz_init_set_ui(temp, 0);
    mpz_sub(phi, temp, phi);
    if (!gcd(e, phi,0))
    { 
        printf("ERROR\n");
        return;
    }

    // 求解d
    mpz_mod(temp, e, phi);
    mpz_sub(temp1, e, temp);
    mpz_div(temp1, temp1, phi);
    mpz_init_set(st[0], temp1);
    while (mpz_cmp_ui(temp, 0) != 0)
    {
        count++;
        mpz_init_set(st[count], temp1);
        mpz_init_set(e, phi);
        mpz_init_set(phi, temp);
        mpz_mod(temp, e, phi);
        mpz_sub(temp1, e, temp);
        mpz_div(temp1, temp1, phi);
    }
    mpz_init_set_ui(temp1, 0);
    mpz_init_set_ui(temp2, 1);
    mpz_sub(phi, temp1, phi);
    for (int i = count; i >= 0; i--)
    {
        mpz_init_set(temp, temp2);
        mpz_mul(temp2, temp2, st[i]);
        mpz_sub(temp2, temp1, temp2);
        mpz_init_set(temp1, temp);
    }
    gmp_printf("%Zd\n", temp1);
}

int main()
{
    int n;
    mpz_t e, p, q;
    mpz_init(e);
    mpz_init(p);
    mpz_init(q);
    
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        gmp_scanf("%Zd%Zd%Zd", e, p, q);
        calc(e, p, q);
    }
    return 0;
}