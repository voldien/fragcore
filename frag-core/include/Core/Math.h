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
		/**
		 *	Get float modular.
		 */
		inline static float modf(float a) {
			double part;
			return ::modf(a, &part);
		}
		inline static float modd(double a) {
			double part;
			return ::modf(a, &part);
		}
		inline static float modi(float a, float b) { return ((a) < (b)) ? (a) : (b); }

		template <typename T> constexpr static T sum(const std::vector<T> &list) {
			static_assert(T::operator+=, "Require");
			T sum = 0;
			for (int i = 0; i < list.size(); i++)
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

		template <typename T, typename U> inline constexpr static T lerpUnClamped(T a, T b, U t) {
			return a + (b - a) * t;
		}

		/**
		 *
		 */
		static float LerpAngle(float a, float b, float t);

		/**
		 *
		 */
		inline static constexpr double E = 2.718281828459045235;
		inline static constexpr double PI = 3.141592653589793238462643383279502884;
		inline static constexpr double PI_half = PI / 2.0;
		inline static constexpr double PI_2 = PI * 2.0;
		inline static constexpr float Epsilon = FLT_EPSILON;
		inline static constexpr float Deg2Rad = Math::PI / 180.0f;
		inline static constexpr float Rad2Deg = 180 / Math::PI;
		inline static constexpr float NegativeInfinity = 0;

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
		static Color CorrelatedColorTemperatureToRGB(float kelvin);

		/**
		 *	Generate 1D guassian.
		 */
		static void guassian1Df(float *guassian, int width, double theta);
		static void guassian1Df(std::vector<float> &guassian, int height, float theta);

		/**
		 *	Generate 2D guassian.
		 */
		static void guassian2Df(float *guassian, int height, float theta);
		static void guassian2Df(std::vector<float> &guassian, int height, float theta);

		/**
		 *
		 */
		static float linearToGammaSpace(float linear);
		static float GameSpaceToLinear(float gamma, float exp);
		static float gammaCorrection(float linear, float exp);

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
