# 3D Camera tools

New laptops are currently shipping with 3D webcams. The official SDK from intel only supports windows. 

This is a collection of small applications I wrote to work out details for a userspace library for realsense cameras. They are as of this moment works in progress, but still very useful as starting points.

camera.c is a viewer that is a much smaller codebase than depthview, and thus easier to understand. At the moment I am hundreds of miles away from my camera, and so I edited it to work with my webcam so I could test to make sure it worked before publishing. Note it seem to only work with the F200 once using the epoll method of fetching frames for unknown reasons. It needs to be reset before it can be used again using epoll. Change the main loop to use a timer instead of an epoll event for reliable operation. On my webcam it works reliably with epoll so it is specific to the F200 camera, and not purely a bug in my code.

find_cameras.c uses the libudev from the systemd project to find all attached cameras, and list enough information to uniquely identify a realsense camera by serial number, and figure out which /dev files are the color, and depth cameras. It finds both camera attached to the computer before it is run, and cameras attached or detached while running. 

prop.c will by default display what it thinks is the temperature from the cameraevery time it changes. It can also with the -p flag dump all of the binary blobs of data I currently know how to fetch from the camera. There are potentially more, and changing the request byte is sufficient to ask for them, but I have no idea if it's possible to brick a camera by poking around with these. You have been warned. The blobs it fetches by default work fine on my camera. It's suspected that at least one of them contains factory calibration. The dumps are in the form of c source character arrays in a format suitible to name the output file with a .h and include it in another program. 

See blog posts about it.  
[First investigation](http://solsticlipse.com/2015/01/09/intel-real-sense-camera-on-linux.html)  
[Camera Controls](http://solsticlipse.com/2015/02/10/intel-real-sense-on-linux-part-2-3d-camera-controls.html)  
[3d model export](http://solsticlipse.com/2015/03/31/intel-real-sense-3d-on-linux-macos.html)  

The camera.c probably needs requires a linux kernel patch to work fully.
Available as another branch on the same repo.
https://github.com/teknotus/depthview/tree/kernelpatchfmt

The other programs do not access the depth data, and do not require kernel patches to run. 
