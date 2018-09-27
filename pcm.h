#ifndef ID14_PCM_H
#define ID14_PCM_H

#include "chip.h"

#include <sound/pcm.h> 
#include <sound/pcm_params.h> 

/**
 * The maximum size data payload is specified in the endpoint descriptor of an Isochronous Endpoint. In our cv
 */
static struct snd_pcm_hardware snd_id14_playback_hw = {
    .info = (SNDRV_PCM_INFO_MMAP | // ?
            SNDRV_PCM_INFO_INTERLEAVED |
            SNDRV_PCM_INFO_BLOCK_TRANSFER | // ?
            SNDRV_PCM_INFO_MMAP_VALID), // ?
    .formats = SNDRV_PCM_FMTBIT_S16_LE | 
               SNDRV_PCM_FMTBIT_S24_LE 
    .rates = SNDRV_PCM_RATE_44100 |
             SNDRV_PCM_RATE_48000 |
             SNDRV_PCM_RATE_88200 |
             SNDRV_PCM_RATE_96000,
    .rate_min =         44100,
    .rate_max =         96000,
    .channels_min =     2,
    .channels_max =     2, // ?
    .buffer_bytes_max = , // ? https://www.edn.com/design/consumer/4376143/Fundamentals-of-USB-Audio
    .period_bytes_min = , // ?
    .period_bytes_max = , // ?
    .periods_min =      , // ?
    .periods_max =      , // ?
};

int snd_id14_playback_open(struct snd_pcm_substream *substream);
int snd_id14_playback_close(struct snd_pcm_substream *substream);

#endif /* ID14_CONTROL_H */
