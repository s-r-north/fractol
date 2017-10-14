#include "../includes/fractol.h"

static int	eval_pixel_burn(t_env *env, double a, double b)
{
	int i;
	double pr;
	double pi;
	double tmp;

	pr = (a - env->xtrans) / ((WIN_W / 4) * env->zoom) + env->x0;
	pi = (b - env->ytrans) / ((WIN_H / 4) * env->zoom) + env->y0;
	a = ((double)env->m->x - env->xtrans) / (WIN_W);
	b = ((double)env->m->y - env->ytrans) / (WIN_H);
	i = -1;
	while (++i < env->maxiter && (a * a) + (b * b) <= 4)
	{
		tmp = (a * a) - (b * b) + pr;
		b = fabs(2 * a * b) + pi;
		a = tmp;
	}
	return (i);
}

static void	set_env(t_env *env)
{
	t_mouse		*mouse;

	if (!env->m)
	{
		mouse = (t_mouse*)malloc(sizeof(t_mouse));
		env->m = mouse;
	}
	env->lock = true;
	env->maxiter = 64;
	env->zoom = 1;
	env->xtrans = 1.25 * (WIN_W / 2);
	env->ytrans = WIN_H / 2;
	env->x0 = 0;
	env->y0 = 0;
	env->m->x = env->xtrans;
	env->m->y = env->ytrans;
	env->func = &eval_pixel_burn;
}



void	burning_ship(t_env *env)
{
	set_env(env);
	init_mlx_pointers(env, "Burning Ship");
	draw(env, 0);
	mlx_hook(env->mlx->win, 6, 0, mouse_pos, env);
	mlx_mouse_hook(env->mlx->win, mouse_hook, env);
	mlx_hook(env->mlx->win, 2, 0, press_key, env);
	mlx_hook(env->mlx->win, 3, 0, release_key, env);
	mlx_loop(env->mlx->mlx);
}
