// TODO: Look into mouse drag hooks

#include "../includes/fractol.h"

void	help(t_env *env, int toggle)
{
	int		i;
	char	**s;

	if (!toggle)
		return ;
	s = ft_strsplit("Press H to toggle this message\n\
					Arrow keys to adjust viewing window\n\
					Mouse wheel to zoom\n\
					F/G to increase and decrease bound\n\
					Move mouse to change constant (not in Mandelbrot)\n\
					Spacebar to toggle constant lock\n\
					I to toggle detailed information\n\
					R to reset\n\
					Esc to exit", '\n');
	i = -1;
	while (++i < 9)
		mlx_string_put(env->mlx->mlx, env->mlx->win, 20, 20 * (i + 1), 0xffffff, s[i]);
}

void	info(t_env *env, int toggle)
{
	int i;
	char *as;
	char **s;

	if (!toggle)
		return ;
	as = (char*)malloc(sizeof(char) * 1000);
	sprintf(as, "Mouse: (%d, %d)\n\
			Constant: (%lf, %lf)\n\
			Max Bound: %d\n\
			Center: (%lf, %lf)\n\
			Current Zoom: %lf",
			env->m->x, env->m->y, ((double)env->m->x - WIN_W / 2) / (WIN_W / 4),
			((double)env->m->y - WIN_H / 2) / (WIN_H / 4), env->maxiter, env->x0,
			env->y0, env->zoom);
	s = ft_strsplit(as, '\n');
	free(as);
	i = -1;
	while (++i < 5)
		mlx_string_put(env->mlx->mlx, env->mlx->win, WIN_W - 400, 20 * (i + 1),
				0xffffff, s[i]);
}

void	init_mlx_pointers(t_env *env, char *s)
{
	t_mlx	*tmp;
	int e;
	int bpp;
	int width;

	tmp = (t_mlx*)malloc(sizeof(t_mlx));
	tmp->mlx = mlx_init();
	tmp->win = mlx_new_window(tmp->mlx, WIN_W, WIN_H, s);
	tmp->img = mlx_new_image(tmp->mlx, WIN_W, WIN_H);
	e = ENDIAN;
	bpp = BITS_PER_PIXEL;
	width = WIN_W;
	tmp->data = (int*)mlx_get_data_addr(tmp->img, &bpp, &width, &e);
	tmp->info = 0;
	tmp->help = 1;
	env->mlx = tmp;
	// printf("tmp:%p, %p, %p, %p\n", tmp->mlx, tmp->win, tmp->img, tmp->data); //Good
}

int		main(int ac, char **av)
{

	t_env	env;

	if (ac != 2)
	{
		write(1, "Usage: ./fractol -[M|J|B]\n", 28);
		exit(0);
	}
	env.m = NULL;
	env.func = NULL;
	env.reset = NULL;
	if (!ft_strcmp(av[1], "-M"))
		mandelbrot(&env);
	if (!ft_strcmp(av[1], "-J"))
		julia(&env);
	if (!ft_strcmp(av[1], "-B"))
		burning_ship(&env);
	else
	{
		write(1, "Usage: ./fractol -[M|J|B]\n", 28);
		exit(0);
	}
}
