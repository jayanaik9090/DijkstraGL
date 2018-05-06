#declare the variable
CC=g++

CFLAGS=-c -Wall 

FILE = main

all: $(FILE)


$(FILE): $(FILE).o
	$(CC) -L"C:\MinGW\lib"  -o $(FILE).exe $(FILE).o -lglut32 -lopengl32 -lglu32 -lwinmm -lgdi32
$(FILE).o: $(FILE).cpp
	$(CC) -w -std=c++11 -fpermissive -g -I"C:\MinGW\include" $(CFLAGS) $(FILE).cpp	

run: all
	$(FILE).exe

clean:
	@echo off & del *.o $(FILE).exe & cls
	
