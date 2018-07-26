# WIP: Linux ALSA module for Audient iD14

```bash
KERNELDIR=~/Project/linux-stable/ make # build the module using the kernel build system
sudo tail -f /var/log/messages # tail kernel messages
insmod hello.ko # insert module into kernel
lsmod  | grep hello # list loaded modules to prove modules is loaded
rmmod hello # remove module from kernel
lsmod  | grep hello # list loaded modules to prove modules is not loaded
```

## Resources

* [Updated LDD3 examples](https://github.com/martinezjavier/ldd3)



