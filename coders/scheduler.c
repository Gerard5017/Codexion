/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarette <emarette@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 07:29:45 by emarette          #+#    #+#             */
/*   Updated: 2026/03/02 07:29:46 by emarette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	*get_request_seq(void)
{
	static long	request_seq = 0;

	return (&request_seq);
}

pthread_mutex_t	*get_scheduler_mutex(void)
{
	static pthread_mutex_t	mutex = PTHREAD_MUTEX_INITIALIZER;

	return (&mutex);
}

void	register_request(t_coder *coder, t_dongle *dongle)
{
	if (coder->requested_dongle == dongle)
		return ;
	coder->requested_dongle = dongle;
	coder->request_seq = ++(*get_request_seq());
	coder->request_deadline = coder->last_compile
		+ coder->codexion.time_to_burnout;
}

int	is_better_candidate(t_coder *current, t_coder *best)
{
	if (best == NULL)
		return (1);
	if (current->codexion.isfifo)
		return (current->request_seq < best->request_seq);
	if (current->request_deadline != best->request_deadline)
		return (current->request_deadline < best->request_deadline);
	return (current->request_seq < best->request_seq);
}

int	is_request_winner(t_coder *coder, t_dongle *dongle)
{
	t_coder	*best;
	long	i;

	best = NULL;
	i = -1;
	while (++i < coder->monitor->count)
	{
		if (coder->monitor->coders[i].requested_dongle == dongle
			&& is_better_candidate(&coder->monitor->coders[i], best))
			best = &coder->monitor->coders[i];
	}
	return (best == coder);
}
