#pragma once
#ifndef PCH
	#include <asn1/ber/decoder/engine.h>
	#include <array>
	#include <iostream>
	#include <iomanip>
#endif

namespace asn1
{
	namespace ber
	{
		template <typename T>
		T& print(T& out, const byte* buffer, const size_t buffer_size)
		{
			for (size_t i = 0; i != buffer_size; ++i)
			{
				out << std::hex << std::setfill('0') << std::setw(2) << int(buffer[i]) << ' ';
			}

			return out;
		}

		namespace decoder
		{
			template <const size_t _BufferSize = 128, typename _Length = uint16_t>
			class printer
			{
			protected:
				using _Engine = engine<printer, _Length>;
				using tag_type = byte;

				std::array<byte, _BufferSize> buffer_;
				_Engine decoder_;
				std::string padding_;
				std::ostream* output_;

				std::ostream& out() const noexcept { return *output_; }

				void on_data(const field::tag& tag, const byte* data, const _Length data_size)
				{
					out() << padding_ << text_of(tag_t(tag.value())) << ' ' << '[' << std::dec << data_size << "]: ";
					print(data, data_size);
					out() << '\n';
					if (tag.is_constructed())
					{
						padding_ += "  ";
					}
				}

				void on_error(const byte decoder_id, const byte error, const byte* buffer, const _Length buffer_size)
				{
					out() << "decoder " << int(decoder_id) << ": error " << int(error) << "\n   buffer: ";
					print(buffer, buffer_size);
				}

				friend _Engine;
			public:
				printer(std::ostream& output = std::cout) noexcept:
					decoder_(buffer_.data(), _BufferSize, this),
					output_(&output)
				{}

				void reset() noexcept
				{
					padding_.clear();
				}

				void operator () (const byte* buffer, const byte* const buffer_end) noexcept
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

				void print(const byte* buffer, const size_t buffer_size)
				{
					ber::print(out(), buffer, buffer_size);
				}
			};
		}
	}
}
