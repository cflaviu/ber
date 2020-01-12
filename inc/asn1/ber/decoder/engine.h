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
		typedef uint32_t size_t;

		namespace decoder
		{
			/*
			struct observer_example
			{
				void on_data(const uint32_t tag, const bool constructed, const byte* data, const uint32_t size);
				void on_error(const decoder_id item, const byte* buffer, const uint32_t buffer_size);
			};
			*/

			template <typename _Observer, const size_t _BufferSize, typename _Tag = uint32_t, typename _Length = uint32_t>
			class engine
			{
			public:
				using _Buffer = std::array<byte, _BufferSize>;
				using _Engine = basic<_Tag, _Length>;
				using state_t = typename _Engine::state_t;
				using error_t = field::error_t;

			protected:
				class internal_data
				{
					const _Engine& decoder_;
					state_t	state_;
					error_t	error_;
					state_t* state_ptr_;
					error_t* error_ptr_;

				public:
					internal_data(const _Engine& decoder) :
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

				bool read(const byte*& buffer, size_t& buffer_size);

				_Buffer buffer_;
				_Engine decoder_;
				internal_data internal_data_;
				_Observer* observer_;

			public:
				engine(_Observer* const observer = nullptr) :
					decoder_(buffer_.data(), static_cast<uint32_t>(buffer_.size())),
					observer_(observer),
					internal_data_(decoder_)
				{}

				state_t state() const { return internal_data_.state(); }

				bool good() const { return internal_data_.good(); }

				_Observer* observer() const { return observer_; }

				void set_observer(_Observer* const item) { observer_ = item; }

				bool operator () (const byte*& buffer, size_t& buffer_size);

				void reset()
				{
					decoder_.reset();
					internal_data_.reset();
				}
			};

			template <typename _Observer, const size_t _BufferSize, typename _Tag = uint32_t, typename _Length = uint32_t>
			bool engine<_Observer, _BufferSize, _Tag, _Length>::read(const byte*& buffer, size_t& buffer_size)
			{
				bool can_continue = true;
				auto next = decoder_(buffer, buffer + buffer_size);
				if (decoder_.state() == state_t::done)
				{
					const auto length = decoder_.length().value();
					auto consumed = length + decoder_.tag().length() + decoder_.length().length2();
					if (consumed <= buffer_size)
					{
						const bool constructed = decoder_.tag().content_type() == content_t::constructed;
						if (observer_ != nullptr)
						{
							observer_->on_data(decoder_.tag().value(), constructed, decoder_.value().buffer(), length);
						}

						buffer = constructed ? next - length : next;
						buffer_size -= consumed;
					}
					else
					{
						constexpr auto error = error_t::wrong_field_size;
						internal_data_.set_error(error);
						can_continue = false;
						if (observer_ != nullptr)
						{
							observer_->on_error(meta_id, static_cast<byte>(error), buffer, buffer_size);
						}
					}
				}

				return can_continue;
			}

			template <typename _Observer, const size_t _BufferSize, typename _Tag = uint32_t, typename _Length = uint32_t>
			bool engine<_Observer, _BufferSize, _Tag, _Length>::operator () (const byte*& buffer, size_t& buffer_size)
			{
				bool can_continue;
				switch (decoder_.state())
				{
					case state_t::done:
					{
						decoder_.reset();
					}
					case state_t::stopped:
					case state_t::reading:
					{
						can_continue = read(buffer, buffer_size);
						break;
					}
					case state_t::error:
					{
						can_continue = false;
						if (observer_ != nullptr)
						{
							observer_->on_error(decoder_.current_decoder(), static_cast<byte>(internal_data_.error()), buffer, buffer_size);
						}

						break;
					}
					default:
					{
						can_continue = false;
					}
				}

				return can_continue;
			}
		}
	}
}
