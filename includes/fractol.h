#ifndef FRACTOL_H

# define FRACTOL_H

#include "../libs/minilibx_macos_10.11/mlx.h"
#include "../libs/libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#define HUE_DEG 120
#define NTHREAD 16
#define BITS_PER_PIXEL 32
#define ENDIAN 1
#define WIN_W 800
#define WIN_H 800

typedef struct		s_mouse
{
	int				x;
	int				y;
	int				drag;
}					t_mouse;

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
	int				(*func)(struct s_env *env, int x, int y);
	pthread_t		tid[NTHREAD];
	int				thread;
	t_mouse			*m;
}					t_env;

void	init_mlx_pointers(t_env *env, char *s);

void	mandelbrot(t_env *env);
void	julia(t_env *env);

void	draw(t_env *env, int re);
void	*set_thread(void *arg);
void	img_place(t_env *env, int y1, int y2);
int		color_pixel(int hue, int max);


int		mouse_hook(int mbutton, int x, int y, t_env *env);
int		press_key(int keycode, t_env *env);
int		release_key(int keycode);

#endif
