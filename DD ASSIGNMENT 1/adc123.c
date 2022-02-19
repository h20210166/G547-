
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/ioctl.h>
#include<linux/random.h>
#include "header.h"

 static char align = 'r'; // intialization of caracter variable for alignment
dev_t dev = 0;				// declaration of dev_t for major number minor number
static struct class *dev_class;	//declaration of class 		
static struct cdev etx_cdev;		//decleration of cdev structure
uint32_t ch=0;				// decleration of variable for ioctl call of select channel
int mode,i=0;				//decleration of variables for ioctl mode selection and loop
/*
** Function Prototypes
*/
static int      __init etx_driver_init(void);					
static void     __exit etx_driver_exit(void);
static int      adc_open(struct inode *inode, struct file *file);
static int      adc_release(struct inode *inode, struct file *file);
static ssize_t  adc_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
//static ssize_t  adc_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long     adc_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

//---------------------------------------------------------------------------------##LINKING the user space and kernel space##-------------------------------------------------------

/* Module Declarations */

/*
 * This structure will hold the functions to be called
 * when a process does something to the device we
 * created.In order to use IOCTL call we need to include that in this file operation structure.
 */

static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = adc_read,
        .open           = adc_open,
        .unlocked_ioctl = adc_ioctl,
        .release        = adc_release,
};

//-----------------------------------------------------------------------------device-open()------------------------------------------------------------------------------

/*
 * This is called whenever a process attempts to open the device file
 */
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int adc_open(struct inode *inode, struct file *file)
{
        pr_info("Device File Opened...!!!\n");
        return 0;
}

//------------------------------------------------------------------------------------------------device_release--------------------------------------------------------------
static int adc_release(struct inode *inode, struct file *file)
{
        pr_info("Device File Closed...!!!\n");
        return 0;
}

//-----------------------------------------------------------------------------device-read()------------------------------------------------------------------------------
/*
 * This function is called whenever a process which has already opened the
 * device file attempts to read from it.
 */
 //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static ssize_t adc_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{



 	uint16_t random;								//random number generator
	uint16_t gen;
	get_random_bytes(&random, sizeof(random));
        gen= random%4096;
	
	printk(KERN_INFO "The random no generated is :%d\n",gen);
	if(align== 'l' || 'r'){
	
	if(align=='l')
	{
		gen= gen*16;
	}
	else gen=gen;
}

	else if(mode ==0 || mode==1){
	
	if(mode==0)
	{
	if(align=='l')
	{
		gen= gen*16;
	}
	else gen=gen;
	}
	 else if(mode ==1){
	 while(i<10){
	 if(align== 'l' || 'r'){
	
	if(align=='r')
	{
		gen= gen*64;
	}
	else gen=gen;
}

	 
}
	
	}
}

	copy_to_user(buf,&gen,sizeof(gen));
	return (sizeof(gen));
       
}



//-----------------------------------------------------------------------------------device_ioctl()--------------------------------------------------------------

/*
 * This function is called whenever a process tries to do an ioctl on our device file. We get two extra parameters (additional to the inode and file
 * structures, which all device functions get): the number of the ioctl called  and the parameter given to the ioctl function.
 *
 * If the ioctl is write or read/write (meaning output is returned to the calling process), the ioctl call returns the output of this function.
 *
 */
 
 //----------------------------------------------------------------------------------------------------------------------------------------------------------------
long adc_ioctl(struct file *file,unsigned int ioctl_num,unsigned long ioctl_param){




                switch(ioctl_num)					//switch case in order to map the ioctl calll of user with in kernel
                
                {
                
        /*
         * This ioctl is usued to select the channel of adc
        */
                
                  case IOCTL_SET_CHANNEL:
			
			if( copy_from_user(&ch ,(int32_t*) ioctl_param, sizeof(ch)) )
                        {
                                pr_err("Data Write : Err!\n");
                        }
                        printk(KERN_INFO"Channel NUmber selected is: %d \n",ch);
                        break;
        
        /*
         * This ioctl is usued to select alignment of adc data register
        */    
           
           
                  case IOCTL_SET_ALIGN:
                  
                  
                  if( copy_from_user(&align,(char*) ioctl_param, sizeof(align)) )
                        {
                                pr_err("Data Write : Err!\n");
                        }
                        
                    	align = (char )ioctl_param;
                    	
			//printk(KERN_INFO "current request code from alignment is %u",ioctl_num);
			
			
                    	printk(KERN_INFO "Alignment slected for 16 bit data is : %c",align);
                    	
                    	
			break;
			
    	/*
         * This ioctl is usued to select conversion mode of adc either one shot convresion or conitnious conversion
        */           
              
    		   case IOCTL_SET_CONVERSION_MODE:
    		
    		
    		   if( copy_from_user(&mode,(uint32_t*) ioctl_param, sizeof(mode)) )
         
                        {
                                pr_err("Data Write : Err!\n");
                        }
         
                       // if((int)ioctl_param)
                       // while ()
         
                        	mode =(int)ioctl_param;
    	
    		   	printk(KERN_INFO"Mode Selecetd is :%d",mode);
        
	
     		   break;
    }

    return 0;
}
 
//----------------------------------------------------------------/ ######### INITIALIZATION FUNCTION ##########-------------------------------------------------------
/*
 * Initialize the module - Register the character device
 */
// STEP 1: Allocating Major number
// STEP 2: Making a cdev structrue and linking it with inode and file structure
// STEP 3: Creating device file using class and device structre 
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

static int __init etx_driver_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "Dev")) <0){
                pr_err("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        /*Creating cdev structure*/
        cdev_init(&etx_cdev,&fops);
 
        /*Adding character device to the system*/
        if((cdev_add(&etx_cdev,dev,1)) < 0){
            pr_err("Cannot add the device to the system\n");
            goto r_class;
        }
 
        /*Creating struct class*/
        if((dev_class = class_create(THIS_MODULE,"class")) == NULL){
            pr_err("Cannot create the struct class\n");
            goto r_class;
        }
 
        /*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"adc-dev")) == NULL){
            pr_err("Cannot create the Device 1\n");
            goto r_device;
        }
        pr_info("Device Driver Insert...Done!!!\n");
        return 0;
 
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}

//---------------------------------------------------------------------------------MODUL EXIT CODE --------------------------------------------------------------------------------
static void __exit etx_driver_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&etx_cdev);
        unregister_chrdev_region(dev, 1);
        pr_info("Device Driver Remove...Done!!!\n");
}
 
 
 //-----------------------------------------------------------------------------MODULE INFORMATION AND LICENSES-------------------------------------------------------------------------
module_init(etx_driver_init);
module_exit(etx_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arpit Shukla");

MODULE_DESCRIPTION("Assignment 1: LINUX Character device driverusing IOCTL");

