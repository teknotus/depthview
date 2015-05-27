#include <linux/videodev2.h>
#include <linux/uvcvideo.h>
/* #include <linux/usb/video.h> */
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
/* #include <errno.h> */
/* #include <unistd.h> */

#define REALSENSE_LASER_CTRL V4L2_CID_PRIVATE_BASE
#define REALSENSE_PATTERN_CTRL (V4L2_CID_PRIVATE_BASE + 1)
#define REALSENSE_EXPOSURE_CTRL (V4L2_CID_PRIVATE_BASE + 2)
#define REALSENSE_FILTER_CTRL (V4L2_CID_PRIVATE_BASE + 3)
#define REALSENSE_CONFIDENCE_CTRL (V4L2_CID_PRIVATE_BASE + 4)

#define REALSENSE_EXTENSION_GUID { \
      0xa1, 0x51, 0x57, 0xa5, 0xc5, 0xf3, 0x5e, 0x4a, \
      0x8d, 0x5a, 0x68, 0x54, 0xb8, 0xfa, 0x27, 0x16 }

int main(int argc, char *argv[]){
  int fd = open(argv[1], O_NONBLOCK | O_RDWR);
  int i;
  if (fd == -1)
    {
      perror("opening device");
      return 0;
    }
  struct uvc_menu_info pattern_menu[] = {
    {
      .name = "11 patterns 50fps",
      .value = 1
    },
    {
      .name = "10 patterns 55fps",
      .value = 2
    },
    {
      .name = "9 patterns 60fps",
      .value = 3
    }
  };
  struct uvc_menu_info filter_menu[] = {
    {
      .name = "Skeleton",
      .value = 0
    },
    {
      .name = "Raw",
      .value = 1
    },
    {
      .name = "Raw + Gradiants",
      .value = 2
    },
    {
      .name = "Range optimized < 350mm",
      .value = 3
    },
    {
      .name = "Range optimized 350mm - 550mm",
      .value = 4
    },
    {
      .name = "Range optimized 550mm - 850mm",
      .value = 5
    },
    {
      .name = "Range optimized 850mm - 1000mm",
      .value = 6
    },
    {
      .name = "Range optimized > 1000mm",
      .value = 7
    }
  };

  struct uvc_xu_control_mapping mapping[] = {
    {
      .id = REALSENSE_LASER_CTRL,
      .name = "Laser Power",
      .entity = REALSENSE_EXTENSION_GUID,
      .selector = 1,
      .size = 8,
      .offset = 0,
      .v4l2_type = V4L2_CTRL_TYPE_INTEGER,
      .data_type = UVC_CTRL_DATA_TYPE_UNSIGNED
    },
    {
      .id = REALSENSE_PATTERN_CTRL,
      .name = "Pattern",
      .entity = REALSENSE_EXTENSION_GUID,
      .selector = 2,
      .size = 8,
      .offset = 0,
      .v4l2_type = V4L2_CTRL_TYPE_MENU,
      .data_type = UVC_CTRL_DATA_TYPE_UNSIGNED,
      .menu_info = pattern_menu,
      .menu_count = 3
    },
    {
      .id = REALSENSE_EXPOSURE_CTRL,
      .name = "Exposure",
      .entity = REALSENSE_EXTENSION_GUID,
      .selector = 3,
      .size = 8,
      .offset = 0,
      .v4l2_type = V4L2_CTRL_TYPE_INTEGER,
      .data_type = UVC_CTRL_DATA_TYPE_UNSIGNED
    },
    {
      .id = REALSENSE_FILTER_CTRL,
      .name = "Filter",
      .entity = REALSENSE_EXTENSION_GUID,
      .selector = 5,
      .size = 8,
      .offset = 0,
      .v4l2_type = V4L2_CTRL_TYPE_MENU,
      .data_type = UVC_CTRL_DATA_TYPE_UNSIGNED,
      .menu_info = filter_menu,
      .menu_count = 8
    },
    {
      .id = REALSENSE_CONFIDENCE_CTRL,
      .name = "Confidence Threshold",
      .entity = REALSENSE_EXTENSION_GUID,
      .selector = 6,
      .size = 8,
      .offset = 0,
      .v4l2_type = V4L2_CTRL_TYPE_INTEGER,
      .data_type = UVC_CTRL_DATA_TYPE_UNSIGNED
    },
  };
  for(i = 0 ; i < 5 ; i++){
    if(-1 == ioctl(fd,UVCIOC_CTRL_MAP,&mapping[i])){
      perror("mapping");
    }
  }
  return 0;
}
