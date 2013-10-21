#ifndef  _PZK_CONTAINER_H 
#define _PZK_CONTAINER_H

#include "types.h"

pzkContainer * createPzkContainer(uint np, uint nz);
uint shrinkPzkContainer(pzkContainer * pzk, uint min);
pzkContainer * deletePzkContainer(pzkContainer * pzk);

int compareMagnitude( complex c1, complex c2 );
int compareQFactor( complex c1, complex c2 );
void reverseComplexList(complex * list, const uint num);
uint findInsertionPoint(real im, complex * list, uint num, char method);
void sortComplexList(complex * list, uint num, char method);
uint sortPzkContainer(pzkContainer * pzk, char sort, char order);

uint addPole(pzkContainer * pzk, complex pole);
uint addZero(pzkContainer * pzk, complex zero);

uint countPoles(pzkContainer * pzk);
uint countZeros(pzkContainer * pzk);
uint countBiquads(pzkContainer * pzk);

int compare1(complex c1, complex c2);
void sortDigitalPZ(pzkContainer * pzk);
int preSortDigitalZeros(pzkContainer * pzk);
int sortDigitalPoles(pzkContainer * pzk);

#endif
