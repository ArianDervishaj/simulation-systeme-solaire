#ifndef _CELESTIAL_BODY_H_
#define _CELESTIAL_BODY_H_

#include "../gfx/gfx.h"
#include "../vec2/vec2.h"
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 1000
#define G 6.67e-11
#define NUMBER_OF_PLANET 6

/// Celestial objects radius in km
#define STAR_RADIUS 50
#define MERCURY_RADIUS 10
#define EARTH_RADIUS 25
#define VENUS_RADIUS 24
#define MARS_RADIUS 13
#define ALPHA_RADIUS 11
#define BETA_RADIUS 16

// Celestial objects masses in kg
#define STAR_MASS 1.989e30
#define MERCURY_MASS 3.301e23
#define EARTH_MASS 5.972e24
#define VENUS_MASS 4.867e24
#define MARS_MASS 6.417e23
#define ALPHA_MASS 4.321e24
#define BETA_MASS 2.321e23

// Celestial objects colors
#define STAR_COLOR COLOR_YELLOW
#define MERCURY_COLOR MAKE_COLOR(183, 184, 185)
#define EARTH_COLOR COLOR_BLUE
#define VENUS_COLOR MAKE_COLOR(255, 198, 73)
#define MARS_COLOR COLOR_RED
#define ALPHA_COLOR MAKE_COLOR(140, 39, 140)
#define BETA_COLOR MAKE_COLOR(102, 165, 134)

// Celestial objects distants from the star in meters
#define MERCURY_DISTANCE 46e9
#define VENUS_DISTANCE 107e9
#define EARTH_DISTANCE 147e9
#define MARS_DISTANCE 206e9
#define ALPHA_DISTANCE 255e9
#define BETA_DISTANCE 289e9

#define VECT_0 vec2_create_zero()

typedef struct _celestial_body {
  char *name;
  double mass;
  double a;
  int color;
  int radius;
  vec2 pos;      // x(t)
  vec2 prec_pos; // x(t + dt)
  vec2 perihelion;
  double e;
} celestial_body_t;

typedef struct _system {
  celestial_body_t star;
  uint32_t nb_planets;
  celestial_body_t *planets;
} system_t;

/// Create a new system with initial configuration.
/// @param delta_t The time step.
/// @return The newly created system.
system_t create_system(double delta_t);

/// Draw the celestial bodies of the system on the screen.
/// @param ctxt The canvas context.
/// @param system The system to draw.
void show_system(struct gfx_context_t *ctxt, system_t *system);

/// Update the state of the system based on the elapsed time.
/// @param system The system to update.
/// @param delta_t The elapsed time since the last update.
void update_system(system_t *system, double delta_t);

/// Calculate the gravitational force between two celestial bodies.
/// @param object1 The first celestial object.
/// @param object2 The second celestial object.
/// @return The gravitational force applied on object2.
vec2 calculate_gravitational_force(celestial_body_t object1,
                                   celestial_body_t object2);

/// Calculate the resultant force on a celestial body within the system.
/// @param system The star system.
/// @param object_i The index of the celestial body for which to calculate the
/// resultant force.
/// @return The resultant force acting on the celestial body.
vec2 calculate_resultant_force(system_t *system, uint32_t object_i);

/// Free the memory allocated for the planets list in the given planetary
/// system.
/// @param system The planetary system to free.
void free_system(system_t *system);

#endif
