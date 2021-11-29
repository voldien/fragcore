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
#ifndef _FRAG_CORE_PACKAGE_H_
#define _FRAG_CORE_PACKAGE_H_ 1
#include"../IO/IO.h"
namespace fragcore {

	class Package {
	  public:
		Package(Ref<IO>& ref);

		bool isEndOfStream() const;

		// Overload of operator >> to read data from the data stream
		Package &operator>>(bool &data);
		Package &operator>>(int8_t &data);
		Package &operator>>(uint8_t &data);
		Package &operator>>(int16_t &data);
		Package &operator>>(uint16_t &data);
		Package &operator>>(int32_t &data);
		Package &operator>>(uint32_t &data);
		Package &operator>>(int64_t &data);
		Package &operator>>(uint64_t &data);
		Package &operator>>(float &data);
		Package &operator>>(double &data);
		Package &operator>>(char *data);
		Package &operator>>(std::string &data);
		Package &operator>>(wchar_t *data);
		Package &operator>>(std::wstring &data);

		// Overload of operator << to write data into the data stream
		Package &operator<<(bool data);
		Package &operator<<(int8_t data);
		Package &operator<<(uint8_t data);
		Package &operator<<(int16_t data);
		Package &operator<<(uint16_t data);
		Package &operator<<(int32_t data);
		Package &operator<<(uint32_t data);
		Package &operator<<(int64_t data);
		Package &operator<<(uint64_t data);
		Package &operator<<(float data);
		Package &operator<<(double data);
		Package &operator<<(const char *data);
		Package &operator<<(const std::string &data);
		Package &operator<<(const wchar_t *data);
		Package &operator<<(const std::wstring &data);

	  private:
		std::size_t readPos = 0;
		std::size_t sendPos = 0;
	};
} // namespace fragcore

#endif
