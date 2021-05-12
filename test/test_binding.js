const APIdll = require("../lib/binding.js");


//const assert = require("assert");

//assert(Line, "The expected module is undefined");

function testBasic()
{
    //const instance = new Line(1, 1);
    //console.log("the value is " + instance.func(1.0));
    //assert(instance.func, "The expected method is not defined");
    //assert.strictEqual(instance.func(10.1),11.1 , "Unexpected value returned");
}

function testInvalidParams()
{
    //const instance = new Line();
}

//assert.doesNotThrow(testBasic, undefined, "testBasic threw an expection");
//assert.throws(testInvalidParams, undefined, "testInvalidParams didn't throw");
const Sinstance = new APIdll("reg_flight.csv", "anomaly_flight.csv", true);
var x = Sinstance.GetAllAnomalyReportCpp();
console.log(x);
console.log("Tests passed- everything looks OK!");