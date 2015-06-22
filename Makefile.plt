CC=clang
CXX=clang++
MAKE=/usr/local/bin/gmake

PLATLDFLAGS= -lstdthreads -lm 

TOXINC+= -I/usr/local/include
TOXLIB+= -L/usr/local/lib -ltoxcore -ltoxdns -lsodium

FLTKINC= $(shell fltk-config --cxxflags)
FLTKLIB= $(shell fltk-config --ldstaticflags) -lxcb
