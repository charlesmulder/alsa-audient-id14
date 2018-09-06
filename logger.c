#include <linux/printk.h>
#include <linux/usb.h>
#include "logger.h"

void print_device(struct usb_device *usb_dev) {

	struct usb_host_interface *host_interface;
    struct usb_host_endpoint *endpoint;
	int c, i, s, e; // (c)onfiguration, (i)nterface, alt (s)etting, (e)ndpoint

    pr_info("device number of configurations: %u", usb_dev->descriptor.bNumConfigurations); // 2
    pr_info("device class: %u", usb_dev->descriptor.bDeviceClass); // 239 = misc
    pr_info("device sub class: %u", usb_dev->descriptor.bDeviceSubClass); // 2
    pr_info("device speed: %s", usb_speed_string(usb_dev->speed)); // high-speed
    pr_info("device state: %s", usb_state_string(usb_dev->state)); // configured

    pr_info("device active configuration");
    pr_info("device active configuration number of interfaces: %u", usb_dev->actconfig->desc.bNumInterfaces); // 5
    pr_info("device active configuration value: %u", usb_dev->actconfig->desc.bConfigurationValue); // 1

    pr_info("iterating through %u configurations", usb_dev->descriptor.bNumConfigurations);
    for(c = 0; c < usb_dev->descriptor.bNumConfigurations; ++c) {
        pr_info("configuration %u", c);
        pr_info("iterating through %u interfaces", usb_dev->config[c].desc.bNumInterfaces);
        for(i = 0; i < usb_dev->config[c].desc.bNumInterfaces; ++i) {
            // TODO: does interface have multiple settings?
            pr_info("configuration %u interface %u", c, i);
            if(usb_dev->config[c].interface[i]) {
                pr_info("interface number of alt settings: %u", usb_dev->config[c].interface[i]->num_altsetting);
                pr_info("interface current alt setting: %s", usb_dev->config[c].interface[i]->cur_altsetting->string);
                pr_info("interface number of endpoints: %u", usb_dev->config[c].interface[i]->cur_altsetting->desc.bNumEndpoints);
                pr_info("interface class: %u", usb_dev->config[c].interface[i]->cur_altsetting->desc.bInterfaceClass);
                pr_info("interface subclass: %u", usb_dev->config[c].interface[i]->cur_altsetting->desc.bInterfaceSubClass);
                pr_info("interface number: %u", usb_dev->config[c].interface[i]->cur_altsetting->desc.bInterfaceNumber);
                pr_info("interface number: 0x%04x", usb_dev->config[c].interface[i]->cur_altsetting->desc.bInterfaceNumber);
                pr_info("interface alternate setting: %u", usb_dev->config[c].interface[i]->cur_altsetting->desc.bAlternateSetting);

                pr_info("iterating through %u alt settings", usb_dev->config[c].interface[i]->num_altsetting);
                for(s = 0; s < usb_dev->config[c].interface[i]->num_altsetting; ++s) {
                    pr_info("configuration %u interface %u alt setting %u", c, i, s);
                    host_interface = usb_altnum_to_altsetting(usb_dev->config[c].interface[i], s); // struct usb_host_interface
                    if(host_interface) {
                        pr_info("host interface number of endpoints %u", host_interface->desc.bNumEndpoints); // ?
                        pr_info("iterating through %u endpoints", host_interface->desc.bNumEndpoints);
                        for(e = 0; e < host_interface->desc.bNumEndpoints; ++e) {
                            pr_info("configuration %u interface %u alt setting %u endpoint %u", c, i, s, e);
                            endpoint = &host_interface->endpoint[e]; // struct usb_host_endpoint
                            pr_info("endpoint not hallo");
                            if(endpoint) {
                                print_endpoint(endpoint);
                            }
                        }
                    }
                }
            }
        }
    }
}

void print_endpoint(struct usb_host_endpoint *endpoint) {
    // TODO: investigate urb_list
    //endpoint->urb_list; // struct list_head
    pr_info("endpoint!");
    pr_info("endpoint in? %d", endpoint->desc.bEndpointAddress & USB_DIR_IN);
    pr_info("endpoint out? %d", endpoint->desc.bEndpointAddress & USB_DIR_OUT);
    pr_info("endpoint isoc? %d", (endpoint->desc.bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_ISOC);
    pr_info("endpoint bulk? %d", (endpoint->desc.bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_BULK);
    pr_info("endpoint control? %d", (endpoint->desc.bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_CONTROL);
    pr_info("endpoint int? %d", (endpoint->desc.bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT);
    pr_info("endpoint enabled %u", endpoint->enabled);
    pr_info("endpoint streams %u", endpoint->streams); // number of USB 3 streams
    pr_info("endpoint length %u", endpoint->desc.bLength);
    pr_info("endpoint descriptor type %u", endpoint->desc.bDescriptorType);
    pr_info("endpoint endpoint address 0x%02x", endpoint->desc.bEndpointAddress);
    pr_info("endpoint endpoint address %d", endpoint->desc.bEndpointAddress);
    pr_info("endpoint attributes 0x%02x", endpoint->desc.bmAttributes);
    pr_info("endpoint attributes %u", endpoint->desc.bmAttributes);
    pr_info("endpoint max packet size 0x%04X", endpoint->desc.wMaxPacketSize);
    pr_info("endpoint max packet size %d", endpoint->desc.wMaxPacketSize);
    pr_info("endpoint interval %u", endpoint->desc.bInterval);
    pr_info("endpoint refresh %u", endpoint->desc.bRefresh);
    pr_info("endpoint synch address %u", endpoint->desc.bSynchAddress);
}



