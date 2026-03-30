/* header files */
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>

/* SDL global variables */
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

/* Physics global variables */
#define DT 0.016f
#define GRAVITY_ACCELARATION 500.0f
#define GROUND_HEIGHT 300.0f
#define BOUNCE_DAMPING 0.6f

/* Data stractures */
typedef struct {
  float x_position, y_position;
} Position2D;

typedef struct {
  float x_velocity, y_velocity;
} Velocity2D;

typedef struct {
  float x_acceleration, y_acceleration;
} Acceleration2D;

typedef struct {
  Position2D position;
  Velocity2D velocity;
  Acceleration2D acceleration;
  float mass;
}Body;

/* --- PHYSICS FUNCTIONS --- */

/* Apply gravity force */
void apply_gravity(Body *body){
  body->acceleration.y_acceleration = GRAVITY_ACCELARATION;  
}

/* Update pos & vel values */
void update_physics_values(Body *body, float dt){
  /* update velocity (vel = accel * Dt) */
  body->velocity.y_velocity += body->acceleration.y_acceleration * dt;
  body->velocity.x_velocity += body->acceleration.x_acceleration * dt;
  
  /* update body position (pos = vel * Dt) */
  body->position.y_position += body->velocity.y_velocity * dt;
  body->position.x_position += body->velocity.x_velocity * dt;
}

/* collision handling */
void handle_collisions(Body *body){
  if(body->position.y_position > GROUND_HEIGHT) {
    body->position.y_position = GROUND_HEIGHT;
    // reverse direction and lose vel 
    body->velocity.y_velocity = -body->velocity.y_velocity * BOUNCE_DAMPING;
    if (fabs(body->velocity.y_velocity) < 5.0f) {
      body->velocity.y_velocity = 0.0f;
    }
  }
}

/* --- RENDERING SDL --- */
void draw_body(SDL_Renderer *renderer, Body *body){
  SDL_Rect rectangle = {
    (int)body->position.x_position,
    (int)body->position.y_position,
    20,
    20
  };
  SDL_RenderFillRect(renderer, &rectangle);
}

/* Main function */
int main(){

  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window = SDL_CreateWindow(
    "My first physics simulator (shitty ahh)",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    0
  );

  SDL_Renderer *renderer = SDL_CreateRenderer(
    window,
    -1,
    SDL_RENDERER_ACCELERATED
  );

  Body body = {
    .position = {80.0f, 120.0f},
    .velocity = {100.0f, 0.0f},
    .acceleration = {0.0f, 0.0f},
    .mass = 2.0f
  };

  int is_running = 1;
  SDL_Event event;

  while (is_running){

    // input 
    while (SDL_PollEvent(&event)){
      if (event.type == SDL_QUIT) {
        is_running = 0;
      }
    }

    /* --- PHYSICS --- */
    // reset accel every frame 
    body.acceleration.x_acceleration = 0.0f;
    body.acceleration.y_acceleration = 0.0f;

    apply_gravity(&body);
    update_physics_values(&body, DT);
    handle_collisions(&body);

    /* --- RENDERING --- */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    draw_body(renderer, &body);

    SDL_RenderDrawLine(
      renderer,
      0,
      (int)GROUND_HEIGHT + 20,
      WINDOW_WIDTH,
      (int)GROUND_HEIGHT + 20
      );

      SDL_RenderPresent(renderer);

      SDL_Delay(16);
  }
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit;

  return 0;
}
