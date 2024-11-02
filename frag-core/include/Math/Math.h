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
#ifndef _FRAGCORE_MATH_H_
#define _FRAGCORE_MATH_H_ 1
#include "../FragDef.h"
#include "../Math/Random.h"
#include "../Math3D/Math3D.h"
#include <cfloat>
#include <cmath>
#include <vector>

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC Math {
	  public:
		/**
		 *
		 */
		template <typename T> inline constexpr static T abs(const T value) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");

			if constexpr (std::is_integral<T>::value) {
				return std::abs(static_cast<long>(value));
			} else if constexpr (std::is_floating_point<T>::value) {
				return std::fabs(value);
			} else {
				assert(0);
			}
		}

		template <class T> inline constexpr static T clamp(const T value, const T min, const T max) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value || std::is_enum<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			return Math::max<T>(min, Math::min<T>(max, value));
		}

		template <typename T> static T *clamp(T *list, const T min, const T max, const size_t nrElements) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value || std::is_enum<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			size_t index = 0;
#ifdef _OPENMP
#pragma omp simd simdlen(4) linear(index : 1)
#endif
			for (index = 0; index < nrElements; index++) {
				list[index] = Math::clamp<T>(list[index], min, max);
			}
			return list;
		}

		/**
		 *	Get max value of a and b.
		 */
		template <typename T> inline constexpr static T max(const T value0, const T value1) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value || std::is_enum<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			return (static_cast<T>(value0) < static_cast<T>(value1)) ? static_cast<T>(value1) : static_cast<T>(value0);
		}

		/**
		 *	Get min value of a and b.
		 */
		template <typename T> inline constexpr static T min(const T value0, const T value1) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value || std::is_enum<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			return (static_cast<T>(value1) < static_cast<T>(value0)) ? static_cast<T>(value1) : static_cast<T>(value0);
		}

		template <typename T> inline constexpr static T frac(const T value) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			T part;
			std::modf(value, &part);
			return part;
		}

		template <typename T, typename U> static std::vector<T> &add(std::vector<T> &list, const U &value) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Type Must Support addition operation.");
			for (size_t i = 0; i < list.size(); i++) {
				list[i] += value;
			}
			return list;
		}

		template <typename T> static inline T sum(const std::vector<T> &list) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value || std::is_enum<T>::value,
						  "Type Must Support addition operation.");
			return Math::sum<T>(list.data(), list.size());
		}

		template <typename T> static T sum(const T *list, const size_t nrElements) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value || std::is_enum<T>::value,
						  "Type Must Support addition operation.");
			T sum = 0;
			T value;
			size_t index;
			// #pragma omp simd reduction(+ : sum) private(value) simdlen(4) linear(index : 1)
			for (index = 0; index < nrElements; index++) {
				value = list[index];
				sum += value;
			}
			return sum;
		}

		template <typename T> static inline T product(const std::vector<T> &list) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Type Must Support addition operation.");
			return Math::product<T>(list.data(), list.size());
		}

		template <typename T> static T product(const T *list, const size_t nrElements) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Type Must Support addition operation.");
			T product_combined = 1;
			size_t index;
#ifdef _OPENMP
#pragma omp simd reduction(* : product_combined) simdlen(4) linear(index : 1)
#endif
			for (index = 0; index < nrElements; index++) {
				product_combined *= list[index];
			}
			return product_combined;
		}

		template <typename T> static T dot(const T *listA, const T *listB, const size_t nrElements) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Type Must Support addition operation.");
			T sum = 0;
			size_t index;
#ifdef _OPENMP
#pragma omp simd reduction(+ : sum) simdlen(4) linear(index : 1)
#endif
			for (index = 0; index < nrElements; index++) {
				sum += listA[index] * listB[index];
			}
			return sum;
		}

		template <typename T> static void pow(const T exponent, T *list, const size_t nrElements) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Type Must Support addition operation.");
			/*	*/
			size_t index;
#ifdef _OPENMP
#pragma omp simd simdlen(4) linear(index : 1)
#endif
			for (index = 0; index < nrElements; index++) {
				list[index] = static_cast<T>(std::pow(list[index], exponent));
			}
		}

		template <typename T> constexpr static T mean(const T *list, const size_t nrElements) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Type Must Support addition operation.");
			/*	*/
			const T sum = Math::sum<T>(list, nrElements);
			const float averageInverse = static_cast<float>(1) / static_cast<float>(nrElements);
			return static_cast<T>(averageInverse * sum);
		}

		template <typename T> constexpr static T mean(const std::vector<T> &list) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Type Must Support addition operation.");
			const T sum = Math::sum<T>(list);
			const float averageInverse = static_cast<float>(1) / static_cast<float>(list.size());
			return static_cast<T>(averageInverse * sum);
		}

		template <typename T> static T variance(const T *list, const size_t nrElements, const T mean) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Type Must Support addition operation.");
			T sum = 0;
			size_t index;
#ifdef _OPENMP
#pragma omp simd reduction(+ : sum) simdlen(4) linear(index : 1)
#endif
			for (index = 0; index < nrElements; index++) {
				sum += (list[index] - mean) * (list[index] - mean);
			}

			return (static_cast<T>(1) / static_cast<T>((nrElements - 1))) * sum;
		}

		template <typename T> static inline T variance(const std::vector<T> &list, const T mean) {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Type Must Support addition operation.");
			return Math::variance<T>(list.data(), list.size(), mean);
		}

		template <typename T> constexpr static inline T standardDeviation(const std::vector<T> &list, const T mean) {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Type Must Support addition operation.");
			return static_cast<T>(std::sqrt(Math::variance<T>(list, mean)));
		}

		template <typename T>
		static T cov(const std::vector<T> &listA, const std::vector<T> &listB, const T meanA, const T meanB) {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Type Must Support addition operation.");

			T sum = 0;

			const size_t nrElements = listB.size();
#ifdef _OPENMP
#pragma omp parallel for simd reduction(+ : sum) shared(listA, listB)
#endif
			for (size_t i = 0; i < nrElements; i++) {
				sum += (listA[i] - meanA) * (listB[i] - meanB);
			}

			return (static_cast<T>(1) / static_cast<T>(listA.size())) * sum;
		}

		template <typename T>
		constexpr static T cor(const std::vector<T> &listA, const std::vector<T> &listB, const T meanA, const T meanB) {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");

			return cov<T>(listA, listB, meanA, meanB) /
				   static_cast<T>(std::sqrt(variance<T>(listB, meanB) * variance<T>(listA, meanA)));
		}

		/**
		 *	Convert degree to radian.
		 */
		template <typename T> inline constexpr static T degToRad(const T deg) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			return deg * static_cast<T>(Deg2Rad);
		}

		/**
		 *	Convert radian to degree.
		 */
		template <typename T> inline constexpr static T radToDeg(const T rad) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			return rad * static_cast<T>(Rad2Deg);
		}

		/**
		 *
		 */
		template <typename T> static T wrapAngle(T angle) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			while (angle > static_cast<T>(Math::PI_2)) {
				angle -= static_cast<T>(Math::PI_2);
			}
			while (angle < 0.0f) {
				angle += static_cast<T>(Math::PI_2);
			}
			return angle;
		}

		/**
		 * @brief Linear interpolation.
		 *
		 * @tparam T
		 * @param a Start point.
		 * @param b End point.
		 * @param t normalized interpolation, between [0,1], a value greater than 1 will not be clamped
		 * 	and will thus exceed eitehr the start or the end point.
		 * @return constexpr T
		 */
		template <typename T, typename U>
		inline constexpr static T lerp(const T value0, const T value1, const U interp) noexcept {
			static_assert(std::is_floating_point<U>::value, "Must be a decimal type(float/double/half).");
			return (value0 + (value1 - value0) * interp);
		}

		template <typename T, typename U>
		inline constexpr static T lerpClamped(const T value0, const T value1, const U interp) noexcept {
			static_assert(std::is_floating_point<U>::value, "Must be a decimal type(float/double/half).");
			return (value0 + (value1 - value0) * Math::clamp<U>(interp, static_cast<U>(0.0), static_cast<U>(1.0)));
		}

		template <typename T> inline constexpr static T mod(const T value, const T mod) noexcept {
			static_assert(std::is_integral<T>::value, "Must be a integer type.");
			return (value % mod + mod) % mod;
		}

		/**
		 *
		 */
		inline static constexpr double E = 2.718281828459045235;
		inline static constexpr double PI = 3.141592653589793238462643383279502884;
		inline static constexpr double PI_half = Math::PI / 2.0;
		inline static constexpr double PI_2 = Math::PI * 2.0;
		inline static constexpr double Epsilon = FLT_EPSILON;
		inline static constexpr double Deg2Rad = Math::PI / 180.0;
		inline static constexpr double Rad2Deg = 180 / Math::PI;
		inline static constexpr double NegativeInfinity = 0;

		template <typename T> static inline constexpr T NextPowerOfTwo(const T value) noexcept {
			static_assert(std::is_integral<T>::value, "Must be a integer type.");
			T res = 1;
			while (res < value) {
				res <<= 1;
			}
			return res;
		}

		/**
		 *
		 */
		template <typename T> static inline constexpr T ClosestPowerOfTwo(const T value) {
			T n = NextPowerOfTwo(value);
			T p = 0;
			return 0;
		}

		template <typename T> static inline constexpr bool IsPowerOfTwo(const T value) noexcept {
			static_assert(std::is_integral<T>::value, "Must be a integer type.");
			return !(value == 0) && !((value - 1) & value);
		}

		/**
		 *	Generate 1D guassian.
		 */
		template <typename T>
		static inline void guassian(std::vector<T> &guassian, const unsigned int height, const T theta,
									const T standard_deviation) {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			guassian.reserve(height);
			Math::guassian<T>(guassian.data(), height, theta, standard_deviation);
		}

		template <typename T>
		static void guassian(T *guassian, const unsigned int height, const T theta, const T standard_deviation) {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");

			const T exp_inverse =
				(static_cast<T>(1.0) / (static_cast<T>(2.0) * standard_deviation * standard_deviation));
			const T sqr_2_pi_inverse = 1.0 / (standard_deviation * static_cast<T>(std::sqrt(2 * Math::PI)));

			const T offset = static_cast<T>(height) / -2;

			// #pragma omp simd
			for (unsigned int i = 0; i < height; i++) {

				const T exp_num_sqrt = (i - theta + offset);

				const T exponent = exp_inverse * -(exp_num_sqrt * exp_num_sqrt);
				const T value = sqr_2_pi_inverse * std::exp(exponent);

				guassian[i] = value;
			}
		}

		/**
		 *	Generate 2D guassian.
		 */
		template <typename T>
		static inline void guassian(std::vector<T> &guassian, const unsigned int width, const unsigned int height,
									const T theta, const T standard_deviation) {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			guassian.reserve(width * height);
			Math::guassian<T>(static_cast<T &>(*guassian.data()), width, height, theta, standard_deviation);
		}

		template <typename T>
		static inline void guassian(const T &guassian, const unsigned int width, const unsigned int height,
									const T standard_deviation) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");

			const T exp_inverse =
				(static_cast<T>(1.0) / (static_cast<T>(2.0) * standard_deviation * standard_deviation));
			const T sqr_2_pi_inverse = 1.0 / (standard_deviation * static_cast<T>(std::sqrt(2 * Math::PI)));

			const T offset = static_cast<T>(height) / -2;

			for (unsigned int y = 0; y < height; y++) {
				for (unsigned int x = 0; x < width; x++) {

					// TODO: add offset
					// const T exp_num_sqrt = (i - theta + offset);

					const T exponent = exp_inverse * -(x * x + y * y);

					guassian[y * width + x] = sqr_2_pi_inverse * std::exp(exponent);
				}
			}
		}

		template <typename T, typename U>
		static constexpr inline T gammaCorrection(const T value, const U gamma) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");

			T exponent = static_cast<T>(1.0) / gamma;
			return static_cast<T>(std::pow(value, exponent));
		}

		template <typename T> static T gameSpaceToLinear(const T gamma, const T exp) noexcept {
			return std::pow(gamma, exp);
		}

		/**
		 *	Generate perlin noise value
		 */
		static float PerlinNoise(const float position_x, const float position_y) noexcept;
		static float PerlinNoise(const Vector2 &point) noexcept;
		static float PerlinNoise(const float position_x, const float position_y, const float position_z) noexcept;
		static float PerlinNoise(const Vector3 &point) noexcept;

		/**
		 * @brief
		 *
		 * @tparam T
		 * @return T
		 */
		template <typename T> static inline T random() {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			return static_cast<T>(drand48());
		}

		template <typename T> static std::vector<T> &random(std::vector<T> &samples) {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			for (size_t i = 0; i < samples.size(); i++) {
				samples[i] = Random::rand<T>();
			}
			return samples;
		}

		template <typename T> static std::vector<T> &jitter(std::vector<T> &samples) {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");

			size_t sqrt_samples = (size_t)(std::sqrt(samples.size()));
			for (size_t i = 0; i < sqrt_samples; i++) {
				for (size_t j = 0; j < sqrt_samples; j++) {
					const T x = (static_cast<T>(i) + Random::rand<T>()) / static_cast<T>(sqrt_samples);
					const T y = (static_cast<T>(j) + Random::rand<T>()) / static_cast<T>(sqrt_samples);

					samples[i * sqrt_samples + j] = x;
					samples[i * sqrt_samples + j] = y;
				}
			}
			return samples;
		}

		template <typename T> static std::vector<T> &nrooks(std::vector<T> &samples) {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			size_t num_samples = samples.size();
			for (size_t i = 0; i < num_samples; i++) {
				samples[i].setX((static_cast<double>(i) + drand48()) / static_cast<double>(num_samples));
				samples[i].setY((static_cast<double>(i) + drand48()) / static_cast<double>(num_samples));
			}

			for (size_t i = num_samples - 2; i >= 0; i--) {
				int target = static_cast<int>((drand48() * static_cast<double>(i)));
				float temp = samples[i + 1].x();
				samples[i + 1].setX(samples[target].x());
				samples[target].setX(temp);
			}
			return samples;
		}

		template <typename T> static std::vector<T> &hammersley(std::vector<T> &samples) {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			return {};
		}

		template <typename T> static inline constexpr T align(const T size, const T alignment) noexcept {
			static_assert(std::is_integral<T>::value, "Must be an integral type.");
			return size + (alignment - (size % alignment));
		}

/*	*/
#ifdef _OPENMP
#pragma omp declare simd uniform(value) simdlen(4)
#endif
		template <typename T> inline static T computeSigmoid(const T value) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			return Math::clamp<T>(static_cast<T>(1) / (std::exp(-value) + static_cast<T>(1)), static_cast<T>(0),
								  static_cast<T>(1));
		}
#ifdef _OPENMP
#pragma omp declare simd uniform(value) simdlen(4)
#endif
		template <typename T> inline static T computeSigmoidDerivate(const T value) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			const T sig = computeSigmoid(value);
			return sig * (static_cast<T>(1) - sig);
		}

#ifdef _OPENMP
#pragma omp declare simd uniform(value) simdlen(4)
#endif
		template <typename T> inline static constexpr T relu(const T value) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			return Math::max<T>(0, value);
		}

#ifdef _OPENMP
#pragma omp declare simd uniform(value) simdlen(4)
#endif
		template <typename T> inline static constexpr T reluDeriviate(const T value) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			if (value >= 0) {
				return 1;
			}
			return 0;
		}
#ifdef _OPENMP
#pragma omp declare simd uniform(value, alpha) simdlen(4)
#endif
		template <typename T> inline static constexpr T leakyRelu(const T alpha, const T value) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			if (value < 0) {
				return alpha * value;
			}
			return std::max<T>(0, value);
		}
#ifdef _OPENMP
#pragma omp declare simd uniform(value, alpha) simdlen(4)
#endif
		template <typename T> inline static constexpr T leakyReluDerivative(const T alpha, const T value) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			if (value >= 0) {
				return 1;
			}
			return alpha;
		}
#ifdef _OPENMP
#pragma omp declare simd uniform(value) simdlen(4)
#endif
		template <typename T> inline static constexpr T computeTanh(const T value) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			return std::tanh(value);
		}
#ifdef _OPENMP
#pragma omp declare simd uniform(value) simdlen(4)
#endif
		template <typename T> inline static constexpr T computeTanhDerivate(const T value) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			return 1.0 - (computeTanh<T>(value) * computeTanh<T>(value));
		}
#ifdef _OPENMP
#pragma omp declare simd uniform(coeff, value) simdlen(4)
#endif
		template <typename T> inline static constexpr T computeLinear(const T coeff, const T value) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			return coeff * value;
		}
#ifdef _OPENMP
#pragma omp declare simd uniform(coeff) simdlen(4)
#endif
		template <typename T> inline static constexpr T computeLinearDerivative(const T coeff) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			return coeff;
		}
#ifdef _OPENMP
#pragma omp declare simd uniform(coeff, value) simdlen(4)
#endif
		template <typename T> inline static constexpr T computeExpLinear(const T coeff, const T value) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			if (value >= 0) {
				return value;
			}
			return coeff * (std::exp(value) - 1);
		}
#ifdef _OPENMP
#pragma omp declare simd uniform(coeff, value) simdlen(4)
#endif
		template <typename T>
		inline static constexpr T computeExpLinearDerivative(const T coeff, const T value) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			if (value >= 0) {
				return 1;
			}
			return coeff * std::exp(value);
		}
#ifdef _OPENMP
#pragma omp declare simd uniform(value, beta) simdlen(4)
#endif
		template <typename T> inline static constexpr T computeSwish(const T value, const T beta) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			return value * computeSigmoid<T>(beta * value);
		}
#ifdef _OPENMP
#pragma omp declare simd uniform(value, beta) simdlen(4)
#endif
		template <typename T> inline static constexpr T computeSwishDerivative(const T value, const T beta) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			const T fvalue = computeSwish(value, beta);
			const T sigValue = computeSigmoid(value);
			return fvalue + sigValue * (beta - fvalue);
		}

	  protected:
	};
} // namespace fragcore

#endif
