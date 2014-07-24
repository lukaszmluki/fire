
message(checking additional libraries...)

PKG_CONFIG_FOUND=NO

system(pkg-config --version > /dev/null 2>&1):PKG_CONFIG_FOUND=YES
message(pkg-config found: $$PKG_CONFIG_FOUND)

equals(PKG_CONFIG_FOUND, "YES") {

    system(pkg-config smbclient > /dev/null 2>&1):HAVE_SMBCLIENT=YES
    message(libsmbclient found: $$HAVE_SMBCLIENT)
    equals(HAVE_SMBCLIENT, "YES") {
        LIBS += `pkg-config smbclient --libs`
        QMAKE_CXXFLAGS += `pkg-config smbclient --cflags` -DHAVE_SMBCLIENT
    }

}
