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
	  };

	  reader.readAsBinaryString(file);
	});
  });
  var number;
});
