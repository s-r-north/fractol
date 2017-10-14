#include "../includes/fractol.h"

static void	set_env(t_env *env)
{
	t_mouse		*mouse;

	if (!env->m)
	{
		mouse = (t_mouse*)malloc(sizeof(t_mouse));
		env->m = mouse;
	}
	env->jul = true;
	env->lock = false;
	env->maxiter = 64;
	env->zoom = 1;
	env->xtrans = WIN_W / 2;
	env->ytrans = WIN_H / 2;
	env->x0 = 0;
	env->y0 = 0;
	env->m->x = 0;
	env->m->y = 0;
	env->func = &eval_pixel_jm;
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
