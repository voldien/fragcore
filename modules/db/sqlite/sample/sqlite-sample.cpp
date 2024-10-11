#include <IO/FileSystem.h>
#include <FragCore.h>
#include <SQLiteIO.h>

using namespace fragcore;

int main(int argc, const char **argv) {

	printf("%s\n", sqlite3_libversion());
	IFileSystem *filesystem = FileSystem::createFileSystem();

	Ref<IO> io = Ref<IO>(filesystem->openFile("example.db", IO::IOMode::ACCESS));

	sqlite3 *database;
	fragcore_sqlite3_open(io, &database);

	char *errMs;
	// int result_code = sqlite3_db_config(db, SQLITE_CONFIG_SINGLETHREAD, nullptr);
	// if (result_code != SQLITE_OK)
	// 	throw RuntimeException("Failed to configure to single threaded: {}", sqlite3_errstr(result_code));
	int result_code = sqlite3_exec(database, "PRAGMA synchronous=OFF", nullptr, nullptr, &errMs);
	if (result_code != SQLITE_OK) {
		throw RuntimeException("Failed to open sqlite3: {}", sqlite3_errstr(result_code));
	}
	result_code = sqlite3_exec(database, "PRAGMA count_changes=OFF", nullptr, nullptr, &errMs);
	if (result_code != SQLITE_OK) {
		throw RuntimeException("Failed to open sqlite3: {}", sqlite3_errstr(result_code));
	}
	result_code = sqlite3_exec(database, "PRAGMA journal_mode=MEMORY", nullptr, nullptr, &errMs);
	if (result_code != SQLITE_OK) {
		throw RuntimeException("Failed to open sqlite3: {}", sqlite3_errstr(result_code));
	}
	result_code = sqlite3_exec(database, "PRAGMA temp_store=MEMORY", nullptr, nullptr, &errMs);
	if (result_code != SQLITE_OK) {
		throw RuntimeException("Failed to open sqlite3: {}", sqlite3_errstr(result_code));
	}
	result_code = sqlite3_exec(database, "PRAGMA cache_size=40000", nullptr, nullptr, &errMs);
	if (result_code != SQLITE_OK) {
		throw RuntimeException("Failed to open sqlite3: {}", sqlite3_errstr(result_code));
	}

	sqlite3_stmt *res1;

	if (result_code != SQLITE_OK) {

		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(database));
		sqlite3_close(database);
		return EXIT_FAILURE;
	}

	result_code = sqlite3_prepare_v2(database, "SELECT SQLITE_VERSION()", -1, &res1, nullptr); // OPT for usage

	if (result_code != SQLITE_OK) {

		fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(database));
		sqlite3_close(database);

		return EXIT_FAILURE;
	}

	result_code = sqlite3_step(res1);

	if (result_code == SQLITE_ROW) {
		printf("%s\n", sqlite3_column_text(res1, 0));
	}

	sqlite3_finalize(res1);

	return EXIT_SUCCESS;
}