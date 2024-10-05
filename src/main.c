#include "app.h"
#include "midi.h"
#include <stdio.h>
#include "midi_stream.h"


int main(void) {
    app_init();
    app_run();
    app_free();
    printf("sizeof(MidiMessage) = %zu\n", sizeof(MidiMessage));
}
