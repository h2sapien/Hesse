#include <openssl/bn.h>
#include "funk.h"
#include <stdio.h>
#include <string.h>

int main() {
    struct Numbers digt = {NULL, NULL, NULL, NULL, NULL};
    BN_dec2bn(&digt.p, p_);
    BN_dec2bn(&digt.D, D_);
    BN_dec2bn(&digt.u_base, u);
    BN_dec2bn(&digt.v_base, v);
    BN_dec2bn(&digt.m, m_);

    struct Numbers crv = {NULL, NULL, NULL, NULL, NULL};
    BN_CTX* tmp = BN_CTX_new ();
    BIGNUM* buf1 = BN_new ();
    BIGNUM* buf2 = BN_new ();
    BN_dec2bn(&crv.Y, "0");
    BN_dec2bn(&crv.X, "0");
    BN_dec2bn(&crv.D1, "0");
    BN_dec2bn(&crv.Z, "0");
    BN_dec2bn(&crv.p1, "0");
    BN_copy(crv.p1, digt.p);
    BN_copy(crv.D1, digt.D);
    BN_copy(crv.Z, digt.v_base);
    BN_nnmod(crv.Z, crv.Z, crv.p1, tmp);
    BN_free(buf1);
    BN_free(buf2);
    BN_CTX_free(tmp);
    struct Coordinates E = {NULL, NULL, NULL};
    makeDot(&E, "1", "-1", "0");


    printf("TEST 1:\n");
    struct Coordinates D = {NULL, NULL, NULL};
    makeDot(&D, u, v, "1");
    struct Coordinates resDoT = {NULL, NULL, NULL};
    makeDot(&resDoT, "1", "-1", "0");
    BIGNUM *k = BN_new();
    BN_rand_range(k, digt.m); //0<=k<m
    findDot(&resDoT, &D, &crv, k);
    if (ifonHesse(&D, &crv)) {
        printf("Success\n\n");
    } else {
        printf("Fail\n\n");
    }

    printf("TEST 2:\n");
    struct Coordinates reDoT = {NULL, NULL, NULL};
    makeDot(&reDoT, "-1", "0", "1");
    findDot(&reDoT, &D, &crv, digt.m);
    if (!(Equal(&reDoT, &E, &crv))) {
        printf("Success\n\n");
    } else {
        printf("Fail\n\n");
    }

    printf("TEST 3:\n");
    BIGNUM *degree = BN_new();
    BN_dec2bn(&degree, "1");
    BN_add(degree, degree, digt.m);
    findDot(&reDoT, &D, &crv, degree);
    if ((Equal(&reDoT, &D, &crv))) {
        printf("Success\n");
    } else {
        printf("Fail\n");
    }

    BN_dec2bn(&degree, "1");
    BN_sub(degree, digt.m, degree);
    findDot(&reDoT, &D, &crv, degree);
    struct Coordinates negP = {NULL, NULL, NULL};
    makeDot(&negP, "1", "-1", "0");
    BN_copy(negP.X1, D.X1);
    BN_copy(negP.Y1, D.Y1);
    BN_copy(negP.Z1, D.Z1);
    BN_set_negative(negP.X1, 1);
    if ((Equal(&reDoT, &negP, &crv))) {
        printf("Success\n\n");
    } else {
        printf("Fail\n\n");
    }

    printf("TEST 4:\n");
    BIGNUM *k1 = BN_new();
    BIGNUM *k2 = BN_new();
    BIGNUM *maxrand = BN_new();
    BN_dec2bn(&maxrand, "100000000000000000");
    BN_rand_range(k1, maxrand);
    BN_rand_range(k2, maxrand);
    BN_add(k, k1, k2);
    struct Coordinates res1 = {NULL, NULL, NULL};
    struct Coordinates res2 = {NULL, NULL, NULL};
    struct Coordinates res3 = {NULL, NULL, NULL};
    makeDot(&res1, "1", "-1", "0");
    makeDot(&res2, "1", "-1", "0");
    makeDot(&res3, "1", "-1", "0");
    findDot(&res1, &D, &crv, k1);
    findDot(&res2, &D, &crv, k2);
    findDot(&res3, &D, &crv, k);
    addDots(&res1, &res2, &reDoT, &crv);

    if(!(Equal(&reDoT, &res3, &crv))){
        printf("[k1]P + [k2]P = [k1 + k2]P\n\n");
    }
    else{
        printf("[k1]P + [k2]P != [k1 + k2]P\n\n");
    }


}


