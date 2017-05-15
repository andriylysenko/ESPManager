function loadWifiNetworksContent() {
	removeContent(document.getElementById("content"));
				
	appendChild(document.getElementById("content"), createSearchBox());
				
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (this.readyState == 1) {
			showLoading();
		}
					
		if (this.readyState == 4) {
			hideLoading();
			if (this.status == 200) {
				document.getElementById("loader").style.display = "none";
			    var data = JSON.parse(this.responseText);
				handleData(data.networks, "Connect", function(row) {
					var formContainer = document.createElement("div");
						
					var inputElement = document.createElement("input");
					inputElement.setAttribute("id", "wifiPassword");
					inputElement.setAttribute("type", "text");
					inputElement.setAttribute("class", "edit");
					inputElement.setAttribute("placeholder", "Enter Password");
					appendChild(formContainer, inputElement);
						
					var buttonDiv = document.createElement("div");
					buttonDiv.setAttribute("style", "height: 45px");
							
					var connectButton = document.createElement("button");
					connectButton.setAttribute("id", "connectButton");
					connectButton.setAttribute("class", "actionButton");
					connectButton.setAttribute("style", "float: right; margin-top: 15px;");
					connectButton.innerHTML = "Connect";
					connectButton.onclick = function() {
						var dataToPost = {};
						dataToPost["ssid"] = row.ssid;
						dataToPost["password"] = document.getElementById("wifiPassword").value;
						var xhttp = new XMLHttpRequest();
						xhttp.onreadystatechange = function() {
							if (this.readyState == 1) {
								showLoading();
							}
								
							if (this.readyState == 4) {
								hideLoading();
								closeModal();
								loadWifiNetworksContent();
							}
						};
						xhttp.open("POST", "http://192.168.1.4/wifi/network/connect", true);
						xhttp.send("payload=" + JSON.stringify(dataToPost));
					};
					appendChild(buttonDiv, connectButton);
					appendChild(formContainer, buttonDiv);								
					openModal("Connect to " + row.ssid + " network", formContainer);	
				});
			} else if (this.status == 0) {
				alert("Network Error");
			}
		}
	}
	xhttp.open("GET", "http://192.168.1.4/wifi/networks", true);
	xhttp.send();
}