GIT_VERSION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --always --tags)

DEFINES += GIT_VERSION=\\"$$GIT_VERSION\\"

#Once that is done, GIT_VERSION becomes usable just like any macro inside your code:
#QString version(GIT_VERSION);

#Adding Windows Executable Version
VERSION = $$GIT_VERSION
win32 {
    VERSION ~= s/-\d+-g[a-f0-9]{6,}//
}

#Adding Mac OS X App Version
macx {
    INFO_PLIST_PATH = $$shell_quote($${OUT_PWD}/$${TARGET}.app/Contents/Info.plist)
    QMAKE_POST_LINK += /usr/libexec/PlistBuddy -c \"Set :CFBundleShortVersionString $${VERSION}\" $${INFO_PLIST_PATH}
}

