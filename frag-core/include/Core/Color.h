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
		Color() = default;
		Color(float r, float g, float b, float a) noexcept : Vector4(r, g, b, a) {}
		Color(uint32_t hex) noexcept {				 /*	Convert */
			float r = ((hex >> 24) & 0xFF) / 255.0f; // Extract the RR byte
			float g = ((hex >> 16) & 0xFF) / 255.0f; // Extract the GG byte
			float b = ((hex >> 8) & 0xFF) / 255.0f;	 // Extract the GG byte
			float a = ((hex)&0xFF) / 255.0f;		 // Extract the BB byte
			this->r(r);
			this->g(g);
			this->b(b);
			this->a(a);
		}

		inline float r() const noexcept { return x(); }
		inline float g() const noexcept { return y(); }
		inline float b() const noexcept { return z(); }
		inline float a() const noexcept { return w(); }

		// TODO resolve
		inline void r(float red) noexcept { /*this->[0] = red; */
		}
		inline void g(float green) noexcept { /* this->[1] = green;*/
		}
		inline void b(float blue) noexcept { /* this->[2] = blue; */
		}
		inline void a(float alpha) noexcept { /* this->[3] = alpha;*/
		}

	  public:
		/**
		 * @brief
		 *
		 * @param gamma
		 * @return Color
		 */
		template <typename T> Color gammaCorrect(T gamma) const noexcept {
			return Color(Math::gammaCorrection(this->r(), static_cast<T>(gamma)),
						 Math::gammaCorrection(this->g(), static_cast<T>(gamma)),
						 Math::gammaCorrection(this->b(), static_cast<T>(gamma)),
						 Math::gammaCorrection(this->a(), static_cast<T>(gamma)));
		}

		/**
		 * @brief
		 *
		 * @tparam T
		 * @param kelvin
		 * @return Color
		 */
		template <typename T> static Color CorrelatedColorTemperatureToRGB(T kelvin) noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
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
		static Color clear() noexcept { return Color(0.0f, 0.0f, 0.0f, 0.0f); }	 // Clear Color {0,0,0,0}
		static Color black() noexcept { return Color(0.0f, 0.0f, 0.0f, 1.0f); }	 // Black Color {0,0,0,1}
		static Color white() noexcept { return Color(1.0f, 1.0f, 1.0f, 1.0f); }	 // White Color {1,1,1,constexpr
		static Color red() noexcept { return Color(1.0f, 0.0f, 0.0f, 1.0f); }	 // Red Color {1,0,0,1}
		static Color green() noexcept { return Color(0.0f, 1.0f, 0.0f, 1.0f); }	 // Green Color {0,1,0,1}
		static Color blue() noexcept { return Color(0.0f, 0.0f, 1.0f, 1.0f); }	 // Blue Color {0,0,0,constexpr
		static Color grey() noexcept { return Color(0.5f, 0.5f, 0.5f, 1.0f); }	 // Gray Color {0,0,0,1}
		static Color yellow() noexcept { return Color(1.0f, 1.0f, 0.0f, 1.0f); } // Yellow Color {0,0,0,1}
		static Color orange() noexcept { return Color(1.0f, 0.5f, 0.0f, 1.0f); } // Orange Color {0,0,0,1}
		static Color pink() noexcept { return Color(1.0f, 0.5f, 1.0f, 1.0f); }	 // Pink Color {1,0.5,1.0,1.0}
		static Color purple() noexcept { return Color(0.5f, 0.0f, 1.0f, 1.0f); } // Purple Color { 0.5, 0, 1, 1.0}
	};
} // namespace fragcore

#endif
