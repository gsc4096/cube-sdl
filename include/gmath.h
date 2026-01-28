#include <cmath>

#define MATH_PI 3.14159265359

class vec2 {
 public:
  float x;
  float y;
  vec2() : x(0.0), y(0.0) {}
  vec2(const int x, const int y) : x(x), y(y) {}
  vec2(const int arr[2]) : x(arr[0]), y(arr[1]) {}
  vec2(const float arr[2]) : x(arr[0]), y(arr[1]) {}
};

class vec3 {
 public:
  float x;
  float y;
  float z;
  vec3() : x(0.0F), y(0.0F), z(0.0F) {}
  vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
  vec3(const float arr[3]) : x(arr[0]), y(arr[1]), z(arr[2]) {}
  float len(vec3 a, vec3 b);
  float angle(vec3 b) { return atan2(b.y - y, b.x - x); };
};

float sign(float x);
float clamp(float x, float min, float max);
