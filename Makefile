all:
	g++ -O2 -o route  left_edge.cpp -lcairo -I/usr/include/cairo `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`
#	g++ `pkg-config --cflags gtk+-3.0` -o Routing  left_edge.cpp  `pkg-config --libs gtk+-3.0`
#	g++ -o Routing  left_edge.cpp -lcairo -I/usr/include/cairo `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
debug:
	g++ -g -o route  left_edge.cpp -lcairo -I/usr/include/cairo `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`
clean:
	rm -rf route

