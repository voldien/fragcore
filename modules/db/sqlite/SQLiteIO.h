#ifndef _KF_SQLITEIO_H_
#define _KF_SQLITEIO_H_ 1
#include <FragCore.h>
#include <sqlite3.h>

namespace fragcore {

	extern "C" FVDECLSPEC int fragcore_sqlite3_open(Ref<IO> &io, sqlite3 **db);

	extern "C" FVDECLSPEC int sqlite3_iovfs_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);
	extern "C" FVDECLSPEC void getIOVFS(sqlite3_vfs *vfs);
} // namespace fragcore

#endif