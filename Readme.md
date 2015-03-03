# Linux Kernal patch

Adds support for video pixel formats of intel real sense F200 camera.

With this patch selecting infrared, and combined depth/IR pixel formats becomes possible with v4l2 API. Combined formats are pixels with 16 bits of depth followed by 8 bits of infrared. 

    $ v4l2-ctl --device /dev/video2 --list-formats
    ioctl: VIDIOC_ENUM_FMT
	Index       : 0
	Type        : Video Capture
	Pixel Format: 'YUYV'
	Name        : YUV 4:2:2 (YUYV)

	Index       : 1
	Type        : Video Capture
	Pixel Format: 'INVZ'
	Name        : Depth 16-bit (INVZ)

	Index       : 2
	Type        : Video Capture
	Pixel Format: 'INZI'
	Name        : Depth:IR 16:8 24-bit (INZI)

	Index       : 3
	Type        : Video Capture
	Pixel Format: 'INVR'
	Name        : Depth 16-bit (INVR)

	Index       : 4
	Type        : Video Capture
	Pixel Format: 'INRI'
	Name        : Depth:IR 16:8 24-bit (INRI)

	Index       : 5
	Type        : Video Capture
	Pixel Format: 'INVI'
	Name        : Infrared 8-bit (INVI)

	Index       : 6
	Type        : Video Capture
	Pixel Format: 'RELI'
	Name        : FlickerIR 8-bit (RELI)

# Installing

Download Linux source tree

## Get Linux source debian/ubuntu

    sudo apt-get build-dep linux
    apt-get source linux-image-$(uname -r)

## Alternatively get source from git

    git clone https://github.com/torvalds/linux.git

# Apply patch

    cd linux
    git apply ../0001-Add-video-formats-for-Intel-real-sense-F200-camera.patch

# Copy header to system include directory

File is in package linux-libc-dev if you want to get back original

    sudo cp include/uapi/linux/videodev2.h /usr/include/linux/videodev2.h

# Compile

    (cd drivers/media/usb/uvc/ && make -C /usr/src/linux-headers-`uname -r` M=`pwd` modules)

# Install into module extra directory

    (cd drivers/media/usb/uvc/ && sudo make -C /usr/src/linux-headers-`uname -r` M=`pwd` modules_install)

# test module

    sudo rmmod uvcvideo
    sudo insmod /lib/modules/`uname -r`/extra/uvcvideo.ko

# install module

    sudo cp /lib/modules/`uname -r`/extra/uvcvideo.ko /lib/modules/`uname -r`/kernel/drivers/media/usb/uvc/uvcvideo.ko
