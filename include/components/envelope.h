#ifndef ENVELOPE_H
#define ENVELOPE_H

#include "midi/midi_stream.h"
#include "rack/component.h"
#include "rack/rack.h"
#include <pthread.h>
#include <raylib.h>

typedef struct {
    Rack rack;
    int value;
} Envelope;

#endif
