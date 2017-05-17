function loadWifiStatusContent() {
	removeContent(document.getElementById("content"));
			
	//appendChild(document.getElementById("content"), createSearchBox());
				
	var startHotspotButton = document.createElement("button");
	startHotspotButton.setAttribute("id", "startHotspotButton");
	startHotspotButton.setAttribute("class", "actionButton");
	startHotspotButton.setAttribute("style", "float: right; margin-bottom: 15px;")
	startHotspotButton.innerHTML = "Start Access Point";
	startHotspotButton.onclick = function() {
		var formContainer = document.createElement("div");
					
		var textElement = document.createElement("span");
		textElement.setAttribute("style", "color: #16235A; font-size: 20px;");
		textElement.innerHTML = "Start Access Point ESP8266_AP/ESP8266_AP";
		appendChild(formContainer, textElement);
					
		var buttonDiv = document.createElement("div");
		buttonDiv.setAttribute("style", "height: 45px");
					
		var startButton = document.createElement("button");
		startButton.setAttribute("id", "startButton");
		startButton.setAttribute("class", "actionButton");
		startButton.setAttribute("style", "float: right; margin-top: 15px;");
		startButton.innerHTML = "Start";
		startButton.onclick = function() {
			var xhttp = new XMLHttpRequest();
			xhttp.onreadystatechange = function() {
				if (this.readyState == 1) {
					showLoading();
				}
							
				if (this.readyState == 4) {
					hideLoading();
					closeModal();
					loadWifiStatusContent();
				}
			};
			xhttp.open("GET", "http://192.168.1.4/wifi/accesspoint/start", true);
			xhttp.send();
		};
		appendChild(buttonDiv, startButton);
		appendChild(formContainer, buttonDiv);
		openModal("Start Access Point", formContainer);
	};
	appendChild(document.getElementById("content"), startHotspotButton);
				
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (this.readyState == 1) {
			showLoading();
		}
					
		if (this.readyState == 4) {
			hideLoading();
			if (this.status == 200) {
				var data = JSON.parse(this.responseText);
				var tableData = [];
				for (var key in data.network) {
					var row = {};
					row["property"] = key;
					row["value"] = data.network[key];
					tableData.push(row);
				}
				handleData(tableData);
			} else if (this.status == 0) {
				alert("Network Error");
			}
		}
	}
	xhttp.open("GET", "http://192.168.1.4/wifi/network/status", true);
	xhttp.send();
}