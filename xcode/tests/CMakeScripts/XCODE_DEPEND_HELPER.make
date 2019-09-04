# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.gmock.Debug:
/Users/yamadapc/projects/file-saver/xcode/lib/Debug/libgmockd.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/lib/Debug/libgmockd.a


PostBuild.gmock_main.Debug:
/Users/yamadapc/projects/file-saver/xcode/lib/Debug/libgmock_maind.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/lib/Debug/libgmock_maind.a


PostBuild.gtest.Debug:
/Users/yamadapc/projects/file-saver/xcode/lib/Debug/libgtestd.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/lib/Debug/libgtestd.a


PostBuild.gtest_main.Debug:
/Users/yamadapc/projects/file-saver/xcode/lib/Debug/libgtest_maind.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/lib/Debug/libgtest_maind.a


PostBuild.runUnitTests.Debug:
PostBuild.gtest.Debug: /Users/yamadapc/projects/file-saver/xcode/tests/Debug/runUnitTests
PostBuild.gmock.Debug: /Users/yamadapc/projects/file-saver/xcode/tests/Debug/runUnitTests
PostBuild.SQLiteCpp.Debug: /Users/yamadapc/projects/file-saver/xcode/tests/Debug/runUnitTests
PostBuild.sqlite3.Debug: /Users/yamadapc/projects/file-saver/xcode/tests/Debug/runUnitTests
PostBuild.leveldb.Debug: /Users/yamadapc/projects/file-saver/xcode/tests/Debug/runUnitTests
PostBuild.gtest.Debug: /Users/yamadapc/projects/file-saver/xcode/tests/Debug/runUnitTests
/Users/yamadapc/projects/file-saver/xcode/tests/Debug/runUnitTests:\
	/Users/yamadapc/projects/file-saver/xcode/lib/Debug/libgtestd.a\
	/Users/yamadapc/projects/file-saver/xcode/lib/Debug/libgmockd.a\
	/usr/local/lib/libboost_filesystem-mt.dylib\
	/usr/local/lib/libboost_system-mt.dylib\
	/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/Debug/libSQLiteCpp.a\
	/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/sqlite3/Debug/libsqlite3.a\
	/Users/yamadapc/projects/file-saver/xcode/vendor/leveldb/Debug/libleveldb.a\
	/Users/yamadapc/projects/file-saver/xcode/lib/Debug/libgtestd.a
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/tests/Debug/runUnitTests


PostBuild.gmock.Release:
/Users/yamadapc/projects/file-saver/xcode/lib/Release/libgmock.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/lib/Release/libgmock.a


PostBuild.gmock_main.Release:
/Users/yamadapc/projects/file-saver/xcode/lib/Release/libgmock_main.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/lib/Release/libgmock_main.a


PostBuild.gtest.Release:
/Users/yamadapc/projects/file-saver/xcode/lib/Release/libgtest.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/lib/Release/libgtest.a


PostBuild.gtest_main.Release:
/Users/yamadapc/projects/file-saver/xcode/lib/Release/libgtest_main.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/lib/Release/libgtest_main.a


PostBuild.runUnitTests.Release:
PostBuild.gtest.Release: /Users/yamadapc/projects/file-saver/xcode/tests/Release/runUnitTests
PostBuild.gmock.Release: /Users/yamadapc/projects/file-saver/xcode/tests/Release/runUnitTests
PostBuild.SQLiteCpp.Release: /Users/yamadapc/projects/file-saver/xcode/tests/Release/runUnitTests
PostBuild.sqlite3.Release: /Users/yamadapc/projects/file-saver/xcode/tests/Release/runUnitTests
PostBuild.leveldb.Release: /Users/yamadapc/projects/file-saver/xcode/tests/Release/runUnitTests
PostBuild.gtest.Release: /Users/yamadapc/projects/file-saver/xcode/tests/Release/runUnitTests
/Users/yamadapc/projects/file-saver/xcode/tests/Release/runUnitTests:\
	/Users/yamadapc/projects/file-saver/xcode/lib/Release/libgtest.a\
	/Users/yamadapc/projects/file-saver/xcode/lib/Release/libgmock.a\
	/usr/local/lib/libboost_filesystem-mt.dylib\
	/usr/local/lib/libboost_system-mt.dylib\
	/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/Release/libSQLiteCpp.a\
	/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/sqlite3/Release/libsqlite3.a\
	/Users/yamadapc/projects/file-saver/xcode/vendor/leveldb/Release/libleveldb.a\
	/Users/yamadapc/projects/file-saver/xcode/lib/Release/libgtest.a
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/tests/Release/runUnitTests


PostBuild.gmock.MinSizeRel:
/Users/yamadapc/projects/file-saver/xcode/lib/MinSizeRel/libgmock.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/lib/MinSizeRel/libgmock.a


PostBuild.gmock_main.MinSizeRel:
/Users/yamadapc/projects/file-saver/xcode/lib/MinSizeRel/libgmock_main.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/lib/MinSizeRel/libgmock_main.a


PostBuild.gtest.MinSizeRel:
/Users/yamadapc/projects/file-saver/xcode/lib/MinSizeRel/libgtest.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/lib/MinSizeRel/libgtest.a


PostBuild.gtest_main.MinSizeRel:
/Users/yamadapc/projects/file-saver/xcode/lib/MinSizeRel/libgtest_main.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/lib/MinSizeRel/libgtest_main.a


PostBuild.runUnitTests.MinSizeRel:
PostBuild.gtest.MinSizeRel: /Users/yamadapc/projects/file-saver/xcode/tests/MinSizeRel/runUnitTests
PostBuild.gmock.MinSizeRel: /Users/yamadapc/projects/file-saver/xcode/tests/MinSizeRel/runUnitTests
PostBuild.SQLiteCpp.MinSizeRel: /Users/yamadapc/projects/file-saver/xcode/tests/MinSizeRel/runUnitTests
PostBuild.sqlite3.MinSizeRel: /Users/yamadapc/projects/file-saver/xcode/tests/MinSizeRel/runUnitTests
PostBuild.leveldb.MinSizeRel: /Users/yamadapc/projects/file-saver/xcode/tests/MinSizeRel/runUnitTests
PostBuild.gtest.MinSizeRel: /Users/yamadapc/projects/file-saver/xcode/tests/MinSizeRel/runUnitTests
/Users/yamadapc/projects/file-saver/xcode/tests/MinSizeRel/runUnitTests:\
	/Users/yamadapc/projects/file-saver/xcode/lib/MinSizeRel/libgtest.a\
	/Users/yamadapc/projects/file-saver/xcode/lib/MinSizeRel/libgmock.a\
	/usr/local/lib/libboost_filesystem-mt.dylib\
	/usr/local/lib/libboost_system-mt.dylib\
	/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/MinSizeRel/libSQLiteCpp.a\
	/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/sqlite3/MinSizeRel/libsqlite3.a\
	/Users/yamadapc/projects/file-saver/xcode/vendor/leveldb/MinSizeRel/libleveldb.a\
	/Users/yamadapc/projects/file-saver/xcode/lib/MinSizeRel/libgtest.a
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/tests/MinSizeRel/runUnitTests


PostBuild.gmock.RelWithDebInfo:
/Users/yamadapc/projects/file-saver/xcode/lib/RelWithDebInfo/libgmock.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/lib/RelWithDebInfo/libgmock.a


PostBuild.gmock_main.RelWithDebInfo:
/Users/yamadapc/projects/file-saver/xcode/lib/RelWithDebInfo/libgmock_main.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/lib/RelWithDebInfo/libgmock_main.a


PostBuild.gtest.RelWithDebInfo:
/Users/yamadapc/projects/file-saver/xcode/lib/RelWithDebInfo/libgtest.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/lib/RelWithDebInfo/libgtest.a


PostBuild.gtest_main.RelWithDebInfo:
/Users/yamadapc/projects/file-saver/xcode/lib/RelWithDebInfo/libgtest_main.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/lib/RelWithDebInfo/libgtest_main.a


PostBuild.runUnitTests.RelWithDebInfo:
PostBuild.gtest.RelWithDebInfo: /Users/yamadapc/projects/file-saver/xcode/tests/RelWithDebInfo/runUnitTests
PostBuild.gmock.RelWithDebInfo: /Users/yamadapc/projects/file-saver/xcode/tests/RelWithDebInfo/runUnitTests
PostBuild.SQLiteCpp.RelWithDebInfo: /Users/yamadapc/projects/file-saver/xcode/tests/RelWithDebInfo/runUnitTests
PostBuild.sqlite3.RelWithDebInfo: /Users/yamadapc/projects/file-saver/xcode/tests/RelWithDebInfo/runUnitTests
PostBuild.leveldb.RelWithDebInfo: /Users/yamadapc/projects/file-saver/xcode/tests/RelWithDebInfo/runUnitTests
PostBuild.gtest.RelWithDebInfo: /Users/yamadapc/projects/file-saver/xcode/tests/RelWithDebInfo/runUnitTests
/Users/yamadapc/projects/file-saver/xcode/tests/RelWithDebInfo/runUnitTests:\
	/Users/yamadapc/projects/file-saver/xcode/lib/RelWithDebInfo/libgtest.a\
	/Users/yamadapc/projects/file-saver/xcode/lib/RelWithDebInfo/libgmock.a\
	/usr/local/lib/libboost_filesystem-mt.dylib\
	/usr/local/lib/libboost_system-mt.dylib\
	/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/RelWithDebInfo/libSQLiteCpp.a\
	/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/sqlite3/RelWithDebInfo/libsqlite3.a\
	/Users/yamadapc/projects/file-saver/xcode/vendor/leveldb/RelWithDebInfo/libleveldb.a\
	/Users/yamadapc/projects/file-saver/xcode/lib/RelWithDebInfo/libgtest.a
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/tests/RelWithDebInfo/runUnitTests




# For each target create a dummy ruleso the target does not have to exist
/Users/yamadapc/projects/file-saver/xcode/lib/Debug/libgmockd.a:
/Users/yamadapc/projects/file-saver/xcode/lib/Debug/libgtestd.a:
/Users/yamadapc/projects/file-saver/xcode/lib/MinSizeRel/libgmock.a:
/Users/yamadapc/projects/file-saver/xcode/lib/MinSizeRel/libgtest.a:
/Users/yamadapc/projects/file-saver/xcode/lib/RelWithDebInfo/libgmock.a:
/Users/yamadapc/projects/file-saver/xcode/lib/RelWithDebInfo/libgtest.a:
/Users/yamadapc/projects/file-saver/xcode/lib/Release/libgmock.a:
/Users/yamadapc/projects/file-saver/xcode/lib/Release/libgtest.a:
/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/Debug/libSQLiteCpp.a:
/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/MinSizeRel/libSQLiteCpp.a:
/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/RelWithDebInfo/libSQLiteCpp.a:
/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/Release/libSQLiteCpp.a:
/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/sqlite3/Debug/libsqlite3.a:
/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/sqlite3/MinSizeRel/libsqlite3.a:
/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/sqlite3/RelWithDebInfo/libsqlite3.a:
/Users/yamadapc/projects/file-saver/xcode/vendor/SQLiteCpp/sqlite3/Release/libsqlite3.a:
/Users/yamadapc/projects/file-saver/xcode/vendor/leveldb/Debug/libleveldb.a:
/Users/yamadapc/projects/file-saver/xcode/vendor/leveldb/MinSizeRel/libleveldb.a:
/Users/yamadapc/projects/file-saver/xcode/vendor/leveldb/RelWithDebInfo/libleveldb.a:
/Users/yamadapc/projects/file-saver/xcode/vendor/leveldb/Release/libleveldb.a:
/usr/local/lib/libboost_filesystem-mt.dylib:
/usr/local/lib/libboost_system-mt.dylib:
