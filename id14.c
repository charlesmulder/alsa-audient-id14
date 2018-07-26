#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kref.h>
#include <linux/usb.h>
#include <linux/uaccess.h>
//#include <alsa/asoundlib.h> 

/* Define these values to match your devices */
#define ID14_VENDOR_ID	0x2708 //0xfff0
#define ID14_PRODUCT_ID	0x0002 //0xfff0

/* table of devices that work with this driver */
static struct usb_device_id id14_table [] = {
	{ USB_DEVICE(ID14_VENDOR_ID, ID14_PRODUCT_ID) },
	{ }					/* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, id14_table);

MODULE_LICENSE("GPL");

static int id14_init(void)
{
    printk(KERN_ALERT "Hello, world\n");
    return 0;
}

static void id14_exit(void)
{
    printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(id14_init);
module_exit(id14_exit);
