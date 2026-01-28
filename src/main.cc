#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_mouse.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <gmath.h>

#include <array>
#include <cmath>
#include <vector>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

const int __START_TIME = SDL_GetTicks();
float getGameTime() { return (SDL_GetTicks() - __START_TIME) / 1000.0F; }

int main() {
  // initialize SDL
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = SDL_CreateWindow("SDL Cube", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                        SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // define geometry
  std::vector<vec3> verts;
  verts.push_back({-0.5, -0.5, -0.5});
  verts.push_back({0.5, -0.5, -0.5});
  verts.push_back({0.5, -0.5, 0.5});
  verts.push_back({-0.5, -0.5, 0.5});
  verts.push_back({-0.5, 0.5, -0.5});
  verts.push_back({0.5, 0.5, -0.5});
  verts.push_back({0.5, 0.5, 0.5});
  verts.push_back({-0.5, 0.5, 0.5});

  std::vector<std::array<int, 2>> lines;
  lines.push_back({0, 1});
  lines.push_back({1, 2});
  lines.push_back({2, 3});
  lines.push_back({3, 0});
  lines.push_back({4, 5});
  lines.push_back({5, 6});
  lines.push_back({6, 7});
  lines.push_back({7, 4});
  lines.push_back({0, 4});
  lines.push_back({1, 5});
  lines.push_back({2, 6});
  lines.push_back({3, 7});

  // define main variables
  float lastFrameTick = SDL_GetTicks();
  float currentVelocity = 0;
  float maxVelocity = MATH_PI / 2;
  float direction = 0;
  float theta = 0;
  vec3 cameraPos(0, 0, -1.25);

  // start main loop
  bool quit = false;
  while (!quit) {
    // handle delta time
    int frameStartTick = SDL_GetTicks();
    float deltaTime = (SDL_GetTicks() - lastFrameTick) / 1000.0;

    // handle input
    SDL_Event ev;
    while (SDL_PollEvent(&ev) != 0) {
      if (ev.type == SDL_QUIT) {
        quit = true;
      } else if (ev.type == SDL_KEYDOWN) {
        switch (ev.key.keysym.sym) {
          case SDLK_ESCAPE:
            quit = true;
            break;
          case SDLK_LEFT:
            direction = -1;
            break;
          case SDLK_RIGHT:
            direction = 1;
            break;
        }
      } else if (ev.type == SDL_KEYUP) {
        switch (ev.key.keysym.sym) {
          case SDLK_LEFT:
            direction = 0;
            break;
          case SDLK_RIGHT:
            direction = 0;
            break;
        }
      }
    }

    // handle acceleration
    currentVelocity += (MATH_PI / 2) * deltaTime * direction;
    currentVelocity = clamp(currentVelocity, -maxVelocity, maxVelocity);
    theta += (currentVelocity)*deltaTime;

    // handle deceleration
    if (direction == 0 && currentVelocity != 0) {
      currentVelocity -= sign(currentVelocity) * maxVelocity * deltaTime;
    }
    if (abs(currentVelocity) < 0.01) {
      currentVelocity = 0;
    }

    // render background
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    // handle rotation
    std::vector<vec3> worldVerts(verts);
    std::vector<vec2> screenVerts(verts.size());

    for (int i = 0, n = worldVerts.size(); i < n; ++i) {
      vec3 &v = worldVerts[i];

      // rotate around zx axis
      float ax = v.x * cos(theta) - v.z * sin(theta);
      float az = v.x * sin(theta) + v.z * cos(theta);
      v = vec3(ax, v.y, az);

      // acquire normalized screen position
      float yrot = atan2(v.z - cameraPos.z, v.x - cameraPos.x) - MATH_PI / 2.0;
      float yrotfactor = -yrot / MATH_PI;
      float zrot = atan2(v.z - cameraPos.z, v.y - cameraPos.y) - MATH_PI / 2.0;
      float zrotfactor = -zrot / MATH_PI;

      // determine real screen position
      int vx = SCREEN_WIDTH / 2.0 + yrotfactor * SCREEN_WIDTH;
      int vy = SCREEN_HEIGHT / 2.0 + zrotfactor * SCREEN_HEIGHT;
      screenVerts[i] = {vx, vy};
    }

    // draw lines and vertex circles
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    for (const auto &vert_index_pair : lines) {
      vec2 v0 = screenVerts[vert_index_pair[0]];
      vec2 v1 = screenVerts[vert_index_pair[1]];
      thickLineColor(renderer, v0.x, v0.y, v1.x, v1.y, 5, 0xFF00007F);
      filledCircleColor(renderer, v0.x, v0.y, 7, 0xFF00007F);
    }

    // update screen
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderPresent(renderer);

    // adjust main loop frame rate
    lastFrameTick = frameStartTick;
    float ideal_frame_duration = 1000.0 / 60.0;
    float frame_duration = SDL_GetTicks() - frameStartTick;
    float time_remaining = ideal_frame_duration - frame_duration;
    if (time_remaining > 0.0) {
      SDL_Delay(time_remaining);
    }
  }

  // gracefully shutdown
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
