kawari.js
=======================================

Programmable pseudo AI KAWARI 8 on JavaScript

Original SHIORI module
---------------------------------------

[KAWARI](http://kawari.github.io/) by KAWARI Development Team

**Caution**:
This module is maintained by [Narazaka](http://narazaka.net/) and not officially supported by KAWARI Development Team.
Do not report bugs or something that is caused by kawari.js to KAWARI Development Team.

Installation
---------------------------------------

```bash
npm install kawari.js
```

Usage
---------------------------------------

This module has only low level API, that is provided by emscripten.

So use the wrapper module, [nativeshiori](https://github.com/Narazaka/nativeshiori), and do like below.

```javascript
// in browser
<script src="encoding.min.js"></script>
<script src="nativeshiori.js"></script>
<script src="nativeshiori-encode.js"></script>
<script src="kawari.js"></script>

// in node.js
const NativeShiori = require("nativeshiori");
const NativeShioriEncode = require("nativeshiori/nativeshiori-encode");
const Encoding = require("encoding-japanese");
const newKawari = require("kawari.js"); // or kawari.js/wasm

var kawarircContent = 'System.Callback.OnGET: test\r\n';
var kawarirc = Encoding.convert(Encoding.stringToCode(kawarircContent), 'SJIS', 'UNICODE');
var storage = {
    'kawarirc.kis': new Uint8Array(kawarirc) // filename: ArrayBufferView
};

newKawari(function (Kawari) {
    // Shiori instance and optional storage (/path/to/ghost/master/*)
    var nativeshiori = new NativeShioriEncode(new NativeShiori(Kawari));

    // write files in storage to FS (/path/to/ghost/master/*)
    nativeshiori.push("/path/to/ghost/master/", storage);

    // write files in storage to FS then load() if storage exists, else load()
    var loadCode = nativeshiori.load('/path/to/ghost/master');

    // request()
    var response = nativeshiori.request('GET SHIORI/3.0\r\nCharset: Shift_JIS\r\nID: OnBoot\r\n\r\n');

    // unload()
    var unloadCode = nativeshiori.unload();

    // read and unlink files in FS and return them (filename: ArrayBuffer)
    var afterStorage = nativeshiori.pull("/path/to/ghost/master/");
});
```

Build
--------------------------------

```bash
make -R
make test
```

LICENSE
--------------------------------

(C) 2018 Narazaka : Licensed under [The BSD 3-Clause License](http://narazaka.net/license/BSD3?2018)
