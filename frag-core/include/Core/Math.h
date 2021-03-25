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
#include"../Def.h"
#include<math.h>
#include<vector>
#include<float.h>

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC Math {
	public:

		template<class T>
		inline constexpr static T clamp(T a, T min, T max){ return Math::max<T>( min , Math::min<T>( max , a ) ); }
		inline constexpr static int clamp(int a, int min, int max){ return Math::clamp<int>(a, min ,max);}
		inline constexpr static float clampf(float a, float min, float max){ return Math::clamp<float>(a, min ,max); }
		inline constexpr static double clampd(double a, double min, double max){ return Math::clamp<double>(a, min ,max); }

		/**
		 *	Get max value of a and b.
		 */
		template<typename T>
		inline constexpr static T max(T a, T b){ return ( (a) < (b) ) ? (b) : (a);}
		inline constexpr static float maxf(float a, float b){ return ( (a) < (b) ) ? (b) : (a);}
		inline constexpr static float maxd(float a, float b){ return ( (a) < (b) ) ? (b) : (a);}
		inline constexpr static float maxi(float a, float b){ return ( (a) < (b) ) ? (b) : (a);}

		/**
		 *	Get min value of a and b.
		 */
		template<typename T>
		inline constexpr static T min(T a, T b){ return ( (a) > (b) ) ? (b) : (a);}
		inline constexpr static float minf(float a, float b) { return ((a) > (b)) ? (b) : (a); }
		inline constexpr static float mind(float a, float b) { return ((a) > (b)) ? (b) : (a); }
		inline constexpr static float mini(float a, float b) { return ((a) > (b)) ? (b) : (a); }

		/**
		 *	Get float modular.
		 */
		inline static float modf(float a){double part; return ::modf(a, &part);}
		inline static float modd(double a){ double part; return ::modf(a, &part);}
		inline static float modi(float a, float b) { return ((a) < (b)) ? (a) : (b); }

		template<typename T>
		constexpr static T sum( const std::vector<T>& list){
			T sum = 0;
			for (int i = 0; i < list.size(); i++)
				sum += list[i];
			return sum;
		}

		/**
		 *	Convert degree to radian.
		 */
		template<class T>
		inline constexpr static T deg2Rad(T deg) {
			return (deg * (T)Math::PI) / 180.0f;
		}

		/**
		 *	Convert radian to degree.
		 */
		template<class T>
		inline constexpr static T radToDeg(T deg) {
			return (deg * (T)180) / (T)Math::PI;
		}

		/**
		 * Linear interpolation.
		 */
		template <typename T, typename U>
		inline constexpr static T lerp(T a, T b, U t)
		{
			return a + (b - a) * Math::clamp(t, (U)0.0, (U)1.0);
		}

		template<typename T, typename U>
		inline constexpr static T lerpUnClamped(T a, T b, U t){
			return a + (b - a) * t;
		}

		/**
		 *
		 */
		static float LerpAngle(float a, float b, float t);

		/**
		 *
		 */
		inline static constexpr float Epsilon = FLT_EPSILON;
		inline static constexpr float PI = 3.14159265358979323846;
		inline static constexpr float Infinite = 0;
		inline static constexpr float Deg2Rad = Math::PI / 180.0f;
		inline static constexpr float Rad2Deg = 180 / Math::PI;
		inline static constexpr float NegativeInfinity = 0;

		/**
		 *
		 */
		template <typename T>
		static constexpr T NextPowerOfTwo(T v) {
			T res = 1;
			while (res < v)res <<= 1;
			return res;
		}

		/**
		 *
		 */
		template <typename T>
		static constexpr T ClosestPowerOfTwo(T v)
		{
			T n = NextPowerOfTwo(v);
			T p = 0;
			return 0;
		}

		/**
		 *
		 */
		template<typename T>
		static constexpr bool IsPowerOfTwo(T v){
			return  (v && ((v - 1) & v));
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
		static void guassian2Df(float* guassian, int height, float theta);
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
		static float PerlinNoise(float x, float y);
		static float PerlinNoise(const Vector2 &point);
		static float PerlinNoise(float x, float y, float z);
		static float PerlinNoise(const Vector3& point);
	};
}

#endif
