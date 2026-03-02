/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarette <emarette@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 22:19:31 by emarette          #+#    #+#             */
/*   Updated: 2026/03/02 06:09:58 by emarette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# define MAX_CODERS 1000

# define COMPILE_MSG "is compiling"
# define REFACTOR_MSG "is refactoring"
# define DEBUG_MSG "is debugging"
# define TAKE_DONGLE_MSG "has taken a dongle"
# define BURNOUT_MSG "burned out"

typedef struct s_codexion
{
	long			coders_count;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	long			compile_rest;
	long			dongle_cooldown;
	int				isfifo;
}					t_codexion;

typedef struct s_dongle
{
	int				available;
	pthread_mutex_t	mutex;
	long			dongle_cooldown;
	long			release_time;
}					t_dongle;

typedef struct s_monitor
{
	struct s_coder	*coders;
	long			count;
	long			compile_rest;
	pthread_mutex_t	mutex;
	int				end;
}					t_monitor;

typedef struct s_coder
{
	unsigned int	id;
	int				end;
	long			start_time;
	long			last_compile;
	t_dongle		*dongle_left;
	t_dongle		dongle_right;
	t_codexion		codexion;
	t_monitor		*monitor;
	t_dongle		*requested_dongle;
	long			request_seq;
	long			request_deadline;
}					t_coder;

long				ft_strtol(const char *str, char **endptr);

int					init_arg(t_codexion *coder, int ac, char **av);

int					init_dongle(t_dongle *dongle, t_codexion *codexion);
int					init_coder(t_coder *coder, t_codexion *codexion,
						unsigned int id);
long				get_time_to_burnout(t_coder *coder);
long				get_timestamp(t_coder *coder);
long				get_time(void);
int					take_dongle(t_coder *coder, t_dongle *dongle);
void				*routine(void *ptr);
int					is_end(t_coder *coder);
void				coder_wait(t_coder *coder, long wait_time);
void				print_state(t_coder *coder, char *str);
void				print_state_ts(t_coder *coder, char *str, long timestamp);
void				dongle_toggle_available(t_dongle *dongle);
int					ft_strcmp(const char *s1, const char *s2);
int					run_simulation(t_codexion *codexion);
void				*monitor(void *ptr);
pthread_mutex_t		*get_scheduler_mutex(void);
void				register_request(t_coder *coder, t_dongle *dongle);
int					is_request_winner(t_coder *coder, t_dongle *dongle);
#endif
