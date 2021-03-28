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
#ifndef _FRAG_CORE_BZ_FILEIO_H_
#define _FRAG_CORE_BZ_FILEIO_H_ 1
#include "FileIO.h"
#include "IO.h"
#include <bzlib.h>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC BZFileIO : public FileIO {
	  public:
		long read(long int nbytes, void *pbuffer) override;

		long write(long int nbytes, const void *pbuffer) override;

		long length(void) override;

		bool eof(void) const override;

		void seek(long int nbytes, Seek seek) override;

		unsigned long getPos(void) override;

		bool isWriteable(void) const override;

		bool isReadable(void) const override;

		bool flush(void) override;

		void close(void) override;

		void open(const char *path, Mode mode) override;

	  protected:
		BZFILE *bzFile;

	  public:
		BZFileIO(const char *path, Mode mode);
	};
} // namespace fragcore

#endif
