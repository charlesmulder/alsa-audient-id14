# WIP: Linux ALSA module for Audient iD14

## To Do

### Output

* determine what end points are marked as isochronous out (feedback endpoint required?)
* determine the size of the isochronous out end point that the device has
* create buffers to hold the data that will be sent to the device
* create a USB urb to write data to the device 

### Input

## Adhere to Kernel coding standards

Please see ~/Products/cm/linux-stable/Documentation/development-process/4.Coding.rst

## Quickstart

```bash
# NB: remove the default loaded module
sudo rmmod snd-usb-audio
# build the module using the kernel build system
KERNELDIR=~/Products/cm/linux-stable/ make 
# tail kernel output
sudo dmesg -w
# insert module into kernel
sudo insmod id14.ko 
# remove module
sudo rmmod id14
```

## Resources

* [Updated LDD3 examples](https://github.com/martinezjavier/ldd3)
* [How to: USB pen driver for Linux](https://opensourceforu.com/2011/10/usb-drivers-in-linux-1/)
* [Reverse engineering a USB sound card](http://kicherer.org/joomla/index.php/en/blog/38-reverse-engineering-a-usb-sound-card-with-midi-interface-for-linux)



