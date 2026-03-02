/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarette <emarette@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 07:28:58 by emarette          #+#    #+#             */
/*   Updated: 2026/03/02 07:29:01 by emarette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dongle_is_available(t_dongle *dongle)
{
	int	is_available;

	pthread_mutex_lock(&dongle->mutex);
	if (dongle->available && get_time()
		- dongle->release_time >= dongle->dongle_cooldown)
		is_available = 1;
	else
		is_available = 0;
	pthread_mutex_unlock(&dongle->mutex);
	return (is_available);
}

void	dongle_toggle_available(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->available = !dongle->available;
	if (dongle->available)
		dongle->release_time = get_time();
	pthread_mutex_unlock(&dongle->mutex);
}

int	try_take_dongle(t_coder *coder, t_dongle *dongle)
{
	int	taken;

	taken = 0;
	pthread_mutex_lock(get_scheduler_mutex());
	register_request(coder, dongle);
	if (is_request_winner(coder, dongle))
	{
		pthread_mutex_lock(&dongle->mutex);
		if (dongle->available && get_time()
			- dongle->release_time >= dongle->dongle_cooldown)
		{
			dongle->available = 0;
			coder->requested_dongle = NULL;
			taken = 1;
		}
		pthread_mutex_unlock(&dongle->mutex);
	}
	pthread_mutex_unlock(get_scheduler_mutex());
	return (taken);
}

int	take_dongle(t_coder *coder, t_dongle *dongle)
{
	while (!try_take_dongle(coder, dongle) && !is_end(coder))
		usleep(500);
	if (!is_end(coder))
	{
		print_state(coder, TAKE_DONGLE_MSG);
		return (1);
	}
	pthread_mutex_lock(get_scheduler_mutex());
	if (coder->requested_dongle == dongle)
		coder->requested_dongle = NULL;
	pthread_mutex_unlock(get_scheduler_mutex());
	return (0);
}
