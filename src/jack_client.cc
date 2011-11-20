#ifdef HAVE_JACK

#include "jack_client.h"
#include "options.h"

#include <iostream>
#include <cmath>
#include <cstring>

extern "C"
{
	int process_c (jack_nframes_t frames, void *arg)
	{
		assert (sizeof (JackClient*) == sizeof (void*));

		JackClient *tmp;

		memcpy (&tmp, &arg, sizeof (JackClient*));
		// std::cout << "woo" << std::endl;
		return tmp->process (frames);
	}
}


JackClient::JackClient (int num_of_ports, const std::string &jack_name) :
	_jack_name (jack_name)
{
	Options *options = Options::get_instance ();

	if (options->_verbose >= 1)
		std::cout << "[JackClient]: Initializing " << num_of_ports << " ports" << std::endl;

	if (num_of_ports == 0)
		return;

	_client = jack_client_new (_jack_name.c_str ());
	if (!_client)
		throw ("[JackClient]: Error: couldn't register as jack client");

	_buffer_size = jack_get_buffer_size (_client);
	_sample_rate = jack_get_sample_rate (_client);

	init_window (_window, _buffer_size);

	for (int i = 0; i < num_of_ports; ++i)
	{
		std::stringstream stream;
		stream << "in" << i;
		_ports.push_back (jack_port_register (_client, stream.str ().c_str (), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput | JackPortIsTerminal, 0));
		_per_port_output_buffers.push_back ((fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * (_buffer_size)));
	}

	_fft_in_buffer = new float [_buffer_size];
	_fft_out_buffer =  ((fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * (_buffer_size)));

	_fft_plan = fftwf_plan_dft_r2c_1d (_buffer_size, _fft_in_buffer, _fft_out_buffer, FFTW_MEASURE);

	jack_set_process_callback (_client, process_c, this);

	jack_activate (_client);
}

JackClient::~JackClient ()
{
	if (_ports.size () == 0)
		return;

	jack_deactivate (_client);
	jack_client_close (_client);
}

float JackClient::get_frequency (int port, float freq)
{
	float ret = 0;

	if (_ports.empty () || port > ((int)_ports.size() - 1))
		return 0;


	fftwf_complex *buffer = _per_port_output_buffers[port];

	if (_mutex.tryLock ())
	{
		
		//int bin = 1 + (int)(((_buffer_size/2)+1)/2 * freq/(_sample_rate/2.0));
		if (freq < 0) freq = 0;
		if (freq > 1) freq = 1;

		int bin = (int)(freq * (_buffer_size-1));

		ret = (1.0 / sqrt(((_buffer_size/2)+1))) * sqrt (buffer[bin][0] *  buffer[bin][0] + buffer[bin][1] *  buffer[bin][1]);

		_mutex.unlock ();
	}
	return ret;
}

void JackClient::init_window (float *buffer, int size)
{
	_window = new float [size];

	for (int i = 0; i < size; ++i)
	{
		_window[i] = 0.53836 - 0.46164 * cos((2 * M_PI * i)/(size - 1));
	}
}

void JackClient::do_windowing (float *buffer, int size)
{
	for (int i = 0; i < size; ++i)
	{
		buffer [i] = buffer [i] * _window [i];
	}
}

int JackClient::process (jack_nframes_t frames)
{
	if (_mutex.tryLock ())
	{
		for (size_t i = 0; i < _ports.size (); ++i)
		{
			memcpy (_fft_in_buffer, jack_port_get_buffer (_ports[i], frames), frames);

			do_windowing (_fft_in_buffer, frames);

			fftwf_execute (_fft_plan);

			for (int j = 0; j < _buffer_size; ++j)
			{
				(_per_port_output_buffers[i])[j][0] = _fft_out_buffer[j][0];
				(_per_port_output_buffers[i])[j][1] = _fft_out_buffer[j][1];
			}
		}
		_mutex.unlock ();
	}
	return 1;
}

#endif
