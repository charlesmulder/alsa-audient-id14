#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kref.h>
#include <linux/usb.h>
//#include <linux/uaccess.h>
#include <linux/usb/audio.h>
#include <sound/core.h>
#include <sound/initval.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include "logger.h"
//#include <alsa/asoundlib.h> 

/**
 * % In the skeleton driver, we determine what end points are marked as bulk-in and bulk-out. We create buffers to hold the data that will be sent and received from the device, and a USB urb to write data to the device is initialized.
 *
 * % continue from page 903 of ldd3
 *
 */

/* Define these values to match your devices */
#define ID14_VENDOR_ID	0x2708 //0xfff0
#define ID14_PRODUCT_ID	0x0002 //0xfff0

/* module parameters (see "Module Parameters") */
/* SNDRV_CARDS: maximum number of cards supported by this module */
static int index[SNDRV_CARDS] = SNDRV_DEFAULT_IDX;
static char *id[SNDRV_CARDS] = SNDRV_DEFAULT_STR;
static bool enable[SNDRV_CARDS] = SNDRV_DEFAULT_ENABLE_PNP;

static DEFINE_MUTEX(devices_mutex);

/* table of devices that work with this driver */
static struct usb_device_id id14_table [] = {
	{ USB_DEVICE(ID14_VENDOR_ID, ID14_PRODUCT_ID) },
	{ }					/* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, id14_table);

 /* Structure to hold all of our device specific stuff */
struct usb_id14 {
    struct usb_device * udev;           /* the usb device for this device */
    struct usb_interface *  interface;      /* the interface for this device */
    struct kref kref;
};

/* Structure to hold all of our device specific stuff */
struct usb_id14 {
    struct usb_device * udev;           /* the usb device for this device */
    struct usb_interface *  interface;      /* the interface for this device */
    unsigned char * isoc_out_buffer;     /* the buffer to receive data */
    size_t          isoc_out_size;       /* the size of the receive buffer */
    __u8            isoc_out_endpointAddr;  /* the address of the bulk out endpoint */
    struct kref     kref;
};

static struct usb_class_driver id14_class = { 
    .name = "usb/id14-%d",
    .fops = &skel_fops,
    .minor_base = USB_SKEL_MINOR_BASE,
};


static int id14_probe(struct usb_interface *interface, const struct usb_device_id *id) 
{
	struct usb_id14 *dev = NULL;
	struct usb_host_interface *host_interface;
    struct usb_host_endpoint *endpoint;
	struct usb_endpoint_descriptor endpoint_desc;
    struct usb_device *usb_dev = usb_get_dev(interface_to_usbdev(interface));

    //struct usb_host_config *config;
	size_t buffer_size;
	int retval = -ENOMEM;
    /* allocate memory for our device state and initialize it */
    dev = kzalloc(sizeof(struct usb_id14), GFP_KERNEL);
    if (!dev) {
        pr_err("Out of memory");
        //goto error;
    }
    kref_init(&dev->kref);

    printk(KERN_INFO "Audient iD14 (%04X:%04X) plugged in\n", id->idVendor, id->idProduct);
	pr_info("interface class: %u", interface->cur_altsetting->desc.bInterfaceClass);
	pr_info("interface subclass: %u", interface->cur_altsetting->desc.bInterfaceSubClass);
	pr_info("interface number: %u", interface->cur_altsetting->desc.bInterfaceNumber);
	pr_info("interface number: 0x%04x", interface->cur_altsetting->desc.bInterfaceNumber);
    pr_info("Audient iD14 probe function called");
    pr_info("Audient iD14 vendor id is: %u", id->idVendor); // 9992
    pr_info("Audient iD14 product id is: %u", id->idProduct); // 2
    pr_info("Audient iD14 device class: %u", id->bDeviceClass); // 0
    pr_info("Audient iD14 interface num_altsetting is: %u", interface->num_altsetting); // 1 (I checked, only a single alt_setting)
	host_interface = interface->cur_altsetting; // struct usb_host_interface *
    pr_info("host interface number of endpoints %u", host_interface->desc.bNumEndpoints); // 0
    
    print_device(usb_dev);

    //usb_set_interface() to specify the appropriate altsetting
    /* save our data pointer in this interface device */
    usb_set_intfdata(interface, dev);

    /* we can register the device now, as it is ready */
    retval = usb_register_dev(interface, &id14_class);
    if (retval) {
        /* something prevented us from registering this driver */
        pr_err("Not able to get a minor for this device.");
        usb_set_intfdata(interface, NULL);
        //goto error;
    }

    /* let the user know what node this device is now attached to */
    dev_info(&interface->dev, "USB iD14 audio device now attached to USB-iD14-%d", interface->minor);
    return 0;
}


/**
 * clean any private data that has been allocated 
 * shut down any pending urbs that are in the USB system.
 */
static void id14_disconnect(struct usb_interface *interface) 
{
    printk(KERN_INFO "Audient iD14 unplugged\n");
    struct usb_skel *dev;
    int minor = interface->minor;

    dev = usb_get_intfdata(interface);
    usb_set_intfdata(interface, NULL);

    /* give back our minor */
    usb_deregister_dev(interface, &skel_class);

    /* decrement our usage count */
    kref_put(&dev->kref, skel_delete);

    dev_info(&interface->dev, "USB Skeleton #%d now disconnected", minor);
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
