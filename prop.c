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

/* compile using
gcc prop.c `pkg-config libusb-1.0 --libs --cflags`
*/

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

unsigned char get_temp(libusb_device_handle *);
uint16_t get_other(libusb_device_handle *);
void print_packet(libusb_device_handle *, char);

int main(int argc, char *argv[]){
  int c, p_flag=0;
  while ((c = getopt (argc, argv, "p")) != -1){
    switch (c)
      {
      case 'p':
	  p_flag=1;
	  break;
      default:
        exit(0);
      }
  }	
  libusb_device **dev_list;
  libusb_device_handle *dh;
  libusb_context *context = NULL;
  int status,i;
  unsigned char temp = 0 , old_temp = 0;
  uint16_t other_temp = 0, old_other_temp = 0;
  ssize_t count;
  bool claimed = false;
  status = libusb_init(&context);
  if(status < 0) {
    fprintf(stderr,"libusb init error\n");
    return 1;
  }
  libusb_set_debug(context, 3);
  count = libusb_get_device_list(context, &dev_list);
  if(count < 0) {
    fprintf(stderr,"get device list error\n");
  }
  printf("// found %d devices\n", (int)count);
  for(i = 0 ; i < count ; i++){
    struct libusb_device_descriptor descriptor;
    int status = libusb_get_device_descriptor(dev_list[i], &descriptor);
    if(status < 0){
      fprintf(stderr, "error getting descriptor\n");
      return 1;
    }
    if(0x8086 == descriptor.idVendor && 0x0a66 == descriptor.idProduct){
      printf("// found realsense camera\n");
      status = libusb_open(dev_list[i], &dh);		
      if(status < 0){
	fprintf(stderr, "failed to open device\n");
	return 1;
      } else {
	status = libusb_claim_interface(dh,4);
	if(status < 0){
	  fprintf(stderr, "could not claim interface\n");
	  return 1;
	} else {
	  claimed = true;
	  break;
	}
      }
    }
  }
  if(claimed){
    if(p_flag){
      print_packet(dh,0x12);
      print_packet(dh,0x3b);
      print_packet(dh,0x3d);
      print_packet(dh,0x52);
      print_packet(dh,0x0a);
      exit(0);
    }
    for(;;){
      old_temp = temp;
      temp = get_temp(dh);
      if(temp != old_temp){
	time_t now = time (NULL);
	char * time_string = asctime (gmtime (&now));
	int len = strlen(time_string);
	if(time_string[len - 1] == '\n') time_string[len - 1]=0;
	printf("%s ",time_string);
	printf("Temperature: %d\n", temp);
      }
      usleep(30000);
    }
  }
  libusb_close(dh);
  libusb_free_device_list(dev_list, 1);
  libusb_exit(context);
  return 0;
}
unsigned char get_temp(libusb_device_handle *dh){
  int transferred,status;

  unsigned char data[24] = {
    0x14, 0x00, 0xab, 0xcd, 0x52, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  unsigned char buffer[1024];
  status = libusb_bulk_transfer(dh,1,data,24,&transferred,0);
  if(status < 0){
    fprintf(stderr, "bulk out failed\n");
  }
  status = libusb_bulk_transfer(dh,0x81,buffer,1024,&transferred,0);
  if(status < 0){
    fprintf(stderr, "bulk in failed\n");
  } else {
    return buffer[4];
  }
}
uint16_t get_other(libusb_device_handle *dh){
  int transferred,status;
  uint16_t val = 0;

  unsigned char data[24] = {
    0x14, 0x00, 0xab, 0xcd, 0x0a, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  unsigned char buffer[1024];
  status = libusb_bulk_transfer(dh,1,data,24,&transferred,0);
  if(status < 0){
    fprintf(stderr, "bulk out failed\n");
  }
  status = libusb_bulk_transfer(dh,0x81,buffer,1024,&transferred,0);
  if(status < 0){
    fprintf(stderr, "bulk in failed\n");
  } else {
    val = buffer[5] << 8;
    val |= buffer[6];
    return val;
  }
}
void print_packet(libusb_device_handle *dh,char id){
  int transferred,status,i;

  unsigned char data[24] = {
    0x14, 0x00, 0xab, 0xcd, 0x52, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  data[4] = id;
  unsigned char buffer[1024];
  status = libusb_bulk_transfer(dh,1,data,24,&transferred,0);
  if(status < 0){
    fprintf(stderr, "bulk out failed\n");
  }
  status = libusb_bulk_transfer(dh,0x81,buffer,1024,&transferred,0);
  if(status < 0){
    fprintf(stderr, "bulk in failed\n");
  } else {
    printf("static const unsigned char pkt%02x[%d] = {\n",id,transferred);
    for(i=0;i<transferred;i++){
      printf("0x%02x",buffer[i]);
      if( i+1 == transferred ){
	printf("\n};\n");
      } else if( i % 8 == 7 ){
	printf(",\n");
      } else {
	printf(", ");
      }
    }
    return;
  }
}
