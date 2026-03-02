/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarette <emarette@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 07:29:24 by emarette          #+#    #+#             */
/*   Updated: 2026/03/02 07:29:25 by emarette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	arg_error(void)
{
	write(2, "Invalid Argument !\n", 19);
	return (1);
}

int	main(int ac, char **av)
{
	t_codexion	codexion;

	if (init_arg(&codexion, ac - 1, av + 1))
		return (arg_error());
	return (run_simulation(&codexion));
}
