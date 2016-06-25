var chosenFileEntry = null;
var data;

document.getElementById('importFile').addEventListener('click', function(e) {
  var table;
  
  chrome.fileSystem.chooseEntry({type: 'openFile', accepts: [{extensions: ['pvd']}]}, function(readOnlyEntry) {

    readOnlyEntry.file(function(file) {
      var reader = new FileReader();
      reader.fileName = file.name;
      //reader.onerror = errorHandler;
      reader.onloadend = function(e) {
        var content = e.target.result;
        var fileType = content.substr(0,8);
        var number;
        var offset = 10 + 41 * 4;
		var filename = document.getElementById("filename");
		if (content.substr(0,8) == "PVD_PV65") {
			filename.innerHTML = e.target.fileName;
			data = [[], [[],[]], [[],[]], []];

			for (var i=0; i<41; i++) {
					number = content.substr(10 + i * 4, 4);
			  data[0].push((number.charCodeAt(3)<<24) +
					 (number.charCodeAt(2)<<16) +
					 (number.charCodeAt(1)<<8) +
					  number.charCodeAt(0));
			};  

			for (var i=0; i<data[0][6]; i++) {
		  
			  data[1][0].push(content.charCodeAt(offset + 0 + i * 2));
			  data[1][1].push(content.charCodeAt(offset + 1 + i * 2));
			  data[2][0].push(content.charCodeAt(offset + 2 * data[0][6] + 0 + i * 2));
			  data[2][1].push(content.charCodeAt(offset + 2 * data[0][6] + 1 + i * 2));
			  data[3].push((data[1][1][i] - data[1][0][i]) / (data[2][1][i] - data[2][0][i]) * data[0][2] / data[0][4] * 100);
			};
			
			//showSettings();
			showHeader();
			drawDiagram();
        } else {
		    filename.innerHTML = 'ERROR: Unrecognized file type!';
		}
      };
	    reader.readAsBinaryString(file);
	  });
  });
});

function showSettings(){
  var tbl = document.getElementById('filedata');
  var row;
  tbl.innerHTML = '';
  for (var i=0; i<41; i++) {
    row = tbl.insertRow(-1);
    row.insertCell(-1).innerHTML = i;
    row.insertCell(-1).innerHTML = data[0][i];
  };
};

function showHeader(){
  document.getElementById('device').innerHTML = (data[0][0] == 31) ? 'PV6503A' : 'UNKNOWN';
  document.getElementById('samples').innerHTML = data[0][6] + ' samples / ' + data[0][1] * 2.5e-5 + 'S';
  document.getElementById('frequency_range').innerHTML = data[0][20] / 1e4 + 'kHz - ' + (data[0][20] + data[0][19]) / 1e4 + 'kHz';
  
  document.getElementById('A1_v_div').innerHTML = data[0][2] * 1e-3 + ' mV / Div';
  document.getElementById('A1_hz_div').innerHTML = data[0][19] * 1e-2 + ' Hz / Div';
  document.getElementById('A2_v_div').innerHTML = data[0][4] * 1e-3 + ' mV / Div';
  document.getElementById('A2_hz_div').innerHTML = data[0][19] * 1e-2 + ' Hz / Div';
  
  var iMin = Math.min.apply(Math, data[3]);
  var iMax = Math.max.apply(Math, data[3]);
  document.getElementById('SR_f').innerHTML = data[0][20] / 1e4 + data[0][19] / 1e4 / (data[0][6] - data[0][7]) * (data[3].indexOf(iMin) - data[0][7]) + ' Hz';
  document.getElementById('SR_i').innerHTML = iMin + ' Ohm';
  document.getElementById('SR_q').innerHTML = 'n/a' ;
  document.getElementById('PR_f').innerHTML = data[0][20] / 1e4 + data[0][19] / 1e4 / (data[0][6] - data[0][7]) * (data[3].indexOf(iMax) - data[0][7]) + ' Hz';
  document.getElementById('PR_i').innerHTML = iMax + ' Ohm';
  document.getElementById('PR_q').innerHTML = 'n/a';
};

function showGrid(context, minX, maxX, minY, maxY){
 context.fillStyle = '#FFFFFF';
 context.font = '12px monospace';
 context.textBaseline = 'top';
 context.textAlign = 'right';
 context.fillText ("40.0k", 45, 20);

}

function drawDiagram(){
  var c = document.getElementById("diagram");
  var Fmin, Fmax, valMin, valMax;
  c.width = window.innerWidth;
  c.height = window.innerHeight - c.offsetTop;

  var ctx = c.getContext("2d");
  ctx.fillRect(0,0,c.width, c.height);
  ctx.strokeStyle="#003F00";
  
  Fmin=10000; Fmax=15000; valMin=0; valMax=10;
  showGrid(ctx, Fmin, Fmax, valMin, valMax); // Drawing the Grid
   
  const leftMargin = 50;
  const bottomMargin = 50;
  ctx.fillStyle="#001F00";
  var i;

  ctx.beginPath();
  //horizontal
  for (i=leftMargin; i<c.width; i +=100){
    ctx.moveTo(i, c.height-bottomMargin);
    ctx.lineTo(i, 0);
  }
 //vertical
  for (i=bottomMargin; i<c.height;i +=50){
    ctx.moveTo(leftMargin, c.height-i);
    ctx.lineTo(c.width, c.height-i);
  }
  ctx.stroke();
  
  ctx.strokeStyle="#00FF00";
  ctx.beginPath();

  ctx.moveTo(leftMargin, c.height-bottomMargin);
  ctx.lineTo(c.width, 500);

  ctx.stroke();
  
};

window.addEventListener("resize", function() {
   drawDiagram()
});

drawDiagram();

