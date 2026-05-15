/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:38:45 by miguandr          #+#    #+#             */
/*   Updated: 2025/02/03 16:45:52 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static t_vector	calculate_ray_direction(t_data *data, int x, int y)
{
	t_vector	pixel_center;
	t_vector	scaled_x;
	t_vector	scaled_y;
	t_vector	ray_direction;

	scaled_x = ft_scale(&data->cam->pixel_delta_x, (float)x);
	scaled_y = ft_scale(&data->cam->pixel_delta_y, (float)y);
	pixel_center = ft_addition(&scaled_x, &scaled_y);
	pixel_center = ft_addition(&pixel_center, &data->cam->pixel00_location);
	ray_direction = ft_subtraction(&pixel_center, &data->cam->origin);
	ray_direction = ft_normalize(&ray_direction);
	return (ray_direction);
}

static void	trace_ray_for_pixel(int x, int y, t_data *data)
{
	t_figure	closest;
	t_figure	temp;
	t_ray		ray;

	closest.distance = INFINITY;
	temp.distance = INFINITY;
	ray.direction = calculate_ray_direction(data, x, y);
	ray.origin = data->cam->origin;
	if (data->pl)
		closest = render_plane(data, ray);
	if (data->sp)
		temp = render_sphere(data, ray);
	if (temp.distance < closest.distance)
		closest = temp;
	if (data->cy)
		temp = render_cylinder(data, ray);
	if (temp.distance < closest.distance)
		closest = temp;
	if (closest.distance < INFINITY)
		generate_pixel_color(data, x, y, closest);
}

void	render_scene(t_data *data)
{
	int	x;
	int	y;

	x = 0;
	clear_image(data);
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			trace_ray_for_pixel(x, y, data);
			y++;
		}
		x++;
	}
	mlx_put_image_to_window(data->mlx->mlx,
		data->mlx->window, data->mlx->img, 0, 0);
}

static void	init_mlx(t_data *data)
{
	data->mlx->mlx = mlx_init();
	if (!data->mlx->mlx)
		handle_error2(data, 0);
	data->mlx->window = mlx_new_window(data->mlx->mlx, WIDTH, HEIGHT, "MiniRT");
	if (!data->mlx->window)
		handle_error2(data, 1);
	data->mlx->img = mlx_new_image(data->mlx->mlx, WIDTH, HEIGHT);
	if (!data->mlx->img)
		handle_error2(data, 2);
	data->mlx->address = mlx_get_data_addr(data->mlx->img, &data->mlx->bitpp,
			&data->mlx->line_lenght, &data->mlx->endian);
	if (!data->mlx->address)
		handle_error2(data, 3);
}

void	render(t_data *data)
{
	init_mlx(data);
	ft_putendl_fd("Rendering scene...", 1);
	init_camera(data->cam);
	render_scene(data);
	ft_putendl_fd("Render done.", 1);
	mlx_key_hook(data->mlx->window, keyboard_exit, data);
	mlx_hook(data->mlx->window, KeyPress, KeyPressMask, key_handle, data);
	mlx_hook(data->mlx->window, ButtonPress,
		ButtonPressMask, mouse_handle, data);
	mlx_hook(data->mlx->window, FINISH_EVENT, 0, mouse_exit, data);
	mlx_loop(data->mlx->mlx);
}
