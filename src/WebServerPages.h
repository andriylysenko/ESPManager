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

const char* TABLE_JS = "function createTable(a,b){var c=document.createElement(\"table\");c.setAttribute(\"id\",a);c.setAttribute(\"class\",b);return c}function createTableBody(){return document.createElement(\"tbody\")}function createTableRow(a){var b=document.createElement(\"tr\");b.setAttribute(\"class\",a);return b}function createHeaderColumn(a,b){var c=document.createElement(\"th\");c.style.width=a;c.innerHTML=b;return c}function createColumn(a){var b=document.createElement(\"td\");void 0!==a&&(b.innerHTML=a);return b}function handleData(a,b,c){var d=createTable(\"table\",\"table\");appendChild(document.getElementById(\"content\"),d);var h=createTableBody();appendChild(d,h);d=createTableRow(\"header\");appendChild(h,d);if(0<a.length){var e=0,f;for(f in a[0])e++;void 0!==c&&e++;for(f in a[0])appendChild(d,createHeaderColumn(100/e+\"%\",f.toUpperCase()));void 0!==c&&appendChild(d,createHeaderColumn(100/e+\"%\",\"ACTION\"));for(var g=0;g<a.length;g++){d=createTableRow();appendChild(h,d);for(f in a[g])appendChild(d,createColumn(a[g][f]));if(void 0!==c){e=document.createElement(\"button\");e.innerHTML=b;e.setAttribute(\"class\",\"actionButton\");e.onclick=function(){var b=a[g];return function(){c(b)}}();var k=createColumn();appendChild(k,e);appendChild(d,k)}}}};";

const char* WIFISTATUS_JS = "function loadWifiStatusContent(){removeContent(document.getElementById(\"content\"));var a=document.createElement(\"button\");a.setAttribute(\"id\",\"startHotspotButton\");a.setAttribute(\"class\",\"actionButton\");a.setAttribute(\"style\",\"float: right; margin-bottom: 15px;\");a.innerHTML=\"Start Access Point\";a.onclick=function(){var a=document.createElement(\"div\"),b=document.createElement(\"span\");b.setAttribute(\"style\",\"color: #16235A; font-size: 20px;\");b.innerHTML=\"Start Access Point ESP8266_AP/ESP8266_AP\";appendChild(a,b);b=document.createElement(\"div\");b.setAttribute(\"style\",\"height: 45px\");var c=document.createElement(\"button\");c.setAttribute(\"id\",\"startButton\");c.setAttribute(\"class\",\"actionButton\");c.setAttribute(\"style\",\"float: right; margin-top: 15px;\");c.innerHTML=\"Start\";c.onclick=function(){var a=new XMLHttpRequest;a.onreadystatechange=function(){1==this.readyState&&showLoading();4==this.readyState&&(hideLoading(),closeModal(),loadWifiStatusContent())};a.open(\"GET\",\"http://192.168.1.4/wifi/accesspoint/start\",!0);a.send()};appendChild(b,c);appendChild(a,b);openModal(\"Start Access Point\",a)};appendChild(document.getElementById(\"content\"),a);a=new XMLHttpRequest;a.onreadystatechange=function(){1==this.readyState&&showLoading();if(4==this.readyState)if(hideLoading(),200==this.status){var a=JSON.parse(this.responseText),b=[],c;for(c in a.network){var d={};d.property=c;d.value=a.network[c];b.push(d)}handleData(b)}else 0==this.status&&alert(\"Network Error\")};a.open(\"GET\",\"http://192.168.1.4/wifi/network/status\",!0);a.send()};";

const char* WIFINETWORKS_JS = "function loadWifiNetworksContent(){removeContent(document.getElementById(\"content\"));var c=new XMLHttpRequest;c.onreadystatechange=function(){1==this.readyState&&showLoading();if(4==this.readyState)if(hideLoading(),200==this.status){document.getElementById(\"loader\").style.display=\"none\";var c=JSON.parse(this.responseText);handleData(c.networks,\"Connect\",function(c){var d=document.createElement(\"div\"),a=document.createElement(\"input\");a.setAttribute(\"id\",\"wifiPassword\");a.setAttribute(\"type\",\"text\");a.setAttribute(\"class\",\"edit\");a.setAttribute(\"placeholder\",\"Enter Password\");appendChild(d,a);a=document.createElement(\"div\");a.setAttribute(\"style\",\"height: 45px\");var b=document.createElement(\"button\");b.setAttribute(\"id\",\"connectButton\");b.setAttribute(\"class\",\"actionButton\");b.setAttribute(\"style\",\"float: right; margin-top: 15px;\");b.innerHTML=\"Connect\";b.onclick=function(){var a={};a.ssid=c.ssid;a.password=document.getElementById(\"wifiPassword\").value;var b=new XMLHttpRequest;b.onreadystatechange=function(){1==this.readyState&&showLoading();4==this.readyState&&(hideLoading(),closeModal(),loadWifiNetworksContent())};b.open(\"POST\",\"http://192.168.1.4/wifi/network/connect\",!0);b.send(\"payload=\"+JSON.stringify(a))};appendChild(a,b);appendChild(d,a);openModal(\"Connect to \"+c.ssid+\" network\",d)})}else 0==this.status&&alert(\"Network Error\")};c.open(\"GET\",\"http://192.168.1.4/wifi/networks\",!0);c.send()};";

const char* MQTTSTATUS_JS = "function loadMqttContent(){removeContent(document.getElementById(\"content\"));var a=document.createElement(\"button\");a.setAttribute(\"id\",\"mcb\");a.setAttribute(\"class\",\"actionButton\");a.setAttribute(\"style\",\"float: right; margin-bottom: 15px;\");a.innerHTML=\"Connect to MQTT\";a.onclick=function(){var a=document.createElement(\"div\"),b=createDivContainer();appendChild(b,createEdit(\"mqttHost\",\"MQTT Host:\",\"Enter Host\"));appendChild(a,b);b=createDivContainer();appendChild(b,createEdit(\"mqttPort\",\"MQTT Port:\",\"Enter Port\"));appendChild(a,b);b=createDivContainer();appendChild(b,createEdit(\"mqttUser\",\"MQTT User:\",\"Enter Username\"));appendChild(a,b);b=createDivContainer();appendChild(b,createEdit(\"mqttPassword\",\"MQTT Password:\",\"Enter Password\"));appendChild(a,b);b=document.createElement(\"div\");b.setAttribute(\"style\",\"height: 45px\");var c=document.createElement(\"button\");c.setAttribute(\"id\",\"cb\");c.setAttribute(\"class\",\"actionButton\");c.setAttribute(\"style\",\"float: right; margin-top: 15px;\");c.innerHTML=\"Connect\";c.onclick=function(){var b={};b.host=document.getElementById(\"mqttHost\").value;b.port=document.getElementById(\"mqttPort\").value;b.user=document.getElementById(\"mqttUser\").value;b.password=document.getElementById(\"mqttPassword\").value;var a=new XMLHttpRequest;a.onreadystatechange=function(){1==this.readyState&&showLoading();4==this.readyState&&(hideLoading(),closeModal(),loadMqttContent())};a.open(\"POST\",\"http://192.168.1.4/mqtt/connect\",!0);a.send(\"payload=\"+JSON.stringify(b))};appendChild(b,c);appendChild(a,b);openModal(\"New MQTT Parameters\",a)};appendChild(document.getElementById(\"content\"),a);a=new XMLHttpRequest;a.onreadystatechange=function(){1==this.readyState&&showLoading();if(4==this.readyState)if(hideLoading(),200==this.status){var a=JSON.parse(this.responseText),b=[],c;for(c in a.mqtt){var d={};d.property=c;d.value=a.mqtt[c];b.push(d)}handleData(b)}else 0==this.status&&alert(\"Network Error\")};a.open(\"GET\",\"http://192.168.1.4/mqtt/status\",!0);a.send()};";

const char* PINSTATUS_JS = "function loadPinsContent(){removeContent(document.getElementById(\"content\"));var c=new XMLHttpRequest;c.onreadystatechange=function(){1==this.readyState&&showLoading();if(4==this.readyState)if(hideLoading(),200==this.status){var c=JSON.parse(this.responseText);handleData(c.pins,\"Set\",function(c){var d=document.createElement(\"div\"),a=document.createElement(\"input\");a.setAttribute(\"id\",\"updatePin\");a.setAttribute(\"type\",\"text\");a.setAttribute(\"class\",\"edit\");a.value=c.value;appendChild(d,a);a=document.createElement(\"div\");a.setAttribute(\"style\",\"height: 45px\");var b=document.createElement(\"button\");b.setAttribute(\"id\",\"updatePinButton\");b.setAttribute(\"class\",\"actionButton\");b.setAttribute(\"style\",\"float: right; margin-top: 15px;\");b.innerHTML=\"Update\";b.onclick=function(){var a={};a.pin=c.pin;a.value=document.getElementById(\"updatePin\").value;var b=new XMLHttpRequest;b.onreadystatechange=function(){1==this.readyState&&showLoading();4==this.readyState&&(hideLoading(),closeModal(),loadPinsContent())};b.open(\"POST\",\"http://192.168.1.4/pin/set\",!0);b.send(\"payload=\"+JSON.stringify(a))};appendChild(a,b);appendChild(d,a);openModal(\"Set \"+c.name+\" value\",d)})}else 0==this.status&&alert(\"Network Error\")};c.open(\"GET\",\"http://192.168.1.4/pin/status\",!0);c.send()};";

const char* MAIN_JS = "function handleEvent(a){for(var b=document.getElementsByClassName(\"menuItem\"),c=0;c<b.length;c++)b[c].className=b[c].className.replace(\" active\",\"\");a.currentTarget.className+=\" active\";a=a.currentTarget.id;\"wifiStatus\"==a?loadWifiStatusContent():\"wifiNetworks\"==a?loadWifiNetworksContent():\"mqttManager\"==a?loadMqttContent():\"pinsManager\"==a&&loadPinsContent()}function removeContent(a){for(;a.firstChild;)a.removeChild(a.firstChild)}function showLoading(){document.getElementById(\"loader\").style.display=\"block\"}function hideLoading(){document.getElementById(\"loader\").style.display=\"none\"}function openModal(a,b){document.getElementById(\"modal\").style.display=\"block\";document.getElementById(\"modalHeaderText\").innerHTML=a;appendChild(document.getElementById(\"modalBody\"),b)}function closeModal(){document.getElementById(\"modal\").style.display=\"none\";removeContent(document.getElementById(\"modalBody\"))}function createEdit(a,b,c){var d=document.createElement(\"label\");d.setAttribute(\"for\",\"id\");d.setAttribute(\"style\",\"color: #16235A; font-size: 18px;\");d.innerHTML=b;b=document.createElement(\"input\");b.setAttribute(\"id\",a);b.setAttribute(\"type\",\"text\");b.setAttribute(\"class\",\"edit\");b.setAttribute(\"placeholder\",c);appendChild(d,b);return d}function createDivContainer(){var a=document.createElement(\"div\");a.setAttribute(\"style\",\"width: 100%;\");return a}function appendChild(a,b){a.appendChild(b)};";

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
