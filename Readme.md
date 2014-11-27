kawari.js
=======================================

Programmable pseudo AI KAWARI 8 on JavaScript

Original SHIORI module
---------------------------------------

[KAWARI](http://kawari.sourceforge.net/) by KAWARI Development Team

**Caution**:
This module is maintained by [Narazaka](http://narazaka.net/) and not officially supported by KAWARI Development Team.
Do not report bugs or something that is caused by kawari.js to KAWARI Development Team.

Installation
---------------------------------------

    bower install kawari.js

or download zip archive and get kawari.js

Usage
---------------------------------------

This module has only low level API, that is provided by emscripten.

So use the wrapper module, [nativeshiori](https://github.com/Narazaka/nativeshiori), and do like below.

    <script src="encoding.min.js"></script>
    <script src="nativeshiori.js"></script>
    <script src="kawari.js"></script>
    <script>
    
    var kawarirc_str = 'System.Callback.OnGET: test\r\n';
    var kawarirc = Encoding.convert(Encoding.stringToCode(request), 'SJIS', 'UNICODE');
    var storage = {
      'kawarirc.kis': new Uint8Array(kawarirc) // filename: ArrayBufferView
    };
    
    // Shiori instance and optional storage (/path/to/ghost/master/*)
    var nativeshiori = new NativeShiori(new Kawari(), storage);
    
    // write files in storage to FS then load() if storage exists, else load()
    var load_code = nativeshiori.load('/path/to/ghost/master'); 
    
    // request()
    var response = nativeshiori.request('GET SHIORI/3.0\r\nCharset: Shift_JIS\r\nID: OnBoot\r\n\r\n');
    
    // unload()
    var unload_code = nativeshiori.unload();
    
    </script>

LICENSE
--------------------------------

(C) 2014 Narazaka : Licensed under [The BSD 3-Clause License](http://narazaka.net/license/BSD3?2014)
