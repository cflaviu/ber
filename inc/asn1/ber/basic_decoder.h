#pragma once
#ifndef PCH
	#include <asn1/ber/field_decoder/base.h>
	#include <asn1/ber/field_decoder/tag.h>
	#include <asn1/ber/field_decoder/length.h>
	#include <asn1/ber/field_decoder/value.h>
#endif

namespace asn1
{
	namespace ber
	{
		enum decoder_id : byte
		{
			tag_id,
			length_id,
			value_id,
			meta_id
		};

		template <typename _Tag = uint32_t, typename _Length = uint32_t>
		class basic_decoder
		{
		public:
			typedef _Tag tag_type;
			typedef _Length length_type;
			typedef field_decoder::tag<tag_type> tag_decoder;
			typedef field_decoder::length<length_type> length_decoder;
			typedef field_decoder::value<length_type> value_decoder;

		protected:
			tag_decoder tag_reader_;
			length_decoder length_reader_;
			value_decoder value_reader_;
			field_decoder::base* const decoders_[3] = { &tag_reader_, &length_reader_, &value_reader_ };
			decoder_id decoder_id_;

			inline field_decoder::base* decoder() const
			{
				return decoders_[decoder_id_];
			}

		public:
			typedef field_decoder::state_t state_t;
			typedef field_decoder::error_t error_t;

			basic_decoder(byte* const buffer, const length_type buffer_size) :
				value_reader_(buffer, buffer_size),
				decoder_id_(tag_id)
			{}

			decoder_id current_decoder() const { return decoder_id_; }

			const tag_decoder& tag() const { return tag_reader_; }

			const length_decoder& length() const { return length_reader_; }

			const value_decoder& value() const { return value_reader_; }

			void reset()
			{
				tag_reader_.reset();
				length_reader_.reset();
				decoder_id_ = tag_id;
			}

			state_t state() const { return decoder()->state(); }
			error_t error() const { return decoder()->error(); }

			bool good() const { return is_good(state()); }

			const byte* operator () (const byte* buffer, const byte* const buffer_end)
			{
				while (buffer < buffer_end)
				{
					auto reader = decoder();
					buffer = (*reader)(buffer, buffer_end);
					switch (reader->state())
					{
						case state_t::done:
						{
							if (buffer < buffer_end)
							{
								switch (decoder_id_)
								{
									case length_id:
									{
										if (!value_reader_.reset(length_reader_.value()))
										{
											decoder_id_ = decoder_id::value_id;
											return nullptr;
										}
									}
									case tag_id:
									{
										decoder_id_ = decoder_id(byte(decoder_id_) + 1);
										break;
									}
									case value_id:
									default:;
									{
										return buffer;
									}
								}
							}

							break;
						}
						case state_t::error:
						{
							return nullptr;
						}
						default:;
					}
				}

				return buffer;
			}
		};
	}
}
