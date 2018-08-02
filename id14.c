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
//#include <alsa/asoundlib.h> 

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

static int id14_probe(struct usb_interface *interface, const struct usb_device_id *id) 
{
	struct usb_id14 *dev = NULL;
	struct usb_host_interface *iface_desc;
    struct usb_host_endpoint *endpoint;
	struct usb_endpoint_descriptor endpoint_desc;
    struct usb_device *usb_dev = interface_to_usbdev(interface);
    int interface_number;

    //struct usb_host_config *config;
	size_t buffer_size;
	int i;
	int retval = -ENOMEM;
    int result;

    //mutex_lock(&devices_mutex);
    usb_get_dev(usb_dev);

    printk(KERN_INFO "Audient iD14 (%04X:%04X) plugged in\n", id->idVendor, id->idProduct);
    pr_info("Audient iD14 probe function called");
    pr_info("Audient iD14 vendor id is: %u", id->idVendor); // 9992
    pr_info("Audient iD14 product id is: %u", id->idProduct); // 2
    pr_info("Audient iD14 device class: %u", id->bDeviceClass); // 0
    pr_info("Audient iD14 interface num_altsetting is: %u", interface->num_altsetting); // 1
	//iface_desc = interface->cur_altsetting; // 0

    pr_info("device number of configurations: %u", usb_dev->descriptor.bNumConfigurations); // 2
    pr_info("device class: %u", usb_dev->descriptor.bDeviceClass); // 239 = misc
    pr_info("device sub class: %u", usb_dev->descriptor.bDeviceSubClass); // 2
    pr_info("device speed: %s", usb_speed_string(usb_dev->speed)); // high-speed
    pr_info("device state: %s", usb_state_string(usb_dev->state)); // configured
    pr_info("device number of interfaces: %u", usb_dev->actconfig->desc.bNumInterfaces); // 5
    pr_info("device config value: %u", usb_dev->actconfig->desc.bConfigurationValue);

    // printing out interfaces for active config
    for(i = 0; i < usb_dev->actconfig->desc.bNumInterfaces; ++i) {
        pr_info("i is %u", i);
        pr_info("interface number of alt settings: %u", usb_dev->actconfig->interface[i]->num_altsetting);
        pr_info("interface current alt setting: %s", usb_dev->actconfig->interface[i]->cur_altsetting->string);
        pr_info("interface number of endpoints: %u", usb_dev->actconfig->interface[i]->cur_altsetting->desc.bNumEndpoints);
        pr_info("interface class: %u", usb_dev->actconfig->interface[i]->cur_altsetting->desc.bInterfaceClass);
        pr_info("interface subclass: %u", usb_dev->actconfig->interface[i]->cur_altsetting->desc.bInterfaceSubClass);
        pr_info("interface number: %u", usb_dev->actconfig->interface[i]->cur_altsetting->desc.bInterfaceNumber);
        pr_info("interface alternate setting: %u", usb_dev->actconfig->interface[i]->cur_altsetting->desc.bAlternateSetting);
    }
    //result = usb_set_interface(usb_dev, interface->cur_altsetting->desc.bInterfaceNumber, 0);
    //mutex_unlock(&devices_mutex);
    //USB_ENDPOINT_XFER_{CONTROL, ISOC, BULK, INT}
    return 0;
}

/**
 * clean any private data that has been allocated 
 * shut down any pending urbs that are in the USB system.
 */
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
