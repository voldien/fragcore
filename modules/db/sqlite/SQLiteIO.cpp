#include "SQLiteIO.h"
#include <IO/IO.h>
#include <cstring>
#include <sqlite3.h>
// #include <sqlite3ext.h>
//  SQLITE_EXTENSION_INIT1
#include <cassert>

namespace fragcore {

	const char *fragcore_sqlite_vfs_name = "iovfs";

	int fragcore_sqlite3_open(Ref<IO> &io, sqlite3 **db) {
		int result_code = 0;
		sqlite3_vfs vfs;
		fragcore::getIOVFS(&vfs);

		/*  */
		result_code = sqlite3_vfs_register(&vfs, 0);
		if (result_code != SQLITE_OK) {
			throw RuntimeException("Failed to register sqlite3 VFS: {}", sqlite3_errstr(result_code));
		}

		/*	*/
		const sqlite3_vfs *iovfsFound = sqlite3_vfs_find(fragcore_sqlite_vfs_name);
		if (!iovfsFound) {
			throw RuntimeException("Failed to find previously registered VFS: {}", sqlite3_errstr(result_code));
		}

		/*	*/
		char bufURI[1024];
		// vfs.pAppData.
		sprintf(bufURI, "ptr=%llu", (sqlite3_uint64)((void *)&io));
		result_code = sqlite3_open_v2(bufURI, db,
									  SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_MAIN_DB |
										  SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_URI,
									  fragcore_sqlite_vfs_name);

		if (result_code != SQLITE_OK) {
			throw RuntimeException("Failed to open sqlite3: {}", sqlite3_errstr(result_code));
		}

		return SQLITE_OK;
	}

	/*
	** Forward declaration of objects used by this utility
	*/
	using MemVfs = struct sqlite3_vfs;
	using MemFile = struct IOFile;

/* Access to a lower-level VFS that (might) implement dynamic loading,
** access to randomness, etc.
*/
#define ORIGVFS(p) ((sqlite3_vfs *)((p)->pAppData))

	/* An open file */
	struct IOFile {
		sqlite3_file base;	 // Base class.  Must be first
		sqlite3_file *pReal; // The real underlying file
		Ref<IO> ref;
	};

	/*
	** Methods for IOFile
	*/
	static int memClose(sqlite3_file * /*pFile*/);

	static int memRead(sqlite3_file * /*pFile*/, void * /*zBuf*/, int iAmt, sqlite3_int64 iOfst);

	static int memWrite(sqlite3_file * /*pFile*/, const void * /*z*/, int iAmt, sqlite3_int64 iOfst);

	static int memTruncate(sqlite3_file * /*pFile*/, sqlite3_int64 size);

	static int memSync(sqlite3_file * /*pFile*/, int flags);

	static int memFileSize(sqlite3_file * /*pFile*/, sqlite3_int64 *pSize);

	static int memLock(sqlite3_file * /*pFile*/, int /*eLock*/);

	static int memUnlock(sqlite3_file * /*pFile*/, int /*eLock*/);

	static int memCheckReservedLock(sqlite3_file * /*pFile*/, int *pResOut);

	static int memFileControl(sqlite3_file * /*pFile*/, int op, void *pArg);

	static int memSectorSize(sqlite3_file * /*pFile*/);

	static int memDeviceCharacteristics(sqlite3_file * /*pFile*/);

	/*
	** Methods for MemVfs
	*/
	static int memOpen(sqlite3_vfs * /*pVfs*/, const char * /*zName*/, sqlite3_file * /*pFile*/, int /*flags*/,
					   int * /*pOutFlags*/);

	static int memDelete(sqlite3_vfs * /*pVfs*/, const char *zName, int syncDir);

	static int memAccess(sqlite3_vfs * /*pVfs*/, const char *zName, int flags, int * /*pResOut*/);

	static int memFullPathname(sqlite3_vfs * /*pVfs*/, const char *zPath, int /*nOut*/, char *zOut);

	static void *memDlOpen(sqlite3_vfs * /*pVfs*/, const char *zPath);

	static void memDlError(sqlite3_vfs * /*pVfs*/, int nByte, char *zErrMsg);

	static void (*memDlSym(sqlite3_vfs *pVfs, void *p, const char *zSym))(void);

	static void memDlClose(sqlite3_vfs * /*pVfs*/, void * /*pHandle*/);

	static int memRandomness(sqlite3_vfs * /*pVfs*/, int nByte, char *zBufOut);

	static int memSleep(sqlite3_vfs * /*pVfs*/, int nMicro);

	static int memCurrentTime(sqlite3_vfs * /*pVfs*/, double * /*pTimeOut*/);

	static int memGetLastError(sqlite3_vfs * /*pVfs*/, int /*a*/, char * /*b*/);

	static int memCurrentTimeInt64(sqlite3_vfs * /*pVfs*/, sqlite3_int64 * /*p*/);

	static sqlite3_vfs mem_vfs = {
		1,						  /* iVersion */
		0,						  /* szOsFile (set when registered) */
		1024,					  /* mxPathname */
		nullptr,				  /* pNext */
		fragcore_sqlite_vfs_name, /* zName */
		nullptr,				  /* pAppData (set when registered) */
		memOpen,				  /* xOpen */
		memDelete,				  /* xDelete */
		memAccess,				  /* xAccess */
		memFullPathname,		  /* xFullPathname */
		memDlOpen,				  /* xDlOpen */
		memDlError,				  /* xDlError */
		memDlSym,				  /* xDlSym */
		memDlClose,				  /* xDlClose */
		memRandomness,			  /* xRandomness */
		memSleep,				  /* xSleep */
		memCurrentTime,			  /* xCurrentTime */
		memGetLastError,		  /* xGetLastError */
		memCurrentTimeInt64		  /* xCurrentTimeInt64 */
	};

	static const sqlite3_io_methods mem_io_methods = {
		1,						  /* iVersion */
		memClose,				  /* xClose */
		memRead,				  /* xRead */
		memWrite,				  /* xWrite */
		memTruncate,			  /* xTruncate */
		memSync,				  /* xSync */
		memFileSize,			  /* xFileSize */
		memLock,				  /* xLock */
		memUnlock,				  /* xUnlock */
		memCheckReservedLock,	  /* xCheckReservedLock */
		memFileControl,			  /* xFileControl */
		memSectorSize,			  /* xSectorSize */
		memDeviceCharacteristics, /* xDeviceCharacteristics */
								  // memShmMap,				  /* xShmMap */
								  // memShmLock,				  /* xShmLock */
								  // memShmBarrier,			  /* xShmBarrier */
								  // memShmUnmap,			  /* xShmUnmap */
								  // memFetch,				  /* xFetch */
								  // memUnfetch				  /* xUnfetch */
	};

	static int memClose(sqlite3_file *pFile) {
		IOFile *p = reinterpret_cast<IOFile *>(pFile);
		p->ref->close();
		return SQLITE_OK;
	}

	static int memRead(sqlite3_file *pFile, void *zBuf, int iAmt, sqlite_int64 iOfst) {
		/*	*/
		IOFile *p = reinterpret_cast<IOFile *>(pFile);
		/*	*/
		Ref<IO> *pStream = reinterpret_cast<Ref<IO> *>(&p->ref);

		/*	*/
		(*pStream)->seek(iOfst, IO::SET);
		long int nBytes = (*pStream)->read(iAmt, zBuf);
		if (nBytes == iAmt) {
			return SQLITE_OK;
		}
		return SQLITE_IOERR_WRITE;
	}

	/*
	** Write data to an mem-file.
	*/
	static int memWrite(sqlite3_file *pFile, const void *z, int iAmt, sqlite_int64 iOfst) {
		IOFile *p = (IOFile *)pFile;

		p->ref->seek(iOfst, IO::SET);
		p->ref->write(iAmt, z);
		//	if( iOfst+iAmt>p->sz ){
		//		if( iOfst+iAmt>p->szMax ) return SQLITE_FULL;
		//		if( iOfst>p->sz ) memset(p->aData+p->sz, 0, iOfst-p->sz);
		//		p->sz = iOfst+iAmt;
		//	}
		//	memcpy(p->aData+iOfst, z, iAmt);
		return SQLITE_OK;
	}

	/*
	** Truncate an mem-file.
	*/
	static int memTruncate(sqlite3_file *pFile, sqlite_int64 size) {
		IOFile *p = (IOFile *)pFile;
		// if (size > p->sz) {
		// if( size>p->szMax ) return SQLITE_FULL;
		// memset(p->aData+p->sz, 0, size-p->sz);
		//}
		// p->sz = size;
		return SQLITE_OK;
	}

	/*
	** Sync an mem-file.
	*/
	static int memSync(sqlite3_file *pFile, int flags) {
		IOFile *p = reinterpret_cast<IOFile *>(pFile);
		if (p->ref->flush()) {
			return SQLITE_OK;
		}
		return SQLITE_FAIL;
	}

	/*
	** Return the current file-size of an mem-file.
	*/
	static int memFileSize(sqlite3_file *pFile, sqlite_int64 *pSize) {
		IOFile *p = (IOFile *)pFile;
		*pSize = p->ref->length();
		return SQLITE_OK;
	}

	/*
	** Lock an mem-file.
	*/
	static int memLock(sqlite3_file *pFile, int eLock) { return SQLITE_OK; }

	/*
	** Unlock an mem-file.
	*/
	static int memUnlock(sqlite3_file *pFile, int eLock) { return SQLITE_OK; }

	/*
	** Check if another file-handle holds a RESERVED lock on an mem-file.
	*/
	static int memCheckReservedLock(sqlite3_file *pFile, int *pResOut) {
		*pResOut = 0;
		return SQLITE_OK;
	}

	/*
	** File control method. For custom operations on an mem-file.
	*/
	static int memFileControl(sqlite3_file *pFile, int op, void *pArg) {
		IOFile *p = reinterpret_cast<IOFile *>(pFile);

		int result_code = SQLITE_NOTFOUND;
		if (op == SQLITE_FCNTL_VFSNAME) {
			//	*(char **) pArg = sqlite3_mprintf("mem(%p,%lld)", p->aData, p->sz);
			result_code = SQLITE_OK;
		}
		return result_code;
	}

	/*
	** Return the sector-size in bytes for an mem-file.
	*/
	static int memSectorSize(sqlite3_file *pFile) { return 1024; }

	/*
	** Return the device characteristic flags supported by an mem-file.
	*/
	static int memDeviceCharacteristics(sqlite3_file *pFile) {
		return SQLITE_IOCAP_ATOMIC | SQLITE_IOCAP_POWERSAFE_OVERWRITE | SQLITE_IOCAP_SAFE_APPEND |
			   SQLITE_IOCAP_SEQUENTIAL | SQLITE_IOCAP_IMMUTABLE;
	}

	/*
	** Open an mem file handle.
	*/
	static int memOpen(sqlite3_vfs *pVfs, const char *zName, sqlite3_file *pFile, int flags, int *pOutFlags) {

		/*	*/
		IOFile *p = (IOFile *)pFile;

		// cevfs_info *pInfo = (cevfs_info *)pVfs->pAppData;
		// sqlite3_vfs *pRoot = pInfo->pRootVfs;

		if ((flags & SQLITE_OPEN_MAIN_DB) == 0) {
			return SQLITE_CANTOPEN;
		}

		pVfs->pAppData = sqlite3_vfs_find(nullptr);

		sqlite3_int64 refV = sqlite3_uri_int64(zName, "ptr", 0);
		Ref<IO> *ref = reinterpret_cast<Ref<IO> *>(refV);

		if (ref == nullptr) {
			return SQLITE_CANTOPEN;
		}

		if (flags & SQLITE_READONLY) {
			(*ref)->isWriteable();
			(*ref)->isReadable();
		}

		if (flags & SQLITE_OPEN_READWRITE) {
			bool readable = (*ref)->isReadable();
			bool writeable = (*ref)->isWriteable();

			if (!(writeable && readable)) {
				return SQLITE_PERM;
			}
		}

		p->ref = *ref; // Ref<IO>(ref->ptr());
		p->base.pMethods = &mem_io_methods;

		return SQLITE_OK;
	}

	/*
	** Delete the file located at zPath. If the dirSync argument is true,
	** ensure the file-system modifications are synced to disk before
	** returning.
	*/
	static int memDelete(sqlite3_vfs *pVfs, const char *zPath, int dirSync) { return SQLITE_IOERR_DELETE; }

	/*
	** Test for access permissions. Return true if the requested permission
	** is available, or false otherwise.
	*/
	static int memAccess(sqlite3_vfs *pVfs, const char *zPath, int flags, int *pResOut) {
		*pResOut = 0;
		return SQLITE_OK;
	}

	/*
	** Populate buffer zOut with the full canonical pathname corresponding
	** to the pathname in zPath. zOut is guaranteed to point to a buffer
	** of at least (INST_MAX_PATHNAME+1) bytes.
	*/
	static int memFullPathname(sqlite3_vfs *pVfs, const char *zPath, int nOut, char *zOut) {
		sqlite3_snprintf(nOut, zOut, "%s", zPath);
		return SQLITE_OK;
	}

	/*
	** Open the dynamic library located at zPath and return a handle.
	*/
	static void *memDlOpen(sqlite3_vfs *pVfs, const char *zPath) {
		return ORIGVFS(pVfs)->xDlOpen(ORIGVFS(pVfs), zPath);
	}

	/*
	** Populate the buffer zErrMsg (size nByte bytes) with a human readable
	** utf-8 string describing the most recent error encountered associated
	** with dynamic libraries.
	*/
	static void memDlError(sqlite3_vfs *pVfs, int nByte, char *zErrMsg) {
		ORIGVFS(pVfs)->xDlError(ORIGVFS(pVfs), nByte, zErrMsg);
	}

	/*
	** Return a pointer to the symbol zSymbol in the dynamic library pHandle.
	*/
	static void (*memDlSym(sqlite3_vfs *pVfs, void *p, const char *zSym))(void) {
		return ORIGVFS(pVfs)->xDlSym(ORIGVFS(pVfs), p, zSym);
	}

	/*
	** Close the dynamic library handle pHandle.
	*/
	static void memDlClose(sqlite3_vfs *pVfs, void *pHandle) { ORIGVFS(pVfs)->xDlClose(ORIGVFS(pVfs), pHandle); }

	/*
	** Populate the buffer pointed to by zBufOut with nByte bytes of
	** random data.
	*/
	static int memRandomness(sqlite3_vfs *pVfs, int nByte, char *zBufOut) {
		return ORIGVFS(pVfs)->xRandomness(ORIGVFS(pVfs), nByte, zBufOut);
	}

	/*
	** Sleep for nMicro microseconds. Return the number of microseconds
	** actually slept.
	*/
	static int memSleep(sqlite3_vfs *pVfs, int nMicro) { return ORIGVFS(pVfs)->xSleep(ORIGVFS(pVfs), nMicro); }

	/*
	** Return the current time as a Julian Day number in *pTimeOut.
	*/
	static int memCurrentTime(sqlite3_vfs *pVfs, double *pTimeOut) {
		return ORIGVFS(pVfs)->xCurrentTime(ORIGVFS(pVfs), pTimeOut);
	}

	static int memGetLastError(sqlite3_vfs *pVfs, int a, char *b) {
		return ORIGVFS(pVfs)->xGetLastError(ORIGVFS(pVfs), a, b);
	}

	static int memCurrentTimeInt64(sqlite3_vfs *pVfs, sqlite3_int64 *p) {
		return ORIGVFS(pVfs)->xCurrentTimeInt64(ORIGVFS(pVfs), p);
	}

	void getIOVFS(sqlite3_vfs *vfs) {
		memcpy(vfs, &mem_vfs, sizeof(sqlite3_vfs));

		/*	*/
		mem_vfs.szOsFile = sizeof(IOFile);
		mem_vfs.pAppData = sqlite3_vfs_find(nullptr);
		mem_vfs.zName = fragcore_sqlite_vfs_name;
	}
} // namespace fragcore