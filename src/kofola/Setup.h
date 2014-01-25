#ifndef __SETUP_
#define __SETUP_

#define POCETLISTU 8
#define POCETKLAVES 128
#define POCETAKTICNICHKLAVES 40

typedef struct
{
  char no_sound;
  char camera_type;
  char fullscreen;
  char ditering;  
  char text_detail;             //Texture-level - Uroven detailu textur (0 = max)
  char text_detail_light;       //Uroven detailu textur lightmap (0 = max)  
  char text_mip_mapping;        //Generovat pro startu mip-mapy + jejich povoleni  TF
  char text_mip_filtr;          //Filtr mip-map 0 - linearni/1 - trilinearni TF
  char channels;
  long pre_load;
  char soundvolume;
  char musicvolume;
  char ambientvolume;
  int  key[POCETKLAVES];
  char debug_vypisy;
  char debug_light;
  char kvalita_castic;
  char kvalita_casticv;
  char kvalita_casticp;
  char ovladani;
  char debug_fps;
  char debug_draty;
  char sada[20][MAX_FILENAME];
  char posouvat_kameru;
  char extra_light_vertex;
  float text_ostrost;
  float p_kamera_alfa;
  float p_kamera_radius;
  char cursor[32];
  char mirror_effects;
  char debug_kamery;
  char text_ans;  
  int  text_ans_stupen;
  int  bugs_highlight;
  int  items_highlight;
  int  ovladani_rohy;
  int  ovladani_rohy_default;
  int  ovladani_rohy_smer;
  float ovladani_rohy_rychlost;
  int  ovladani_pr_posun;
  int  animace_okoli;
  char camera_intro;
} SETUP;

void Load_ini(void);
void Save_ini(void);

extern SETUP setup;

#endif
