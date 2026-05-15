/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 19:12:28 by marvin            #+#    #+#             */
/*   Updated: 2024/11/01 19:12:28 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../libft/libft.h"
# include <signal.h>
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <X11/X.h>
# include <X11/keysym.h>
# include "../minilibx/mlx.h"

# ifndef M_PI
#  define M_PI 3.14159265358979323846264338327950288
# endif

/*-Macros-*/
# define HEIGHT 800
# define WIDTH 800
# define FINISH_EVENT 17

/*Quadratic Structure*/
typedef struct s_quadratic
{
	float		a;
	float		b;
	float		c;
	float		dist1;
	float		dist2;
	float		radius;
	float		square;
}				t_quadratic;

/*Color Structure*/
typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}					t_color;

/*Vector Structure*/
typedef struct s_vector
{
	float	x;
	float	y;
	float	z;
}			t_vector;

/*Ambient Structure*/
typedef struct s_ambient
{
	int		q;
	float	ratio;
	t_color	color;
}					t_ambient;

/*Light Structure*/
typedef struct s_light
{
	int			q;
	float		ratio;
	t_vector	origin;
}					t_light;

/*Camera Structure*/
typedef struct s_camera
{
	int			q;
	float		fov;
	float		focal_length;
	t_vector	origin;
	t_vector	aim;
	t_vector	right;
	t_vector	up;
	t_vector	pixel_delta_x;
	t_vector	pixel_delta_y;
	t_vector	pixel00_location;
}					t_camera;

/*Sphere Structure*/
typedef struct s_sphere
{
	t_vector		origin;
	t_color			color;
	float			diameter;
	struct s_sphere	*next;
	t_quadratic		qdtc;
}					t_sphere;

/*Plane Structure*/
typedef struct s_plane
{
	t_vector		origin;
	t_vector		normal;
	t_color			color;
	struct s_plane	*next;
}					t_plane;

/*Cylinder Structure*/
typedef struct s_cylinder
{
	t_vector			origin;
	t_vector			normal;
	float				diameter;
	float				height;
	t_color				color;
	struct s_cylinder	*next;
	bool				cy_cap;
	t_quadratic			qdtc;
}					t_cylinder;

/*MLX Structure*/
typedef struct s_mlx
{
	void		*mlx;
	void		*window;
	void		*img;
	char		*address;
	int			line_lenght;
	int			endian;
	int			bitpp;
	int			rendered;
}					t_mlx;

typedef struct s_figure
{
	float		distance;
	t_vector	intersection;
	t_vector	normal;
	t_sphere	*sphere;
	t_plane		*plane;
	t_cylinder	*cylinder;
}				t_figure;

typedef struct s_ray
{
	t_vector	origin;
	t_vector	direction;
}			t_ray;

/*Data Structure*/
typedef struct s_data
{
	t_mlx		*mlx;
	t_ambient	*amb;
	t_light		*light;
	t_camera	*cam;
	t_sphere	*sp;
	t_plane		*pl;
	t_cylinder	*cy;
	t_quadratic	*qdtc;
}					t_data;

/*Error Funtions*/
int			handle_error(t_data *data, int error);
int			handle_error2(t_data *data, int error);

/*Parser Functions*/
int			parser(char *file, t_data *data);
void		parse_ambient(char **array, t_data *data);
void		parse_light(char **array, t_data *data);
void		parse_sphere(char **array, t_data *data);
void		parse_camera(char **array, t_data *data);
void		parse_plane(char **array, t_data *data);
void		parse_cylinder(char **array, t_data *data);

/*Parser Utils Functions*/
int			check_color(char **rgb);
int			check_vectors(char **xyz, char **to_free, int flag);

/*Render functions*/
void		render(t_data *data);
void		render_scene(t_data *data);
int			key_handle(int keysym, t_data *data);
int			mouse_handle(int button, int x, int y, t_data *data);
int			mouse_exit(t_data *data);
int			keyboard_exit(int keysym, void *data);
void		init_camera(t_camera *camera);

/*Events Utils*/
void		move_objects(int keysym, t_data *data);
void		move_light(int keysym, t_data *data);
void		move_camera(int keysym, t_data *data);
void		rotate_objects(int keysym, t_data *data);

/*Color Funtions & Utils*/
void		generate_pixel_color(t_data *data, int x, int y, t_figure figure);
int			check_shadow(t_data *data, t_vector *point,
				t_vector *normal, t_vector *light_direction);

/*Figures Functions*/
t_figure	render_plane(t_data *data, t_ray ray);
t_figure	render_sphere(t_data *data, t_ray ray);
t_figure	render_cylinder(t_data *data, t_ray ray);
float		cap_distance(t_cylinder *cylinder, t_ray ray,
				t_quadratic *qdtc, float closest);
int			check_height(t_cylinder *cy, t_ray ray, float dist);

/*Vector Utils*/
t_vector	ft_addition(t_vector *a, t_vector *b);
t_vector	ft_subtraction(t_vector *a, t_vector *b);
t_vector	ft_cross(t_vector *a, t_vector *b);
float		ft_dot(t_vector *a, t_vector *b);
t_vector	ft_normalize(t_vector *a);
float		ft_lenght(t_vector *a);
t_vector	ft_scale(t_vector *a, float scalar);
t_vector	ft_perpendicular(t_vector *a, t_vector *b);

/*Utils Functions*/
void		normalize_whitespace(char *str);
void		free_memory(t_data *data);
void		print_menu(void);
void		clear_image(t_data *data);

#endif
