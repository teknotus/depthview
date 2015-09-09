
all: camera find-cameras prop

camera: camera.c
	g++ camera.c `pkg-config --cflags --libs opencv` -o camera

find-cameras: find_cameras.c
	gcc find_cameras.c -ludev -o find-cameras

prop: prop.c
	gcc prop.c `pkg-config libusb-1.0 --libs --cflags` -o prop

