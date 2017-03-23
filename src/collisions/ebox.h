#ifndef _estimated_collision_boxes_h_
#define _estimated_collision_boxes_h_

#include "allegro.h"

class EBox{
public:

	EBox( int ax1, int ay1, int ax2, int ay2 );
	void add( EBox * d );
	void remove();
	bool collide( EBox * hit, int mx, int my, int ax, int ay );
	
	int x1, y1, x2, y2;
	EBox * next;
	
};

class ECollide{
public:

	ECollide();
	ECollide( BITMAP * work, int size, int mask, double enough );
	bool Collision( ECollide * hit, int mx, int my, int ax, int ay );
	void display( BITMAP * buf, int x, int y );
	
	void add( int x1, int y1, int x2, int y2 );
	void add( EBox * arch );
	void remove();
	void finalize();
	bool Single( int mx, int my, int ax, int ay );
	int size();
	
	int CX1( int x );
	int CY1( int y );
	int CX2( int x );
	int CY2( int y );

	~ECollide();
	
	int ul_x, ul_y, lr_x, lr_y;

protected:
	void reset();

	void Next();
	void mapBox_hori( int ** map, int MX, int MY, int x, int y, int size, bool who );
	void mapBox_vert( int ** map, int MX, int MY, int x, int y, int size, bool who );
	void do_rect( int ** map, int x, int y, int MX, int MY, int sq, int size );
	EBox * getBox( int x, int y );
	EBox * Box();
	EBox * head;
	EBox * current;

	int xs, ys; //length and width of bitmap

	EBox *** box_map;
	int my_size;
	int SMAX_X, SMAX_Y;
	
};

#endif

