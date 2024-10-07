#include "rack/rack.h"
#include <stdlib.h>
#include <assert.h>

Rack *rack_init(int size) {
    Rack *rack = malloc(sizeof(Rack));
    assert(rack != NULL);

    rack->components = calloc(size * size, sizeof(Component *));
    assert(rack->components != NULL);
    rack->size = size;

    pthread_mutex_init(&rack->lock, NULL);

    return rack;
}

void rack_mount(Rack *rack, Component *component, int x, int y) {
    pthread_mutex_lock(&rack->lock);
    int index = rack->size * y + x;
    Component *previous = rack->components[index];
    if (previous != NULL) {
        component_free(previous);
    }
    rack->components[index] = component;
    pthread_mutex_unlock(&rack->lock);
}

void rack_mount_vec(Rack *rack, Component *component, Vector2 position) {
    pthread_mutex_lock(&rack->lock);
    int index = rack->size * (int) position.y + (int) position.x;
    Component *previous = rack->components[index];
    if (previous != NULL) {
        component_free(previous);
    }
    rack->components[index] = component;
    pthread_mutex_unlock(&rack->lock);
}

void rack_unmount(Rack *rack, int x, int y) {
    pthread_mutex_lock(&rack->lock);
    int index = rack->size * y + x;
    Component *component = rack->components[index];
    if (component != NULL) {
        component_free(component);
    }
    rack->components[index] = NULL;
    pthread_mutex_unlock(&rack->lock);
}

void rack_unmount_vec(Rack *rack, Vector2 position) {
    pthread_mutex_lock(&rack->lock);
    int index = rack->size * (int) position.y + (int) position.x;
    Component *component = rack->components[index];
    if (component != NULL) {
        component_free(component);
    }
    rack->components[index] = NULL;
    pthread_mutex_unlock(&rack->lock);
}

void rack_next(Rack *rack, MidiStream *midi_stream, float *buffer, unsigned int buffer_size) {
    pthread_mutex_lock(&rack->lock);
    Component *component;
    for (int i = 0; i < rack->size * rack->size; ++i) {
        component = rack->components[i];
        if (component != NULL) {
            component_next(component, midi_stream, buffer, buffer_size);
        }
    }
    pthread_mutex_unlock(&rack->lock);
}

void rack_free(Rack *rack) {
    Component *component;
    for (int i = 0; i < rack->size * rack->size; ++i) {
        component = rack->components[i];
        if (component != NULL) {
            component_free(component);
        }
    }

    pthread_mutex_destroy(&rack->lock);

    free(rack->components);
    free(rack);
}
