echo "PKG_CONFIG_PATH"
echo $PKG_CONFIG_PATH

echo "pkg-config --cflags --libs opencv"
pkg-config --cflags --libs opencv

echo "make"
make

