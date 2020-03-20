#pragma once

class Color : public Animation
{
public:
  using Animation::Animation;

  void reset()
  {
  }

  void loop()
  {
    fill_solid(strtol((const char *)&Config.color[0], NULL, 16));

    betterShow();
  }
};
