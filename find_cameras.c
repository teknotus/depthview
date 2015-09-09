/*
MIT License
Copyright (c) 2015 Daniel Patrick Johnson <teknotus@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:
.
The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.
.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* compile using */
/* gcc find_cameras.c -ludev */

#include <stdio.h>
#include <string.h>
#include <libudev.h>
#include <sys/epoll.h>
#define MAX_EVENTS 10

void get_camera_properties(struct udev_device *);
void print_everything(struct udev_device *);
void print_attributes(struct udev_device *);
void print_properties(struct udev_device *);
struct camera_properties {
  char * device_file;
  char * v4l_syspath;
  char * interface_syspath;
  char * device_syspath;
  char * vid;
  char * pid;
  char * bcdDevice;
  char * serial;
  char * bInterfaceNumber;
};

int main(void){
  struct udev *udev;
  struct udev_monitor *udev_monitor;
  int udev_fd,epoll_fd;
  int i;
  struct udev_enumerate *camera_enum;
  struct udev_list_entry *camera_list, *camera_list_item;
  struct udev_device *camera;
  struct epoll_event event, events[MAX_EVENTS];

  udev = udev_new();
  if (!udev){
    fprintf(stderr,"so much fail!");
    return 1;
  }

  udev_monitor = udev_monitor_new_from_netlink(udev, "udev");
  udev_monitor_filter_add_match_subsystem_devtype(udev_monitor, "video4linux", NULL);
  udev_monitor_enable_receiving(udev_monitor);
  udev_fd = udev_monitor_get_fd(udev_monitor);
  epoll_fd = epoll_create1(0);
  if(epoll_fd == -1){
    perror("epoll_create");
  }
  memset(&event,0,sizeof(struct epoll_event));
  event.events = EPOLLIN;
  event.data.fd = udev_fd;
  if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,udev_fd,&event) == -1){
    perror("EPOLL_CTL_ADD udev_fd");
  }

  camera_enum = udev_enumerate_new(udev);
  udev_enumerate_add_match_subsystem(camera_enum, "video4linux");
  udev_enumerate_scan_devices(camera_enum);
  camera_list = udev_enumerate_get_list_entry(camera_enum);
  udev_list_entry_foreach(camera_list_item, camera_list) {
    const char *syspath;
    syspath = udev_list_entry_get_name(camera_list_item);
    camera = udev_device_new_from_syspath(udev,syspath);
    get_camera_properties(camera);
    /* print_everything(camera); */
    udev_device_unref(camera);
  }
  udev_enumerate_unref(camera_enum);
  for(;;){
    int evcount = 0;
    evcount = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if(evcount < 0){
      perror("evcount < 0");
    } else {
      printf("got %d events!\n", evcount);
      for (i = 0; i < evcount; i++) {
	struct udev_device *dev;
	dev = udev_monitor_receive_device(udev_monitor);
	get_camera_properties(dev);
	/* print_everything(dev); */
	udev_device_unref(dev);
      }
    }
  }
  return 0;
}

void get_camera_properties(struct udev_device * camera){
  struct udev_device *camera_usb_device,*camera_uvc_device;
  printf("action: %s\n", udev_device_get_action(camera));
    camera_uvc_device = udev_device_get_parent_with_subsystem_devtype
      (camera, "usb", "usb_interface");
    if(!camera_uvc_device){
      fprintf(stderr, "find uvc parent fail!");
    }
    camera_usb_device = udev_device_get_parent_with_subsystem_devtype
      (camera, "usb", "usb_device");
    if (!camera_usb_device) {
      fprintf(stderr, "find usb parent fail!");
    }
    printf("device file: %s\n",udev_device_get_devnode(camera));
    printf("v4l syspath: %s\n",udev_device_get_syspath(camera));
    printf("interface syspath: %s\n", udev_device_get_syspath(camera_uvc_device));
    printf("usb syspath: %s\n", udev_device_get_syspath(camera_usb_device));
    printf("%s:%s serial: %s bcdDevice: %s bInterfaceNumber: %s\n",
	   udev_device_get_sysattr_value(camera_usb_device, "idVendor"),
	   udev_device_get_sysattr_value(camera_usb_device, "idProduct"),
	   udev_device_get_sysattr_value(camera_usb_device, "serial"),
	   udev_device_get_sysattr_value(camera_usb_device, "bcdDevice"),
	   udev_device_get_sysattr_value(camera_uvc_device, "bInterfaceNumber")
	   );
}

void print_everything(struct udev_device * device){
    while(device){
      printf("\nsyspath: %s", udev_device_get_syspath(device));
      printf("\n---\nattributes\n---\n");
      print_attributes(device);
      printf("\nproperties\n---\n");
      print_properties(device);
      device = udev_device_get_parent(device);
    }
}

void print_attributes(struct udev_device * device){
  struct udev_list_entry *list, *list_item;
    list = udev_device_get_sysattr_list_entry(device);
    udev_list_entry_foreach(list_item, list){
	const char * name = udev_list_entry_get_name(list_item);
	const char * value =  udev_device_get_sysattr_value
	  (device, name);
	printf("%s: %s\n", name, value);
    }
}

void print_properties(struct udev_device * device){
  struct udev_list_entry *list, *list_item;
    list = udev_device_get_properties_list_entry(device);
    udev_list_entry_foreach(list_item, list){
	const char * name = udev_list_entry_get_name(list_item);
	const char * value =  udev_device_get_property_value
	  (device, name);
	printf("%s: %s\n", name, value);
    }
}
