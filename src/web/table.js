function createTable(id, className) {
	var table = document.createElement("table");
	table.setAttribute("id", id);
	table.setAttribute("class", className);
	return table;
}
			
function createTableBody() {
	var tableBody = document.createElement("tbody");
	return tableBody;
}
			
function createTableRow(className) {
	var tr = document.createElement("tr");
	tr.setAttribute("class", className);
	return tr;
}
			
function createHeaderColumn(width, name) {
	var th = document.createElement("th");
	th.style.width = width;
	th.innerHTML = name;
	return th;
}
			
function createColumn(data) {
	var td = document.createElement("td");
	if (data !== undefined) {
		td.innerHTML = data;
	}
	return td;
}
			
function handleData(data, actionName, actionFunction) {
	var table = createTable("table", "table");				
	appendChild(document.getElementById("content"), table);
				
	var tableBody = createTableBody();
	appendChild(table, tableBody);
		
	var tr = createTableRow("header");
	appendChild(tableBody, tr);
				
	if (data.length > 0) {
		var keysCount = 0;
		for (var key in data[0]) {
			keysCount++;
		}
		if (actionFunction !== undefined) {
			keysCount++;
		}
		for (var key in data[0]) {
			appendChild(tr, createHeaderColumn(100/keysCount + "%", key.toUpperCase()));
		}
		if (actionFunction !== undefined) {
			appendChild(tr, createHeaderColumn(100/keysCount + "%", "ACTION"));
		}
			
		for (var i = 0; i < data.length; i++) {
			var trData = createTableRow();
			appendChild(tableBody, trData);
			for (var key in data[i]) {
				appendChild(trData, createColumn(data[i][key]));
			}
			if (actionFunction !== undefined) {
				var button = document.createElement("button");
				button.innerHTML = actionName;
				button.setAttribute("class", "actionButton");							
				button.onclick = (function() {
					var row = data[i];
					return function() {
						actionFunction(row);
					}
				})();
				var col = createColumn();
				appendChild(col, button);
				appendChild(trData, col);
			}
		}
	}
}