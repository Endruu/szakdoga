#ifndef _DIAGNOSTICS_H
#define _DIAGNOSTICS_H

void printPzkContainer(pzkContainer * pzk);
void print4Matlab(pzkContainer * pzk);
void simulateFilter(const filterInfo * fi, const char * simfile);

#endif