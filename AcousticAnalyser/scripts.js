var chosenFileEntry = null;
var data;

document.getElementById('importFile').addEventListener('click', function(e) {
  var table;
  
  chrome.fileSystem.chooseEntry({type: 'openFile'}, function(readOnlyEntry) {

    readOnlyEntry.file(function(file) {
      var reader = new FileReader();

      //reader.onerror = errorHandler;
      reader.onloadend = function(e) {
        var content = e.target.result;
        var fileType = content.substr(0,8);
        var number;
        var offset = 10 + 41 * 4;

        data = [[], [[],[]], [[],[]]];

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
          data[2][1].push(content.charCodeAt(offset + 2 * data[0][6] + 0 + i * 2));
        };
        
        showSettings();
        showHeader();
        drawDiagram();
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

};

function drawDiagram(){
  var c = document.getElementById("diagram");
  var ctx = c.getContext("2d");
  ctx.fillRect(0,0,1024,768);
  ctx.strokeStyle="#003F00";
  
  const leftMargin = 50;
  const bottomMargin = 50;
  ctx.fillStyle="#001F00";
  var i;
  // Drawing the Grid
  ctx.beginPath();
  //horizontal
  for (i=leftMargin; i<1024; i +=100){
    ctx.moveTo(i, 768-bottomMargin);
    ctx.lineTo(i, 0);
  }
 //vertical
  for (i=bottomMargin; i<768;i +=50){
    ctx.moveTo(leftMargin, 768-i);
    ctx.lineTo(1024, 768-i);
  }
  ctx.stroke();
  
  ctx.strokeStyle="#00FF00";
  ctx.beginPath();

  ctx.moveTo(leftMargin, 768-bottomMargin);
  ctx.lineTo(1024, 500);

  ctx.stroke();
  
};