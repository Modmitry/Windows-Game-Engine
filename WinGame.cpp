#include "WinEngine.h"
#include <stdlib.h>
#include <memory.h>

// init game data
void Init()
{
    // .... //

}
//------------------------------------------------------
// update game data,
// dt - time elapsed since the previous update (seconds)
void Act(float dt)
{
  if (is_key_pressed(VK_ESCAPE))
      Quit();


  // .... //

}
//------------------------------------------------------
// filling buffer 
// uint32_t buffer[HEIGHT_SCREEN][WIDTH_SCREEN] - is an array of 32-bit colors (8 bits per R, G, B)
void Draw()
{
  // clear buffer
  memset(buff, 0, HEIGHT_SCREEN * WIDTH_SCREEN * sizeof(uint32_t));

  // .... //

}
//------------------------------------------------------
// game results, free game data
void Finish()
{
    // .... //

}
//------------------------------------------------------
