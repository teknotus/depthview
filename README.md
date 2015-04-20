# 3D Camera viewer

New laptops will soon be shipping with 3D webcams. The official SDK from intel only supports windows.
This is a tool to get the data feed, and show video on Linux. 

Old Demo  
https://www.youtube.com/watch?v=Ht9PzVjWOgI

See blog posts about it.  
[First investigation](http://solsticlipse.com/2015/01/09/intel-real-sense-camera-on-linux.html)  
[Camera Controls](http://solsticlipse.com/2015/02/10/intel-real-sense-on-linux-part-2-3d-camera-controls.html)  
[3d model export](http://solsticlipse.com/2015/03/31/intel-real-sense-3d-on-linux-macos.html)  

This branch currently requires a linux kernel patch to work.
Available as another branch on the same repo.
https://github.com/teknotus/depthview/tree/kernelpatchfmt

Prepatched Linux kernel available from [Ubuntu 14.10 PPA](https://launchpad.net/~teknotus/+archive/ubuntu/rs-kernel-ppa)

    sudo apt-add-repository ppa:teknotus/rs-kernel-ppa

There is also a userspace driver that works with the camera that I hope to add support for soon. 

https://github.com/mcguire-steve/libuvc

