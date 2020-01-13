#include "asn1/ber/pch.h"
#ifndef  PCH
	#include <string>
#endif
#include <asn1/ber/encoder/structure.h>
#include <asn1/ber/encoder/value.h>

int main()
{
	using namespace asn1::ber::encoder;
	uint32_t value1_tag = 3;
	uint32_t value2_tag = 4;
	uint32_t value3_tag = 5;

	std::array<asn1::byte, 4> value_buffer1 { 0x0A, 0x0B, 0x0C, 0x0D };
	std::array<asn1::byte, 6> value_buffer2 { 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
	std::array<asn1::byte, 8> value_buffer3 { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
	std::array<asn1::byte, 24> output_buffer {};

	value<> value1(value1_tag, value_buffer1.data(), static_cast<uint32_t>(value_buffer1.size()));
	value<> value2(value2_tag, value_buffer2.data(), static_cast<uint32_t>(value_buffer2.size()));
	structure<> seq(asn1::ber::sequence);
	seq << value1 << value2;

	value<> value3(value3_tag, value_buffer3.data(), static_cast<uint32_t>(value_buffer3.size()));
	structure<> main_seq(asn1::ber::sequence);
	main_seq << value3 << seq;

	main_seq.encode_to(output_buffer.data(), static_cast<uint32_t>(output_buffer.size()));
}
