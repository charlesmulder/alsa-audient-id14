#include "pcm.h"

/* operators */
static struct snd_pcm_ops snd_id14_playback_ops = {
    .open =        snd_id14_playback_open,
    .close =       snd_id14_playback_close,
    .ioctl =       snd_pcm_lib_ioctl,
    .hw_params =   snd_id14_pcm_hw_params,
    .hw_free =     snd_id14_pcm_hw_free,
    .prepare =     snd_id14_pcm_prepare,
    .trigger =     snd_id14_pcm_trigger,
    .pointer =     snd_id14_pcm_pointer,
};

/* create a pcm device */
static int snd_mychip_new_pcm(struct snd_id14_chip *chip)
{
        struct snd_pcm *pcm;
        int err;

        err = snd_pcm_new(chip->card, "Audient iD14", 0, 1, 1, &pcm);
        if (err < 0)
            return err;
        pcm->private_data = chip;
        strcpy(pcm->name, "Audient iD14");
        chip->pcm = pcm;
        /* set operators */
        snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_PLAYBACK,
                        &snd_mychip_playback_ops);
        //snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_CAPTURE,
                        &snd_mychip_capture_ops);
        /* pre-allocation of buffers */
        /* NOTE: this may fail */
        snd_pcm_lib_preallocate_pages_for_all(pcm, SNDRV_DMA_TYPE_DEV,
                                              snd_dma_pci_data(chip->pci),
                                              64*1024, 64*1024);
        return 0;
}
