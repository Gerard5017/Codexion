/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsellier <lsellier@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 05:05:58 by emarette          #+#    #+#             */
/*   Updated: 2026/03/02 02:59:01 by lsellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_end(t_coder *coder)
{
	t_monitor	*monitor;
	int			is_burnout;
	int			is_ended;
	long		burnout_ts;

	monitor = coder->monitor;
	is_burnout = 0;
	burnout_ts = 0;
	pthread_mutex_lock(&monitor->mutex);
	if (!coder->end && !monitor->end && get_time_to_burnout(coder) <= 0)
	{
		is_burnout = 1;
		burnout_ts = get_timestamp(coder);
		monitor->end = 1;
	}
	if (monitor->end)
		coder->end = 1;
	is_ended = coder->end;
	pthread_mutex_unlock(&monitor->mutex);
	if (is_burnout)
		print_state_ts(coder, BURNOUT_MSG, burnout_ts);
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

void	print_state_ts(t_coder *coder, char *str, long timestamp)
{
	static pthread_mutex_t	mutex = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&mutex);
	printf("%ld %u %s\n", timestamp, coder->id, str);
	pthread_mutex_unlock(&mutex);
}

void	print_state(t_coder *coder, char *str)
{
	print_state_ts(coder, str, get_timestamp(coder));
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
