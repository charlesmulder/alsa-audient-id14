#ifndef ID14_CONTROL_H
#define ID14_CONTROL_H

#include "common.h"

int snd_id14_control_init(struct snd_id14_chip *chip);
void snd_id14_control_abort(struct snd_id14_chip *chip);
void snd_id14_control_destroy(struct snd_id14_chip *chip);
#endif /* ID14_CONTROL_H */
