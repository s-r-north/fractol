#include "../includes/fractol.h"

void	draw(t_env *env, int re)
{
	int i;

	if (re)
	{
		mlx_clear_window(env->mlx->mlx, env->mlx->win);
		ft_bzero(env->mlx->data, sizeof(int) * WIN_W * WIN_H);
	}
	i = -1;
	while (++i < NTHREAD)
		pthread_create(&env->tid[i], NULL, set_thread, env);
	i = -1;
	while (++i < NTHREAD)
		pthread_join(env->tid[i], NULL);
	mlx_put_image_to_window(env->mlx->mlx, env->mlx->win, env->mlx->img, 0, 0);
}

void	*set_thread(void *arg)
{
	t_env *env;
	int i;

	env = (t_env*)arg;
	i = -1;
	while (++i < NTHREAD)
	{
		if (pthread_equal(pthread_self(), env->tid[i]))
		{
			env->thread = i;
			img_place(env, i * (WIN_W / NTHREAD) - 1, (i + 1) * (WIN_W / NTHREAD));
		}
	}
	return (NULL);
}

void	img_place(t_env *env, int y, int y2)
{
	int x;
	int i;

	while (++y < y2)
	{
		x = 0;
		while (x < WIN_W)
		{
			i = env->func(env, (double)x, (double)y);
			if (i < env->maxiter)
				env->mlx->data[x + (y * WIN_W)] = color_pixel(i, env->maxiter);
			x++;
		}
	}
}

int		eval_pixel_jm(t_env *env, double a, double b)
{
	int i;
	double pr;
	double pi;
	double tmp;

	a = (a - env->xtrans) / ((WIN_W / 4) * env->zoom) + env->x0;
	b = (b - env->ytrans) / ((WIN_H / 4) * env->zoom) + env->y0;
	pr = env->jul ? ((((double)env->m->x - WIN_W) / (WIN_W / 2))) : a;
	pi = env->jul ? ((((double)env->m->y - WIN_H) / (WIN_H / 2))) : b;
	i = -1;
	while (++i < env->maxiter && (a * a) + (b * b) <= 4)
	{
		tmp = (a * a) - (b * b) + pr;
		b = (2 * a * b) + pi;
		a = tmp;
	}
	return (i);
}

int		color_pixel(int hue, int max)
{
	int rgb[3];
	int i;

	if (hue >= max)
		return(0);
	hue %= HUE_DEG;
	i = -1;
	ft_bzero(rgb, 3 * (sizeof(int)));
	rgb[0] = (0xff / (HUE_DEG / 6)) * (HUE_DEG / 6);
	while (++i < hue)
	{
		if (i < (HUE_DEG / 6))
			rgb[1] += 0xff / (HUE_DEG / 6);
		if (i >= (HUE_DEG / 6) && i < (2 * HUE_DEG / 6))
			rgb[0] -= 0xff / (HUE_DEG / 6);
		if (i >= (2 * HUE_DEG / 6) && i < (3 * HUE_DEG / 6))
			rgb[2] += 0xff / (HUE_DEG / 6);
		if (i >= (3 * HUE_DEG / 6) && i < (4 * HUE_DEG / 6))
			rgb[1] -= 0xff / (HUE_DEG / 6);
		if (i >= (4 * HUE_DEG / 6) && i < (5 * HUE_DEG / 6))
			rgb[0] += 0xff / (HUE_DEG / 6);
		if (i >= (5 * HUE_DEG / 6))
			rgb[2] -= 0xff / (HUE_DEG / 6);
	}
	return (((rgb[0] & 0xff) << 16) | ((rgb[1] & 0xff) << 8) | (rgb[2] & 0xff));
}
