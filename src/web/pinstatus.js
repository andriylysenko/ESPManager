function loadPinsContent() {
	removeContent(document.getElementById("content"));
				
	//appendChild(document.getElementById("content"), createSearchBox());
			
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (this.readyState == 1) {
			showLoading();
		}
					
		if (this.readyState == 4) {
			hideLoading();
			if (this.status == 200) {
				var data = JSON.parse(this.responseText);
				handleData(data.pins, "Set", function(row) {
					var formContainer = document.createElement("div");
						
					var inputElement = document.createElement("input");
					inputElement.setAttribute("id", "updatePin");
					inputElement.setAttribute("type", "text");
					inputElement.setAttribute("class", "edit");
					inputElement.value = row.value;
					appendChild(formContainer, inputElement);
								
					var buttonDiv = document.createElement("div");
					buttonDiv.setAttribute("style", "height: 45px");
								
					var updatePinButton = document.createElement("button");
					updatePinButton.setAttribute("id", "updatePinButton");
					updatePinButton.setAttribute("class", "actionButton");
					updatePinButton.setAttribute("style", "float: right; margin-top: 15px;")
					updatePinButton.innerHTML = "Update";
					updatePinButton.onclick = function() {
						var dataToPost = {};
						dataToPost["pin"] = row.pin;
						dataToPost["value"] = document.getElementById("updatePin").value;
						var xhttp = new XMLHttpRequest();
						xhttp.onreadystatechange = function() {
							if (this.readyState == 1) {
								showLoading();
							}
										
							if (this.readyState == 4) {
								hideLoading();
								closeModal();
								loadPinsContent();
							}
						};
						xhttp.open("POST", "http://192.168.1.4/pin/set", true);
						xhttp.send("payload=" + JSON.stringify(dataToPost));
					};
					appendChild(buttonDiv, updatePinButton);
					appendChild(formContainer, buttonDiv);								
					openModal("Set " + row.name + " value", formContainer);
				});
			} else if (this.status == 0) {
				alert("Network Error");
			}
		}
	}
	xhttp.open("GET", "http://192.168.1.4/pin/status", true);
	xhttp.send();
}