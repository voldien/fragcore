#include <IO/FileSystem.h>
#include <FragCore.h>
#include <SQLiteIO.h>

using namespace fragcore;

int main(int argc, const char **argv) {

	printf("%s\n", sqlite3_libversion());
	IFileSystem *filesystem = FileSystem::createFileSystem();

	Ref<IO> io = Ref<IO>(filesystem->openFile("example.db", IO::IOMode::ACCESS));

	sqlite3 *db;
	fragcore_sqlite3_open(io, &db);

	char *errMs;
	// int rc = sqlite3_db_config(db, SQLITE_CONFIG_SINGLETHREAD, nullptr);
	// if (rc != SQLITE_OK)
	// 	throw RuntimeException("Failed to configure to single threaded: {}", sqlite3_errstr(rc));
	int rc = sqlite3_exec(db, "PRAGMA synchronous=OFF", NULL, NULL, &errMs);
	if (rc != SQLITE_OK) {
		throw RuntimeException("Failed to open sqlite3: {}", sqlite3_errstr(rc));
	}
	rc = sqlite3_exec(db, "PRAGMA count_changes=OFF", NULL, NULL, &errMs);
	if (rc != SQLITE_OK) {
		throw RuntimeException("Failed to open sqlite3: {}", sqlite3_errstr(rc));
	}
	rc = sqlite3_exec(db, "PRAGMA journal_mode=MEMORY", NULL, NULL, &errMs);
	if (rc != SQLITE_OK) {
		throw RuntimeException("Failed to open sqlite3: {}", sqlite3_errstr(rc));
	}
	rc = sqlite3_exec(db, "PRAGMA temp_store=MEMORY", NULL, NULL, &errMs);
	if (rc != SQLITE_OK) {
		throw RuntimeException("Failed to open sqlite3: {}", sqlite3_errstr(rc));
	}
	rc = sqlite3_exec(db, "PRAGMA cache_size=40000", NULL, NULL, &errMs);
	if (rc != SQLITE_OK) {
		throw RuntimeException("Failed to open sqlite3: {}", sqlite3_errstr(rc));
	}

	sqlite3_stmt *res1;

	if (rc != SQLITE_OK) {

		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return EXIT_FAILURE;
	}

	rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &res1, 0); // OPT for usage

	if (rc != SQLITE_OK) {

		fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);

		return EXIT_FAILURE;
	}

	rc = sqlite3_step(res1);

	if (rc == SQLITE_ROW) {
		printf("%s\n", sqlite3_column_text(res1, 0));
	}

	sqlite3_finalize(res1);

	return EXIT_SUCCESS;
}