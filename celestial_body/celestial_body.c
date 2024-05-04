#include "celestial_body.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

void calculate_next_pos(double delta_t, vec2 initial_pos, double e, double a,
                        system_t *system, u_int32_t i);

// Function to create a new system with celestial bodies initialized with
// specific parameters.
system_t create_system(double delta_t) {

  // Initialization of celestial bodies (star and planets)
  celestial_body_t star = {"Soleil", STAR_MASS, 0,      STAR_COLOR, STAR_RADIUS,
                           VECT_0,   VECT_0,    VECT_0, 0};

  celestial_body_t mercury = {"Mercure",
                              MERCURY_MASS,
                              5.340644e10,
                              MERCURY_COLOR,
                              MERCURY_RADIUS,
                              vec2_create(MERCURY_DISTANCE, 0),
                              vec2_create(MERCURY_DISTANCE, 0),
                              vec2_create(MERCURY_DISTANCE, 0),
                              0.207};

  celestial_body_t venus = {"Venus",
                            VENUS_MASS,
                            1.081593e11,
                            VENUS_COLOR,
                            VENUS_RADIUS,
                            vec2_create(VENUS_DISTANCE, 0),
                            vec2_create(VENUS_DISTANCE, 0),
                            vec2_create(VENUS_DISTANCE, 0),
                            0.007};

  celestial_body_t earth = {"Terre",
                            EARTH_MASS,
                            1.495979e11,
                            EARTH_COLOR,
                            EARTH_RADIUS,
                            vec2_create(EARTH_DISTANCE, 0),
                            vec2_create(EARTH_DISTANCE, 0),
                            vec2_create(EARTH_DISTANCE, 0),
                            0.01671022};

  celestial_body_t mars = {"Mars",
                           MARS_MASS,
                           2.279872e11,
                           MARS_COLOR,
                           MARS_RADIUS,
                           vec2_create(MARS_DISTANCE, 0),
                           vec2_create(MARS_DISTANCE, 0),
                           vec2_create(MARS_DISTANCE, 0),
                           0.093};

  celestial_body_t alpha = {"Alpha",
                            ALPHA_MASS,
                            2.5432e11,
                            ALPHA_COLOR,
                            ALPHA_RADIUS,
                            vec2_create(ALPHA_DISTANCE, 0),
                            vec2_create(ALPHA_DISTANCE, 0),
                            vec2_create(ALPHA_DISTANCE, 0),
                            0.0049934122};

  celestial_body_t beta = {"Beta",
                           BETA_MASS,
                           2.9324e11,
                           BETA_COLOR,
                           BETA_RADIUS,
                           vec2_create(BETA_DISTANCE, 0),
                           vec2_create(BETA_DISTANCE, 0),
                           vec2_create(BETA_DISTANCE, 0),
                           0.02434122};

  celestial_body_t *planets_list =
      malloc(NUMBER_OF_PLANET * sizeof(celestial_body_t));

  if (planets_list == NULL) {
    exit(EXIT_FAILURE);
  }

  planets_list[0] = mercury;
  planets_list[1] = venus;
  planets_list[2] = earth;
  planets_list[3] = mars;
  planets_list[4] = alpha;
  planets_list[5] = beta;

  system_t system = {star, NUMBER_OF_PLANET, planets_list};

  for (uint32_t i = 0; i < system.nb_planets; ++i) {

    celestial_body_t planet = system.planets[i];
    calculate_next_pos(delta_t, planet.pos, planet.e, planet.a, &system, i);
  }

  return system;
}

vec2 calculate_gravitational_force(celestial_body_t object1,
                                   celestial_body_t object2) {

  vec2 r = vec2_sub(object1.pos, object2.pos);

  double r_cube = vec2_norm_sqr(r) * vec2_norm(r);

  // If r_cube = 0, then object1 and object2 are the same celestial object
  if (r_cube == 0.0) {
    return VECT_0;
  }

  double interaction_gravitational_force =
      G * object1.mass * object2.mass / r_cube;

  vec2 gravitational_force = vec2_mul(interaction_gravitational_force, r);

  return gravitational_force;
}

vec2 calculate_resultant_force(system_t *system, uint32_t object_i) {
  vec2 resultant_force = VECT_0;

  vec2 force_from_sun =
      calculate_gravitational_force(system->star, system->planets[object_i]);
  resultant_force = vec2_add(resultant_force, force_from_sun);

  vec2 force_from_all_planets = VECT_0;

  for (uint32_t i = 0; i < system->nb_planets; i++) {

    vec2 force_from_planet = calculate_gravitational_force(
        system->planets[object_i], system->planets[i]);

    force_from_all_planets =
        vec2_add(force_from_planet, force_from_all_planets);
  }

  resultant_force = vec2_add(resultant_force, force_from_all_planets);
  return resultant_force;
}

void calculate_next_pos(double delta_t, vec2 initial_pos, double e, double a,
                        system_t *system, u_int32_t i) {

  double scalar = sqrt(STAR_MASS * G * (1 + e) / (a * (1 - e)));

  vec2 init_velocity_t = vec2_mul(
      scalar * delta_t, vec2_normalize(vec2_perpendicular(initial_pos)));

  vec2 half_a0_t_square =
      vec2_mul(pow(delta_t, 2) / (2 * system->planets[i].mass),
               calculate_resultant_force(system, i));

  vec2 next_pos =
      vec2_add(initial_pos, vec2_add(init_velocity_t, half_a0_t_square));

  system->planets[i].pos = next_pos;
}

void update_system(system_t *system, double delta_t) {

  for (uint32_t i = 0; i < system->nb_planets; i++) {

    celestial_body_t planet = system->planets[i];

    vec2 newPos;
    newPos = vec2_mul(2, planet.pos);
    newPos = vec2_sub(newPos, planet.prec_pos);

    vec2 acc_part = vec2_mul(pow(delta_t, 2) / planet.mass,
                             calculate_resultant_force(system, i));

    newPos = vec2_add(newPos, acc_part);

    planet.prec_pos = planet.pos;
    planet.pos = newPos;

    system->planets[i] = planet;
  }
}

// Function to draw the celestial bodies in the system on the screen
void show_system(struct gfx_context_t *ctxt, system_t *system) {

  celestial_body_t star = system->star;

  coordinates star_coord =
      vec2_to_coordinates(star.pos, SCREEN_WIDTH, SCREEN_HEIGHT);

  draw_full_circle(ctxt, star_coord.column, star_coord.row, star.radius,
                   star.color);

  for (uint32_t i = 0; i < system->nb_planets; i++) {

    coordinates coord = vec2_to_coordinates(system->planets[i].pos,
                                            SCREEN_WIDTH, SCREEN_HEIGHT);

    draw_full_circle(ctxt, coord.column, coord.row, system->planets[i].radius,
                     system->planets[i].color);
  }
}

void free_system(system_t *system) { free(system->planets); }