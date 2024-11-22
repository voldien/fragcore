/*
 *	FragCore - Fragment Core - Engine Core
 *	Copyright (C) 2018 Valdemar Lindberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program;
 */
#ifndef _FRAGCORE_NORMAL_DISTRIBUTION_H_
#define _FRAGCORE_NORMAL_DISTRIBUTION_H_ 1
#include "../FragDef.h"
#include <random>

namespace fragcore {

	/**
	 * @brief
	 *
	 * @tparam T
	 */
	template <typename T = float> class FVDECLSPEC IRandom {
		static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
					  "Must be a decimal type(float/double/half) or integer.");

	  public:
		using DType = T;
		const size_t DTypeSize = sizeof(DType);

		IRandom() = default;
		virtual ~IRandom() = default;

		virtual DType rand() noexcept = 0;

		virtual void reset() noexcept = 0;

	  protected:
		std::random_device random_device; // Will be used to obtain a seed for the random number engine
	};

	/**
	 * @brief
	 *
	 * @tparam U
	 */
	template <typename U> class FVDECLSPEC RandomUniform : public IRandom<U> {
	  public:
		RandomUniform(const U min = 0.0, const U max = 1.0, const size_t seed = 0) {
			this->distribution = std::uniform_real_distribution<U>(min, max);
			this->generator.seed(seed);
		}

		U rand() noexcept override { return this->distribution(this->generator); }
		void reset() noexcept override { this->distribution.reset(); }

	  private:
		std::uniform_real_distribution<U> distribution;
		std::default_random_engine generator;
	};

	/**
	 * @brief
	 *
	 * @tparam U
	 */
	template <typename U> class FVDECLSPEC RandomNormal : public IRandom<U> {
	  public:
		RandomNormal(const U mean = 0.0, const U stddev = 1.0, const size_t seed = 118284) {
			this->distribution = std::normal_distribution<U>(mean, stddev);
			std::random_device random_device;
			this->gen = std::mt19937(random_device()); // Standard mersenne_twister_engine seeded with rd()
			this->gen.seed(seed);
		}

		U rand() noexcept override { return this->distribution(this->gen); }
		void reset() noexcept override { this->distribution.reset(); }

	  private:
		std::normal_distribution<U> distribution;
		std::mt19937 gen;
	};

	/**
	 * @brief
	 *
	 * @tparam U
	 */
	template <typename U> class FVDECLSPEC RandomBernoulli : public IRandom<U> {
	  public:
		RandomBernoulli(const U perc = 1.0, const size_t seed = 512523) {
			this->distribution = std::bernoulli_distribution(perc);
			std::random_device random_device;
			this->generator = std::mt19937(random_device()); // Standard mersenne_twister_engine seeded with rd()
			this->generator.seed(seed);
		}

		U rand() noexcept override { return this->distribution(this->generator); }
		void reset() noexcept override { this->distribution.reset(); }

	  private:
		std::bernoulli_distribution distribution;
		std::mt19937 generator;
	};

	template <typename U> class FVDECLSPEC RandomBinomial : public IRandom<U> {
	  public:
		RandomBinomial(const U perc = 0.5, const size_t seed = 123456) {
			this->distribution = std::binomial_distribution<>(seed, perc);
			std::random_device random_device;
			this->generator = std::mt19937(random_device());
			this->generator.seed(seed);
		}

		U rand() noexcept override { return this->distribution(this->generator); }
		void reset() noexcept override { this->distribution.reset(); }

	  private:
		std::binomial_distribution<> distribution;
		std::mt19937 generator;
	};
} // namespace fragcore

#endif
