#include "celestial_body/celestial_body.h"
#include "gfx/gfx.h"
#include "vec2/vec2.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main() {

  srand(time(NULL));
  struct gfx_context_t *ctxt =
      gfx_create("Planetary system", SCREEN_WIDTH, SCREEN_HEIGHT);
  if (!ctxt) {
    fprintf(stderr, "Graphics initialization failed!\n");
    return EXIT_FAILURE;
  }

  double delta_t = 3600;

  system_t system = create_system(delta_t);

  show_system(ctxt, &system);

  while (true) {

    gfx_present(ctxt);
    gfx_clear(ctxt, COLOR_BLACK);

    // Update the positions of the planets in the planetary system
    update_system(&system, delta_t);

    // Display the updated state of the planetary system
    show_system(ctxt, &system);

    if (gfx_keypressed() == SDLK_ESCAPE) {
      break;
    }
  }

  free_system(&system);
  gfx_destroy(ctxt);
  return EXIT_SUCCESS;
}
