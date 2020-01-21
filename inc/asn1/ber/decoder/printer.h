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
			template <typename _Length = uint16_t>
			class printer
			{
			protected:
				using _Engine = engine<printer, 128, _Length>;
				using tag_type = byte;
				_Engine decoder_;
				std::string padding_;

				void on_data(const tag_type tag, const bool constructed, const byte* data, const size_t size)
				{
					std::cout << padding_ << text_of(tag_t(tag)) << ' ' << '[' << std::dec << size << "]: ";
					print(data, size);
					std::cout << '\n';
					if (constructed)
					{
						padding_ += ' ';
					}
				}

				void on_error(const byte decoder_id, const byte error, const byte* buffer, const  byte* const buffer_size)
				{
					std::cout << "decoder " << int(decoder_id) << ": error " << int(error) << "\n   buffer: ";
					print(buffer, static_cast<size_t>(std::distance(buffer, buffer_size)));
				}

				friend _Engine;
			public:
				printer():
					decoder_(this)
				{}

				void reset()
				{
					padding_.clear();
				}

				void operator () (const byte* buffer, const byte* const buffer_end)
				{
					for(std::pair<bool, const byte*> result; buffer != buffer_end; ) 
					{
						result = decoder_(buffer, buffer_end);
						if (!decoder_.good())
						{
							break;
						}

						buffer = result.second;
					}
				}

				static void print(const byte* buffer, const size_t buffer_size)
				{
					for (auto i = 0; i != buffer_size; ++i)
					{
						std::cout << std::hex << std::setfill('0') << std::setw(2) << int(buffer[i]) << ' ';
					}
				}
			};
		}
	}
}
