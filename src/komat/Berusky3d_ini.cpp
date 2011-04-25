/*
  Berusci smazba
*/
#define  WM_MOUSEWHEEL  0x020A

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#include "3d_all.h"

#include "Object.h"
#include "game_main.h"

#include "Berusky_universal.h"
#include "Berusky3d_castice.h"
#include "Berusky3d.h"
#include "Berusky3d_ini.h"
#include "Berusky3d_load.h"
#include "Berusky3d_render.h"
#include "Berusky3d_animace.h"
#include "Berusky3d_kofola_interface.h"

dword      system_timer = 0;
dword      system_kurzor = 0;
dword      karmin_aktivni = 1;
dword      obsluha_okna = 0;
dword      play_demo = 0;
HWND       hwnd_hry;
HINSTANCE  hinst;
char       ini_file[300];
int        mouse_move = FALSE;

#ifdef DEBUG_OKNO
  dword      start_debug;
#endif

char work_dir[200] = ".";

void flip(void)
{
  AGE_MAIN *p_age = p_ber->p_age;
  GRAPH3D *p_grf = p_age->graph_get();
  p_grf->flip();
}


/*
inline void nacti_polohu_mysi(WPARAM wParam, LPARAM lParam)
{    
  mi.dx = LOWORD(lParam)- mi.x;
  mi.dy = HIWORD(lParam)- mi.y;
  mi.x = LOWORD(lParam);
  mi.y = HIWORD(lParam);
}                                     

inline void nacti_tlacitka_mysi(WPARAM wParam, LPARAM lParam)
{                                     
  mi.ot1 = mi.dt1 = mi.t1 = LOWORD(wParam) & MK_LBUTTON;
  mi.ot2 = mi.dt2 = mi.t2 = LOWORD(wParam) & MK_RBUTTON;  
}                                     
*/
inline int filtr_klaves(int scancode)
{   
  return(gl_Allow_Key(scancode) ? scancode : 0);
}

/*
long CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{  
  switch(message) {
  case WM_MOUSEWHEEL:
    
    if(!PRCameraFlag)
      break;
    
    PRCameraFlagChange = FALSE;
    
    if((signed short)HIWORD(wParam) > 0)
      key[K_PLUS] = TRUE;
    else
      key[K_MINUS] = TRUE;
    break;                     

  case WM_ACTIVATEAPP:   
    if(wParam) {
      karmin_aktivni = TRUE;
      system_kurzor = FALSE;
      if(obsluha_okna)
        maximalizuj_hru();
    } else {
      karmin_aktivni = FALSE;
      system_kurzor = TRUE;
      if(obsluha_okna)
        minimalizuj_hru();
    }
    break;
  case WM_CLOSE:
    PostQuitMessage(0);
    return(FALSE);
  case WM_QUIT: // ukonceni aplikace - hodit sem konec!
    break;  
  case WM_KEYDOWN:
    {
      int scan = filtr_klaves(MapVirtualKey(wParam,0));
      if(scan) {
        key[scan] = TRUE;
        key[0] = TRUE;
      }           
      break;
    } 
  case WM_KEYUP:
    {
      int scan = filtr_klaves(MapVirtualKey(wParam,0));
      if(scan) {
        key[scan] = FALSE;
        key[0] = FALSE;
      }         
      break;
    }
  case WM_CHAR:
    key_pressed = (TCHAR) wParam;
    break;
  case WM_MOUSEMOVE:    
    nacti_polohu_mysi(wParam, lParam);
    break;
  case WM_LBUTTONUP:
    nacti_tlacitka_mysi(wParam, lParam);
    break;
  case WM_RBUTTONUP:
    nacti_tlacitka_mysi(wParam, lParam);
    break;
  case WM_LBUTTONDOWN: 
    nacti_tlacitka_mysi(wParam, lParam);
    break;
  case WM_RBUTTONDOWN:
    nacti_tlacitka_mysi(wParam, lParam);
    break;
  case WM_SETCURSOR:
    if(!mouse_move)
      mouse_move = FALSE;
    break;    
    
  }  
  return DefWindowProc( hwnd, message, wParam, lParam );
}
*/

void minimalizuj_hru(void)
{
/*
  if(hwnd_hry) {
    grf_prehod_mod_zpet(&hwconf);
    ShowWindow(hwnd_hry,SW_MINIMIZE);
    UpdateWindow(hwnd_hry);
    gl_Kofola_Minimalize();
    clip_ret();
  }
*/
}

void maximalizuj_hru(void)
{
/*
  if(hwnd_hry) {
    grf_prehod_mod_hra(&hwconf);
    ShowWindow(hwnd_hry,SW_RESTORE);
    UpdateWindow(hwnd_hry);
    nastav_okno(&hwconf,FALSE);
    SetFocus(hwnd_hry);
    gl_Kofola_Maximalize();
  }
*/
}

int nahraj_konfig(void)
{
  /* Nahraje konfiguraci grafiky
  */
  nahraj_device_config(ini_file,"hra",&hwconf);
  nahraj_universal_device_config(ini_file,"hra",&hwconf);
  nahraj_texture_config(ini_file, &txconf);

  return(TRUE);
}

int kom_graf_init(void)
{
  int full;

  nahraj_konfig();

  full = hwconf.fullscreen;

  /* Nahodi graficky rezim
  */
  if(full) {    
//    nastav_okno(&hwconf,FALSE);
    if(!grf_prehod_mod_hra(&hwconf))
      return(FALSE);
//    hwnd_hry = zavri_okno(hwnd_hry);
//    hwnd_hry = otevri_okno(hinst,hwconf.fullscreen,0,0,&hwconf);
  }
  
  kprintf(1,"Screen mode %d ok",full);
  
  /* Nahodi grafiku
  */
/*
  if(!grf_start(hinst,hwnd_hry,ini_file,NULL,TRUE))
    chyba("Inicializace");
*/
  /* Nahozeni renderovacich funkci
  */ 
  ber_nahod_render_funkce();

  /* Nastavni rozliseni
  */
  p_ber->wx = hwconf.xstart;
  p_ber->wy = hwconf.ystart;
  p_ber->wxres = hwconf.xres;
  p_ber->wyres = hwconf.yres;

  /* Vyflushni veci z fronty
  */
  spracuj_spravy(TRUE);

  /* Nahodim svoji obsluhu okna
  */
  obsluha_okna = TRUE;

  return(TRUE);
}

void kom_ret_default_text_config(void)
{ 
 txconf.text_mip_mapping = GetPrivateProfileInt(TXT_SEKCE,"text_mip_mapping",1,ini_file);
 txconf.text_mip_filtr =   GetPrivateProfileInt(TXT_SEKCE,"text_mip_filtr",1,ini_file);
 txconf.text_wrap_x = 1;
 txconf.text_wrap_y = 1;
}

void kom_graf_konec(int menu)
{
  kprintf(TRUE,"kom_graf_konec...");

  /* Shodim svoji obsluhu okna
  */
  obsluha_okna = FALSE;

  /* Shodim GL
  */
  konec(FALSE);
  
  if(menu) { // - zpet do menu - full screen
    grf_prehod_mod_menu();
//    nastav_okno(&hwconf,TRUE);
    spracuj_spravy(0);
  }
}

void konec(int konec)
{
  /* Stop grafiky
  */
  grf_stop();
  
  /* Zapne kurzor
  */
  zapni_kurzor();

  if(konec) {    
    exit(0);
  }
}


/* 
 * Camera commands
 */
#define CAMERA_POLAR_NAME "CONTROL_CAMERA"

static CAMERA_POLAR * camera_control_get(AGE_MAIN *p_age)
{
  SCENE *p_scene = p_age->scene_get();
  
  CAMERA_POLAR *p_camera = (CAMERA_POLAR *)p_scene->camera_get(CAMERA_POLAR_NAME);
  if(!p_camera) {
    p_camera = p_scene->camera_polar_create(CAMERA_POLAR_NAME);
    p_camera->target_set(0,0,0);
    p_camera->distance_set(20);
    p_camera->elevation_set(DEG2RAD(60));
    p_camera->plane_auto_adjust_set(TRUE);    
  }
  
  p_scene->camera_active_set(p_camera);
  
  assert(p_camera);
  return(p_camera);
}

static CAMERA_POLAR * camera_control_center_get(AGE_MAIN *p_age)
{
  CAMERA_POLAR *p_camera = camera_control_get(p_age);
  SCENE   *p_scene = p_age->scene_get();
  int     sx,sy,sdx,sdy;
  
  p_camera->view_get(&sx,&sy,&sdx,&sdy);
  float depth = p_scene->scene_box_depth_get();
  VECT v1((float)(sx+sdx/2),(float)(sy+sdy/2),depth);
  
  p_camera->screen_to_world(&v1);
  p_camera->target_set(&v1);
  p_camera->distance_set(depth);
  
  return(p_camera);
}

bool scene_reset(AGE_MAIN *p_age)
{
  CAMERA_POLAR *p_camera = camera_control_get(p_age);
  SCENE *p_scene = p_age->scene_get();
  
  VECT  s_center;
  VECT  s_length;
  
  p_scene->scene_box_center_get(&s_center);  // Return center of the scene
  p_scene->scene_box_length_get(&s_length);  // Return size of the scene
    
  p_camera->target_set(&s_center);
  p_camera->elevation_set(DEG2RAD(60));
  p_camera->rotation_set(DEG2RAD(0));
  p_camera->distance_set(s_length.size()*2);
    
  return(TRUE);
}


bool main_callback(AGE_MAIN *p_age)
{
  // Draw frame
  p_age->graph_draw();
  return(TRUE);
}

int main(int argc, char **argv)
{
  char *p_level;
  char pom[200];
  
  getcwd(ini_file,500);
  strcat(ini_file,"//berusky3d.ini");
  if(!efile(ini_file)) {
   strcpy(ini_file,"berusky3d.ini");
  } 
  
  nahraj_konfig();
  
  if(GetPrivateProfileInt("debug","debug_file",0,ini_file)) {
   GetPrivateProfileString("hra","log_file","c:\\berusky2log.txt",pom,500,ini_file);
   p_ber->debug_file = fopen(pom,"a");   
  } 
  
  ber_konfiguruj_berusky(&ber);

  AGE_MAIN *p_age = p_ber->p_age = new AGE_MAIN(main_callback);

  //p_age->graph_set(XRES_MENU,YRES_MENU,BPP_DEFAULT,hwconf.fullscreen);
  p_age->graph_set(XRES_MENU,YRES_MENU);
  if(!p_age->graph_screen_create())
    return(FALSE);
  
  SCENE *p_scene = p_age->scene_new();
  p_age->scene_active_set(p_scene);
  
  GRAPH3D *p_grf = p_age->graph_get();
  if(p_grf) {
    p_grf->config_draw_grid(FALSE);
    p_grf->config_draw_mouse_cursor(FALSE);
    p_grf->config_draw_boxes(FALSE);
    p_grf->config_draw_console(FALSE);
    p_grf->config_draw_fps(FALSE);
    p_grf->config_draw_normals(FALSE);
    p_grf->config_opengl_lighting(FALSE);
    p_grf->config_draw_all_objects(TRUE);
    p_grf->config_draw_pivots(TRUE);
    p_grf->config_draw_selection(TRUE);    
  }

  //p_scene->load(NULL, "/home/komat/auto.b2m");  

  scene_reset(p_age);

  //p_age->run();

  p_level = (argc > 1) ? argv[1] : (char *)"a";
  winmain_Game_Run(p_level);

  return(TRUE);
}

int spracuj_spravy(int param)
{
/*
 MSG  msg;
 
 while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )) {
   TranslateMessage(&msg);
   DispatchMessage(&msg);
   if(msg.message == WM_QUIT) {
     gl_Kofola_End(bDXAktivni);
   }
 }
 return(1);
*/
}
