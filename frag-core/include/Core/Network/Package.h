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