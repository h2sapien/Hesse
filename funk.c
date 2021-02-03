//
// Created by Никита on 29.10.2020.
//

#include "funk.h"

void addDots(struct Coordinates* D1, struct Coordinates* D2, struct Coordinates* D3, struct Numbers* crv){
    BN_CTX *tmp = BN_CTX_new();
    BIGNUM *T1 = BN_new(), *T2 = BN_new(), *T3= BN_new(), *T4= BN_new(), *T5= BN_new(), *T6= BN_new(), *T7= BN_new();
    BN_copy(T1, D1->X1);
    BN_copy(T2, D1->Y1);
    BN_copy(T3, D1->Z1);
    BN_copy(T4, D2->X1);
    BN_copy(T5, D2->Y1);
    BN_copy(T6, D2->Z1);
    BN_mod_mul(T7, T1, T6, crv->p1, tmp); //T1*T6=T7
    BN_mod_mul(T1, T1, T5, crv->p1, tmp); //T1*T5=T1
    BN_mod_mul(T5, T3, T5, crv->p1, tmp); //T3*T5=T5
    BN_mod_mul(T3, T3, T4, crv->p1, tmp); //T3*T4=T3
    BN_mod_mul(T4, T2, T4, crv->p1, tmp); //T2*T4=T4
    BN_mod_mul(T2, T2, T6, crv->p1, tmp); //T2*T6=T2
    BN_mod_mul(T6, T2, T7, crv->p1, tmp); //T2*T7=T6
    BN_mod_mul(T2, T2, T4, crv->p1, tmp); //T2*T4=T2
    BN_mod_mul(T4, T3, T4, crv->p1, tmp); //T3*T4=T4
    BN_mod_mul(T3, T3, T5, crv->p1, tmp); //T3*T5=T3
    BN_mod_mul(T5, T1, T5, crv->p1, tmp); //T1*T5=T5
    BN_mod_mul(T1, T1, T7, crv->p1, tmp); //T1*T7=T1
    BN_mod_sub(T1, T1, T4, crv->p1, tmp); //T1-T4=T1
    BN_mod_sub(T2, T2, T5, crv->p1, tmp); //T2-T5=T2
    BN_mod_sub(T3, T3, T6, crv->p1, tmp); //T3-T6=T3
    BN_copy(D3->X1, T2);
    BN_copy(D3->Y1, T1);
    BN_copy(D3->Z1, T3);
    BN_CTX_free(tmp);
    BN_free(T1);
    BN_free(T2);
    BN_free(T3);
    BN_free(T4);
    BN_free(T5);
    BN_free(T6);
    BN_free(T7);
}

void makeDot(struct Coordinates* dot, char* X, char* Y, char* Z){
    BN_dec2bn(&dot->X1, X);
    BN_dec2bn(&dot->Y1, Y);
    BN_dec2bn(&dot->Z1, Z);
}

void swapAFF(struct Coordinates* affD, struct Coordinates* D, struct Numbers* crv){
    BN_CTX *tmp = BN_CTX_new ();
    BIGNUM *x = BN_new();
    BIGNUM *y = BN_new();
    BIGNUM *z = BN_new();
    BN_mod_inverse(x, D->Z1, crv->p1, tmp);
    BN_mod_mul(x, x, D->X1, crv->p1, tmp);
    BN_mod_inverse(y, D->Z1, crv->p1, tmp);
    BN_mod_mul(y, y, D->Y1, crv->p1, tmp);
    BN_dec2bn(&z, "0");
    BN_copy(affD->X1, x);
    BN_copy(affD->Y1, y);
    BN_copy(affD->Z1, z);
    BN_CTX_free(tmp);
    BN_free(x);
    BN_free(y);
    BN_free(z);
}

void findDot(struct Coordinates* DK, struct Coordinates* D, struct Numbers* crv, BIGNUM* Num){

    int bits = BN_num_bits(Num);
    struct Coordinates R = {BN_new (), BN_new (), BN_new ()};
    struct Coordinates Q = {NULL, NULL, NULL};
    makeDot(&Q, "-1", "0", "1");
    BN_copy(R.X1, D->X1);
    BN_copy(R.Y1, D->Y1);
    BN_copy(R.Z1, D->Z1);
    for(int i = bits - 1; i >= 0; --i){
        if (BN_is_bit_set(Num, i)){
            addDots(&Q, &R, &Q, crv);
            addDots(&R, &R, &R, crv);
        }
        else{
            addDots(&R, &Q, &R, crv);
            addDots(&Q, &Q, &Q, crv);
        }
    }
    BN_copy(DK->X1, Q.X1);
    BN_copy(DK->Y1, Q.Y1);
    BN_copy(DK->Z1, Q.Z1);
    clearDoT(&Q);
    clearDoT(&R);
}

int Equal(struct Coordinates* FirstD, struct Coordinates* SecondD, struct Numbers* crv){
    struct Coordinates FirstPinAff = {NULL, NULL, NULL};
    struct Coordinates SecondPinAff = {NULL, NULL, NULL};
    makeDot(&FirstPinAff, "-1", "0", "1");
    makeDot(&SecondPinAff, "-1", "0", "1");
    swapAFF(&FirstPinAff, FirstD, crv);
    swapAFF(&SecondPinAff, SecondD, crv);
    int ans;
    if(!(BN_cmp(FirstPinAff.X1, SecondPinAff.X1) && BN_cmp(FirstPinAff.Y1, SecondPinAff.Y1))){
        ans = 0;
    }
    else ans = -1;
    clearDoT(&FirstPinAff);
    clearDoT(&SecondPinAff);
    return ans;
}

int ifonHesse (struct Coordinates* D, struct Numbers* crv){
    BN_CTX* tmp = BN_CTX_new ();
    BIGNUM* left  = BN_new ();
    BIGNUM* right = BN_new ();
    BIGNUM* buf1  = BN_new (); // пустой слот для промежуточных результатов
    BIGNUM* num3  = BN_new ();
    //*X1^3+Y1^3+1=3*D1*x*y- подставить точки сравнить левую правую часть
    BN_dec2bn(&num3, "3");                                 // num3 = 3
    BN_mod_exp(left, D->X1, num3, crv->p1, tmp);          // left = X1^3
    BN_mod_exp(buf1, D->Y1, num3, crv->p1, tmp);          // left = y^3
    BN_mod_add(left, left, buf1, crv->p1, tmp);          // left = X1^3+y^3
    BN_dec2bn(&buf1, "1");                            // помещаю 1 в буффер
    BN_mod_add(left, left, buf1, crv->p1, tmp);          // X1^3+Y1^3+1
    BN_mod_mul(right, D->X1, D->Y1, crv->p1, tmp);     // right = x*y
    BN_mod_mul(right, right, crv->D1, crv->p1, tmp);     // right = D1*x*y
    BN_mod_mul(right, right, num3, crv->p1, tmp);     // right = 3*D1*x*y
    BN_mod_sub(buf1, right, left, crv->p1, tmp);     // вычитая правую и левую часть те проверям равны ли они
    int result = BN_is_zero(buf1);
    BN_CTX_free(tmp);
    BN_free(left);
    BN_free(right);
    BN_free(buf1);
    BN_free(num3);
    return result;
}

void clearDoT(struct Coordinates* P){
    BN_free(P->X1);
    BN_free(P->Y1);
    BN_free(P->Z1);
}

