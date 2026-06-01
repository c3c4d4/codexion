/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wagyu <wagyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 12:00:00 by wagyu          #+#    #+#             */
/*   Updated: 2026/05/31 12:00:00 by wagyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_sim		t_sim;
typedef struct s_coder		t_coder;

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_request
{
	t_coder		*coder;
	long long	sequence;
	long long	deadline;
}	t_request;

typedef struct s_heap
{
	t_request	**items;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_dongle
{
	pthread_mutex_t	lock;
	t_heap			queue;
	long long		ready_at;
	int				owner;
}	t_dongle;

struct s_coder
{
	pthread_t	thread;
	t_sim		*sim;
	t_request	left_request;
	t_request	right_request;
	long long	last_compile;
	int			id;
	int			left;
	int			right;
	int			compile_count;
};

struct s_sim
{
	pthread_mutex_t	lock;
	pthread_mutex_t	output_lock;
	pthread_cond_t	changed;
	pthread_t		monitor;
	t_coder			*coders;
	t_dongle		*dongles;
	long long		start;
	long long		burnout;
	long long		compile;
	long long		debug;
	long long		refactor;
	long long		cooldown;
	long long		sequence;
	int				coder_count;
	int				required;
	int				completed;
	int				stopped;
	int				dongles_initialized;
	int				base_initialized;
	t_scheduler		scheduler;
};

int			parse_args(t_sim *sim, int argc, char **argv);
long long	now_ms(void);
void		to_timespec(long long timestamp, struct timespec *value);
int			wait_until(t_sim *sim, long long timestamp);
int			wait_for_stop(t_sim *sim);
int			request_before(t_sim *sim, t_request *one, t_request *two);
int			heap_push(t_sim *sim, t_heap *heap, t_request *request);
t_request	*heap_pop(t_sim *sim, t_heap *heap);
t_request	*heap_peek(t_heap *heap);
int			init_sim(t_sim *sim);
void		cleanup_sim(t_sim *sim);
void		log_action(t_sim *sim, int id, char *action);
void		stop_simulation(t_sim *sim);
int			burnout_due(t_sim *sim);
void		lock_dongles(t_coder *coder);
void		unlock_dongles(t_coder *coder);
void		enqueue_requests(t_coder *coder);
void		release_dongles(t_coder *coder);
void		claim_dongles(t_coder *coder, long long now);
int			acquire_dongles(t_coder *coder);
void		*coder_routine(void *data);
void		*monitor_routine(void *data);
int			start_threads(t_sim *sim);
void		join_threads(t_sim *sim, int count, int monitor_started);

#endif
