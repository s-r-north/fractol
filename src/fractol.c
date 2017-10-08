// TODO: Add other fractals, look into mouse drag hooks

#include "../libs/minilibx_macos_10.11/mlx.h"
#include "../libs/libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#define HUE_DEG 120
#define NTHREAD 8
#define BITS_PER_PIXEL 32
#define ENDIAN 1
#define WIN_W 1500
#define WIN_H 1500

typedef struct		s_mlx
{
	void			*mlx;
	void			*win;
	void			*img;
	int				*data;
	int				bpp;
	int				endian;
	int				win_x;
}					t_mlx;

typedef struct		s_env
{
	t_mlx			*mlx;
	double			preal;
	double			pimag;
	int				maxiter;
	int				iter_cur;
	double			zoom;
	double			xtrans;
	double			ytrans;
	pthread_t		tid[NTHREAD];
	int				thread;
}					t_env;

typedef struct		s_thread
{
	int				tid;
	t_env			*env;
}					t_thread;

int		release_key(int keycode)
{
	if (keycode == 53)
		exit(0);
	return (0);
}

void	init_mlx_pointers(t_env *env, char *s)
{
	t_mlx	*tmp;

	tmp = (t_mlx*)malloc(sizeof(t_mlx));
	tmp->mlx = mlx_init();
	tmp->win = mlx_new_window(tmp->mlx, WIN_W, WIN_H, s);
	tmp->img = mlx_new_image(tmp->mlx, WIN_W, WIN_H);
	tmp->endian = ENDIAN;
	tmp->bpp = BITS_PER_PIXEL;
	tmp->win_x = WIN_W;
	tmp->data = (int*)mlx_get_data_addr(tmp->img, &(tmp->bpp), &(tmp->win_x), &(tmp->endian));
	env->mlx = tmp;
	// printf("tmp:%p, %p, %p, %p\n", tmp->mlx, tmp->win, tmp->img, tmp->data); //Good
}

int		color_pixel(int hue, int max)
{
	int rgb[3];
	int color;
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
	color = ((rgb[0] & 0xff) << 16) | ((rgb[1] & 0xff) << 8) | (rgb[2] & 0xff);
	// printf("%d: %.2x %.2x %.2x\n", hue, rgb[0], rgb[1], rgb[2]);
	return (color);
}

int		eval_pixel(t_env *env, int x, int y)
{
	int		i;
	double	tab[6];

	tab[2] = ((double)x - env->xtrans) / ((WIN_W / 4) * env->zoom) + env->preal;
	tab[3] = ((double)y - env->ytrans) / ((WIN_H / 4) * env->zoom) + env->pimag;
	tab[0] = tab[2];
	tab[1] = tab[3];
	i = 0;
	while (i < env->maxiter && tab[2] + tab[3] <= 16)
	{
		tab[4] = (tab[2] * tab[2]) - (tab[3] * tab[3]);
		tab[5] = 2 * tab[2] * tab[3];
		tab[2] = tab[4] + tab[0];
		tab[3] = tab[5] + tab[1];
		i++;
	}
	return (i);
}

void	img_place(t_env *env, int y1, int y2)
{
	int x;
	int i;

	while (++y1 < y2)
	{
		x = 0;
		while (x < WIN_W)
		{
			i = eval_pixel(env, x, y1);
			if (i < env->maxiter)
				env->mlx->data[x + (y1 * WIN_W)] = color_pixel(i, env->maxiter);
			x++;
		}
	}
}

void	set_env(t_env *env)
{
	env->maxiter = 64;
	env->zoom = 1;
	env->xtrans = WIN_W / 2;
	env->ytrans = WIN_H / 2;
	env->preal = 0;
	env->pimag = 0;
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
			img_place(env, i * (WIN_W / NTHREAD), (i + 1) * (WIN_W / NTHREAD));
		}
	}
	return (NULL);
}

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

int		press_key(int keycode, t_env *env)
{
	if (keycode == 53)
		exit(0);
	if (keycode == 8)
		env->zoom *= 1.05;
	if (keycode == 6)
		env->zoom /= 1.05;
	if (keycode == 125)
		env->ytrans -= 5 * sqrt(env->zoom);
	if (keycode == 126)
		env->ytrans += 5 * sqrt(env->zoom);
	if (keycode == 124)
		env->xtrans -= 5 * sqrt(env->zoom);
	if (keycode == 123)
		env->xtrans += 5 * sqrt(env->zoom);
	if (keycode == 3 && env->maxiter > 5)
		env->maxiter -= 5;
	if (keycode == 5 && env->maxiter < 65536)
		env->maxiter += 5;
	if (keycode == 29)

	draw(env, 1);
	return (0);
}

int		mouse_hook(int mbutton, int x, int y, t_env *env)
{
	if (mbutton == 4 || mbutton == 5)
	{
		env->preal += (x - env->xtrans) / ((WIN_W / 4) * env->zoom);
		env->pimag += (y - env->ytrans) / ((WIN_W / 4) * env->zoom);
		env->zoom *= ((mbutton == 4) ? 1.05 : 1 / 1.05);
		env->xtrans = x;
		env->ytrans = y;
		draw(env, 1);
	}
	return (0);
}

void	mandelbrot(t_env *env)
{
	set_env(env);
	init_mlx_pointers(env, "Mandelbrot");
	draw(env, 0);
	mlx_mouse_hook(env->mlx->win, mouse_hook, env);
	mlx_hook(env->mlx->win, 2, 0, press_key, env);
	mlx_hook(env->mlx->win, 3, 0, release_key, env);
	mlx_loop(env->mlx->mlx);
}

// void	julia()
// {

// }

// void	cantor()
// {

// }

int		main(int ac, char **av)
{

	t_env	env;

	if (ac != 2)
	{
		write(1, "Usage: ./fractol -[M|J|C]\n", 26);
		exit(0);
	}
	// init_mlx_pointers(&env);
	if (!ft_strcmp(av[1], "-M"))
		mandelbrot(&env);
	else
	{
		write(1, "Usage: ./fractol -[M|J|S]\n", 26);
		exit(0);
	}

}