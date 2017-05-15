#ifndef WEBSERVERPAGES_H_
#define WEBSERVERPAGES_H

const char* MAIN_CSS =
"#loader {\
	display: none;\
	position: absolute;\
	left: 50%;\
	top: 50%;\
	z-index: 100;\
	width: 120px;\
	height: 120px;\
	margin: -75px 0 0 -75px;\
	border: 18px solid #888C46;\
	border-radius: 50%;\
	border-top: 18px solid #16235A;\
	border-bottom: 18px solid #16235A;\
	-webkit-animation: spin 2s linear infinite;\
	animation: spin 2s linear infinite;\
}\
\
@keyframes spin {\
	0% { transform: rotate(0deg); }\
	100% { transform: rotate(360deg); }\
}\
\
div.menu button {\
	display: block;\
	background-color: inherit;\
	color: #16235A;\
	background-color: #888C46;\
	padding: 32px 16px;\
	width: 100%;\
	border: none;\
	outline: none;\
	text-align: left;\
	cursor: pointer;\
	transition: 0.3s;\
	font-size: 18px;\
	font-weight: nornal;\
}\
\
div.menu button:hover {\
	background-color: #777C46;\
}\
\
div.menu button.active {\
	background-color: #F2EAED;\
	font-weight: bold;\
}\
\
.edit {\
	width: 100%;\
	font-size: 16px;\
	padding: 12px 20px 12px 8px;\
	border: 2px solid #A4A4BF;\
}\
\
.search {\
	margin-bottom: 22px;\
}\
\
.table {\
	border-collapse: collapse;\
	width: 100%;\
	border: 2px solid #A4A4BF;\
	border-left: none;\
	border-right: none;\
	font-size: 18px;\
	color: #16235A;\
}\
\
.table th, .table td {\
	text-align: left;\
	padding: 12px;\
}\
\
.table tr.header, .table tr:hover {\
	background-color: #A4A4BF;\
}\
\
.table tr {\
	border-bottom: 2px solid #A4A4BF;\
}\
\
.actionButton {\
	padding: 4px;\
	background-color: #F2EAED;\
	color: #16235A;\
	font-size: 18px;\
	border: 3px solid #A4A4BF;\
	border-radius: 8px;\
	transition-duration: 0.4s;\
}\
\
.actionButton:hover {\
	background-color: #888C46;\
	border: 3px solid #888C46;\
}\
\
.modal {\
	display: none;\
	position: fixed;\
	z-index: 1;\
	padding-top: 15%;\
	left: 0;\
	top: 0;\
	width: 100%;\
	height: 100%;\
	overflow: auto;\
	background-color: rgba(0,0,0,0.4);\
}\
\
.modalContent {\
	position: relative;\
	background-color: #F2EAED;\
	margin: auto;\
	padding: 0;\
	border: 2px solid #A4A4BF;\
	width: 50%;\
	box-shadow: 0 4px 8px 0 rgba(0,0,0,0.2),0 6px 20px 0 rgba(0,0,0,0.19);\
	-webkit-animation-name: animatetop;\
	-webkit-animation-duration: 0.6s;\
	animation-name: animatetop;\
	animation-duration: 0.6s\
}\
\
@keyframes animatetop {\
	from {top:-300px; opacity:0}\
	to {top:0; opacity:1}\
}\
\
.closeModal {\
	color: #F2EAED;\
	float: right;\
	font-size: 28px;\
	font-weight: bold;\
}\
\
.closeModal:hover, .closeModal:focus {\
	color: #16235A;\
	text-decoration: none;\
	cursor: pointer;\
}\
\
.modalHeader {\
	padding: 0px 16px 0px;\
	background-color: #888C46;\
	color: #F2EAED;\
	height: 40px;\
}\
\
.modalHeaderText {\
	padding: 8px;\
	font-size: 18px;\
}\
\
.modalBody {\
	padding: 20px 16px;\
}\
";

const char* TABLE_JS =
"function createTable(id, className) {\
	var table = document.createElement(\"table\");\
	table.setAttribute(\"id\", id);\
	table.setAttribute(\"class\", className);\
	return table;\
}\
function createTableBody() {\
	var tableBody = document.createElement(\"tbody\");\
	return tableBody;\
}\
function createTableRow(className) {\
	var tr = document.createElement(\"tr\");\
	tr.setAttribute(\"class\", className);\
	return tr;\
}\
function createHeaderColumn(width, name) {\
	var th = document.createElement(\"th\");\
	th.style.width = width;\
	th.innerHTML = name;\
	return th;\
}\
function createColumn(data) {\
	var td = document.createElement(\"td\");\
	if (data !== undefined) {\
		td.innerHTML = data;\
	}\
	return td;\
}\
function handleData(data, actionName, actionFunction) {\
	var table = createTable(\"table\", \"table\");\
	appendChild(document.getElementById(\"content\"), table);\
	var tableBody = createTableBody();\
	appendChild(table, tableBody);\
	var tr = createTableRow(\"header\");\
	appendChild(tableBody, tr);\
	if (data.length > 0) {\
		var keysCount = 0;\
		for (var key in data[0]) {\
			keysCount++;\
		}\
		if (actionFunction !== undefined) {\
			keysCount++;\
		}\
		for (var key in data[0]) {\
			appendChild(tr, createHeaderColumn(100/keysCount + \"%\", key.toUpperCase()));\
		}\
		if (actionFunction !== undefined) {\
			appendChild(tr, createHeaderColumn(100/keysCount + \"%\", \"ACTION\"));\
		}\
		for (var i = 0; i < data.length; i++) {\
			var trData = createTableRow();\
			appendChild(tableBody, trData);\
			for (var key in data[i]) {\
				appendChild(trData, createColumn(data[i][key]));\
			}\
			if (actionFunction !== undefined) {\
				var button = document.createElement(\"button\");\
				button.innerHTML = actionName;\
				button.setAttribute(\"class\", \"actionButton\");\
				button.onclick = (function() {\
					var row = data[i];\
					return function() {\
						actionFunction(row);\
					}\
				})();\
				var col = createColumn();\
				appendChild(col, button);\
				appendChild(trData, col);\
			}\
		}\
	}\
}\
";

const char* WIFISTATUS_JS =
"function loadWifiStatusContent() {\
	removeContent(document.getElementById(\"content\"));\
	appendChild(document.getElementById(\"content\"), createSearchBox());\
	var startHotspotButton = document.createElement(\"button\");\
	startHotspotButton.setAttribute(\"id\", \"startHotspotButton\");\
	startHotspotButton.setAttribute(\"class\", \"actionButton\");\
	startHotspotButton.setAttribute(\"style\", \"float: right; margin-bottom: 15px;\");\
	startHotspotButton.innerHTML = \"Start Access Point\";\
	startHotspotButton.onclick = function() {\
		var formContainer = document.createElement(\"div\");\
		var textElement = document.createElement(\"span\");\
		textElement.setAttribute(\"style\", \"color: #16235A; font-size: 20px;\");\
		textElement.innerHTML = \"Start Access Point ESP8266_AP/ESP8266_AP\";\
		appendChild(formContainer, textElement);\
		var buttonDiv = document.createElement(\"div\");\
		buttonDiv.setAttribute(\"style\", \"height: 45px\");\
		var startButton = document.createElement(\"button\");\
		startButton.setAttribute(\"id\", \"startButton\");\
		startButton.setAttribute(\"class\", \"actionButton\");\
		startButton.setAttribute(\"style\", \"float: right; margin-top: 15px;\");\
		startButton.innerHTML = \"Start\";\
		startButton.onclick = function() {\
			var xhttp = new XMLHttpRequest();\
			xhttp.onreadystatechange = function() {\
				if (this.readyState == 1) {\
					showLoading();\
				}\
				if (this.readyState == 4) {\
					hideLoading();\
					closeModal();\
					loadWifiStatusContent();\
				}\
			};\
			xhttp.open(\"GET\", \"/wifi/accesspoint/start\", true);\
			xhttp.send();\
		};\
		appendChild(buttonDiv, startButton);\
		appendChild(formContainer, buttonDiv);\
		openModal(\"Start Access Point\", formContainer);\
	};\
	appendChild(document.getElementById(\"content\"), startHotspotButton);\
	var xhttp = new XMLHttpRequest();\
	xhttp.onreadystatechange = function() {\
		if (this.readyState == 1) {\
			showLoading();\
		}\
		if (this.readyState == 4) {\
			hideLoading();\
			if (this.status == 200) {\
				var data = JSON.parse(this.responseText);\
				var tableData = [];\
				for (var key in data.network) {\
					var row = {};\
					row[\"property\"] = key;\
					row[\"value\"] = data.network[key];\
					tableData.push(row);\
				}\
				handleData(tableData);\
			} else if (this.status == 0) {\
				alert(\"Network Error\");\
			}\
		}\
	};\
	xhttp.open(\"GET\", \"/wifi/network/status\", true);\
	xhttp.send();\
}";

const char* WIFINETWORKS_JS =
"function loadWifiNetworksContent() {\
	removeContent(document.getElementById(\"content\"));\
	appendChild(document.getElementById(\"content\"), createSearchBox());\
	var xhttp = new XMLHttpRequest();\
	xhttp.onreadystatechange = function() {\
		if (this.readyState == 1) {\
			showLoading();\
		}\
		if (this.readyState == 4) {\
			hideLoading();\
			if (this.status == 200) {\
				document.getElementById(\"loader\").style.display = \"none\";\
			    var data = JSON.parse(this.responseText);\
				handleData(data.networks, \"Connect\", function(row) {\
					var formContainer = document.createElement(\"div\");\
					var inputElement = document.createElement(\"input\");\
					inputElement.setAttribute(\"id\", \"wifiPassword\");\
					inputElement.setAttribute(\"type\", \"text\");\
					inputElement.setAttribute(\"class\", \"edit\");\
					inputElement.setAttribute(\"placeholder\", \"Enter Password\");\
					appendChild(formContainer, inputElement);\
					var buttonDiv = document.createElement(\"div\");\
					buttonDiv.setAttribute(\"style\", \"height: 45px\");\
					var cb = document.createElement(\"button\");\
					cb.setAttribute(\"id\", \"cb\");\
					cb.setAttribute(\"class\", \"actionButton\");\
					cb.setAttribute(\"style\", \"float: right; margin-top: 15px;\");\
					cb.innerHTML = \"Connect\";\
					cb.onclick = function() {\
						var dataToPost = {};\
						dataToPost[\"ssid\"] = row.ssid;\
						dataToPost[\"password\"] = document.getElementById(\"wifiPassword\").value;\
						var xhttp = new XMLHttpRequest();\
						xhttp.onreadystatechange = function() {\
							if (this.readyState == 1) {\
								showLoading();\
							}\
							if (this.readyState == 4) {\
								hideLoading();\
								closeModal();\
								loadWifiNetworksContent();\
							}\
						};\
						xhttp.open(\"POST\", \"/wifi/network/connect\", true);\
						xhttp.send(\"payload=\" + JSON.stringify(dataToPost));\
					};\
					appendChild(buttonDiv, cb);\
					appendChild(formContainer, buttonDiv);\
					openModal(\"Connect to \" + row.ssid + \" network\", formContainer);\
				});\
			} else if (this.status == 0) {\
				alert(\"Network Error\");\
			}\
		}\
	};\
	xhttp.open(\"GET\", \"/wifi/networks\", true);\
	xhttp.send();\
}";

const char* MQTTSTATUS_JS =
"function loadMqttContent() {\
	removeContent(document.getElementById(\"content\"));\
	appendChild(document.getElementById(\"content\"), createSearchBox());\
	var mcb = document.createElement(\"button\");\
	mcb.setAttribute(\"id\", \"mcb\");\
	mcb.setAttribute(\"class\", \"actionButton\");\
	mcb.setAttribute(\"style\", \"float: right; margin-bottom: 15px;\");\
	mcb.innerHTML = \"Connect to MQTT\";\
	mcb.onclick = function() {\
		var formContainer = document.createElement(\"div\");\
		var hic = createDivContainer();\
		appendChild(hic, createEdit(\"mqttHost\", \"MQTT Host:\", \"Enter Host\"));\
		appendChild(formContainer, hic);\
		var pic = createDivContainer();\
		appendChild(pic, createEdit(\"mqttPort\", \"MQTT Port:\", \"Enter Port\"));\
		appendChild(formContainer, pic);\
		var uic = createDivContainer();\
		appendChild(uic, createEdit(\"mqttUser\", \"MQTT User:\", \"Enter Username\"));\
		appendChild(formContainer, uic);\
		var pwdic = createDivContainer();\
		appendChild(pwdic, createEdit(\"mqttPassword\", \"MQTT Password:\", \"Enter Password\"));\
		appendChild(formContainer, pwdic);\
		var bd = document.createElement(\"div\");\
		bd.setAttribute(\"style\", \"height: 45px\");\
		var cb = document.createElement(\"button\");\
		cb.setAttribute(\"id\", \"cb\");\
		cb.setAttribute(\"class\", \"actionButton\");\
		cb.setAttribute(\"style\", \"float: right; margin-top: 15px;\");\
		cb.innerHTML = \"Connect\";\
		cb.onclick = function() {\
			var dataToPost = {};\
			dataToPost[\"host\"] = document.getElementById(\"mqttHost\").value;\
			dataToPost[\"port\"] = document.getElementById(\"mqttPort\").value;\
			dataToPost[\"user\"] = document.getElementById(\"mqttUser\").value;;\
			dataToPost[\"password\"] = document.getElementById(\"mqttPassword\").value;\
			var xhttp = new XMLHttpRequest();\
			xhttp.onreadystatechange = function() {\
				if (this.readyState == 1) {\
					showLoading();\
				}\
				if (this.readyState == 4) {\
					hideLoading();\
					closeModal();\
					loadMqttContent();\
				}\
			};\
			xhttp.open(\"POST\", \"/mqtt/connect\", true);\
			xhttp.send(\"payload=\" + JSON.stringify(dataToPost));\
		};\
		appendChild(bd, cb);\
		appendChild(formContainer, bd);\
		openModal(\"New MQTT Parameters\", formContainer);\
	};\
	appendChild(document.getElementById(\"content\"), mcb);\
	var xhttp = new XMLHttpRequest();\
	xhttp.onreadystatechange = function() {\
		if (this.readyState == 1) {\
			showLoading();\
		}\
		if (this.readyState == 4) {\
			hideLoading();\
			if (this.status == 200) {\
				var data = JSON.parse(this.responseText);\
				var tableData = [];\
				for (var key in data.mqtt) {\
					var row = {};\
					row[\"property\"] = key;\
					row[\"value\"] = data.mqtt[key];\
					tableData.push(row);\
				}\
				handleData(tableData);\
			} else if (this.status == 0) {\
				alert(\"Network Error\");\
			}\
		}\
	};\
	xhttp.open(\"GET\", \"/mqtt/status\", true);\
	xhttp.send();\
}";

const char* PINSTATUS_JS =
"function loadPinsContent() {\
	removeContent(document.getElementById(\"content\"));\
	appendChild(document.getElementById(\"content\"), createSearchBox());\
	var xhttp = new XMLHttpRequest();\
	xhttp.onreadystatechange = function() {\
		if (this.readyState == 1) {\
			showLoading();\
		}\
		if (this.readyState == 4) {\
			hideLoading();\
			if (this.status == 200) {\
				var data = JSON.parse(this.responseText);\
				handleData(data.pins, \"Set\", function(row) {\
					var formContainer = document.createElement(\"div\");\
					var inputElement = document.createElement(\"input\");\
					inputElement.setAttribute(\"id\", \"updatePin\");\
					inputElement.setAttribute(\"type\", \"text\");\
					inputElement.setAttribute(\"class\", \"edit\");\
					inputElement.value = row.value;\
					appendChild(formContainer, inputElement);\
					var buttonDiv = document.createElement(\"div\");\
					buttonDiv.setAttribute(\"style\", \"height: 45px\");\
					var updatePinButton = document.createElement(\"button\");\
					updatePinButton.setAttribute(\"id\", \"updatePinButton\");\
					updatePinButton.setAttribute(\"class\", \"actionButton\");\
					updatePinButton.setAttribute(\"style\", \"float: right; margin-top: 15px;\");\
					updatePinButton.innerHTML = \"Update\";\
					updatePinButton.onclick = function() {\
						var dataToPost = {};\
						dataToPost[\"pin\"] = row.pin;\
						dataToPost[\"value\"] = document.getElementById(\"updatePin\").value;\
						var xhttp = new XMLHttpRequest();\
						xhttp.onreadystatechange = function() {\
							if (this.readyState == 1) {\
								showLoading();\
							}\
							if (this.readyState == 4) {\
								hideLoading();\
								closeModal();\
								loadPinsContent();\
							}\
						};\
						xhttp.open(\"POST\", \"/pin/set\", true);\
						xhttp.send(\"payload=\" + JSON.stringify(dataToPost));\
					};\
					appendChild(buttonDiv, updatePinButton);\
					appendChild(formContainer, buttonDiv);								\
					openModal(\"Set \" + row.name + \" value\", formContainer);\
				});\
			} else if (this.status == 0) {\
				alert(\"Network Error\");\
			}\
		}\
	};\
	xhttp.open(\"GET\", \"/pin/status\", true);\
	xhttp.send();\
}";

const char* MAIN_JS =
"function handleEvent(event) {\
	var menuItems = document.getElementsByClassName(\"menuItem\");\
	for (var i = 0; i < menuItems.length; i++) {\
		menuItems[i].className = menuItems[i].className.replace(\" active\", \"\");\
	}\
	event.currentTarget.className += \" active\";\
	var menuId = event.currentTarget.id;\
	if (menuId == \"wifiStatus\") {\
		loadWifiStatusContent();\
	} else if (menuId == \"wifiNetworks\") {\
		loadWifiNetworksContent();\
	} else if (menuId == \"mqttManager\") {\
		loadMqttContent();\
	} else if (menuId == \"pinsManager\") {\
		loadPinsContent();\
	} \
}\
function removeContent(container) {\
	while (container.firstChild) {\
		container.removeChild(container.firstChild);\
	}\
}\
function showLoading() {\
	document.getElementById(\"loader\").style.display = \"block\";\
}\
function hideLoading() {\
	document.getElementById(\"loader\").style.display = \"none\";\
}\
function openModal(title, content) {\
	document.getElementById(\"modal\").style.display = \"block\";			\
	document.getElementById(\"modalHeaderText\").innerHTML = title;\
	appendChild(document.getElementById(\"modalBody\"), content);\
}\
function closeModal() {\
	document.getElementById(\"modal\").style.display = \"none\";\
	removeContent(document.getElementById(\"modalBody\"));\
}\
function createSearchBox() {\
	var inputElement = document.createElement(\"input\");\
	inputElement.setAttribute(\"id\", \"search\");\
	inputElement.setAttribute(\"type\", \"text\");\
	inputElement.setAttribute(\"class\", \"search edit\");\
	inputElement.setAttribute(\"placeholder\", \"Search for Items...\");\
	return inputElement;\
}\
function createEdit(id, label, placeholder) {\
	var labelEl = document.createElement(\"label\");\
	labelEl.setAttribute(\"for\", \"id\");\
	labelEl.setAttribute(\"style\", \"color: #16235A; font-size: 18px;\");\
	labelEl.innerHTML = label;\
	var inputEl = document.createElement(\"input\");\
	inputEl.setAttribute(\"id\", id);\
	inputEl.setAttribute(\"type\", \"text\");\
	inputEl.setAttribute(\"class\", \"edit\");\
	inputEl.setAttribute(\"placeholder\", placeholder);\
	appendChild(labelEl, inputEl);\
	return labelEl;\
}\
function createDivContainer() {\
	var div = document.createElement(\"div\");\
	div.setAttribute(\"style\", \"width: 100%;\");\
	return div;\
}\
function appendChild(parent, child) {\
	parent.appendChild(child);\
}";

const char* MAIN_HTML =
"<html>\
	<head>\
		<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\
		<title>test post</title>\
		<link rel=\"stylesheet\" type=\"text/css\" href=\"/main.css\">		\
		<script async=\"false\" src=\"/table.js\"></script>\
		<script async=\"false\" src=\"/wifistatus.js\"></script>\
		<script async=\"false\" src=\"/wifinetworks.js\"></script>\
		<script async=\"false\" src=\"/mqttstatus.js\"></script>\
		<script async=\"false\" src=\"/pinstatus.js\"></script>\
		<script async=\"false\" src=\"/main.js\"></script>\
		<script async=\"false\" type=\"text/javascript\">\
			window.onload = main;\
			function main() {\
				loadWifiStatusContent();\
			}\
		</script>\
	</head>\
	<body>\
		<div id=\"loader\"></div>\
		<div id=\"mainContainer\" style=\"width:100%\">\
			<div id=\"topContainer\" style=\"float:top; width:100%; height:20%; background:#888C46\">\
				<div id=\"topContainerHeader\" style=\"position: relative; top: 50%; transform: translateY(-50%); text-align: center; font-size: 50px; color: #16235A;\">\
					ESP8266 Manager\
				</div>\
			</div>\
			<div id=\"bottomContainer\" style=\"float:bottom; width:100%; height:80%; background:#888C46\">\
				<div id=\"leftContainer\" style=\"float:left; width:15%; height:100%; background:green\">\
					<div class=\"menu\" style=\"float:left; background:#888C46; width:100%; height:100%;\">\
						<button id=\"wifiStatus\" class=\"menuItem active\" onclick=\"handleEvent(event)\">WiFi Status</button>\
						<button id=\"wifiNetworks\" class=\"menuItem\" onclick=\"handleEvent(event)\">WiFi Networks</button>\
						<button id=\"mqttManager\" class=\"menuItem\" onclick=\"handleEvent(event)\">MQTT Manager</button>\
						<button id=\"pinsManager\" class=\"menuItem\" onclick=\"handleEvent(event)\">Pins Manager</button>\
					</div>\
				</div>\
				<div id=\"rightContainer\" style=\"float:right; width:85%; height:100%; background:#F2EAED; overflow: auto;\">\
					<div id=\"content\" style=\"width: 90%; height: 90%; position: relative; margin: 0 auto; top: 50%; transform: translateY(-50%);\">\
					</div>\
				</div>\
			</div>\
		</div>\
		<div id=\"modal\" class=\"modal\">\
			<div id=\"modalContent\" class=\"modalContent\">\
				<div id=\"modalHeader\" class=\"modalHeader\">\
					<span id=\"closeModal\" class=\"closeModal\" onclick=\"closeModal();\">&times;</span>\
					<div id=\"modalHeaderText\" class=\"modalHeaderText\"></div>\
				</div>\
				<div id=\"modalBody\" class=\"modalBody\">\
				</div>\
			</div>\
		</div>\
	</body>\
</html>";
#endif /* WEBSERVERPAGES_H_ */
