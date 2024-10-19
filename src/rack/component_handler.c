#include <assert.h>
#include "rack/component.h"
#include "rack/rack.h"
#include "rack/component_handler.h"
#include "components/oscillator.h"

#define LIBRARY_COMPONENT_ENTRY(type, _factory, _preview_callback) \
    library_component_table[type].factory = _factory; \
    library_component_table[type].preview_callback = _preview_callback;

void component_handler_init() {
    LIBRARY_COMPONENT_ENTRY(COMPONENT_OSCILLATOR, oscillator_init,     oscillator_preview);
    LIBRARY_COMPONENT_ENTRY(COMPONENT_RACK,       rack_component_init, rack_preview);
}

Component *component_create(ComponentType type, Rack *parent) {
    LibraryComponent library_component = library_component_table[type];
    assert(library_component.factory != NULL);
    return (*library_component.factory)(parent);
}

void component_preview(ComponentType type, Vector2 position) {
    LibraryComponent library_component = library_component_table[type];
    assert(library_component.preview_callback != NULL);
    return (*library_component.preview_callback)(position);
}
