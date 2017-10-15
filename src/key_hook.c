/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 14:59:27 by snorth            #+#    #+#             */
/*   Updated: 2017/06/20 19:43:20 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

int		press_key(int keycode, t_env *env)
{
	if (keycode == 15)
		env->reset(env);
	if (keycode == 53)
		exit(0);
	if (keycode == 125)
		env->ytrans -= 5;
	if (keycode == 126)
		env->ytrans += 5;
	if (keycode == 124)
		env->xtrans -= 5;
	if (keycode == 123)
		env->xtrans += 5;
	if (keycode == 3 && env->maxiter > 4)
		env->maxiter -= 4;
	if (keycode == 5 && env->maxiter < BOUND_MAX)
		env->maxiter += 4;
	if (keycode == 49)
		env->lock = env->lock ? false : true;
	if (keycode == 34)
		env->mlx->info = env->mlx->info ? false : true;
	if (keycode == 4)
		env->mlx->help = env->mlx->help ? false : true;
	draw(env, 1);
	return (0);
}

int		release_key(int keycode)
{
	if (keycode == 53)
		exit(0);
	return (0);
}

int		mouse_pos(int x, int y, t_env *env)
{
	if (env->lock || (env->m->x == x && env->m->y == y))
		return (0);
	env->m->x = x;
	env->m->y = y;
	draw(env, 1);
	return (0);
}

int		mouse_hook(int mbutton, int x, int y, t_env *env)
{
	if (mbutton == 4 || mbutton == 5)
	{
		env->x0 += (x - env->xtrans) / ((WIN_W / 4) * env->zoom);
		env->y0 += (y - env->ytrans) / ((WIN_W / 4) * env->zoom);
		env->zoom *= ((mbutton == 4) ? 1.05 : 1 / 1.05);
		env->xtrans = x;
		env->ytrans = y;
		draw(env, 1);
	}
	return (0);
}
