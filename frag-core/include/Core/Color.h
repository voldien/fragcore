#ifndef _FRAG_CORE_COLOR_H_
#define _FRAG_CORE_COLOR_H_ 1
#include "Def.h"
#include "Math.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class Color : public Vector4 {
	  public:
		Color(float r, float g, float b, float a) noexcept : Vector4(r, g, b, a) {}
		Color(unsigned int hex) noexcept { /*	Convert */
			throw NotImplementedException();
		}

		inline float r(void) const noexcept { return x(); }
		inline float g(void) const noexcept { return y(); }
		inline float b(void) const noexcept { return z(); }
		inline float a(void) const noexcept { return w(); }

		inline void r(float red) noexcept { setX(red); }
		inline void g(float green) noexcept { setY(green); }
		inline void b(float blue) noexcept { setZ(blue); }
		inline void a(float alpha) noexcept { setW(alpha); }

	  public:
		/**
		 * @brief
		 *
		 * @param gamma
		 * @return Color
		 */
		Color gammaCorrect(float gamma) const noexcept {
			return Color(Math::gammaCorrection(r(), gamma), Math::gammaCorrection(g(), gamma),
						 Math::gammaCorrection(b(), gamma), Math::gammaCorrection(a(), gamma));
		}

		/**
		 * @brief
		 *
		 * @tparam T
		 * @param kelvin
		 * @return Color
		 */
		template <typename T> static Color CorrelatedColorTemperatureToRGB(T kelvin) noexcept {
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

			return Color(Math::clamp<T>(red, 0, 255) / 255.0f, Math::clamp<T>(green, 0, 255) / 255.0f,
						 Math::clamp<T>(blue, 0, 255) / 255.0f, 1);
		}

	  public:
		static Color clear(void) noexcept { return Color(0.0f, 0.0f, 0.0f, 0.0f); }	 // Clear Color {0,0,0,0}
		static Color black(void) noexcept { return Color(0.0f, 0.0f, 0.0f, 1.0f); }	 // Black Color {0,0,0,1}
		static Color white(void) noexcept { return Color(1.0f, 1.0f, 1.0f, 1.0f); }	 // White Color {1,1,1,constexpr
		static Color red(void) noexcept { return Color(1.0f, 0.0f, 0.0f, 1.0f); }	 // Red Color {1,0,0,1}
		static Color green(void) noexcept { return Color(0.0f, 1.0f, 0.0f, 1.0f); }	 // Green Color {0,1,0,1}
		static Color blue(void) noexcept { return Color(0.0f, 0.0f, 1.0f, 1.0f); }	 // Blue Color {0,0,0,constexpr
		static Color grey(void) noexcept { return Color(0.5f, 0.5f, 0.5f, 1.0f); }	 // Gray Color {0,0,0,1}
		static Color yellow(void) noexcept { return Color(1.0f, 1.0f, 0.0f, 1.0f); } // Yellow Color {0,0,0,1}
		static Color orange(void) noexcept { return Color(1.0f, 0.5f, 0.0f, 1.0f); } // Orange Color {0,0,0,1}
		static Color pink(void) noexcept { return Color(1.0f, 0.5f, 1.0f, 1.0f); }	 // Pink Color {1,0.5,1.0,1.0}
		static Color purple(void) noexcept { return Color(0.5f, 0.0f, 1.0f, 1.0f); } // Purple Color { 0.5, 0, 1, 1.0}
	};
} // namespace fragcore

#endif
