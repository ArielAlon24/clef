#include "app.h"
#include "texture_handler.h"
#include "raylib.h"
#include <stdio.h>
#include "components/envelope.h"
#include <stdlib.h>

int main(void) {
    app_init();
    app_run();
    app_free();
}
