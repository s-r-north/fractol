/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   julia.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 14:59:27 by snorth            #+#    #+#             */
/*   Updated: 2017/06/20 19:43:20 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

static void	set_env(t_env *env)
{
	t_mouse	*mouse;

	if (!env->m)
	{
		mouse = (t_mouse*)malloc(sizeof(t_mouse));
		env->m = mouse;
	}
	env->jul = true;
	env->lock = false;
	env->maxiter = 72;
	env->zoom = 1.25;
	env->xtrans = WIN_W / 2;
	env->ytrans = WIN_H / 2;
	env->x0 = 0;
	env->y0 = 0;
	env->m->x = 578;
	env->m->y = 658;
	env->func = &eval_pixel_jm;
	env->reset = &set_env;
}

void		julia(t_env *env)
{
	set_env(env);
	if (!env->mlx)
		init_mlx_pointers(env, "Julia");
	draw(env, 0);
	mlx_hook(env->mlx->win, 6, 0, mouse_pos, env);
	mlx_mouse_hook(env->mlx->win, mouse_hook, env);
	mlx_hook(env->mlx->win, 2, 0, press_key, env);
	mlx_hook(env->mlx->win, 3, 0, release_key, env);
	mlx_loop(env->mlx->mlx);
}
