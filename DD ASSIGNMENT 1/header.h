#ifndef header_H
#define header_H

#include <linux/ioctl.h>

#define MAJOR_NUM 241


#define IOCTL_SET_CHANNEL _IOR(MAJOR_NUM, 1, int *)


#define IOCTL_SET_ALIGN _IOR(MAJOR_NUM, 2, char *)

#define IOCTL_SET_CONVERSION_MODE _IOR(MAJOR_NUM, 3, int *)




#endif
