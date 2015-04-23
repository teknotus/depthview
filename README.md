# Define Camera Controls

The Linux kernel handles vendor extensions to UVC (USB Camera) via definitions added dynamically from userspace. This program loads definitions for the controls on Intel RealSense 3d camera F200. There is a udev rule to load the control definitions as soon as the camera is plugged in. 

Without the definitions the controls need to be accessed by index. With the definitions any program can ask the kernel for a list of all controls with user friendly names. 

After this program has been run v4l2-ctl can list, and use all of the camera controls. 

    $ v4l2-ctl --device=/dev/video2 --list-ctrls-menus
                    laser_power (int)    : min=0 max=16 step=1 default=16 value=16
                        pattern (menu)   : min=0 max=2 default=1 value=1
				0: 11 patterns 50fps
				1: 10 patterns 55fps
				2: 9 patterns 60fps
                       exposure (int)    : min=0 max=100 step=1 default=0 value=0
                         filter (menu)   : min=0 max=7 default=5 value=5
				0: Skeleton
				1: Raw
				2: Raw + Gradiants
				3: Range optimized < 350mm
				4: Range optimized 350mm - 550mm
				5: Range optimized 550mm - 850mm
				6: Range optimized 850mm - 1000mm
				7: Range optimized > 1000mm
           confidence_threshold (int)    : min=0 max=15 step=1 default=6 value=6

For example the program "vlc" picks up these definitions automatically, and is able to set all of the controls from a GUI available at.

tools-> Effects and filters-> v4l2 controls

The video is green because vlc does not understand the pixel formats, but it can control the other settings.

# Install

    make
    sudo make install

# Usage

The program can be used without installing by providing the device filename as the first argument. If it's installed with udev it should run automatically when the USB cable is plugged in. 

