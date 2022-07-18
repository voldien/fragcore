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
#ifndef _FRAG_CORE_HASH_H_
#define _FRAG_CORE_HASH_H_ 1
#include "../FragDef.h"
#include "IO/IO.h"
#include "Object.h"
#include "Ref.h"
#include <vector>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC Hash : public Object {
	  public:
		enum class ALGORITHM {
			HashNone = 0,
			MD4,	/*  */
			MD5,	/*  */
			SHA128, /*  */
			SHA224, /*  */
			SHA256, /*  */
			SHA512, /*  */
		};
		Hash(ALGORITHM algorithm);
		Hash(Hash &&other);

		~Hash();

		/**
		 * @brief
		 *
		 * @param pdata
		 * @param nbytes
		 */
		void update(const void *pdata, size_t nbytes);

		/**
		 * @brief
		 *
		 * @param io
		 */
		void update(Ref<IO> &io);

		/**
		 * @brief Construct the final hash value.
		 *
		 * @param hash
		 */
		void final(std::vector<unsigned char> &hash);

		/**
		 * @brief
		 *
		 */
		void reset() noexcept;

		/**
		 * @brief Get the Result Size object
		 *
		 * @return unsigned int
		 */
		unsigned int getResultSize() const;

		/**
		 * @brief Get the Byte Read object
		 *
		 * @return unsigned long int
		 */
		size_t getByteRead() const;

		ALGORITHM getAlgorithm() const noexcept;

	  protected:
		void initHash(ALGORITHM algorithm);

		void getHashResult(const void *pData, const size_t nrBytes);

	  private:
		void *context;
		ALGORITHM algorithm; /**/
		size_t nbytes;

		Hash(const Hash &other) = delete;
		Hash() = delete;
	};

} // namespace fragcore

#endif
