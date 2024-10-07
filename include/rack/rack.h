#ifndef RACK_H
#define RACK_H

#include "midi_stream.h"
#include "rack/component.h"
#include <raylib.h>
#include <pthread.h>

typedef struct {
    int size;
    pthread_mutex_t lock;
    Component **components;
} Rack;

Rack *rack_init(int size);

void rack_mount(Rack *rack, Component *component, int x, int y);
void rack_mount_vec(Rack *rack, Component *component, Vector2 position);

void rack_unmount(Rack *rack, int x, int y);
void rack_unmount_vec(Rack *rack, Vector2 position);

void rack_next(Rack *rack, MidiStream *midi_stream, float *buffer, unsigned int buffer_size);

void rack_free(Rack *rack);

#endif
