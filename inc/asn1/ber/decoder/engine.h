#pragma once
#ifndef PCH
	#include <asn1/ber/decoder/field/base.h>
	#include <asn1/ber/decoder/field/tag.h>
	#include <asn1/ber/decoder/field/length.h>
	#include <asn1/ber/decoder/field/value.h>
	#include <asn1/ber/decoder/basic.h>
#endif

namespace asn1
{
	namespace ber
	{
		namespace decoder
		{
			/*
			struct observer_example
			{
				void on_data(const byte tag, const bool constructed, const byte* data, const uint32_t data_size);
				void on_error(const decoder_id item, const byte* buffer, const uint32_t buffer_size);
			};
			*/

			template <typename _Observer, typename _Length = uint16_t>
			class engine
			{
			public:
				using tag_type = byte;
				using length_type = _Length;
				using state_t = typename basic<_Length>::state_t;

			protected:
				using _Decoder = basic<_Length>;

				class internal_data
				{
					const _Decoder& decoder_;
					state_t	state_;
					error_t	error_;
					state_t* state_ptr_;
					error_t* error_ptr_;

				public:
					internal_data(const _Decoder& decoder) :
						decoder_(decoder),
						state_(state_t::stopped),
						error_(error_t::none),
						state_ptr_(nullptr),
						error_ptr_(nullptr)
					{}

					state_t state() const
					{
						return (state_ptr_ == nullptr) ? decoder_.state() : *state_ptr_;
					}

					error_t error() const
					{
						return (error_ptr_ == nullptr) ? decoder_.error() : *error_ptr_;
					}

					bool good() const { return field::is_good(state()); }

					void set_error(const error_t item)
					{
						if (state_ptr_ == nullptr)
						{
							state_ptr_ = &state_;
							error_ptr_ = &error_;
						}

						state_ = state_t::error;
						error_ = item;
					}

					void reset()
					{
						if (state_ptr_ != nullptr)
						{
							state_ptr_ = nullptr;
							error_ptr_ = nullptr;
						}
					}
				};

				std::pair<bool, const byte*> read(const byte* buffer, const byte* const buffer_end);

				_Decoder decoder_;
				internal_data internal_data_;
				_Observer* observer_;

			public:
				engine(byte* decoding_buffer, const length_type decoding_buffer_size, _Observer* const observer = nullptr) :
					decoder_(decoding_buffer, decoding_buffer + decoding_buffer_size),
					observer_(observer),
					internal_data_(decoder_)
				{}

				state_t state() const { return internal_data_.state(); }

				bool good() const { return internal_data_.good(); }

				_Observer* observer() const { return observer_; }

				void set_observer(_Observer* const item) { observer_ = item; }

				std::pair<bool, const byte*> operator () (const byte* buffer, const byte* const buffer_size);

				void reset()
				{
					decoder_.reset();
					internal_data_.reset();
				}
			};

			template <typename _Observer, typename _Length>
			std::pair<bool, const byte*> engine<_Observer, _Length>::read(const byte* buffer, const byte* const buffer_end)
			{
				std::pair<bool, const byte*> result = std::make_pair(true, buffer_end); 
				auto next = decoder_(buffer, buffer_end);
				if (decoder_.state() == state_t::done)
				{
					const auto length = decoder_.length().value();
					const auto header_length = decoder_.tag().bytes() + decoder_.length().bytes();
					auto span = header_length + length;
					if (span <= std::distance(buffer, buffer_end))
					{
						const bool constructed = decoder_.tag().content_type() == content_t::constructed;
						if (observer_ != nullptr)
						{
							observer_->on_data(decoder_.tag().value(), constructed, decoder_.value().buffer(), length);
						}

						result.second = constructed ? next - length : next;
					}
					else
					{
						constexpr auto error = error_t::wrong_field_size;
						internal_data_.set_error(error);
						result.first = false;
						if (observer_ != nullptr)
						{
							observer_->on_error(meta_id, static_cast<byte>(error), buffer, static_cast<_Length>(buffer_end - buffer));
						}
					}
				}

				return result;
			}

			template <typename _Observer, typename _Length>
			std::pair<bool, const byte*> engine<_Observer, _Length>::operator () (const byte* buffer, const byte* const buffer_end)
			{
				std::pair<bool, const byte*> result;
				switch (decoder_.state())
				{
					case state_t::done:
					{
						decoder_.reset();
					}
					case state_t::stopped:
					case state_t::reading:
					{
						result = read(buffer, buffer_end);
						break;
					}
					case state_t::error:
					{
						result.first = false;
						if (observer_ != nullptr)
						{
							observer_->on_error(decoder_.current_decoder(), static_cast<byte>(internal_data_.error()), buffer, static_cast<_Length>(buffer_end - buffer));
						}

						break;
					}
					default:
					{
						result.first = false;
					}
				}

				return result;
			}
		}
	}
}
