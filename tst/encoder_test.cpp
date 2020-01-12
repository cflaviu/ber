#include "asn1/ber/pch.h"
#ifndef  PCH
	#include <string>
#endif
#include <asn1/ber/encoder/sequence.h>
#include <asn1/ber/encoder/value.h>

int main()
{
	using namespace asn1::ber::encoder;
	uint32_t seq_tag = 2;
	uint32_t value1_tag = 3;
	uint32_t value2_tag = 4;
	uint32_t value3_tag = 5;

	asn1::byte* value_buffer1 = nullptr;
	asn1::byte* value_buffer2 = nullptr;
	asn1::byte* value_buffer3 = nullptr;
	asn1::byte* output_buffer = nullptr;
	uint32_t value_buffer1_size = 10;
	uint32_t value_buffer2_size = 20;
	uint32_t value_buffer3_size = 30;
	uint32_t output_buffer_size = 100;

	value<> value1(value1_tag, value_buffer2, value_buffer1_size);
	value<> value2(value2_tag, value_buffer2, value_buffer1_size);
	sequence<> seq;
	seq << value1 << value2;

	value<> value3(value3_tag, value_buffer3, value_buffer3_size);
	sequence<> main_seq;
	main_seq << value3 << seq;

	main_seq.encode_to(output_buffer, output_buffer_size);
}
