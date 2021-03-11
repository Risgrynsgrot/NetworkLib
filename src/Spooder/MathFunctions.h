#pragma once

float clamp(float d, float min, float max) 
{
  const float t = d < min ? min : d;
  return t > max ? max : t;
}