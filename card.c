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

#include "chip.h"
#include "logger.h"

MODULE_AUTHOR("Charles Mulder <charlesmulder@pm.me>");
MODULE_DESCRIPTION("Audient iD14 audio driver");
MODULE_LICENSE("GPL v2");
MODULE_SUPPORTED_DEVICE("{{Audient iD14}}");

static int index[SNDRV_CARDS] = SNDRV_DEFAULT_IDX;  /* Index 0-MAX */
static char *id[SNDRV_CARDS] = SNDRV_DEFAULT_STR;   /* ID for this card */
static bool enable[SNDRV_CARDS] = SNDRV_DEFAULT_ENABLE_PNP;/* Enable this card */
static struct snd_id14_chip *chips[SNDRV_CARDS] = SNDRV_DEFAULT_PTR;
static struct usb_device *devices[SNDRV_CARDS] = SNDRV_DEFAULT_PTR;

module_param_array(index, int, NULL, 0444);
MODULE_PARM_DESC(index, "Index value for the iD14 sound device");
module_param_array(id, charp, NULL, 0444);
MODULE_PARM_DESC(id, "ID string for the iD14 sound device.");
module_param_array(enable, bool, NULL, 0444);
MODULE_PARM_DESC(enable, "Enable the iD14 sound device.");

static DEFINE_MUTEX(register_mutex);

static void snd_id14_chip_abort(struct snd_id14_chip *chip)
{
	if (chip) {
		if (chip->card) {
			snd_card_disconnect(chip->card);
			snd_card_free_when_closed(chip->card);
			chip->card = NULL;
		}
	}
}

static void snd_id14_chip_destroy(struct snd_id14_chip *chip)
{
	if (chip) {
		if (chip->card)
			snd_card_free(chip->card);
	}
}

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
	int ret;
	int i;
    struct usb_device *device = interface_to_usbdev(intf);
    struct snd_id14_chip *chip = NULL;
    int regidx = -1; /* index in module parameter array */
    struct snd_card *card = NULL;

	/* look if we already serve this card and return if so */
	mutex_lock(&register_mutex);
    pr_info("sndrv cards %u", SNDRV_CARDS);
	for (i = 0; i < SNDRV_CARDS; i++) {
		if (devices[i] == device) {
			if (chips[i])
				chips[i]->intf_count++;
			usb_set_intfdata(intf, chips[i]);
			mutex_unlock(&register_mutex);
            pr_info("card already registered");
            pr_info("device already registered");
            pr_info("chip already registered");
			return 0;
		} else if (!devices[i] && regidx < 0)
			regidx = i;
	}
    pr_info("card being registered with id %u", regidx);
	if (regidx < 0) {
		mutex_unlock(&register_mutex);
		dev_err(&intf->dev, "too many cards registered.\n");
		return -ENODEV;
	}
	devices[regidx] = device;
    pr_info("device being registered");
	mutex_unlock(&register_mutex);

	/* if we are here, card can be registered in alsa. */
	if (usb_set_interface(device, 0, 0) != 0) {
		dev_err(&intf->dev, "can't set first interface.\n");
		return -EIO;
	}
	ret = snd_card_new(&intf->dev, index[regidx], id[regidx],
			   THIS_MODULE, sizeof(struct snd_id14_chip), &card);
	if (ret < 0) {
		dev_err(&intf->dev, "cannot create alsa card.\n");
		return ret;
	}
	strcpy(card->driver, "iD14");
	strcpy(card->shortname, "Audient iD14");
	sprintf(card->longname, "%s at %d:%d", card->shortname,
			device->bus->busnum, device->devnum);

	chip = card->private_data;
	chips[regidx] = chip;
	chip->dev = device;
	chip->regidx = regidx;
	chip->intf_count = 1;
	chip->card = card;

	ret = snd_card_register(card);
	if (ret < 0) {
		dev_err(&intf->dev, "cannot register card.");
		goto destroy_chip;
	}
    pr_info("chip being registered");
	usb_set_intfdata(intf, chip);
	return 0;

destroy_chip:
	snd_id14_chip_destroy(chip);
	return ret;
}

/**
 * clean any private data that has been allocated 
 * shut down any pending urbs that are in the USB system.
 * 
 * we need to take care of counter, since disconnection can be called also
 * many times as well as usb_id14_audio_probe().
 */
static void usb_id14_audio_disconnect(struct usb_interface *intf) 
{
    struct snd_id14_chip *chip;

    pr_info("iD14 disconnect called");

	chip = usb_get_intfdata(intf);
	if (chip) { /* if !chip, fw upload has been performed */
		chip->intf_count--;
		if (!chip->intf_count) {
			mutex_lock(&register_mutex);
			devices[chip->regidx] = NULL;
			chips[chip->regidx] = NULL;
			mutex_unlock(&register_mutex);

			chip->shutdown = true;
			snd_id14_chip_abort(chip);
			snd_id14_chip_destroy(chip);
		}
	}

}

/* table of devices that work with this driver */
static struct usb_device_id usb_id14_audio_ids [] = {
	{ USB_DEVICE(0x2708, 0x0002) },
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
