BUILD_DIR = ./kawari/build/src

all:
	patch -u -d $(BUILD_DIR) < gcc.mak.patch
	cd $(BUILD_DIR) && $(MAKE) -f gcc.mak
	cp ./kawari/build/mach/js/shiori.js kawari.js

clean:
	cd $(BUILD_DIR) && $(MAKE) -f gcc.mak clean
	patch -R -u -d $(BUILD_DIR) < gcc.mak.patch
