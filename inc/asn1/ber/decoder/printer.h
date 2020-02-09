#pragma once
#ifndef PCH
    #include <asn1/ber/decoder/tag_length.h>
	#include <array>
	#include <iostream>
	#include <iomanip>
#endif

namespace asn1
{
	namespace ber
	{
		template <typename T>
        T& print_buffer(T& out, const byte* buffer, const size_t buffer_size)
		{
			for (size_t i = 0; i != buffer_size; ++i)
			{
				out << std::hex << std::setfill('0') << std::setw(2) << int(buffer[i]) << ' ';
			}

			return out;
		}

		namespace decoder
		{
            template <typename _Length = uint16_t>
			class printer
			{
			protected:
                tag_length<_Length> decoder_;
				std::string padding_;
				std::ostream* output_;

				std::ostream& out() const noexcept { return *output_; }

			public:
				printer(std::ostream& output = std::cout) noexcept:
					output_(&output)
				{}

				void reset() noexcept
				{
					padding_.clear();
				}

                void operator () (const byte*& buffer, const byte* const buffer_end)
				{
                    auto error = error_t::none;
                    while(buffer != buffer_end)
					{
                        error = decoder_(buffer, buffer_end);
                        if (error == error_t::none)
						{
                            auto length = decoder_.length();
                            auto& tag = decoder_.tag();
                            out() << padding_ << text_of(tag_t(tag.id())) << ' ' << '[' << std::dec << length << "]: ";
                            print(buffer, length);
                            out() << '\n';
                            if (tag.is_constructed())
                            {
                                padding_ += "  ";
                            }
						}
                        else
                        {
                            out() << "error " << int(error) << "\n   buffer: ";
                            print(buffer, std::distance(buffer, buffer_end));
                            break;
                        }
					}
				}

				void print(const byte* buffer, const size_t buffer_size)
				{
                    ber::print_buffer(out(), buffer, buffer_size);
				}
			};
		}
	}
}
