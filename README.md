TBasteroids
===========

Very simple 2D game created for educational purposes back in 2008. It's written in C++ and uses [allegro 4](http://liballeg.org/) library. 

Recently the game was compiled and tested successfully on Fedora 25 with g++. Should compile and run under Windows/Mac also [gcc](http://www.mingw.org/).


[Gameplay video](https://www.youtube.com/watch?v=KcC2fgvpMlc)

[![asteroids video screenshot](https://img.youtube.com/vi/KcC2fgvpMlc/0.jpg)](https://www.youtube.com/watch?v=KcC2fgvpMlc)


## How to compile and run (tested under Fedora 25)

Compile:

1. install allegro 4.4.2
2. `git clone https://github.com/tolivb/asteroids-cpp`
3. `cd asteroids-cpp/src`
4. `make` && `make install` - will compile and move the binary `tbasteroids` to the `bin` dir of the project

Run:

1. `cd ../bin` && `./tbasteroids` - run the game from the `bin` dir (if the binary is not in the same dir as the pictures it wont run!!!),

## How to compile allegro 4.4.2 (tested under Fedora 25)

Compile: 

1. Download the old version of allegro [liballegro442](http://download.gna.org/allegro/allegro/4.4.2/allegro-4.4.2.zip)
2. `unzip` to allegro442
3. `cd allegro442` && `cmake .` - make sure the dependencies are satisfied
4. `make` && `make install`

If the install is successfull the following files should be present:

 1. /usr/local/include/allegro.h - your c++ compiler must know about this path( -I/usr/local/include/)
 2. /usr/local/lib/liballeg.so

You might need to run `export LD_LIBRARY_PATH=/usr/local/lib` in order to prevent errors like this one:
```
./tbasteroids: error while loading shared libraries: liballeg.so.4.4: 
cannot open shared object file: No such file or directory
```

## Basic features/rules/controlls:

Controlls:
 
* move up `UP ARROW`
* move down `DOWN ARROW`
* move left `LEFT ARROW`
* move down `DOWN ARROW`
* fire `SPACE`

Rules:

1. Space ship can crash on asteroids
2. All bullets are absorbed by asteroids - you can hide behind asteroid
3. When enemy bullet(red) hits our bullet(blue) they destroy each other
4. Enemy ships are not affected by asteroids - they can fly over
5. Enemy ships are resurrected - the game may never end

## TODOs

Many things can be improved:

1. Add sound effects - onFire, onCrash, onExplosion
2. Add visual effects - onFire, onCrash, onExplosion
3. Add more fancy guns and  enemies
4. Migrate to allegro 5 - v4.4.2 is too old and probably not supported
5. Better collisions
6. Bugfixes...
