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

static int id14_probe(struct usb_interface *interface, const struct usb_device_id *id) 
{
    printk(KERN_INFO "Audient iD14 (%04X:%04X) plugged in\n", id->idVendor, id->idProduct);
    return 0;
}

static void id14_disconnect(struct usb_interface *interface) 
{
    printk(KERN_INFO "Audient iD14 unplugged\n");
}

static struct usb_driver id14_driver = {
    .name = "id14",
    .id_table = id14_table,
    .probe = id14_probe,
    .disconnect = id14_disconnect
};

/**
 * Returns 0 on success
 */
static int __init id14_init(void)
{
    int result;

    /* register this driver with the USB subsystem */
    result = usb_register(&id14_driver);
    if (result)
        printk(KERN_ERR "Audient iD14 USB registration failed, return code %d\n", result);

    printk(KERN_INFO "Audient iD14 has been registered with the USB subsystem\n");
    return result;
}

static void __exit id14_exit(void) 
{
    printk(KERN_INFO "Audient iD14 has been deregisterd\n");
    usb_deregister(&id14_driver);
}

module_init(id14_init);
module_exit(id14_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("charlesmulder.net");
MODULE_DESCRIPTION("Audient iD14 ALSA driver");
