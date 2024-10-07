#include "midi/midi.h"
#include <math.h>

/* TODO: Transform into an array lookup */
float note_number_to_frequency(Byte note_number) {
    return powf(2.0f, (note_number - 69.0f) / 12.0f) * 440.0f;
}
