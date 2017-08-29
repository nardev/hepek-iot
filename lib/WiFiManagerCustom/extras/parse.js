'use strict';

const fs = require('fs');

console.log('gzip scripts and  files starting');
console.log("==============================================");
// gzip/compress js scripts and place them in /data directory

// http://nodejs.org/api.html#_child_processes
var sys = require('sys')
var exec = require('child_process').exec;
function puts(error, stdout, stderr) { sys.puts(stdout) }

var sourcePath = '/home/nardev/Desktop/NODEMCU-IOT-TRANSITION/lib/WiFiManager/extras/';
var destinationPath = '/home/nardev/Desktop/NODEMCU-IOT-TRANSITION/data/';

var toGzip = {
  hepekJs: ['hepek.js', 'hepek.js.gz'],
  jscolorJs:['jscolor.min.js', 'jscolor.min.js.gz'],
  jqueryJs:['jquery.js', 'jquery.js.gz'],
  hepekCSS: ['hepek.css', 'hepek.css.gz'],
}

Object.keys(toGzip).forEach(function(key) {
  const inFileToGzip = sourcePath+toGzip[key][0];
  const outFileToGzip = destinationPath+toGzip[key][1];

  console.log("==============================================");
  console.log(key, inFileToGzip);
	console.log(key, outFileToGzip);
  console.log("==============================================");

  exec("gzip -c "+inFileToGzip+" > "+outFileToGzip, puts);
});


console.log("==============================================");


console.log('parsin starting');
var toParse = {
  wifimanager: ['wifimanager.template.html', 'wifimanager.template.h'],
  mqttsettings:['mqttsettings.template.html', 'mqttsettings.template.h']
}
Object.keys(toParse).forEach(function(key) {
  console.log("==============================================");
  console.log(key, toParse[key][0]);
	console.log(key, toParse[key][1]);
  console.log("==============================================");

  const inFile = toParse[key][0];
  const outFile = toParse[key][1];

  parseIT(inFile, outFile)
});


function parseIT(inFile, outFile) {

  const defineRegEx = /<!-- ([A-Z_]+) -->/gm;
  console.log('parsing', inFile);

  fs.readFile(inFile, 'utf8', function (err,data) {
    if (err) {
      return console.log(err);
    }
    //console.log(data);

    let defines = data.match(defineRegEx);

    //console.log(defines);
    var stream = fs.createWriteStream(outFile);
    stream.once('open', function(fd) {
      for (const i in defines) {

        const start = defines[i];
        const end = start.replace('<!-- ', '<!-- /')
        defineRegEx.lastIndex = 0;
        const constantName = defineRegEx.exec(start)[1];

        console.log(constantName);
        var extractRE = new RegExp(start + '([\\s\\S]+)' + end, 'gm');
        let extractArray = extractRE.exec(data);
        if(extractArray.length > 1) {
          let def = extractArray[1];
          //console.log(def);
          //minimise a bit
          def = def.replace(/\s+/g, ' ');
          def = def.replace(/>\s+</g, '><');
          def = def.trim();
          //more extraneous spaces - possible bad results, needs to be checked
          //def = def.replace(/(\w)\s(\W)|(\W)\s(\w)|(\W)\s(\W)/g, '$1$2$3$4$5$6');
          def = def.replace(/(\w)\s(\W)|(\W)\s(\w)/g, '$1$2$3$4');
          //escape double quotes
          def = def.replace(/\\([\s\S])|(")/g, "\\$1$2");


          console.log(def);
          //const char HTTP_HEAD[] PROGMEM            =
          let string = 'const char ' + constantName + '[] PROGMEM';
          for (let i = string.length; i < 42; i++) {
            string += ' ';
          }
          string += '= "' + def + '";\n';
          stream.write(string);
        }
      }
      stream.end();
    });
  });



}
