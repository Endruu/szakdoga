#ifndef  _PZK_CONTAINER_H 
#define _PZK_CONTAINER_H

#include "types.h"

pzkContainer * createPzkContainer(uint np, uint nz);
uint shrinkPzkContainer(pzkContainer * pzk, uint min);
void deletePzkContainer(pzkContainer * pzk);

void sortPZ(complex * list, uint num);
void sortPzkContainer(pzkContainer * pzk);

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
