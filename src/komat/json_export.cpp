/*
 *        .þÛÛþ þ    þ þÛÛþ.     þ    þ þÛÛÛþ.  þÛÛÛþ .þÛÛþ. þ    þ
 *       .þ   Û Ûþ.  Û Û   þ.    Û    Û Û    þ  Û.    Û.   Û Ûþ.  Û
 *       Û    Û Û Û  Û Û    Û    Û   þ. Û.   Û  Û     Û    Û Û Û  Û
 *     .þþÛÛÛÛþ Û  Û Û þÛÛÛÛþþ.  þþÛÛ.  þþÛÛþ.  þÛ    Û    Û Û  Û Û
 *    .Û      Û Û  .þÛ Û      Û. Û   Û  Û    Û  Û.    þ.   Û Û  .þÛ
 *    þ.      þ þ    þ þ      .þ þ   .þ þ    .þ þÛÛÛþ .þÛÛþ. þ    þ
 *
 * AGE (C) AnakreoN
 * Martin Stransky <stransky@anakreon.cz> 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include "3d_all.h"
#include "json_export.h"
#include <string>

FILE * export_file = NULL;

void json_export_matrix(FILE *f, GLMATRIX *p_matrix, const char *p_name)
{
  fprintf(f,"{\n");
  fprintf(f,"    \"name\" : \"%s\",\n", p_name);
  fprintf(f,"    \"x-vector\" : [ %f, %f, %f, %f ],\n", p_matrix->_11,p_matrix->_21,p_matrix->_31,p_matrix->_41);
  fprintf(f,"    \"y-vector\" : [ %f, %f, %f, %f ],\n", p_matrix->_12,p_matrix->_22,p_matrix->_32,p_matrix->_42);
  fprintf(f,"    \"z-vector\" : [ %f, %f, %f, %f ],\n", p_matrix->_13,p_matrix->_23,p_matrix->_33,p_matrix->_43);
  fprintf(f,"    \"w-vector\" : [ %f, %f, %f, %f ]\n", p_matrix->_14,p_matrix->_24,p_matrix->_34,p_matrix->_44);
  fprintf(f,"  },\n");
}

void json_export_texture(FILE *f, EDIT_TEXT *p_text)
{
  fprintf(f,"{\n");
  fprintf(f," \"type\" : \"texture\",\n");
  fprintf(f," \"name\" : \"%s\",\n", p_text->jmeno);
  //fprintf(f," file: %s\n", p_text->p_bmp);
  fprintf(f,"}\n");
}

void json_export_material(FILE *f, EDIT_MATERIAL *p_mat)
{
  fprintf(f,"{\n");
  fprintf(f,"  \"type\" : \"material\",\n");
  fprintf(f,"  \"name\" : \"%s\",\n", p_mat->jmeno);
  fprintf(f,"  \"textures\" : [ ");
  int i;
  for(i = 0; i < MAT_TEXTUR; i++) {
    std::string text;
    text.append(p_mat->textfile[i]);
    if(text.compare("")){
      if(i == 0){
        fprintf(f,"\"%s\"", p_mat->textfile[i]);
      } 
      else{
        fprintf(f,", \"%s\"", p_mat->textfile[i]);
      }
     }
  }
  fprintf(f," ]\n");
  fprintf(f,"}");
}

void json_export_materialy(FILE *f, EDIT_MATERIAL ** p_mat, int max_mat)
{
  int i;
  for(i = 0; i < max_mat; i++) {
    if(p_mat[i]) {
      json_export_material(f,p_mat[i]);
      if(i + 1 < max_mat) fprintf(f, ",\n");
    }
  }
}


/*
// Konstni animace objektu
typedef struct _EDIT_OBJEKT
{
  char jmeno[MAX_JMENO];
  char jmeno_parent[MAX_JMENO];
  int Objekt_ID;                // ID objektu

  word vertexnum;
  OBJ_VERTEX *p_vertex;
  BOD *p_vertex_kosti;          // kostni vertexy
  word facenum;
  word *p_face;
  dword *p_fsous;
  GLenum face_typ;              // typ facu
  int optnum;                   // optimalizovane facy
  int *p_opt;

  word linenum;
  word *p_line;
  GLenum line_typ;              // typ car
  int material;                 // pouzity material (z tabulky materialu)
  int material_sub;
  float float_scale;            // aktualni float-scale
  float float_scale_last;       // minuly float scale

  word x_rect;                  // x ctvercu objektu
  word y_rect;                  // y ctvercu objektu
  char kreslit;

  BOD pivot;                    // pivot-point objektu (relativni)
  char trans;                   // transformovane svetlo (je preveden na billboard a spocitane okoli)
  dword m1flag;                 // flagy objektu
  dword m2flag;                 // flagy objektu
  dword oflag;

  TRACK_INFO *p_track;          // track info k animaci  
  GLMATRIX m;                   // matrix k animacim

  OBB_OLD obb;
  void *p_tmp_top;              // pomocny pointer na kontejner
  struct _EDIT_OBJEKT *p_tmp_next;      // pomocny pointer na dalsi objekt

  JOINT_ANIMACE *p_joit_animace;        // pointer na joint-animace
  struct _EDIT_OBJEKT *p_lod;   // LOD objekty tohodlenctoho

  SIMPLE_TRACK_INFO *p_tmp_sim;
} EDIT_OBJEKT;
*/
void json_export_object(FILE *f, EDIT_OBJEKT *p_obj, 
                        EDIT_MATERIAL ** p_mat, int max_mat)
{
  fprintf(f,"   {\n");
  fprintf(f,"    \"name\" : \"%s\",\n", p_obj->jmeno);
  fprintf(f,"    \"name_parent\" : \"%s\",\n", p_obj->jmeno_parent);
  fprintf(f,"    \"object_id\" : \"%d\",\n", p_obj->Objekt_ID);
  fprintf(f,"    \"material\" : \"%s\",\n", p_mat[p_obj->material]->jmeno);

  fprintf(f,"    \"vertexPositions\" : [");
  int i;
  for(i = 0; i < p_obj->vertexnum-1; i++) {
    fprintf(f,"%f,%f,%f,",p_obj->p_vertex[i].x,p_obj->p_vertex[i].y,p_obj->p_vertex[i].z);
  }
  fprintf(f,"%f,%f,%f",p_obj->p_vertex[p_obj->vertexnum-1].x,
                       p_obj->p_vertex[p_obj->vertexnum-1].y,
                       p_obj->p_vertex[p_obj->vertexnum-1].z);
  fprintf(f,"],\n");

  fprintf(f,"    \"vertexNormals\" : [");
  for(i = 0; i < p_obj->vertexnum-1; i++) {
    fprintf(f,"%f,%f,%f,",p_obj->p_vertex[i].nx,p_obj->p_vertex[i].ny,p_obj->p_vertex[i].nz);
  }
  fprintf(f,"%f,%f,%f",p_obj->p_vertex[p_obj->vertexnum-1].nx,
                       p_obj->p_vertex[p_obj->vertexnum-1].ny,
                       p_obj->p_vertex[p_obj->vertexnum-1].nz);
  fprintf(f,"],\n");

  fprintf(f,"    \"vertexTextureCoords0\" : [");
  for(i = 0; i < p_obj->vertexnum-1; i++) {
    fprintf(f,"%f,%f,",p_obj->p_vertex[i].tu1,p_obj->p_vertex[i].tv1);
  }
  fprintf(f,"%f,%f",p_obj->p_vertex[p_obj->vertexnum-1].tu1,
                    p_obj->p_vertex[p_obj->vertexnum-1].tv1);
  fprintf(f,"],\n");

  fprintf(f,"    \"vertexTextureCoords1\" : [");
  for(i = 0; i < p_obj->vertexnum-1; i++) {
    fprintf(f,"%f,%f,",p_obj->p_vertex[i].tu2,p_obj->p_vertex[i].tv2);
  }
  fprintf(f,"%f,%f",p_obj->p_vertex[p_obj->vertexnum-1].tu2,
                    p_obj->p_vertex[p_obj->vertexnum-1].tv2);
  fprintf(f,"],\n");

  fprintf(f,"    \"indices\" : [");
  for(i = 0; i < p_obj->facenum-1; i++) {
    fprintf(f,"%d,",p_obj->p_face[i]);
  }
  fprintf(f,"%d",p_obj->p_face[p_obj->facenum-1]);
  fprintf(f,"]\n");

  fprintf(f,"   }");
}

void json_export_kont(FILE *f, EDIT_KONTEJNER *p_kont, 
                      EDIT_MATERIAL ** p_mat, int max_mat)
{
  fprintf(f,"{\n");
  fprintf(f,"  \"type\" : \"geometry_container\",\n");
  fprintf(f,"  \"name\" : \"%s\",\n", p_kont->jmeno);
  fprintf(f,"  \"object_id\" : \"%d\",\n", p_kont->kontejner_ID);
  fprintf(f,"  \"matrix\" : ");
  json_export_matrix(f, &p_kont->world, "world");

  fprintf(f,"  \"geometry_objects\" : [\n");
  int i;
  for(i = 0; i < MAX_KONT_OBJEKTU; i++) {
    if(p_kont->p_obj[i]) {
      if(i != 0){
        fprintf(f, ",\n");
      }
      json_export_object(f,p_kont->p_obj[i], p_mat, max_mat);
      //if(i + 1 < MAX_KONT_OBJEKTU){
      //  fprintf(f, ",");
      //}
    }
  }
  fprintf(f,"\n  ]\n");

  fprintf(f,"}");
}

void json_export_kontejnery(FILE *f, EDIT_KONTEJNER **p_kont, int max_kont,
                            EDIT_MATERIAL ** p_mat, int max_mat)
{
  int i;
  for(i = 0; i < max_kont; i++) {
    if(p_kont[i]) {
      if(i != 0) { fprintf(f, ",\n"); }
      json_export_kont(f,p_kont[i], p_mat, max_kont);
    }
  }
}

/*
// pouze na staticke objekty
typedef struct _EDIT_MESH_POLY
{                               //celej kontejner    
  char jmeno[MAX_JMENO];        // jmeno poly
  int facenum;                  // co face to objekt
  TEXT_KOORD *p_koord;          // list texture koordinatu - plosky po 3

  LIGHTMAP_FACE *p_ind;         // lightmapovaci indicie, je jich facenum/3
  dword *p_fsous;               // sousedni plosky
  dword m1flag;                 // flag materialu -> stejne jako u kontejneru
  dword m2flag;                 // flag mat 2 -> stejne jako u kontejneru
  dword kflag;                  // flag kontejneru
  dword k2flag;                 // flag kontejneru
  int material;                 // material poly
  int kont;                     // cislo kontejneru, ze ktereho bylo poly vyrobeno
  int poly;                     // poradove cislo poly v seznamu polyu

  MLZNA_KOSTKA *p_mlha;         // mlzna kostka polyho
  int lightnum;                 // pocet lightmap
  int *p_lightnum;              // pocty light-map
  EDIT_TEXT **p_light;          // pole  light-map    
  char kreslit;
  int mail;                     // mailbox poly :)
  OBB_OLD obb;
  // Svetla poly - dynamicke 
  void **p_dlight;              // pole svetel meshu 
  int lightmax;                 // velikost pole na svetla
  int lightakt;                 // aktualni vekikost

  // Svetla poly - extra-dynamicke
  int top_edlight;              // top-svetlo
  void **p_edlight;             // pole extra svetel meshu 
  int edlightmax;               // velikost pole na extra svetla
  int edlightakt;               // aktualni velikost

  VERTEX_ARRAYS varray;         // vertex-arrays
} EDIT_MESH_POLY;

for (l = 0; l < p_poly->lightnum; l++) {
  glBegin(GL_TRIANGLES);
  p_koord = p_poly->p_koord + last;
  for (i = 0; i < p_poly->p_lightnum[l]; i++) {
    glTexCoord2fv(&p_koord[i].tu1);
    glColor4fv(&p_koord[i].dr);
    glNormal3fv(&p_koord[i].nx);
    glVertex3fv(&p_koord[i].x);
  }
  glEnd();
  last += p_poly->p_lightnum[l];
}

*/
void json_export_poly(FILE *f, EDIT_MESH_POLY *p_poly,
                      EDIT_MATERIAL ** p_mat, int max_mat)
{
  fprintf(f,"{\n");
  fprintf(f,"  \"type\" : \"geometry_container\",\n");
  fprintf(f,"  \"name\" : \"%s\",\n", p_poly->jmeno);
  fprintf(f,"  \"object_id\" : \"%d\",\n", -1);

  GLMATRIX m;
  init_matrix(&m);
  fprintf(f,"  \"matrix\" : ");
  json_export_matrix(f, &m, "world");
  
  fprintf(f,"  \"geometry_objects\" : [\n");
  fprintf(f,"   {\n");
  fprintf(f,"    \"name\" : \"%s\",\n", p_poly->jmeno);
  fprintf(f,"    \"material\" : \"%s\",\n", p_mat[p_poly->material]->jmeno);

  fprintf(f,"    \"vertexPositions\" : [");
  int i;
  TEXT_KOORD *p_koord = p_poly->p_koord;
  for(i = 0; i < p_poly->facenum-1; i++, p_koord++) {
    fprintf(f,"%f,%f,%f,",p_koord->x,p_koord->y,p_koord->z);
  }
  fprintf(f,"%f,%f,%f",p_koord->x,p_koord->y,p_koord->z);
  fprintf(f,"],\n");

  fprintf(f,"    \"vertexNormals\" : [");
  p_koord = p_poly->p_koord;
  for(i = 0; i < p_poly->facenum-1; i++, p_koord++) {
    fprintf(f,"%f,%f,%f,",p_koord->nx,p_koord->ny,p_koord->nz);
  }
  fprintf(f,"%f,%f,%f",p_koord->nx,p_koord->ny,p_koord->nz);
  fprintf(f,"],\n");

  fprintf(f,"    \"vertexTextureCoords0\" : [");
  p_koord = p_poly->p_koord;
  for(i = 0; i < p_poly->facenum-1; i++, p_koord++) {
    fprintf(f,"%f,%f,",p_koord->tu1,p_koord->tv1);
  }
  fprintf(f,"%f,%f",p_koord->tu1,p_koord->tv1);
  fprintf(f,"],\n");

  fprintf(f,"    \"vertexTextureCoords1\" : [");
  p_koord = p_poly->p_koord;
  for(i = 0; i < p_poly->facenum-1; i++, p_koord++) {
    fprintf(f,"%f,%f,",p_koord->tu2,p_koord->tv2);
  }
  fprintf(f,"%f,%f",p_koord->tu2,p_koord->tv2);
  fprintf(f,"],\n");

  fprintf(f,"    \"indices\" : [");  
  for (i = 0; i < p_poly->facenum-1; i++) {
    fprintf(f,"%d,",i);
  }
  fprintf(f,"%d",i);    
  fprintf(f,"]\n");

  fprintf(f,"   }\n  ]\n");
  fprintf(f,"}");
}

void json_export_poly(EDIT_MESH_POLY *p_poly, int polynum,
                      EDIT_MATERIAL ** p_mat, int max_mat)
{
  fprintf((FILE*)export_file, ",\n");
  //fprintf((FILE*)export_file, "\n\n\nPOLY EXPORT BEGIN\n\n\n");
  if(export_file) {
    int i;
    for(i = 0; i < polynum; i++) {
      if(i != 0) {fprintf((FILE*)export_file, ",\n");}
      json_export_poly(export_file, p_poly+i, p_mat, max_mat);
    }
  }
  fprintf((FILE*)(export_file), "\n]");
}

void json_export_level(EDIT_KONTEJNER **p_kont, int max_kont, 
                       EDIT_MATERIAL ** p_mat, int max_mat)                       
{
  if(export_file) {
    fprintf((FILE*)(export_file), "[\n");
    json_export_materialy(export_file, p_mat, max_mat);
    json_export_kontejnery(export_file, p_kont, max_kont, p_mat, max_mat);
    //fprintf((FILE*)(export_file), "\n]");
  }
}

void json_export_start(char *p_file)
{
  if(!export_file) {
    export_file = fopen(p_file,"w");
  }
}

void json_export_end(void)
{
  if(export_file) {
    fclose(export_file);
    export_file = NULL;
  }
}
