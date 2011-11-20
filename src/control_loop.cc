/* Copyright 2006 Florian Paul Schmidt */

#include <iostream>

#include "unistd.h"

#include "control_loop.h"
#include "scgraph.h"

#include <cmath>

ControlLoop::ControlLoop ():
	_first_time (true),
	_sum_delta_t (0),
	_stats_sum_delta_t (0),
	_iteration_count (0),
	_quit (false),
	_freq (Options::get_instance()->_control_rate)
{
	int ret;
	ret = pthread_mutex_init (&_mutex, 0);

}


ControlLoop::~ControlLoop ()
{

}


void ControlLoop::start ()
{
	Options *options = Options::get_instance ();

	int ret;
	ret = pthread_create (&_thread, 0, c_control_thread_func, this);

	if (options->_verbose >= 2)
		std::cout << "[ControlLoop]: Running!" << std::endl;
}

void ControlLoop::stop ()
{
	int ret;

	ret = pthread_mutex_lock (&_mutex);
	_quit = true;
	ret = pthread_mutex_unlock (&_mutex);

	ret = pthread_join (_thread, 0);
}


void ControlLoop::set_rate (int rate)
{
	int ret = pthread_mutex_lock (&_mutex);
	_freq = rate;
	ret = pthread_mutex_unlock (&_mutex);
}

void *c_control_thread_func (void *arg)
{
	return ((ControlLoop*)arg)->thread_func ();
}

void *ControlLoop::thread_func ()
{
	Options *options = Options::get_instance ();
	ScGraph *scgraph = ScGraph::get_instance ();

	if (options->_control_prio > 0)
	{
		struct sched_param params;
		params.sched_priority = options->_control_prio;

		if (pthread_setschedparam (pthread_self (), SCHED_FIFO, &params) != 0)
		{
			std::cout << "[ControlLoop]: Couldn't set realtime prio. Exiting..." << std::endl;
			exit (EXIT_FAILURE);
		}
	}

	while (true)
	{
		if (options->_verbose >= 5)
			std::cout << "[ControlLoop]: Iterate!" << std::endl;

		pthread_mutex_lock (&_mutex);
		if (_quit)
		{
			pthread_mutex_unlock (&_mutex);
			break;
		}

		pthread_mutex_unlock (&_mutex);

		int ret;

		if (_first_time)
		{
			_first_time = false;
			ret = gettimeofday (&_tv, 0);
			if (ret == -1)	
			{
				std::cout << "[GraphicLoop]: Error: gettimeofday () failed. Exiting... " << std::endl;
				exit (EXIT_FAILURE);
			}
			continue;
		}

		struct timeval tv;
		gettimeofday (&tv, 0);

		double delta_t = (double)(tv.tv_sec - _tv.tv_sec) + (double)(1E-6 * (tv.tv_usec - _tv.tv_usec));

		_sum_delta_t += delta_t;

		_stats_sum_delta_t += delta_t;

		// std::cout << delta_t << " " << _sum_delta_t << std::endl;

		if (_stats_sum_delta_t >= 5.0)
		{
			if (options->_verbose >= 4)
				std::cout << "[ControlLoop]: avg fps: " << _iteration_count/_stats_sum_delta_t << std::endl;

			_iteration_count = 0;
			_stats_sum_delta_t -= 5.0;
		}

		while (_sum_delta_t >= 1.0/_freq)
		{
			scgraph->run_one_control_cycle (1.0/_freq);

			++_iteration_count;
			_sum_delta_t -= 1.0/_freq;
		}


		_tv = tv;

		usleep ((int)(1000.0));
	}

	if (options->_verbose >= 2)
		std::cout << "[ControlLoop]: Thread quitting!" << std::endl;

	return 0;
}

