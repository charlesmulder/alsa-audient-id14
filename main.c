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

#include <linux/interrupt.h> 
#include <asm/io.h> 

#include "logger.h"
//#include <alsa/asoundlib.h> 

/* Define these values to match your devices */
#define ID14_VENDOR_ID	0x2708 
#define ID14_PRODUCT_ID	0x0002 

#define CARD_NAME "Audient iD14"

/* SNDRV_CARDS: maximum number of cards supported by this module */
static int index[SNDRV_CARDS] = SNDRV_DEFAULT_IDX; // values of index array with length SNDRV_CARDS initialized to -1 
static char id[SNDRV_CARDS] = SNDRV_DEFAULT_STR; // values of id array with length SNDRV_CARDS initialized to NULL
static bool enable[SNDRV_CARDS] = SNDRV_DEFAULT_ENABLE_PNP; // values of enable array with length SNDRV_CARDS initialized to 1

/* definition of the chip-specific record */
struct snd_id14_chip {
    struct snd_card *card;
    /* the rest of the implementation will be in section
     * "PCI Resource Management"
     */
};

/* table of devices that work with this driver */
static struct usb_device_id id14_table [] = {
	{ USB_DEVICE(ID14_VENDOR_ID, ID14_PRODUCT_ID) },
	{ }					/* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, id14_table);

/* Structure to hold all of our device specific stuff */
struct id14 {
    struct usb_device *device;           /* the usb device for this device */
    struct usb_interface *interface;      /* the interface for this device */
    struct snd_card *card;
};

/*
static struct usb_class_driver id14_class = { 
    .name = "usb/id14-%d",
    .fops = &skel_fops,
    .minor_base = USB_SKEL_MINOR_BASE,
};
*/

/**
 * Probe function is called for each USB device interface
 */
static int usb_id14_probe(struct usb_interface *interface, const struct usb_device_id *usb_id) 
{
	struct id14 id14;
    struct id14 *saved_id14;

	struct usb_host_interface *host_interface;
    struct usb_host_endpoint *endpoint;
	struct usb_endpoint_descriptor endpoint_desc;
    struct usb_device *usb_dev = usb_get_dev(interface_to_usbdev(interface));

    static int snd_dev;
    struct snd_card *card;
    struct snd_id14_chip *chip; // TODO: implement
    int snd_err;

    pr_info("how many sound cards can we support? %d", SNDRV_CARDS);
    pr_info("what is the value of snd_dev? %d", snd_dev);
    /*
    if (snd_dev >= SNDRV_CARDS) { // snd card = usb device
        // module has reached the maximum number of devices 
        pr_err("maximum snd devices has been reached");
        return -ENODEV;
    }

    if (!snd_enable[snd_dev]) {
        // increment snd_dev
        pr_info("incrementing snd_dev");
        snd_dev++;
        return -ENOENT;
    }
    */

    // creates and initializes a soundcard structure
    // not sure we need to do this every time, surely only once?
    //err = snd_card_new(&usb_dev->dev, index[snd_dev], id[snd_dev], THIS_MODULE, 0, &card);
    
    pr_info("getting private data");
    //saved_id14 = usb_get_intfdata(interface);
    saved_id14 = dev_get_drvdata(&usb_dev->dev);
    if (!saved_id14) {
        pr_info("no private data");
        id14.device = usb_dev;
        id14.interface = interface;
        //snd_err = snd_card_new(&usb_dev->dev, snd_index[snd_dev], snd_id[snd_dev], THIS_MODULE, sizeof(struct snd_id14_chip), &card);
        snd_err = snd_card_new(&usb_dev->dev, index, id, THIS_MODULE, sizeof(struct snd_id14_chip), &card);
        if (snd_err < 0) {
            pr_err("error creating new sound card");
            return snd_err;
        }
        chip = card->private_data;
        id14.card = card;
        saved_id14 = &id14;
        pr_info("setting private data");
        dev_set_drvdata(&usb_dev->dev, saved_id14);
    }
    
    print_card(saved_id14->card);

    //usb_set_intfdata(interface, saved_id14);

    /* the way this is called will depend on the current usb interface */
    //snd_device_new(card, SNDRV_DEV_PCM, chip, &ops);
    
    /* if registration fails */
    //err = snd_card_register(card);
    //if (err < 0) {
    //    snd_card_free_when_closed(card)
    //    return err;
    //}

    printk(KERN_INFO "Audient iD14 (%04X:%04X) plugged in\n", usb_id->idVendor, usb_id->idProduct);
	pr_info("interface class: %u", interface->cur_altsetting->desc.bInterfaceClass);
	pr_info("interface subclass: %u", interface->cur_altsetting->desc.bInterfaceSubClass);
	pr_info("interface number: %u", interface->cur_altsetting->desc.bInterfaceNumber);
	pr_info("interface number: 0x%04x", interface->cur_altsetting->desc.bInterfaceNumber);
    pr_info("Audient iD14 probe function called");
    pr_info("Audient iD14 vendor id is: %u", usb_id->idVendor); // 9992
    pr_info("Audient iD14 product id is: %u", usb_id->idProduct); // 2
    pr_info("Audient iD14 device class: %u", usb_id->bDeviceClass); // 0
    pr_info("Audient iD14 interface num_altsetting is: %u", interface->num_altsetting); // 1 (I checked, only a single alt_setting)
	host_interface = interface->cur_altsetting; // struct usb_host_interface *
    pr_info("host interface number of endpoints %u", host_interface->desc.bNumEndpoints); // 0
    
    //print_device(usb_dev);

    //usb_set_interface() to specify the appropriate altsetting
    /* save our data pointer in this interface device */
    //usb_set_intfdata(interface, dev);

    /* we can register the device now, as it is ready */
    //retval = usb_register_dev(interface, &id14_class);
    //if (retval) {
        /* something prevented us from registering this driver */
        //pr_err("Not able to get a minor for this device.");
        //usb_set_intfdata(interface, NULL);
        //goto error;
    //}

    /* let the user know what node this device is now attached to */
    //dev_info(&interface->dev, "USB iD14 audio device now attached to USB-iD14-%d", interface->minor);
    return 0;
}


/**
 * clean any private data that has been allocated 
 * shut down any pending urbs that are in the USB system.
 */
static void usb_id14_disconnect(struct usb_interface *interface) 
{
    printk(KERN_INFO "Audient iD14 unplugged\n");
    //struct usb_skel *dev;
    //int minor = interface->minor;

    //dev = usb_get_intfdata(interface);
    //usb_set_intfdata(interface, NULL);

    /* give back our minor */
    //usb_deregister_dev(interface, &skel_class);

    /* decrement our usage count */
    //kref_put(&dev->kref, skel_delete);

    //dev_info(&interface->dev, "USB Skeleton #%d now disconnected", minor);
}

static struct usb_driver id14_driver = {
    .name = "id14",
    .id_table = id14_table,
    .probe = usb_id14_probe,
    .disconnect = usb_id14_disconnect
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
