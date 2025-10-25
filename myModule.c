/***************************************************************************//**
*  \file       hello_world.c
*
*  \details    Simple hello world driver
*
*  \author     EmbeTronicX
*
* *******************************************************************************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/device.h>
#include<linux/err.h>
#include<linux/cdev.h>

int valueETX, arr_valueETX[4];
char *nameETX;
int cb_valueETX = 0;

dev_t dev =0;

module_param(valueETX, int, S_IRUSR|S_IWUSR);                      //integer value
module_param(nameETX, charp, S_IRUSR|S_IWUSR);                     //String
module_param_array(arr_valueETX, int, NULL, S_IRUSR|S_IWUSR);      //Array of integers

static int      etx_open(struct inode *inode, struct file *file);
static int      etx_release(struct inode *inode, struct file *file);
static ssize_t  etx_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  etx_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = etx_read,
    .write      = etx_write,
    .open       = etx_open,
    .release    = etx_release,
};


static struct class *dev_class;
static struct cdev etx_cdev;


 
/*----------------------Module_param_cb()--------------------------------*/
int notify_param(const char *val, const struct kernel_param *kp)
{
        int res = param_set_int(val, kp); // Use helper for write variable
        if(res==0) {
                printk(KERN_INFO "Call back function called...\n");
                printk(KERN_INFO "New value of cb_valueETX = %d\n", cb_valueETX);
                return 0;
        }
        else 
        {
           int test = cb_valueETX + 1;
           printk(KERN_INFO "NEW VALUE TESTER %d\n",test);
        }
        return -1;
}
 
const struct kernel_param_ops my_param_ops = 
{
        .set = &notify_param, // Use our setter ...
        .get = &param_get_int, // .. and standard getter
};
 
module_param_cb(cb_valueETX, &my_param_ops, &cb_valueETX, S_IRUSR|S_IWUSR ); //IRUGO
/*-------------------------------------------------------------------------*/


/*
** This function will be called when we open the Device file
*/
static int etx_open(struct inode *inode, struct file *file)
{
        pr_info("Driver Open Function Called...!!!\n");
        return 0;
}
/*
** This function will be called when we close the Device file
*/
static int etx_release(struct inode *inode, struct file *file)
{
        pr_info("Driver Release Function Called...!!!\n");
        return 0;
}
/*
** This function will be called when we read the Device file
*/
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        pr_info("Driver Read Function Called...!!!\n");
        return 0;
}
/*
** This function will be called when we write the Device file
*/
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        pr_info("Driver Write Function Called...!!!\n");
        return len;
}

/*
** Module init function
*/
static int __init hello_world_init(void)
{
        int i;
        printk(KERN_INFO "ValueETX = %d  \n", valueETX);
        printk(KERN_INFO "cb_valueETX = %d  \n", cb_valueETX);
        printk(KERN_INFO "NameETX = %s \n", nameETX);
        for (i = 0; i < (sizeof arr_valueETX / sizeof (int)); i++) {
                printk(KERN_INFO "Arr_value[%d] = %d\n", i, arr_valueETX[i]);
        }
        printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
	if((alloc_chrdev_region(&dev,6,1,"Ext_Device")>0))
	{
		printk("Allocation of major number issue\n");
		return -1;
	}
	pr_info("Major = %d, Minor = %d\n",MAJOR(dev),MINOR(dev));
                /*Creating cdev structure*/
        cdev_init(&etx_cdev,&fops);
        /*Adding character device to the system*/
        if((cdev_add(&etx_cdev,dev,1)) < 0){
            pr_err("Cannot add the device to the system\n");
            goto r_class;
       }
       
	dev_class = class_create(THIS_MODULE,"My Device");
	if(IS_ERR(dev_class)){
	 goto r_class;
	}
	if(IS_ERR(device_create(dev_class,NULL,dev,NULL, "MY_DEVICE"))){
	 goto r_device;
	}
	pr_info("kernel device creation success \n ");
        return 0;

r_device: class_destroy(dev_class);
r_class: unregister_chrdev_region(dev,1); return -1;

}

/*
** Module Exit function
*/
static void __exit hello_world_exit(void)
{
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    unregister_chrdev_region(dev,1);
    printk(KERN_INFO "Kernel Module Removed Successfully...\n");

}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("A simple hello world driver");
MODULE_VERSION("1.0");
