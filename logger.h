#ifndef USB_ID14_LOGGER
#define USB_ID14_LOGGER

void print_endpoint(struct usb_host_endpoint *endpoint);

void print_device(struct usb_device *usb_dev);

#endif /* USB_ID14_LOGGER */

#ifndef SND_ID14_LOGGER
#define SND_ID14_LOGGER

void print_card(struct snd_card *card);

#endif /* SND_ID14_LOGGER */



