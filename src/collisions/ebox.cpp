/*
 * EBox version 3
 *
 * This software is liscened under the GPL( GNU Public Liscense.). Look it up if you have a qustion
 * as to what it does. Use this code for whatever you want, but no guarantee is in inherint in the 
 * workings of it. 
 * Enjoy the sauce!
 * 
 * by Jon Rafkind *
 *
 */



#include "ebox.h"
#include "allegro.h"
#include "trigtable.h"

#include <stdio.h>

EBox::EBox( int ax1, int ay1, int ax2, int ay2 ):
x1( ax1 ),
y1( ay1 ),
x2( ax2 ),
y2( ay2 ) {
	next = NULL;
}


void EBox::add( EBox * d ) {
	next = d;
}


void EBox::remove() {
	next = NULL;
}


bool EBox::collide( EBox * hit, int mx, int my, int ax, int ay ) {
	if ( hit == NULL ) return false;
	if ( prect( x1+mx, y1+my, hit->x1+ax, hit->y1+ay, hit->x2+ax, hit->y2+ay ) ||
		prect( x1+mx, y2+my, hit->x1+ax, hit->y1+ay, hit->x2+ax, hit->y2+ay ) ||
		prect( x2+mx, y1+my, hit->x1+ax, hit->y1+ay, hit->x2+ax, hit->y2+ay ) ||
		prect( x2+mx, y2+my, hit->x1+ax, hit->y1+ay, hit->x2+ax, hit->y2+ay ) ) return true;

	int zx1 = x1+mx;
	int zy1 = y1+my;
	int zx2 = x2+mx;
	int zy2 = y2+my;

	int zx3 = hit->x1+ax;
	int zy3 = hit->y1+ay;
	int zx4 = hit->x2+ax;
	int zy4 = hit->y2+ay;

	if ( ( (zx1 >= zx3 && zx1<= zx4) || ( zx2 >= zx3 && zx2 <= zx4) ) &&
		( (zy3 >= zy1 && zy3 <= zy2 ) || (zy4 >= zy1 && zy4 <= zy2 ) ) ) return true;
	if ( ( (zx3 >= zx1 && zx3 <= zx2) || ( zx4 >= zx1 && zx4 <= zx2 ) ) &&
		( (zy1 >= zy3 && zy1 <= zy4 ) || (zy2 >= zy3 && zy2 <= zy4 ) ) ) return true;

	return false;
}


void ECollide::do_rect( int ** map, int x, int y, int MX, int MY, int sq, int size ) {

	if ( x + sq >= MX ) return;
	if ( y + sq >= MY ) return;

	int xs = sq;
	if ( x+xs >= MX ) xs = MX-x-1;
	int ys = sq;
	if ( y+ys >= MY ) ys = MY-y-1;

	bool total = true;
	for ( int ay = y; ay < y + ys; ay++ )
		for ( int ax = x; ax < x + xs; ax++ )
			if ( map[ay][ax] != 1 ) return;
	if ( total ) {
		EBox * junk = new EBox( x*size, y*size, (x+xs)*size, (y+ys)*size );
		//add( x*size,y*size,(x+xs)*size,(y+ys)*size );
		add( junk );

		for ( int ay = y; ay < y + ys; ay++ )
		for ( int ax = x; ax < x + xs; ax++ ) {
			map[ay][ax] = 2;
			box_map[ay][ax] = junk;
		}

	}

}


void ECollide::mapBox_hori( int ** map, int MX, int MY, int x, int y, int size, bool who ) {

	int qx = x;
	while ( qx < MX && map[y][qx] == 1 ) qx++;
	if ( qx-x < 2 && who ) return;

	EBox * junk = new EBox( x*size, y*size, qx*size, y*size+size );

	for ( int g = x; g < qx; g++ ) {
		map[y][g] = 2;
		box_map[y][g] = junk;
	}

	//add( x*size, y*size,qx*size,y*size+size);
	add( junk );
	return;

}


void ECollide::mapBox_vert( int ** map, int MX, int MY, int x, int y, int size, bool who ) {

	int qy = y;
	while ( qy < MY && map[qy][x] == 1 ) qy++;
	if ( qy-y < 2 && who ) return;

	EBox * junk = new EBox( x * size, y * size, x*size+size, qy*size );

	for ( int g = y; g < qy; g++ ) {
		map[g][x] = 2;
		box_map[g][x] = junk;
	}

	//add( x*size, y*size,x*size+size,qy*size);
	add( junk );
	return;

}


ECollide::ECollide():
xs( 0 ),
ys( 0 ),
ul_x( -1 ),
ul_y( -1 ),
lr_x( -1 ),
lr_y( -1 ) {
	head = new EBox( 0, 0, 0, 0 );
	current = head;

	box_map = NULL;
	my_size = 0;
}


ECollide::ECollide( BITMAP * work, int size, int mask, double enough ) {

	head = new EBox( 0, 0, 0, 0 );
	current = head;
	xs = work->w/2;
	ys = work->h/2;
	my_size = size;

	if ( enough < 0 ) enough = 50;
	if ( enough > 100 ) enough = 100;

	int map_x = work->w/size+1;
	int map_y = work->h/size+1;

	int ** map = new int*[ map_y ];
	for ( int y = 0; y < map_y; y++ )
		map[y] = new int[ map_x ];

	for ( int y = 0; y < map_y; y++ )
		for ( int x = 0; x < map_x; x++ )
			map[y][x] = 0;

	box_map = new EBox**[ map_y ];
	for ( int y = 0; y < map_y; y++ )
		box_map[y] = new EBox*[ map_x ];
	for ( int y = 0; y < map_y; y++ )
		for ( int x = 0; x < map_x; x++ )
			box_map[y][x] = NULL;

	SMAX_X = work->w/size+1;
	SMAX_Y = work->h/size+1;

	int mx = 0, my = 0;
	for ( int y = 0, my=0; y < work->h; y += size, my++ ) {
		for ( int x = 0, mx=0; x < work->w; x += size, mx++ ) {
			BITMAP * sub = create_sub_bitmap( work, x, y, size, size );
			int total = 0;
			for ( int qx = 0; qx < sub->w; qx++ )
				for ( int qy = 0; qy < sub->h; qy++ )
					if ( getpixel(sub,qx,qy) != mask )
						total++;
			double percent = 100.0 * (double)total / (double)(sub->w*sub->h);
			if ( percent > enough )
				map[my][mx] = 1;
			destroy_bitmap( sub );
		}
	}

	for ( int sq = map_x>map_y?map_x:map_y; sq > 2; sq-- )
		for ( int y = 0; y < map_y; y++ )
			for ( int x = 0; x < map_x; x++ )
				do_rect( map, x, y, map_x, map_y, sq, size );

	for ( int y = 0; y < map_y; y++ )
		for ( int x = 0; x < map_x; x++ )
			if ( map[y][x] == 1 )
				mapBox_hori( map, map_x, map_y, x, y, size, true );

	for ( int y = 0; y < map_y; y++ )
		for ( int x = 0; x < map_x; x++ )
			if ( map[y][x] == 1 )
				mapBox_vert( map, map_x, map_y, x, y, size, true );

	for ( int y = 0; y < map_y; y++ )
		for ( int x = 0; x < map_x; x++ )
			if ( map[y][x] == 1 )
				mapBox_hori( map, map_x, map_y, x, y, size, false );

	for ( int x = 0; x < map_y; x++ )
		delete[] map[x];
	delete[] map;

	finalize();

}


void ECollide::finalize() {
	int gx = 0;
	int gy = 0;
	int lx = 0;
	int ly = 0;
	reset();
	if ( current != NULL ) {
		gx = current->x1;
		gy = current->y1;
	}
	while ( current != NULL ) {

		if ( current->x1 < gx ) gx = current->x1;
		if ( current->y1 < gy ) gy = current->y1;
		if ( current->x2 > lx ) lx = current->x2;
		if ( current->y2 > ly ) ly = current->y2;
		Next();

	}
	ul_x = gx;
	ul_y = gy;
	lr_x = lx;
	lr_y = ly;
}


void ECollide::display( BITMAP * buf, int x, int y ) {
	reset();
	while( current != NULL ) {
		rect( buf, CX1(current->x1+x), CY1(current->y1+y), CX1(current->x2+x), CY1(current->y2+y), makecol(255,255,255) );
		Next();
	}

}


void ECollide::add( int x1, int y1, int x2, int y2 ) {
	EBox * junk = new EBox( x1, y1, x2, y2 );
	current = head;
	while ( current->next != NULL ) current=current->next;
	current->add( junk );
}


void ECollide::add( EBox * arch ) {
	current = head;
	while ( current->next != NULL ) current=current->next;
	current->add( arch );
}


void ECollide::remove() {
	current = head;
	EBox * junk = current;
	while ( current->next != NULL ) current=current->next;
	while ( junk->next != current ) junk=junk->next;
	delete current;
	junk->remove();
}


int ECollide::CX1( int x ) {
	return x-xs;
}


int ECollide::CX2( int x ) {
	return x+xs;
}


int ECollide::CY1( int y ) {
	return y-ys;
}


int ECollide::CY2( int y ) {
	return y+ys;
}


bool ECollide::Single( int mx, int my, int ax, int ay ) {

	if ( ul_x != -1 )
		if ( !prect( ax, ay, CX1(ul_x+mx), CY1(ul_y+my), CX1(lr_x+mx), CY1(lr_y+my) ) ) return false;

	EBox * m_box = getBox( ax-CX1(mx), ay-CY1(my) );
	if ( m_box == NULL ) return false;
	if ( prect( ax, ay, CX1(m_box->x1+mx), CY1(m_box->y1+my),
		CX1(m_box->x2+mx), CY1(m_box->y2+my) ) ) return true;
	return false;

}


EBox * ECollide::getBox( int x, int y ) {

	if( box_map == NULL ) return NULL;

	x /= my_size;
	y /= my_size;

	if ( x >= SMAX_X || y >= SMAX_Y || x < 0 || y < 0 ) return NULL;
	return box_map[y][x];

}

int ECollide::size(){
	return my_size;
}

bool ECollide::Collision( ECollide * hit, int mx, int my, int ax, int ay ) {

	int zx1, zx2, zy1, zy2;

	zx1 = hit->CX1(ax+hit->ul_x) > CX1(mx+ul_x ) ? hit->CX1(ax+hit->ul_x) : CX1(mx+ul_x);
	zx2 = hit->CX1(ax+hit->lr_x) > CX1(mx+lr_x ) ? CX1(mx+lr_x) : hit->CX1(ax+hit->lr_x);

	zy1 = hit->CY1(ay+hit->ul_y) > CY1(my+ul_y) ? hit->CY1(ay+hit->ul_y) : CY1(my+ul_y);
	zy2 = hit->CY1(ay+hit->lr_y) > CY1(my+lr_y) ? CY1(my+lr_y) : hit->CY1(ay+hit->lr_y);


	int size_inc = size() > size() ? size() : size();

	for ( int vx = zx1; vx <= zx2; vx += size_inc )
	for ( int vy = zy1; vy <= zy2; vy += size_inc ) {
	
		EBox * m_box = getBox( vx-CX1(mx), vy-CY1(my) );
		EBox * s_box = hit->getBox( vx - hit->CX1(ax), vy - hit->CY1(ay) );

		if ( m_box != NULL && s_box != NULL )
			if ( m_box->collide( s_box, CX1(mx), CY1(my),
			hit->CX1(ax), hit->CY1(ay) ) )
				return true;
		

	}
	return false;

}


/*
bool ECollide::Collision( ECollide * hit, int mx, int my, int ax, int ay ) {

	if ( ul_x != -1 && hit->ul_x != -1 ) {
		int zx1 = CX1(ul_x+mx);
		int zy1 = CY1(ul_y+my);
		int zx2 = CX1(lr_x+mx);
		int zy2 = CY1(lr_y+my);

		int zx3 = hit->CX1(hit->ul_x+ax);
		int zy3 = hit->CY1(hit->ul_y+ay);
		int zx4 = hit->CX1(hit->lr_x+ax);
		int zy4 = hit->CY1(hit->lr_y+ay);

		bool cy = ( prect( zx1, zy1, zx3, zy3, zx4, zy4 ) ||
			prect( zx2, zy1, zx3, zy3, zx4, zy4 ) ||
			prect( zx1, zy2, zx3, zy3, zx4, zy4 ) ||
			prect( zx2, zy2, zx3, zy3, zx4, zy4 ) );

		bool s1 = ( ( (zx1 >= zx3 && zx1<= zx4) || ( zx2 >= zx3 && zx2 <= zx4) ) &&
			( (zy3 >= zy1 && zy3 <= zy2 ) || (zy4 >= zy1 && zy4 <= zy2 ) ) );
		bool s2 = ( ( (zx3 >= zx1 && zx3 <= zx2) || ( zx4 >= zx1 && zx4 <= zx2 ) ) &&
			( (zy1 >= zy3 && zy1 <= zy4 ) || (zy2 >= zy3 && zy2 <= zy4 ) ) );

		if ( !cy && !s1 && !s2 ) return false;

	} else return false;

	int zx1 = CX1(mx+ul_x);
	int zy1 = CY1(my+ul_y);
	int zx2 = hit->CX1(mx+hit->ul_x);
	int zy2 = hit->CY1(my+hit->ul_y);
	int zx1, zx2, zy1, zy2;

	if ( CX1(mx+ul_x) < hit->CX1(ax+hit->ul_x) ) {
		zx1 = hit->CX1(ax+hit->ul_x);
		zx2 = CX1(mx+lr_x);
	}
	else {
		zx1 = CX1(mx+ul_x);
		zx2 = hit->CX1(ax+hit->lr_x);
	}
	zx1 = hit->CX1(ax+hit->ul_x) > CX1(mx+ul_x ) ? hit->CX1(ax+hit->ul_x) : CX1(mx+ul_x);
	zx2 = hit->CX1(ax+hit->lr_x) > CX1(mx+lr_x ) ? CX1(mx+lr_x) : hit->CX1(ax+hit->lr_x);

	zy1 = hit->CY1(ay+hit->ul_y) > CY1(my+ul_y) ? hit->CY1(ay+hit->ul_y) : CY1(my+ul_y);
	zy2 = hit->CY1(ay+hit->lr_y) > CY1(my+lr_y) ? CY1(my+lr_y) : hit->CY1(ay+hit->lr_y);

	if ( CY1(my+ul_y) < hit->CY1(ay+ul_y) ) {
		zy1 = hit->CY1(ay+hit->ul_y);
		zy2 = CY1(my+lr_y);
	}
	else {
		zy1 = CY1(my+ul_y);
		zy2 = hit->CY1(ay+hit->lr_y);
	}

	//if ( zx1 > zx2 ) swap( zx1, zx2 );
	//if ( zy1 > zy2 ) swap( zy1, zy2 );

	//rect( screen, zx1, zy1, zx2, zy2, makecol(255,255,0) );
	
	int size_inc = size() > size() ? size() : size();

	for ( int vx = zx1; vx <= zx2; vx += size_inc )
	for ( int vy = zy1; vy <= zy2; vy += size_inc ) {
	
		//rect( screen, vx, vy, vx+my_size, vy+my_size, makecol(255,128,196) );

		EBox * m_box = getBox( vx-CX1(mx), vy-CY1(my) );
		EBox * s_box = hit->getBox( vx - hit->CX1(ax), vy - hit->CY1(ay) );

		if ( m_box != NULL && s_box != NULL )
			if ( m_box->collide( s_box, CX1(mx), CY1(my),
			hit->CX1(ax), hit->CY1(ay) ) ){
				//rectfill( screen, CX1(mx),CY1(my),CX2(mx),CY2(my), makecol(0,0,255) );
				//rectfill( screen, hit->CX1(mx),hit->CY1(my),hit->CX2(mx),hit->CY2(my), makecol(0,255,0) );
				//rectfill(screen,CX1(m_box->x1+mx),CY1(m_box->y1+my),CX1(m_box->x2+mx),CX1(m_box->y2+my),makecol(0,0,255) );
				//rectfill(screen,hit->CX1(s_box->x1+ax),hit->CY1(s_box->y1+ay),hit->CX1(s_box->x2+ax),hit->CY1(s_box->y2+ay),makecol(0,255,0) );
				return true;
			}

	}
	return false;

	reset();
	while ( current != NULL ) {
		hit->reset();
		while ( hit->Box() != NULL ) {
			if ( current->collide( hit->Box(), CX1(mx), CY1(my), hit->CX1(ax), hit->CY1(ay)  ) || hit->Box()->collide( current, hit->CX1(ax), hit->CY1(ay), CX1(mx), CY1(my) ) ) return true;
			hit->Next();
		}
		Next();
	}
	return false;

}
*/


EBox * ECollide::Box() {
	return current;
}


void ECollide::Next() {
	current = current->next;
}


void ECollide::reset() {
	current = head->next;
}


ECollide::~ECollide() {
	if ( box_map != NULL ) {
		for ( int y = 0; y < SMAX_Y; y++ )
			delete[] box_map[y];
		delete[] box_map;
	}
	reset();
	while ( current != NULL ) {
		EBox * junk = current;
		Next();
		delete junk;
	}
	delete head;
}
