// TODO: Look into mouse drag hooks ; Implement reset, info, help ; Look at Koch Snowflake

#include "../includes/fractol.h"

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
	env->mlx = tmp;
	// printf("tmp:%p, %p, %p, %p\n", tmp->mlx, tmp->win, tmp->img, tmp->data); //Good
}

int		main(int ac, char **av)
{

	t_env	env;

	if (ac != 2)
	{
		write(1, "Usage: ./fractol -[M|J|K|B]\n", 28);
		exit(0);
	}
	if (!ft_strcmp(av[1], "-M"))
		mandelbrot(&env);
	if (!ft_strcmp(av[1], "-J"))
		julia(&env);
	if (!ft_strcmp(av[1], "-B"))
		burning_ship(&env);
	else
	{
		write(1, "Usage: ./fractol -[M|J|K|B]\n", 28);
		exit(0);
	}
}
