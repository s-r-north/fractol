#include "../libs/minilibx_macos_10.11/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NTHREAD 8
#define BITS_PER_PIXEL 32
#define ENDIAN 1
#define WIN_W 1000
#define WIN_H 1000

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
}					t_env;

typedef struct		s_thread
{
	int				tid;
	t_env			*env;
}					t_thread;

int		press_key(int keycode)
{
	if (keycode == 53)
		exit(0);
	return (0);
}

int		release_key(int keycode)
{
	if (keycode == 53)
		exit(0);
	return (0);
}

void	init_mlx_pointers(t_env *env)
{
	t_mlx	*tmp;

	tmp = (t_mlx*)malloc(sizeof(t_mlx));
	tmp->mlx = mlx_init();
	tmp->win = mlx_new_window(tmp->mlx, WIN_W, WIN_H, "Placeholder");
	tmp->img = mlx_new_image(tmp->mlx, WIN_W, WIN_H);
	tmp->endian = ENDIAN;
	tmp->bpp = BITS_PER_PIXEL;
	tmp->win_x = WIN_W;
	tmp->data = (int*)mlx_get_data_addr(tmp->img, &(tmp->bpp), &(tmp->win_x), &(tmp->endian));
	env->mlx = tmp;
	// printf("tmp:%p, %p, %p, %p\n", tmp->mlx, tmp->win, tmp->img, tmp->data); //Good
}

// void	set_env(t_env *env)
// {

// }

void	*proof(void *arg)
{
	const static int shitcolor[NTHREAD] = {0xff0000, 0xff8000, 0xffff00, 0x00ff00, 0x00ffff, 0x0000ff, 0x8000ff, 0xff00ff};
	t_thread *t;
	int x;
	int y;

	t = (t_thread*)arg;
	y = ((WIN_H / NTHREAD) * t->tid) - 1;
	while (++y < (WIN_H / NTHREAD) * (t->tid + 1))
	{
		x = -1;
		while (++x < WIN_W)
		{
			if (x >= 0 && y >= 0 && x + (y * WIN_W) < (WIN_W * WIN_H) - 1)
				t->env->mlx->data[x + (y * WIN_W)] = shitcolor[t->tid];
		}
	}
	//if (y == WIN_H)
		//mlx_put_image_to_window(t->env->mlx->mlx, t->env->mlx->win, t->env->mlx->img, 0, 0);
	return (NULL);
}

pthread_t	set_thread(t_env *env, int i)
{
	pthread_t	ret;
	t_thread *t;

	t = (t_thread*)malloc(sizeof(t_thread));
	t->tid = i;
	t->env = env;
	pthread_create(&ret, NULL, proof, t);
	return (ret);
}

void	draw(t_env *env)
{
	pthread_t	thread[NTHREAD];
	int i;

	i = -1;
	while (++i < NTHREAD)
		thread[i] = set_thread(env, i);
	i = -1;
	while (++i < NTHREAD)
		pthread_join(thread[i], NULL);
	mlx_put_image_to_window(env->mlx->mlx, env->mlx->win, env->mlx->img, 0, 0);
}

void	julia(t_env *env)
{
	// set_env(env);
	draw(env);
	// printf("tmp:%p, %p, %p, %p\n", env->mlx->mlx, env->mlx->win, env->mlx->img, env->mlx->data); //Good
	mlx_hook(env->mlx->win, 2, 0, press_key, env);
	mlx_hook(env->mlx->win, 3, 0, release_key, env);
	mlx_loop(env->mlx->mlx);
}

int		main()
{
	t_env	env;

	init_mlx_pointers(&env);
	julia(&env);

}