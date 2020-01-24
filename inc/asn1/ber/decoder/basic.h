#pragma once
#ifndef PCH
	#include <asn1/ber/decoder/field/base.h>
	#include <asn1/ber/decoder/field/tag.h>
	#include <asn1/ber/decoder/field/length.h>
	#include <asn1/ber/decoder/field/value.h>
#endif

namespace asn1
{
	namespace ber
	{
		namespace decoder
		{
			enum id : byte
			{
				tag_id,
				length_id,
				value_id,
				meta_id
			};

			template <typename _Length = uint16_t>
			class basic
			{
			public:
				using tag_type = byte;
				using length_type = _Length;
				using tag_decoder = field::tag;
				using length_decoder = field::length<length_type>;
				using value_decoder = field::value<length_type>;

			protected:
				tag_decoder tag_reader_;
				length_decoder length_reader_;
				value_decoder value_reader_;
				field::base* const decoders_[3] = { &tag_reader_, &length_reader_, &value_reader_ };
				id decoder_id_;

				inline field::base* decoder() const
				{
					return decoders_[decoder_id_];
				}

			public:
				using state_t = field::state_t;

                basic(byte* const buffer, byte* const buffer_end) noexcept :
					value_reader_(buffer, buffer_end),
					decoder_id_(decoder::tag_id)
				{}

                id current_decoder() const noexcept { return decoder_id_; }

                const tag_decoder& tag() const noexcept { return tag_reader_; }

                const length_decoder& length() const noexcept { return length_reader_; }

                const value_decoder& value() const noexcept { return value_reader_; }

                void reset() noexcept
				{
					tag_reader_.reset();
					length_reader_.reset();
					decoder_id_ = tag_id;
				}

                state_t state() const noexcept { return decoder()->state(); }
                error_t error() const noexcept{ return decoder()->error(); }

                bool good() const noexcept { return is_good(state()); }

                const byte* operator () (const byte* buffer, const byte* const buffer_end) noexcept;
			};


			template <typename _Length>
            const byte* basic<_Length>::operator () (const byte* buffer, const byte* const buffer_end) noexcept
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
											decoder_id_ = id::value_id;
											return nullptr;
										}
									}
									case tag_id:
									{
										decoder_id_ = id(byte(decoder_id_) + 1);
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
		}
	}
}
