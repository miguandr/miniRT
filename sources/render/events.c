/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 21:24:06 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 21:24:06 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

void	resize_objects(int keysym, t_data *data)
{
	t_sphere	*current_sp;
	t_cylinder	*current_cy;
	double		size;

	size = 1;
	if (keysym == XK_2)
		size = -1;
	if (data->sp || data->cy)
	{
		current_sp = data->sp;
		while (current_sp)
		{
			current_sp->diameter += size;
			current_sp = current_sp->next;
		}
		current_cy = data->cy;
		while (current_cy)
		{
			current_cy->diameter += size;
			current_cy->height += size;
			current_cy = current_cy->next;
		}
	}
}

int	key_handle(int keysym, t_data *data)
{
	if (keysym == XK_m)
		print_menu();
	if (keysym == XK_i || keysym == XK_k || keysym == XK_j
		|| keysym == XK_l || keysym == XK_o || keysym == XK_p)
		move_light(keysym, data);
	if (keysym == XK_w || keysym == XK_s || keysym == XK_a || keysym == XK_d)
		move_camera(keysym, data);
	if (keysym == XK_Up || keysym == XK_Down || keysym == XK_Right
		|| keysym == XK_Left)
		move_objects(keysym, data);
	if (keysym == XK_z || keysym == XK_x || keysym == XK_c || keysym == XK_v)
		rotate_objects(keysym, data);
	if (keysym == XK_1 || keysym == XK_2)
		resize_objects(keysym, data);
	render_scene(data);
	return (0);
}

int	keyboard_exit(int keysym, void *param)
{
	if (keysym == XK_Escape)
	{
		free_memory((t_data *)param);
		exit (0);
	}
	return (0);
}

int	mouse_handle(int button, int x, int y, t_data *data)
{
	(void)x;
	(void)y;
	data->cam->fov /= 3.14159265358979323846 / 180.0f;
	if (button == Button4)
		data->cam->fov -= 10;
	else if (button == Button5)
		data->cam->fov += 10;
	init_camera(data->cam);
	render_scene(data);
	return (0);
}

int	mouse_exit(t_data *data)
{
	free_memory(data);
	exit (0);
	return (0);
}
