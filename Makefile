BUILD_DIR = ./kawari/build/src

all: js wasm

js:
	patch -u -d $(BUILD_DIR) < gcc.mak.js.patch
	cd $(BUILD_DIR) && $(MAKE) -f gcc.mak
	patch -R -u -d $(BUILD_DIR) < gcc.mak.js.patch
	mkdir -p lib/js
	cp ./kawari/build/mach/js/kawari.js lib/js/kawari.js

clean-js:
	patch -u -d $(BUILD_DIR) < gcc.mak.js.patch
	cd $(BUILD_DIR) && $(MAKE) -f gcc.mak clean
	patch -R -u -d $(BUILD_DIR) < gcc.mak.js.patch

wasm:
	patch -u -d $(BUILD_DIR) < gcc.mak.wasm.patch
	cd $(BUILD_DIR) && $(MAKE) -f gcc.mak
	patch -R -u -d $(BUILD_DIR) < gcc.mak.wasm.patch
	mkdir -p lib/wasm
	cp ./kawari/build/mach/wasm/kawari.js lib/wasm/kawari.js
	cp ./kawari/build/mach/wasm/kawari.wasm lib/wasm/kawari.wasm

wasm-clean:
	patch -u -d $(BUILD_DIR) < gcc.mak.wasm.patch
	cd $(BUILD_DIR) && $(MAKE) -f gcc.mak clean
	patch -R -u -d $(BUILD_DIR) < gcc.mak.wasm.patch

test: node-install
	npm test

node-install:
	npm install
