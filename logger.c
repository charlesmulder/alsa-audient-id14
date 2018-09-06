#include <linux/printk.h>
#include <linux/usb.h>
#include "logger.h"

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



