#ifndef _TRIGTABLE_H_
#define _TRIGTABLE_H_

#include <math.h>

long int tsqr( const long int r );
long int tsqrt( const long int q );
long int tcube( const long int c );
long int dist( int x1, int y1, int x2, int y2 );
double zdist( int x, int y, int x1, int y1 );
long int area( int x1, int y1, int x2, int y2, int x3, int y3 );
bool prect( int px1, int py1, int x1, int y1, int x2, int y2 );
bool ptriangle( int tx1, int ty1, int tx2, int ty2, int tx3, int ty3, int x1, int y1 );
int sarctan( const double tr );
int barctan( const double tr );
int lineIntersect( int ax, int ay, int bx, int by, int cx, int cy, int dx, int dy, int *ix, int *iy );
int gang( int x1, int y1, int x2, int y2 );
int arcsine( double x );
void set_trig();
#endif

