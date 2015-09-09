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

/* To Compile
g++ camera.c `pkg-config --cflags --libs opencv`
*/

#include <linux/videodev2.h>
#include <linux/uvcvideo.h>
#include <linux/usb/video.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <sys/epoll.h>
#include <signal.h>
#define MAX_EVENTS 10

enum pixel_format {
  YUYV = 0x56595559,
  INVZ = 0x5a564e49,
  INZI = 0x495a4e49,
  INVR = 0x52564e49,
  INRI = 0x49524e49,
  INVI = 0x49564e49,
  RELI = 0x494c4552
};

struct camera {
  int fd;
  __u32 buffercount;
  struct v4l2_buffer * buffers;
  struct v4l2_requestbuffers request_buffers;
  void * ptr;
};

void list_formats(int);
void set_format_simple(int, __u32, __u32, __u32);
void buffer_init(struct camera *);
void buffer_destroy(struct camera *);
void image_callback(__u8 *,const void *);

void camstart(struct camera *,const char *);
void camstop(struct camera *);
void sig_then(int, siginfo_t *, void *);

volatile sig_atomic_t el_continue = 1; /* event loop continue */

int main(void){
  struct sigaction sa;
  memset(&sa,0,sizeof(sa));
  sa.sa_sigaction = sig_then;
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGTERM, &sa, NULL);
  sigaction(SIGINT, &sa, NULL);
  
  struct camera cam, cam1;
  const char * vid0 = "/dev/video0";
  const char * vid1 = "/dev/video1";
  camstart(&cam,vid0);
  //camstart(&cam1,vid1);

  cam.ptr = (void *)"my webcam";
  //cam1.ptr = (void *)"rs color";
  cv::namedWindow((const char *)cam.ptr, CV_WINDOW_AUTOSIZE);
  //cv::namedWindow((const char *)cam1.ptr, CV_WINDOW_AUTOSIZE);

  int epoll_fd;
  struct epoll_event event, events[MAX_EVENTS];
  epoll_fd = epoll_create1(0);
  if(epoll_fd == -1){
    perror("epoll_create");
  }

  memset(&event,0,sizeof(struct epoll_event));
  event.events = EPOLLIN;
  event.data.ptr = &cam;
  if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,cam.fd,&event) == -1){
    perror("EPOLL_CTL_ADD fd");
  }
  /*
  memset(&event,0,sizeof(struct epoll_event));
  event.events = EPOLLIN;
  event.data.ptr = &cam1;
  if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,cam1.fd,&event) == -1){
    perror("EPOLL_CTL_ADD fd1");
  }
  */
  struct v4l2_buffer dqbuf;

  while(el_continue){
    int evcount = 0;
    int i;
    evcount = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if(evcount < 0){
      perror("evcount < 0");
    } else {
      printf("got %d events!\n", evcount);
      for (i = 0; i < evcount; i++) {
	struct camera * my_cam = (struct camera *)events[i].data.ptr;
	memset(&dqbuf, 0, sizeof(dqbuf));
	dqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; // cannot fetch buffer
	dqbuf.memory = V4L2_MEMORY_MMAP;          // without these fields set

	// get info of buffer that is oldest in queue "dequeue buffer"
	if (-1 == ioctl(my_cam->fd, VIDIOC_DQBUF, &dqbuf)) {
	  if(errno == EAGAIN){
	    fprintf(stderr, "nope!\n");
	    // frame not ready yet
	  } else if(errno == ENODEV){
	    // stop everything?
	  } else {
	    perror("readbuf");
	  }
	} else {
	  printf("buffer processing\n");
	  // VIDIOC_DQBUF doesn't actually return pointer to data when using mmap
	  // Structure has index that can be used to look up pointer
	  __u8 * data = (__u8 *)my_cam->buffers[dqbuf.index].m.userptr;
	  image_callback(data, (const char *)my_cam->ptr);

	  // tell driver it can reuse framebuffer
	  ioctl(my_cam->fd, VIDIOC_QBUF, &dqbuf);
	}
      }
    }
  }
  //camstop(&cam);
  if(epoll_ctl(epoll_fd,EPOLL_CTL_DEL,cam.fd,&event) == -1){
    perror("EPOLL_CTL_DEL fd1");
  }
  close(epoll_fd);
  camstop(&cam);
  return 0;
}

void sig_then(int signum, siginfo_t *info, void *ptr){
  printf("got signal\n");
  el_continue = 0;
}

void camstart(struct camera * cam, const char * dev){
  int fd = open(dev, O_NONBLOCK | O_RDWR);
  if (fd == -1)
    {
      perror("opening device");
    }

  list_formats(fd);
  set_format_simple(fd, YUYV, 640, 480);
  memset(cam, 0, sizeof(struct camera));
  cam->fd = fd;
  buffer_init(cam);

  if (-1 == ioctl(fd, VIDIOC_STREAMON, &cam->request_buffers.type)){
    perror("start video stream");
  }
}
void camstop(struct camera * cam){
  printf("shutting down camera\n");
  if (-1 == ioctl(cam->fd,VIDIOC_STREAMOFF, &cam->request_buffers.type)){
    perror("stop video stream");
  }
  buffer_destroy(cam);
  if(-1 == close(cam->fd)){
    perror("closing device");
  }
}

void buffer_init(struct camera * cam){
  int fd = cam->fd;
  __u32 buffercount;
  struct v4l2_buffer * buffers;
  struct v4l2_requestbuffers request_buffers;
  unsigned int i;
  buffercount = 2; // Double buffer

  memset(&request_buffers, 0, sizeof(request_buffers));
  request_buffers.count = buffercount;
  request_buffers.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  request_buffers.memory = V4L2_MEMORY_MMAP;

  // this allocates memory, but does not return a pointer to it
  if (-1 == ioctl (fd, VIDIOC_REQBUFS, &request_buffers)) {
    perror("getting buffers");
  }

  buffers = (struct v4l2_buffer *) calloc(buffercount,sizeof(struct v4l2_buffer));
  if(!buffers){
    fprintf(stderr, "failed to calloc buffers\n");
  }
  for (i = 0; i < request_buffers.count; i++) {
    struct v4l2_buffer buffer;
    memset(&buffer, 0, sizeof(buffer));
    buffer.type = request_buffers.type;
    buffer.memory = request_buffers.memory;
    buffer.index = i;

    // Sets buffer.length and buffer.m.offset
    if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buffer)) {
      perror("did not get buffer parameters");
    }
    void * map = mmap(NULL,                   // Kernel picks address
		      buffer.length,          // Driver dictates size
		      PROT_READ | PROT_WRITE, // Must match open flags
		      MAP_SHARED,             // Allows forking
		      fd,
		      buffer.m.offset);       // Driver dictated

    // use pointer field in buffer to store mmap pointer location
    // needed both for retrieving data, and freeing memory
    buffers[i] = buffer;
    buffers[i].m.userptr = (long unsigned int)map;
    if (MAP_FAILED == map) { // if mmap fail unmap all buffers
      perror("mmap");
      for( ; i >= 1 ; i-- ){
	if (-1 == munmap((void *)buffers[i -1].m.userptr, buffers[i -1].length)){
	  perror("munmap");
	}
      }
      // TODO free buffers when mmap fails
    }
    if(-1 == ioctl(fd, VIDIOC_QBUF, &buffer)){
      perror("qbuf");
    }
  }
  cam->buffercount = buffercount;
  cam->buffers = buffers;
  cam->request_buffers = request_buffers;
}

void buffer_destroy(struct camera * cam){
    bool errors = false;
    for (unsigned int i = 0; i < cam->buffercount ; i++) {
        v4l2_buffer buffer = cam->buffers[i];
        if (-1 == munmap((void *)buffer.m.userptr, buffer.length)){
            perror("munmap");
            errors = true;
        }
    }

    memset(&cam->request_buffers, 0, sizeof(cam->request_buffers));
    cam->request_buffers.count = 0; // Requesting zero buffers frees them
    cam->request_buffers.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    cam->request_buffers.memory = V4L2_MEMORY_MMAP;

    if (-1 == ioctl (cam->fd, VIDIOC_REQBUFS, &cam->request_buffers)) {
        perror("freeing buffers");
    }

    free(cam->buffers);
}

void image_callback(__u8 * data, const void * user_ptr){
  cv::Mat color_cv, color_cv_rgb;
  color_cv = cv::Mat(480,640, CV_8UC2, data);
  cv::cvtColor(color_cv,color_cv_rgb,CV_YUV2RGB_YVYU);
  cv::imshow((const char *)user_ptr, color_cv_rgb);
  cvWaitKey(1);
}
void set_format_simple(int fd, __u32 fourcc, __u32 width, __u32 height){
  struct v4l2_format pixFormat;
  memset(&pixFormat, 0, sizeof(pixFormat));       // Clear struct
  pixFormat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;   // Required
  if (-1 == ioctl(fd, VIDIOC_G_FMT, &pixFormat)){ // Get current video format
    perror("getting format");
  }
  //pixFormat.fmt.pix.pixelformat = fourcc;
  pixFormat.fmt.pix.width       = width;
  pixFormat.fmt.pix.height      = height;
  if (-1 == ioctl(fd, VIDIOC_S_FMT, &pixFormat)){ // Set video format
    perror("setting format");
  }
}

void list_formats(int fd){
  struct v4l2_fmtdesc fmtdesc;
  int i, trys;
  i = 0;
  __u8 * pixfmt = (__u8 *)calloc(5, sizeof(__u8));
  pixfmt[4] = 0;
  trys = 255;
  while(trys-- > 0){
    memset(&fmtdesc, 0, sizeof(fmtdesc));
    fmtdesc.index = i;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc)){  // Get video format
      perror("enum fmt");
      break;
    }
    *((__u32 *)pixfmt) = fmtdesc.pixelformat;
    printf("index: %d format: %s description: %s \n",
	   i, pixfmt, fmtdesc.description);
    i++;
  }
  free(pixfmt);
}
