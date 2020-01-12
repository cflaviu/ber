#pragma once
#ifndef PCH
	#include <asn1/ber/decoder/engine.h>
#endif

namespace asn1
{
	namespace ber
	{
		namespace decoder
		{
			class printer
			{
			protected:
				typedef engine<printer, 128> _Engine;
				_Engine decoder_;
				std::string padding_;

				static void print_buffer(const byte* buffer, const size_t buffer_size);

				void on_data(const uint32_t tag, const bool constructed, const byte* data, const size_t size);
				void on_error(const byte decoder_id, const byte error, const byte* buffer, const size_t buffer_size);

				friend _Engine;
			public:
				printer();

				void reset();

				void operator () (const byte* buffer, size_t buffer_size);
			};
		}
	}
}