function handleEvent(event) {
	var menuItems = document.getElementsByClassName("menuItem");
	for (var i = 0; i < menuItems.length; i++) {
		menuItems[i].className = menuItems[i].className.replace(" active", "");
	}
	event.currentTarget.className += " active";
			
	var menuId = event.currentTarget.id;
	if (menuId == "wifiStatus") {
		loadWifiStatusContent();
	} else if (menuId == "wifiNetworks") {
		loadWifiNetworksContent();
	} else if (menuId == "mqttManager") {
		loadMqttContent();
	} else if (menuId == "pinsManager") {
		loadPinsContent();
	} 
}
			
function removeContent(container) {
	while (container.firstChild) {
		container.removeChild(container.firstChild);
	}
}
						
function showLoading() {
	document.getElementById("loader").style.display = "block";
}
			
function hideLoading() {
	document.getElementById("loader").style.display = "none";
}
			
function openModal(title, content) {
	document.getElementById("modal").style.display = "block";			
	document.getElementById("modalHeaderText").innerHTML = title;
	appendChild(document.getElementById("modalBody"), content);
}
			
function closeModal() {
	document.getElementById("modal").style.display = "none";
	removeContent(document.getElementById("modalBody"));
}
			
function createSearchBox() {
	var inputElement = document.createElement("input");
	inputElement.setAttribute("id", "search");
	inputElement.setAttribute("type", "text");
	inputElement.setAttribute("class", "search edit");
	inputElement.setAttribute("placeholder", "Search for Items...");
	return inputElement;
}

function createEdit(id, label, placeholder) {
	var labelEl = document.createElement("label");
	labelEl.setAttribute("for", "id");
	labelEl.setAttribute("style", "color: #16235A; font-size: 18px;");
	labelEl.innerHTML = label;
				
	var inputEl = document.createElement("input");
	inputEl.setAttribute("id", id);
	inputEl.setAttribute("type", "text");
	inputEl.setAttribute("class", "edit");
	inputEl.setAttribute("placeholder", placeholder);
				
	appendChild(labelEl, inputEl);
	return labelEl;
}

function createDivContainer() {
	var div = document.createElement("div");
	div.setAttribute("style", "width: 100%;");
	return div;
}

function appendChild(parent, child) {
	parent.appendChild(child);
}