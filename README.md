# Channel_Routing
Channel routing is a special routing problem. Wires are connected within a rectangular
routing region, denoted as channel. All pins are located along the opposite sides of the
channel. The cost/area of a routing channel is represented by the number of required routing
tracks inside the channel. Conventionally, the channel is oriented horizontally and all pins are
on the top and bottom of the channel. In this project, you are required to implement a 3-layer
(VHV model) channel router according to the following requirements:

1. Read a pin list file with at most 10,000 pins in each side of a channel.
2. Perform left-edge channel routing algorithm.
3. Report the number of required tracks in the channel.
4. Create an X window based graphical display for your routing result.
5. Create necessary user interface to access the graphical display.
6. Output your routing result in (at least) one graphics file format.
https://en.wikipedia.org/wiki/Image_file_formats
7. Upload your source code tarball (*.tgz) to moodle (including your Makefile).
(NOTE: The uploaded file name should be the same with your student ID.)


Pin list Example: sample.pin \
0 1 6 1 2 5 3 5 \
6 3 5 6 0 4 2 4 \
SYNOPSIS \
%> route PIN_LIST_FILE \
Run-time Example: \
%> route sample.pin \
Number of tracks: 4 \
%> ls \
route sample.pin sample.eps sample.png 
