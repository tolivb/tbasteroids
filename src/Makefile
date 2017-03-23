CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-c -Wall
LDFLAGS=
LDLIBS=-lalleg
GAMENAME=tbasteroids

SRCS=main.cpp gameManager.cpp functions.cpp objectsManager.cpp weapons/weapon.cpp \
     weapons/weaponShip.cpp weapons/weaponEnemy.cpp weapons/weaponsFactory.cpp \
      spaceObjects/bullet.cpp spaceObjects/enemyShip.cpp \
     spaceObjects/spaceObject.cpp spaceObjects/spaceObjectsFactory.cpp \
     collisions/ebox.cpp collisions/trigtable.cpp \
     spaceObjects/asteroid.cpp spaceObjects/spaceShip.cpp
     

OBJS=$(subst .cpp,.o,$(SRCS))

all: tbasteroids

tbasteroids: $(OBJS)
	$(CXX) $(LDFLAGS) -o $(GAMENAME) $(OBJS) $(LDLIBS) 

clean:
	$(RM) $(OBJS)

install: 
	mv $(GAMENAME) ../bin/$(GAMENAME)
