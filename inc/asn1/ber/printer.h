#pragma once
#ifndef PCH
	#include <asn1/ber/decoder.h>
#endif

namespace asn1
{
	namespace ber
	{
		class printer
		{
		protected:
			typedef decoder<printer, 128> _Decoder;
			_Decoder decoder_;
			std::string padding_;

			static void print_buffer(const byte* buffer, const size_t buffer_size);

			void on_data(const uint32_t tag, const bool constructed, const byte* data, const uint32_t size);
			void on_error(const byte decoder_id, const byte error);

			friend _Decoder;
		public:
			printer();

			void reset();

			void operator () (const byte* buffer, size_t buffer_size);
		};
	}
}
