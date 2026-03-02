/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarette <emarette@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 07:30:01 by emarette          #+#    #+#             */
/*   Updated: 2026/03/02 07:30:05 by emarette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

long	get_timestamp(t_coder *coder)
{
	return (get_time() - coder->start_time);
}

long	get_time_to_burnout(t_coder *coder)
{
	return (coder->codexion.time_to_burnout - (get_time()
			- coder->last_compile));
}
