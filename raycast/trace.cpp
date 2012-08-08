#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include "global.h"
#include "sphere.h"
#include <iostream>
//
// Global variables
//
extern int win_width;
extern int win_height;

extern GLfloat frame[WIN_HEIGHT][WIN_WIDTH][3];  

extern float image_width;
extern float image_height;

extern Point eye_pos;
extern float image_plane;
extern RGB_float background_clr;
extern RGB_float null_clr;

extern Spheres *scene;

// light 1 position and color
extern Point light1;
extern float light1_ambient[3];
extern float light1_diffuse[3];
extern float light1_specular[3];

// global ambient term
extern float global_ambient[3];

// light decay parameters
extern float decay_a;
extern float decay_b;
extern float decay_c;


extern bool supersampling_on;
extern bool shadow_on;
extern bool reflection_on;
extern bool refraction_on;
extern bool chessboard_on;
extern bool stochastic_ray_gen_on;

extern int step_max;

extern Vector N_plane;
extern Point p0;

float epsilon = 0.0001;
float x_grid_size = image_width / float(win_width);
float y_grid_size = image_height / float(win_height);	


void globalIllumination(Spheres *sph, RGB_float *color)
{
	color->r += global_ambient[0]*sph->reflectance;
	color->g += global_ambient[1]*sph->reflectance;
	color->b += global_ambient[2]*sph->reflectance;
}

void localIllumination(Spheres *sph, RGB_float *color)
{
	color->r += global_ambient[0]*sph->reflectance;
	color->g += global_ambient[1]*sph->reflectance;
	color->b += global_ambient[2]*sph->reflectance;
}

void diffuseIllumination(Spheres *sph, RGB_float *color, Vector surf_norm, Vector light_vec, float c)
{
	color->r += c*light1_diffuse[0]*sph->mat_diffuse[0]*vec_dot(surf_norm, light_vec);
	color->g += c*light1_diffuse[1]*sph->mat_diffuse[1]*vec_dot(surf_norm, light_vec); 
	color->b += c*light1_diffuse[2]*sph->mat_diffuse[2]*vec_dot(surf_norm, light_vec);
}

void specularIllumination(Spheres *sph, RGB_float *color, Vector reflect_vec, Vector eye_vec, float c)
{
	color->r += c*(light1_specular[0]*sph->mat_specular[0]*(pow(vec_dot(reflect_vec, eye_vec), sph->mat_shineness)));
	color->g += c*(light1_specular[1]*sph->mat_specular[1]*(pow(vec_dot(reflect_vec, eye_vec), sph->mat_shineness)));
	color->b += c*(light1_specular[2]*sph->mat_specular[2]*(pow(vec_dot(reflect_vec, eye_vec), sph->mat_shineness)));
}

Vector getReflectedRay(Vector surface_norm, Vector light_ray)
{
	float cosTheta1 = vec_dot(surface_norm, vec_scale(light_ray,-1));
	Vector reflect_ray = vec_plus(light_ray, vec_scale(surface_norm, 2*cosTheta1));
	return reflect_ray;

}

Vector getRefractedRay(float initial_refraction, Spheres *sph, Vector surface_norm, Vector light_ray)
{
	float refraction_index = sph->refraction_index;
	float n = initial_refraction/refraction_index;

	Vector refract_ray;
	//n = .9;
	//light_ray.x = 0.707107;
	//light_ray.y = -0.707107;
	//light_ray.z = 0;
	//surface_norm.x = 0;
	//surface_norm.y = 1;
	//surface_norm.z = 0;

	float cosTheta1 = vec_dot(surface_norm, vec_scale(light_ray,-1));
	float cosTheta2 = sqrt(1.0f-pow(n,2)*(1-(cosTheta1*cosTheta1)));
	
	Vector a = vec_scale(light_ray, n);
	Vector b = vec_scale(surface_norm, n*cosTheta1 - cosTheta2);

	if(cosTheta1 > 0.0f)
	{
		refract_ray = vec_plus(vec_scale(light_ray,n), vec_scale(surface_norm, n*cosTheta1-cosTheta2));
	}
	else
	{
		refract_ray = vec_minus(vec_scale(light_ray,n), vec_scale(surface_norm, n*cosTheta1-cosTheta2));
	}
	return refract_ray;
}

bool intersect_plane(Point p, Vector u, Vector plane_normal, Point plane_point, Point *hit)
{
	RGB_float color;
	
	Vector temp_vec;
	temp_vec.x = p.x - plane_point.x;
	temp_vec.y = p.y - plane_point.y;
	temp_vec.z = p.z - plane_point.z;

	float numerator = vec_dot(plane_normal, temp_vec);
	float denom = vec_dot(plane_normal, u);
	float t = -numerator/denom;

	if(denom == 0 && numerator !=0) return false; //no intersection
	else if(t>0){
		hit->x = p.x + t*u.x;
		hit->y = p.y + t*u.y;
		hit->z = p.z + t*u.z;
		return true;
	}
	else return false; //intersection in 1 point
}

RGB_float colorPlane(Point p)
{
	RGB_float color;
	if(int(p.x) % 2 == 0 && int(p.z) % 2 == 0)
	{
		color = {0,0,0};
	}
	else if (int(p.x) % 2 != 0 && int(p.z) % 2 == 0) color = {1,1,1};
	else if (int(p.x) % 2 == 0 && int(p.z) % 2 != 0) color = {1,1,1};
	else color = {0,0,0};

	return color;
}

/////////////////////////////////////////////////////////////////////

/*********************************************************************
 * Phong illumination - you need to implement this!
 *********************************************************************/
RGB_float phong(Point p, Vector eye_vec, Vector surf_norm, Spheres *sph) {

	Vector light_vec = get_vec(p, light1);
	float d = vec_len(get_vec(p, light1));
	normalize(&light_vec);

	Vector reflect_vec = getReflectedRay(surf_norm, light_vec);
	normalize(&reflect_vec);

	RGB_float ambient = {0,0,0};
	RGB_float diffuse = {0,0,0};
	RGB_float specular = {0,0,0};
	RGB_float color = {0,0,0};

	

	float attenuation_coeff = 1/(decay_a + decay_b*d + decay_c*pow(d,2));
	
	//global ambience
	globalIllumination(sph, &ambient);
	//local ambience
	localIllumination(sph, &ambient);

	//diffuse
	diffuseIllumination(sph, &color, surf_norm, light_vec, attenuation_coeff);

	//specular
	specularIllumination(sph, &color, reflect_vec, eye_vec, attenuation_coeff);

	//add diffuse + specular
	//diffuse = clr_add(diffuse, specular);

	//scale by attenuation coefficient
	//diffuse = clr_scale(diffuse, attenuation_coeff);

	//add ambient to diffuse+specular
	//color = clr_add(ambient, diffuse);

	Vector shadow_vec = get_vec(p, light1);

	if(inShadow(p, shadow_vec, scene, sph) && shadow_on)
	{
		color = ambient;
	}

	return color;
}

/************************************************************************
 * This is the recursive ray tracer - you need to implement this!
 * You should decide what arguments to use.
 ************************************************************************/

RGB_float recursive_ray_trace(Vector ray, Point p, int step) {

	RGB_float color = background_clr;
	RGB_float reflected_color = {0,0,0};
	RGB_float refracted_color = {0,0,0};
	
	Spheres *closest_sph;
	Point *hit = new Point;
	closest_sph = intersect_scene(p, ray, scene, hit);
	

	//get the point color here
	//intersects a sphere
	Point *plane_hit = new Point;
	color = background_clr;
	if(chessboard_on && intersect_plane(p, ray, N_plane, p0, plane_hit))
	{
		Vector eye_vec = get_vec(*plane_hit, eye_pos);
		Vector light_vec = get_vec(*plane_hit, p);
		normalize(&light_vec);
		normalize(&eye_vec);
		color = colorPlane(*plane_hit);
		Vector shadow_vec = get_vec(*plane_hit, light1);
		Spheres *sph = NULL;
		if(inShadow(*plane_hit, shadow_vec, scene, sph) && shadow_on)
		{
			color = clr_scale(color, .5);
		}

	}

	if(closest_sph != NULL)
	{
		Vector eye_vec = get_vec(*hit, eye_pos);
		Vector surf_norm = sphere_normal(*hit, closest_sph);
		Vector light_vec = get_vec(*hit, p);
		normalize(&light_vec);
		normalize(&surf_norm);
		normalize(&eye_vec);

		color = phong(*hit, eye_vec, surf_norm, closest_sph);

		if(step < step_max && reflection_on)
		{
			Vector reflect_vec = vec_minus(vec_scale(surf_norm, vec_dot(surf_norm, light_vec)*2), light_vec);
			step += 1;
			normalize(&reflect_vec);

			reflected_color = recursive_ray_trace(reflect_vec, *hit, step);
			reflected_color = clr_scale(reflected_color, closest_sph->reflectance);
			color = clr_add(color, reflected_color);

		}

		if(step < step_max && refraction_on)
		{
			Vector refracted_ray = getRefractedRay(1.51, closest_sph, surf_norm, light_vec);
			step += 1;
			normalize(&refracted_ray);
			
			refracted_ray.x = hit->x + refracted_ray.x;
			refracted_ray.y = hit->x + refracted_ray.y;
			refracted_ray.z = hit->x + refracted_ray.z;
			refracted_color = recursive_ray_trace(refracted_ray, *hit, step);
			color = clr_add(color, reflected_color);

		}
		return color;
	}
	else
	{

		return color;
	}


}



/*********************************************************************
 * This function traverses all the pixels and cast rays. It calls the
 * recursive ray tracer and assign return color to frame
 *
 * You should not need to change it except for the call to the recursive
 * ray tracer. Feel free to change other parts of the function however,
 * if you must.
 *********************************************************************/
 RGB_float supersample(Point cur_pixel_pos, Vector ray, RGB_float initial_color)
 {
	RGB_float ret_color = initial_color;
	Vector ss_ray;
	RGB_float ss_color = {0,0,0};
	Point ss_pixel_pos;

	//first ray through center of pixel
	//ss_color = recursive_ray_trace(ray, eye_pos, 0);

	//set pixel to be supersampled to be the current pixel
	ss_pixel_pos = cur_pixel_pos;

	//supersample the 4 pixels with respect to x_grid_size and y_grid_size
	for(float dx = -x_grid_size/2; dx <= x_grid_size/2; dx+=x_grid_size){
		for(float dy = -y_grid_size/2; dy <= y_grid_size/2; dy+=y_grid_size){
			ss_pixel_pos.x = cur_pixel_pos.x + dx;
			ss_pixel_pos.y = cur_pixel_pos.y + dy;
			ss_ray = get_vec(eye_pos, ss_pixel_pos);
			ret_color = recursive_ray_trace(ss_ray, eye_pos, 0);
			ss_color = clr_add(ss_color, ret_color); //accumulate the colors from all ss rays
		}
	}
	ret_color = clr_scale(ss_color, 0.2);
	return ret_color;
 }

/*********************************************************************
 * This function traverses all the pixels and cast rays. It calls the
 * recursive ray tracer and assign return color to frame
 *
 * You should not need to change it except for the call to the recursive
 * ray tracer. Feel free to change other parts of the function however,
 * if you must.
 *********************************************************************/
void ray_trace() {
	int i, j;
	float x_start = -0.5 * image_width;
	float y_start = -0.5 * image_height;
	RGB_float ret_color;
	Point cur_pixel_pos;

	Vector ray;

	// ray is cast through center of pixel
	cur_pixel_pos.x = x_start + 0.5 * x_grid_size;
	cur_pixel_pos.y = y_start + 0.5 * y_grid_size;
	cur_pixel_pos.z = image_plane;

	for (i=0; i<win_height; i++) {
		for (j=0; j<win_width; j++) {

			ray = get_vec(eye_pos, cur_pixel_pos);
			ret_color = recursive_ray_trace(ray, eye_pos, 0);
			if(supersampling_on) //supersampling
			{
				ret_color = supersample(cur_pixel_pos, ray, ret_color);
			}
			frame[i][j][0] = GLfloat(ret_color.r);
			frame[i][j][1] = GLfloat(ret_color.g);
			frame[i][j][2] = GLfloat(ret_color.b);

			cur_pixel_pos.x += x_grid_size;
		}
	cur_pixel_pos.y += y_grid_size;
	cur_pixel_pos.x = x_start;
	}
}
