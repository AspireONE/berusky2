#ifndef __ogg_h__
#define __ogg_h__

#include <AL/al.h>
#include "ogg.h"
#include "vorbisfile.h"

#define BUFFER_SIZE (4096 * 22)


   int ogg_open(char *file, float gain);
   void ogg_release();
   char ogg_playback();
   char ogg_playing();
   char ogg_update();
   void ogg_gain(float gain);

#endif // __ogg_h__
