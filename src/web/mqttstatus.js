function loadMqttContent() {
	removeContent(document.getElementById("content"));

	appendChild(document.getElementById("content"), createSearchBox());

	var mcb = document.createElement("button");
	mcb.setAttribute("id", "mcb");
	mcb.setAttribute("class", "actionButton");
	mcb.setAttribute("style", "float: right; margin-bottom: 15px;")
	mcb.innerHTML = "Connect to MQTT";
	mcb.onclick = function() {
		var formContainer = document.createElement("div");

		var hic = createDivContainer();
		appendChild(hic, createEdit("mqttHost", "MQTT Host:", "Enter Host"));
		appendChild(formContainer, hic);

		var pic = createDivContainer();
		appendChild(pic, createEdit("mqttPort", "MQTT Port:", "Enter Port"));
		appendChild(formContainer, pic);

		var uic = createDivContainer();
		appendChild(uic, createEdit("mqttUser", "MQTT User:", "Enter Username"));
		appendChild(formContainer, uic);

		var pwdic = createDivContainer();
		appendChild(pwdic, createEdit("mqttPassword", "MQTT Password:", "Enter Password"));
		appendChild(formContainer, pwdic);

		var bd = document.createElement("div");
		bd.setAttribute("style", "height: 45px");

		var cb = document.createElement("button");
		cb.setAttribute("id", "cb");
		cb.setAttribute("class", "actionButton");
		cb.setAttribute("style", "float: right; margin-top: 15px;");
		cb.innerHTML = "Connect";
		cb.onclick = function() {
			var dataToPost = {};
			dataToPost["host"] = document.getElementById("mqttHost").value;
			dataToPost["port"] = document.getElementById("mqttPort").value;
			dataToPost["user"] = document.getElementById("mqttUser").value;;
			dataToPost["password"] = document.getElementById("mqttPassword").value;

			var xhttp = new XMLHttpRequest();
			xhttp.onreadystatechange = function() {
				if (this.readyState == 1) {
					showLoading();
				}

				if (this.readyState == 4) {
					hideLoading();
					closeModal();
					loadMqttContent();
				}
			};
			xhttp.open("POST", "http://192.168.1.4/mqtt/connect", true);
			xhttp.send("payload=" + JSON.stringify(dataToPost));
		};
		appendChild(bd, cb);
		appendChild(formContainer, bd);
		openModal("New MQTT Parameters", formContainer);
	};
	appendChild(document.getElementById("content"), mcb);

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
				for (var key in data.mqtt) {
					var row = {};
					row["property"] = key;
					row["value"] = data.mqtt[key];
					tableData.push(row);
				}
				handleData(tableData);
			} else if (this.status == 0) {
				alert("Network Error");
			}
		}
	}
	xhttp.open("GET", "http://192.168.1.4/mqtt/status", true);
	xhttp.send();
}