/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _FRAG_CORE_MATH_H_
#define _FRAG_CORE_MATH_H_ 1
#include "../FragDef.h"
#include "Math3D.h"
#include "Random.h"
#include <cfloat>
#include <cmath>
#include <vector>

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC Math {
	  public:
		template <class T> inline constexpr static T clamp(T a, T min, T max) {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			return Math::max<T>(min, Math::min<T>(max, a));
		}

		/**
		 *	Get max value of a and b.
		 */
		template <typename T> inline constexpr static T max(T a, T b) {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			return (static_cast<T>(a) < static_cast<T>(b)) ? static_cast<T>(b) : static_cast<T>(a);
		}

		/**
		 *	Get min value of a and b.
		 */
		template <typename T> inline constexpr static T min(T a, T b) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			return (static_cast<T>(b) < static_cast<T>(a)) ? static_cast<T>(b) : static_cast<T>(a);
		}

		template <typename T> inline constexpr static T frac(T a) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			T part;
			std::modf(a, &part);
			return part;
		}

		template <typename T> constexpr static T sum(const std::vector<T> &list) noexcept {
			// TODO add check if support adding assign operation.
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Type Must Support addition operation.");
			T sum = 0;
			for (unsigned int i = 0; i < list.size(); i++)
				sum += list[i];
			return sum;
		}

		/**
		 *	Convert degree to radian.
		 */
		template <typename T> inline constexpr static T degToRad(T deg) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			return deg * static_cast<T>(Deg2Rad);
		}

		/**
		 *	Convert radian to degree.
		 */
		template <typename T> inline constexpr static T radToDeg(T deg) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			return deg * static_cast<T>(Rad2Deg);
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
		template <typename T> inline constexpr static T lerp(T a, T b, T t) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			return (a + (b - a) * t);
		}
		template <typename T> inline constexpr static T lerpClamped(T a, T b, T t) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			return (a + (b - a) * Math::clamp<T>(t, static_cast<T>(0.0), static_cast<T>(1.0)));
		}

		/**
		 *
		 */
		template <typename T> static T wrapAngle(T angle) {}

		/**
		 *
		 */
		// TODO add macro for inline.
		inline static constexpr double E = 2.718281828459045235;
		inline static constexpr double PI = 3.141592653589793238462643383279502884;
		inline static constexpr double PI_half = Math::PI / 2.0;
		inline static constexpr double PI_2 = Math::PI * 2.0;
		inline static constexpr double Epsilon = FLT_EPSILON;
		inline static constexpr double Deg2Rad = Math::PI / 180.0;
		inline static constexpr double Rad2Deg = 180 / Math::PI;
		inline static constexpr double NegativeInfinity = 0;

		template <typename T> static constexpr T NextPowerOfTwo(T v) {
			static_assert(std::is_integral<T>::value, "Must be a integer type.");
			T res = 1;
			while (res < v)
				res <<= 1;
			return res;
		}

		/**
		 *
		 */
		template <typename T> static constexpr T ClosestPowerOfTwo(T v) {
			T n = NextPowerOfTwo(v);
			T p = 0;
			return 0;
		}

		template <typename T> static constexpr bool IsPowerOfTwo(T v) {
			static_assert(std::is_integral<T>::value, "Must be a integer type.");
			return !(v == 0) && !((v - 1) & v);
		}

		/**
		 *	Generate 1D guassian.
		 */
		template <typename T>
		static void guassian(std::vector<T> &guassian, unsigned int height, T theta, T standard_deviation) {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			Math::guassian<T>(static_cast<T &>(*guassian.data()), height, theta, standard_deviation);
		}

		template <typename T>
		static void guassian(T &guassian, unsigned int height, T theta, T standard_deviation) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			const T a = (static_cast<T>(1.0) / (standard_deviation * static_cast<T>(std::sqrt(2.0 * Math::PI))));

			/*	*/
			T *pGuass = static_cast<T *>(&guassian);

			for (unsigned int i = 0; i < height; i++) {
				const T b = (-1.0f / 2.0f) * std::pow<T>(((i - standard_deviation) / theta), 2.0f);
				pGuass[i] = a * std::pow<T>(Math::E, b);
			}
		}

		/**
		 *	Generate 2D guassian.
		 */
		template <typename T>
		static void guassian(const std::vector<T> &guassian, unsigned int width, unsigned int height, T theta,
							 T standard_deviation) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			/*	TODO validate size.	*/

			Math::guassian<T>(static_cast<T &>(*guassian.data()), width, height, theta, standard_deviation);
		}

		template <typename T>
		static void guassian(const T &guassian, unsigned int width, unsigned int height, T theta,
							 T standard_deviation) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			for (unsigned int i = 0; i < height; i++) {
				// guassian(guassian[i * width],)
			}
		}

		template <typename T, typename U> static T gammaCorrection(T x, U gamma) noexcept {

			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			// TODO add support for using vector components.
			T exponent = static_cast<T>(1.0) / gamma;

			return static_cast<T>(std::pow(x, exponent));
		}

		template <typename T> static T gameSpaceToLinear(T gamma, T exp) noexcept {}

		/**
		 *	Generate perlin noise value
		 */
		static float PerlinNoise(float x, float y) noexcept;
		static float PerlinNoise(const Vector2 &point) noexcept;
		static float PerlinNoise(float x, float y, float z) noexcept;
		static float PerlinNoise(const Vector3 &point) noexcept;

		/**
		 * @brief
		 *
		 * @tparam T
		 * @return T
		 */
		template <typename T> T random() {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			return {drand48(), drand48()};
		}

		template <typename T> std::vector<T> &random(std::vector<T> &samples) {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			for (int i = 0; i < samples.size(); i++) {
				samples[i] = Random::rand<T>();
			}
			return samples;
		}

		template <typename T> std::vector<T> &jitter(std::vector<T> &samples) {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");

			int sqrt_samples = (int)(std::sqrt(samples.size()));
			for (int i = 0; i < sqrt_samples; i++) {
				for (int j = 0; j < sqrt_samples; j++) {
					T x = ((T)i + Random::rand<T>()) / (T)sqrt_samples;
					T y = ((T)j + Random::rand<T>()) / (T)sqrt_samples;

					samples[i * sqrt_samples + j] = x;
					samples[i * sqrt_samples + j] = y;
				}
			}
			return samples;
		}

		template <typename T> std::vector<T> &nrooks(std::vector<T> &samples) {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			int num_samples = samples.size();
			for (int i = 0; i < num_samples; i++) {
				samples[i].setX(((double)i + drand48()) / (double)num_samples);
				samples[i].setY(((double)i + drand48()) / (double)num_samples);
			}

			for (int i = num_samples - 2; i >= 0; i--) {
				int target = int(drand48() * (double)i);
				float temp = samples[i + 1].x();
				samples[i + 1].setX(samples[target].x());
				samples[target].setX(temp);
				continue;
			}
			return samples;
		}
		template <typename T> T phi(int j) {
			T x = 0.0;
			T f = 0.5;
			while (j) {
				// x += j /= f *=

				// 	f * (double)(!j & 1);
				// 2;
				// 0.5;
			}
			return (x);
		}

		template <typename T> std::vector<T> &hammersley(std::vector<T> &samples) {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			return {};
		}

		// void multijitter(Vector2 *samples, int num_samples) { int sqrt_samples = (int)sqrt(num_samples); }
	};
} // namespace fragcore

#endif
