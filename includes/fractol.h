/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 14:59:27 by snorth            #+#    #+#             */
/*   Updated: 2017/06/20 19:43:20 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H

# define FRACTOL_H

# include "../libs/minilibx_macos_10.11/mlx.h"
# include "../libs/libft/libft.h"
# include <stdio.h>
# include <pthread.h>
# include <math.h>

# define HUE_DEG 120
# define NTHREAD 16
# define BITS_PER_PIXEL 32
# define ENDIAN 1
# define WIN_W 1024
# define WIN_H 1024
# define BOUND_MAX 1024

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
	int				info;
	int				help;
}					t_mlx;

typedef struct		s_env
{
	t_mlx			*mlx;
	int				jul;
	int				lock;
	double			x0;
	double			y0;
	int				maxiter;
	int				iter_cur;
	double			zoom;
	double			xtrans;
	double			ytrans;
	int				(*func)(struct s_env *env, double x, double y);
	void			(*reset)(struct s_env *env);
	pthread_t		tid[NTHREAD];
	int				thread;
	t_mouse			*m;
}					t_env;

void				init_mlx_pointers(t_env *env, char *s);
void				help(t_env *env, int toggle);
void				info(t_env *env, int toggle);

void				mandelbrot(t_env *env);
void				julia(t_env *env);
void				burning_ship(t_env *env);

void				draw(t_env *env, int re);
void				*set_thread(void *arg);
void				img_place(t_env *env, int y1, int y2);
int					eval_pixel_jm(t_env *env, double a, double b);
int					color_pixel(int hue, int max);

int					mouse_hook(int mbutton, int x, int y, t_env *env);
int					mouse_pos(int x, int y, t_env *env);
int					press_key(int keycode, t_env *env);
int					release_key(int keycode);

#endif
