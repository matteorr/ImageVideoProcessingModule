all:main

main:IVPM.o TemplateMatx3.o main.o
	g++ -o main IVPM.o TemplateMatx3.o main.o -L/usr/local/lib -lopencv_core -lpng -lopencv_highgui

main.o:main.cpp
	g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -I/usr/include/c++/4.5.2 -O0 -g3 -Wall -fmessage-length=0 -std=c++0x -c main.cpp

TemplateMatx3.o:TemplateMatx3.cpp
	g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -I/usr/include/c++/4.5.2 -O0 -g3 -Wall -fmessage-length=0 -std=c++0x -c TemplateMatx3.cpp

IVPM.o:IVPM.cpp
	g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -I/usr/include/c++/4.5.2 -O0 -g3 -Wall -fmessage-length=0 -std=c++0x -c IVPM.cpp

clean:
	rm -rf *o main
