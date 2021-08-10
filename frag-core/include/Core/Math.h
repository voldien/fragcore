/**
	FragEngine, A Two layer Game Engine.
	Copyright (C) 2018  Valdemar Lindberg

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _FRAG_CORE_MATH_H_
#define _FRAG_CORE_MATH_H_ 1
#include "../Def.h"
#include <float.h>
#include <math.h>
#include <vector>

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC Math {
	  public:
		template <class T> inline constexpr static T clamp(T a, T min, T max) {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must be a decimal type(float/double/half) or integar.");
			return Math::max<T>(min, Math::min<T>(max, a));
		}

		/**
		 *	Get max value of a and b.
		 */
		template <typename T> inline constexpr static T max(T a, T b) {
			return (static_cast<T>(a) < static_cast<T>(b)) ? static_cast<T>(b) : static_cast<T>(a);
		}

		/**
		 *	Get min value of a and b.
		 */
		template <typename T> inline constexpr static T min(T a, T b) noexcept {
			return (static_cast<T>(b) < static_cast<T>(a)) ? static_cast<T>(b) : static_cast<T>(a);
		}

		template <typename T> inline constexpr static T frac(T a) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			T part;
			std::modf(a, &part);
			return part;
		}

		template <typename T> constexpr static T sum(const std::vector<T> &list) noexcept {
			static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
						  "Must Suppot additiona operation.");
			T sum = 0;
			for (unsigned int i = 0; i < list.size(); i++)
				sum += list[i];
			return sum;
		}

		/**
		 *	Convert degree to radian.
		 */
		template <typename T> inline constexpr static T deg2Rad(T deg) noexcept {
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
		 * Linear interpolation.
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
		inline static constexpr double E = 2.718281828459045235;
		inline static constexpr double PI = 3.141592653589793238462643383279502884;
		inline static constexpr double PI_half = PI / 2.0;
		inline static constexpr double PI_2 = PI * 2.0;
		inline static constexpr double Epsilon = FLT_EPSILON;
		inline static constexpr double Deg2Rad = Math::PI / 180.0f;
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

		/**
		 *
		 */
		template <typename T> static constexpr bool IsPowerOfTwo(T v) {
			static_assert(std::is_integral<T>::value, "Must be a integer type.");
			return (v && ((v - 1) & v));
		}

		template <typename T> static Color CorrelatedColorTemperatureToRGB(T kelvin) {
			T temp = kelvin / static_cast<T>(100.0);

			T red, green, blue;

			if (temp <= 66) {

				red = 255;

				green = temp;
				green = 99.4708025861 * log(green) - 161.1195681661;

				if (temp <= 19) {

					blue = 0;
				} else {

					blue = temp - 10;
					blue = 138.5177312231 * log(blue) - 305.0447927307;
				}
			} else {

				red = temp - 60;
				red = 329.698727446 * pow(red, -0.1332047592);

				green = temp - 60;
				green = 288.1221695283 * pow(green, -0.0755148492);

				blue = 255;
			}

			return Color(clamp<T>(red, 0, 255) / 255.0f, clamp<T>(green, 0, 255) / 255.0f,
						 clamp<T>(blue, 0, 255) / 255.0f, 1);
		}

		/**
		 *	Generate 1D guassian.
		 */
		template <typename T>
		static void guassian(std::vector<T> &guassian, unsigned int height, T theta, T standard_deviation) {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			const T a = (1.0f / (standard_deviation * static_cast<T>(std::sqrt(2.0f * Math::PI))));

			for (unsigned int i = 0; i < height; i++) {
				const T b = (-1.0f / 2.0f) * std::pow<T>(((i - standard_deviation) / theta), 2.0f);
				guassian[i] = a * std::pow<T>(Math::E, b);
			}
		}
		template <typename T>
		static void guassian(T &guassian, unsigned int height, T theta, T standard_deviation) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
		}

		/**
		 *	Generate 2D guassian.
		 */
		template <typename T>
		static void guassian(std::vector<T> &guassian, unsigned int width, unsigned int height, T theta,
							 T standard_deviation) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			for (unsigned int i = 0; i < height; i++) {
				// guassian(guassian[i * width],)
			}
		}

		template <typename T>
		static void guassian(T &guassian, unsigned int width, unsigned int height, T theta,
							 T standard_deviation) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
		}

		template <typename T> static T gammaCorrection(T x, T gamma) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			return static_cast<T>(std::pow(x, gamma));
		}

		template <typename T> static T gameSpaceToLinear(T gamma, T exp) noexcept {}

		/**
		 *	Generate perlin noise value
		 */
		static float PerlinNoise(float x, float y) noexcept;
		static float PerlinNoise(const Vector2 &point) noexcept;
		static float PerlinNoise(float x, float y, float z) noexcept;
		static float PerlinNoise(const Vector3 &point) noexcept;
	};
} // namespace fragcore

#endif
