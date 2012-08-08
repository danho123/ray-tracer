//
// this provide functions to set up the scene
//

#include "sphere.h"
#include <stdio.h>

extern Point light1;
extern float light1_ambient[3];
extern float light1_diffuse[3];
extern float light1_specular[3];

extern float global_ambient[3];
extern Spheres *scene;

extern RGB_float background_clr;
extern float decay_a;
extern float decay_b;
extern float decay_c;

//////////////////////////////////////////////////////////////////////////

/*******************************************
 * set up the default scene - DO NOT CHANGE
 *******************************************/
void set_up_default_scene() {
  // set background color
  background_clr.r = 0.5;
  background_clr.g = 0.05;
  background_clr.b = 0.8;

  // setup global ambient term
  global_ambient[0] = global_ambient[1] = global_ambient[2] = 0.2;

  // setup light 1
  light1.x = -2.0;
  light1.y = 5.0;
  light1.z = 1.0;
  light1_ambient[0] = light1_ambient[1] = light1_ambient[2] = 0.1;
  light1_diffuse[0] = light1_diffuse[1] = light1_diffuse[2] = 1.0;
  light1_specular[0] = light1_specular[1] = light1_specular[2] = 1.0;

  // set up decay parameters
  decay_a = 0.5;
  decay_b = 0.3;
  decay_c = 0.0;

  // sphere 1
  Point sphere1_ctr = {1.5, -0.2, -3.2};
  float sphere1_rad = 1.23;
  float sphere1_ambient[] = {0.7, 0.7, 0.7};
  float sphere1_diffuse[] = {0.1, 0.5, 0.8};
  float sphere1_specular[] = {1.0, 1.0, 1.0};
  float sphere1_shineness = 10;
  float sphere1_reflectance = 0.4;
  float sphere1_refraction = 1.51;
  float sphere1_alpha = 1;
  scene = add_sphere(scene, sphere1_ctr, sphere1_rad, sphere1_ambient,
             sphere1_diffuse, sphere1_specular, sphere1_shineness,
		     sphere1_reflectance, 1, sphere1_refraction, sphere1_alpha);

  // sphere 2
  Point sphere2_ctr = {-1.5, 0.0, -3.5};
  float sphere2_rad = 1.5;
  float sphere2_ambient[] = {0.6, 0.6, 0.6};
  float sphere2_diffuse[] = {1.0, 0.0, 0.25};
  float sphere2_specular[] = {1.0, 1.0, 1.0};
  float sphere2_shineness = 6;
  float sphere2_reflectance = 0.3;
  float sphere2_refraction = 1.51;
  float sphere2_alpha = 1;
  scene = add_sphere(scene, sphere2_ctr, sphere2_rad, sphere2_ambient,
             sphere2_diffuse, sphere2_specular, sphere2_shineness,
		     sphere2_reflectance, 2, sphere2_refraction, sphere2_alpha);

  // sphere 3
  Point sphere3_ctr = {-0.35, 1.75, -2.25};
  float sphere3_rad = 0.5;
  float sphere3_ambient[] = {0.2, 0.2, 0.2};
  float sphere3_diffuse[] = {0.0, 1.0, 0.25};
  float sphere3_specular[] = {0.0, 1.0, 0.0};
  float sphere3_shineness = 30;
  float sphere3_reflectance = 0.3;
  float sphere3_refraction = 1.51;
  float sphere3_alpha = 1;
  scene = add_sphere(scene, sphere3_ctr, sphere3_rad, sphere3_ambient,
             sphere3_diffuse, sphere3_specular, sphere3_shineness,
		     sphere3_reflectance, 3, sphere3_refraction, sphere3_alpha);
 
}

/***************************************
 * You can create your own scene here
 ***************************************/
void set_up_user_scene() 
{

 
}
