# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.file_saver.Debug:
/Users/yamadapc/projects/file-saver/xcode/Debug/file_saver:\
	/usr/local/lib/libboost_filesystem-mt.a\
	/usr/local/lib/libboost_system-mt.a
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/Debug/file_saver


PostBuild.libfilesaver.Debug:
/Users/yamadapc/projects/file-saver/xcode/Debug/liblibfilesaver.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/Debug/liblibfilesaver.a


PostBuild.file_saver.Release:
/Users/yamadapc/projects/file-saver/xcode/Release/file_saver:\
	/usr/local/lib/libboost_filesystem-mt.a\
	/usr/local/lib/libboost_system-mt.a
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/Release/file_saver


PostBuild.libfilesaver.Release:
/Users/yamadapc/projects/file-saver/xcode/Release/liblibfilesaver.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/Release/liblibfilesaver.a


PostBuild.file_saver.MinSizeRel:
/Users/yamadapc/projects/file-saver/xcode/MinSizeRel/file_saver:\
	/usr/local/lib/libboost_filesystem-mt.a\
	/usr/local/lib/libboost_system-mt.a
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/MinSizeRel/file_saver


PostBuild.libfilesaver.MinSizeRel:
/Users/yamadapc/projects/file-saver/xcode/MinSizeRel/liblibfilesaver.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/MinSizeRel/liblibfilesaver.a


PostBuild.file_saver.RelWithDebInfo:
/Users/yamadapc/projects/file-saver/xcode/RelWithDebInfo/file_saver:\
	/usr/local/lib/libboost_filesystem-mt.a\
	/usr/local/lib/libboost_system-mt.a
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/RelWithDebInfo/file_saver


PostBuild.libfilesaver.RelWithDebInfo:
/Users/yamadapc/projects/file-saver/xcode/RelWithDebInfo/liblibfilesaver.a:
	/bin/rm -f /Users/yamadapc/projects/file-saver/xcode/RelWithDebInfo/liblibfilesaver.a




# For each target create a dummy ruleso the target does not have to exist
/usr/local/lib/libboost_filesystem-mt.a:
/usr/local/lib/libboost_system-mt.a:
