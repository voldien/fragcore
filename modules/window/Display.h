/*
 *	FragCore - Fragment Core
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
#ifndef _FRAGCORE_DISPLAY_H_
#define _FRAGCORE_DISPLAY_H_ 1
#include <Core/Object.h>
#include <FragDef.h>
#include <vector>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC Display : public Object {
	  public:
		enum class DisplayFormat : unsigned int {
			PIXELFORMAT_UNKNOWN,
			PIXELFORMAT_INDEX1LSB,
			PIXELFORMAT_INDEX1MSB,
			PIXELFORMAT_INDEX4LSB,
			PIXELFORMAT_INDEX4MSB,
			PIXELFORMAT_INDEX8,
			PIXELFORMAT_RGB332,
			PIXELFORMAT_RGB444,
			PIXELFORMAT_RGB555,
			PIXELFORMAT_BGR555,
			PIXELFORMAT_ARGB4444,
			PIXELFORMAT_RGBA4444,
			PIXELFORMAT_ABGR4444,
			PIXELFORMAT_BGRA4444,
			PIXELFORMAT_ARGB1555,
			PIXELFORMAT_RGBA5551,
			PIXELFORMAT_ABGR1555,
			PIXELFORMAT_BGRA5551,
			PIXELFORMAT_RGB565,
			PIXELFORMAT_BGR565,
			PIXELFORMAT_RGB24,
			PIXELFORMAT_BGR24,
			PIXELFORMAT_RGB888,
			PIXELFORMAT_RGBX8888,
			PIXELFORMAT_BGR888,
			PIXELFORMAT_BGRX8888,
			PIXELFORMAT_ARGB8888,
			PIXELFORMAT_RGBA8888,
			PIXELFORMAT_ABGR8888,
			PIXELFORMAT_BGRA8888,
			PIXELFORMAT_ARGB2101010,
			PIXELFORMAT_RGBA32,
			PIXELFORMAT_ARGB32,
			PIXELFORMAT_BGRA32,
			PIXELFORMAT_ABGR32,
			PIXELFORMAT_YV12,
			PIXELFORMAT_IYUV,
			PIXELFORMAT_YUY2,
			PIXELFORMAT_UYVY,
			PIXELFORMAT_YVYU,
			PIXELFORMAT_NV12,
			PIXELFORMAT_NV21
		};

		/**
		 * @brief Get display position
		 */
		virtual unsigned int x() const = 0;

		/**
		 * @brief
		 */
		virtual unsigned int y() const = 0;

		/**
		 * @brief
		 */
		virtual unsigned int width() const = 0;

		/**
		 * @brief
		 */
		virtual unsigned int height() const = 0;

		/**
		 * @brief
		 */
		virtual unsigned int refreshRate() const = 0;

		using Mode = struct mode_t {
			int x;
			int y;
			unsigned int width;
			unsigned int height;
			unsigned int refreshRate;
			DisplayFormat format;
		};

		using DPI = struct dpi_t {
			float ddpi;
			float hdpi;
			float vdpi;
		};

		/**
		 * @brief Look up Table for each
		 * pixels value should be remapped to.
		 *
		 */
		using LUT = struct lut_t {};

		/**
		 * @brief Get the Modes object
		 */
		virtual std::vector<Mode> getModes() const = 0;
		virtual void setMode(const Mode &mode) = 0;

		/**
		 * @brief
		 */
		virtual void getDPI(DPI *dpi) = 0;

		/**
		 * @brief
		 */
		virtual DisplayFormat getFormat() = 0;

		~Display() override = default;
	};

} // namespace fragcore

#endif
