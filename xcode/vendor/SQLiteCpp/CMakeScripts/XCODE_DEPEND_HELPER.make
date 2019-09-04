# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.SQLiteCpp.Debug:
/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/Debug/libSQLiteCpp.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/Debug/libSQLiteCpp.a


PostBuild.sqlite3.Debug:
/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/sqlite3/Debug/libsqlite3.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/sqlite3/Debug/libsqlite3.a


PostBuild.SQLiteCpp.Release:
/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/Release/libSQLiteCpp.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/Release/libSQLiteCpp.a


PostBuild.sqlite3.Release:
/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/sqlite3/Release/libsqlite3.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/sqlite3/Release/libsqlite3.a


PostBuild.SQLiteCpp.MinSizeRel:
/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/MinSizeRel/libSQLiteCpp.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/MinSizeRel/libSQLiteCpp.a


PostBuild.sqlite3.MinSizeRel:
/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/sqlite3/MinSizeRel/libsqlite3.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/sqlite3/MinSizeRel/libsqlite3.a


PostBuild.SQLiteCpp.RelWithDebInfo:
/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/RelWithDebInfo/libSQLiteCpp.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/RelWithDebInfo/libSQLiteCpp.a


PostBuild.sqlite3.RelWithDebInfo:
/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/sqlite3/RelWithDebInfo/libsqlite3.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/sqlite3/RelWithDebInfo/libsqlite3.a




# For each target create a dummy ruleso the target does not have to exist
