//
// Created by Никита on 30.10.2020.
//

#ifndef HESSE_CONST_H
#define HESSE_CONST_H
#include <openssl/bn.h>

#define p_               "115792089237316195423570985008687907853269984665640564039457584007913111864739"
#define D_               "3"
#define u          "93528601524582384978654134272795468222405403055717890280271688132874849008326"
#define v          "14443324612566128911211262381388707474030458136470034119105598903952521080679"
#define m_               "115792089237316195423570985008687907852907080286716537199505774922796921406320"

struct Numbers{
    BIGNUM* p;
    BIGNUM* D;
    BIGNUM* u_base;
    BIGNUM* v_base;
    BIGNUM* m;
    BIGNUM* Y;
    BIGNUM* X;
    BIGNUM* D1;
    BIGNUM* Z;
    BIGNUM* p1;
};
struct Coordinates{
    BIGNUM* X1;
    BIGNUM* Y1;
    BIGNUM* Z1;
};
#endif //HESSE_CONST_H