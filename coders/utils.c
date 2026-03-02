/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarette <emarette@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 07:30:12 by emarette          #+#    #+#             */
/*   Updated: 2026/03/02 07:42:55 by emarette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_end(t_coder *coder)
{
	t_monitor	*monitor;
	int			is_burnout;
	int			is_ended;

	monitor = coder->monitor;
	is_burnout = 0;
	pthread_mutex_lock(&monitor->mutex);
	if (!coder->end && !monitor->end && get_time_to_burnout(coder) <= 0)
	{
		is_burnout = 1;
		monitor->end = 1;
	}
	if (monitor->end)
		coder->end = 1;
	is_ended = coder->end;
	pthread_mutex_unlock(&monitor->mutex);
	if (is_burnout)
		print_state(coder, BURNOUT_MSG);
	return (is_ended);
}

void	coder_wait(t_coder *coder, long wait_time)
{
	long	time_to_burnout;

	pthread_mutex_lock(&coder->monitor->mutex);
	time_to_burnout = get_time_to_burnout(coder);
	pthread_mutex_unlock(&coder->monitor->mutex);
	if (time_to_burnout < wait_time)
		wait_time = time_to_burnout;
	if (0 < wait_time)
		usleep(wait_time * 1000);
}

void	print_state(t_coder *coder, char *str)
{
	static pthread_mutex_t	mutex = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&mutex);
	printf("%ld %u %s\n", get_timestamp(coder), coder->id, str);
	pthread_mutex_unlock(&mutex);
}


int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (s1 == NULL && s2 == NULL)
		return (0);
	while (s1[i] && s2[i])
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (s1[i] != s2[i])
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}
