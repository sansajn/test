const Echo = require("../lib/binding.js");
const assert = require("assert");

assert(Echo, "The expected function is undefined");

function testBasic()
{
    const expected = "hello";
    Echo(expected, function(err, result){
        assert.strictEqual(result, expected, "Unexpected value returned");
    });
}

assert.doesNotThrow(testBasic, undefined, "testBasic threw an expection");

console.log("Tests passed- everything looks OK!");
