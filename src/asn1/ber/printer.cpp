#include "asn1/ber/pch.h"
#include "asn1/ber/decoder/printer.h"

namespace asn1 {
namespace ber {
namespace decoder {

printer::printer():
	decoder_(this)
{}

void printer::reset()
{
	padding_.clear();
}

void printer::print_buffer(const byte* buffer, const size_t buffer_size)
{
	for (auto i = 0; i != buffer_size; ++i)
	{
		std::cout << std::hex << std::setfill('0') << std::setw(2) << int(buffer[i]) << ' ';
	}
}

void printer::on_data(const uint32_t tag, const bool constructed, const byte* data, const uint32_t size)
{
	std::cout << padding_ << text_of(tag_t(tag)) << ' ' << '[' << std::dec << size << "]: ";
	print_buffer(data, size);
	std::cout << '\n';
	if (constructed)
	{
		padding_ += ' ';
	}
}

void printer::on_error(const byte decoder_id, const byte error, const byte* buffer, const size_t buffer_size)
{
	std::cout << "decoder " << int(decoder_id) << ": error " << int(error) << "\n   buffer: ";
	print_buffer(buffer, buffer_size);
}

void printer::operator () (const byte* buffer, size_t buffer_size)
{
	do
	{
		decoder_(buffer, buffer_size);
	}
	while (decoder_.good() && buffer_size != 0);
}

}}}
