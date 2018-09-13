
#define ID14_VENDOR_ID	0x2708 
#define ID14_PRODUCT_ID	0x0002 

/*
 * probe the active usb device
 *
 * note that this can be called multiple times per a device, when it
 * includes multiple audio control interfaces.
 *
 * thus we check the usb device pointer and creates the card instance
 * only at the first time.  the successive calls of this function will
 * append the pcm interface to the corresponding card.
 */
static int usb_id14_audio_probe(struct usb_interface *intf, const struct usb_device_id *usb_id)
{
    struct usb_device *dev = interface_to_usbdev(intf);
    struct snd_usb_id14_audio *chip;
}

/**
 * clean any private data that has been allocated 
 * shut down any pending urbs that are in the USB system.
 * 
 * we need to take care of counter, since disconnection can be called also
 * many times as well as usb_id14_audio_probe().
 */
static void usb_id14_audio_disconnect(struct usb_interface *interface) 
{
    printk(KERN_INFO "Audient iD14 unplugged\n");
}

/* table of devices that work with this driver */
static struct usb_device_id usb_id14_audio_ids [] = {
	{ USB_DEVICE(ID14_VENDOR_ID, ID14_PRODUCT_ID) },
	{ }					/* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, usb_id14_audio_ids);

static struct usb_driver usb_id14_audio_driver = {
	.name =		"snd-usb-id14-audio",
	.probe =	usb_id14_audio_probe,
	.disconnect =	usb_id14_audio_disconnect,
	.id_table =	usb_id14_audio_ids,
};

module_usb_driver(usb_id14_audio_driver);
