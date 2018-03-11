const NativeShiori = require("nativeshiori");
const NativeShioriEncode = require("nativeshiori/nativeshiori-encode");
const Encoding = require("encoding-japanese");
const assert = require("assert");

const crlf = "\x0d\x0a";

module.exports = function (newKawari) {
  const loadKawari = () => new Promise((resolve) => newKawari(resolve));
  describe("Kawari", () => {
    const kawarircContent = `System.Callback.OnGET: test${crlf}System.Callback.OnNOTIFY: $(save save.txt foo)${crlf}foo: bar${crlf}`;
    const kawarirc = Encoding.convert(Encoding.stringToCode(kawarircContent), "SJIS", "UNICODE");
    const storage = {
      "kawarirc.kis": new Uint8Array(kawarirc) // filename: ArrayBuffer or Uint8Array
    };

    it("works", async () => {
      const Kawari = await loadKawari();
      const nativeshiori = new NativeShioriEncode(new NativeShiori(Kawari)); // Shiori instance (with auto charset convert)
      nativeshiori.push("/path/to/ghost/master/", storage); // write files in storage to FS (/path/to/ghost/master/*)

      const loadResult = nativeshiori.load("/path/to/ghost/master/"); // load() **CAUTION**: SHIORI/3.0 load() expects path separator (ex. "/") at the end of dirpath
      assert.equal(loadResult, 1);

      const response1 = nativeshiori.request(`GET SHIORI/3.0${crlf}Charset: Shift_JIS${crlf}ID: OnBoot${crlf}${crlf}`); // request()
      assert.equal(response1, `SHIORI/3.0 200 OK${crlf}Charset: Shift_JIS${crlf}Value: test${crlf}${crlf}`);

      const response2 = nativeshiori.request(`NOTIFY SHIORI/3.0${crlf}Charset: Shift_JIS${crlf}ID: OnFoo${crlf}${crlf}`); // request()
      assert.equal(response2, `SHIORI/3.0 204 No Content${crlf}Charset: Shift_JIS${crlf}${crlf}`);

      const unloadResult = nativeshiori.unload(); // unload()
      assert.equal(unloadResult, 1);

      const afterStorage = nativeshiori.pull("/path/to/ghost/master/"); // read and unlink files in FS and return them (filename: ArrayBuffer)
      assert.deepEqual(Object.keys(afterStorage).sort(), ["kawarirc.kis", "save.txt"]);
      const saveContent = Encoding.codeToString(Encoding.convert(new Uint8Array(afterStorage["save.txt"]), "UNICODE", "SJIS"));
      assert.ok(/Kawari saved file/.test(saveContent));
      assert.ok(/foo : "bar"/.test(saveContent));
    })
  });
}
