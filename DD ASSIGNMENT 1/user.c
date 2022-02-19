
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>
#include "header.h"
#include <stdint.h>



/*int ioctl_set_alignment(int file_desc,char align)
{
    int ret_val;

    ret_val = ioctl(file_desc, IOCTL_SET_ALIGN, align);

    if (ret_val < 0) {
        printf("ioctl select alignment failed:%d\n", ret_val);
        exit(-1);
    }
    return 0;
    }
*/
 
int main()
{
        int fd;
        uint32_t channel;
        int mode,i=0;
	static char align;
	uint16_t   var;
        printf("*********************************\n");
 
        printf("\nOpening Driver\n");
        fd = open("/dev/adc-dev", O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }
 
        printf("Enter Channel number\n");
        scanf("%d",&channel);
        
        ioctl(fd,IOCTL_SET_CHANNEL, (int32_t*) &channel); 
        
        int fflush(FILE *stream);
        
 	printf("Select alignment of the adc result (l or r):\n");
	scanf(" %c",&align);
	
   	

	if((channel<0)||(channel>7)||(align!='l' && align!='r'))
	     {
		printf("Invalid data inserted\n");
		exit(-1);
	      }
	      
	      
	      //ioctl_set_alignment(fd,align);
	ioctl(fd,IOCTL_SET_ALIGN, (char*)&align);

	int fflush(FILE *stream);
	
	if(read( fd,&var,sizeof(var)))
	 {

	if (align=='l'){

	printf("The received value from adc  is %i\n",var);
	var=var/16;
	printf("The actual 12 bit result from adc  is %i\n",var);
	
	}
	else {
		printf("The actual 12 bit result from adc  is %i\n",var);
		
		}
	}
	printf("Enter the Conversion Mode Number (0:Continous Conversion Mode, 1:Single Shot Conversion Mode):-\n");
    	scanf("%d",&mode);
    	
    	if(mode==1){
    	read( fd,&var,sizeof(var));
    	
    	}
    	else if (mode==0){
    	
    	while(i<10)
    	read( fd,&var,sizeof(var));
    	}
    	
     
     
	ioctl(fd, IOCTL_SET_CONVERSION_MODE, (uint32_t*) &mode);
 	
        printf("Closing Driver\n");
        close(fd);
}
