//
// Created by Никита on 29.10.2020.
//

#ifndef HESSE_FUNK_H
#define HESSE_FUNK_H
#include <openssl/bn.h>
#include "const.h"

void addDots(struct Coordinates* D1, struct Coordinates* D2, struct Coordinates* D3, struct Numbers* crv);
void makeDot(struct Coordinates* dot, char* X, char* Y, char* Z);
int ifonHesse (struct Coordinates* D, struct Numbers* crv);
int Equal(struct Coordinates* FirstD, struct Coordinates* SecondD, struct Numbers* crv);
void findDot(struct Coordinates* DK, struct Coordinates* D, struct Numbers* crv, BIGNUM* Num);
void swapAFF(struct Coordinates* affD, struct Coordinates* D, struct Numbers* crv);
void clearDoT(struct Coordinates* P);
#endif //HESSE_FUNK_H
