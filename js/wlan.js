
function getAvailableNetworkList() {
//alert("WlanGetAvailableNetworkList ");
	// 發送 Ajax 查詢請求並處理
	var request = new XMLHttpRequest();
	
	request.open("GET", "/getAvailableNetworkList");//scofield
	
	request.send();

	request.onreadystatechange = function() {
		// 伺服器請求完成
		if (request.readyState === 4) {
			// 伺服器回應成功
			if (request.status === 200) {
				/*var type = request.getResponseHeader("Content-Type");	// 取得回應類型

				// 判斷回應類型，這裡使用 JSON
				if (type.indexOf("application/json") === 0) {				
					var data = JSON.parse(request.responseText);

					if (data.number) {
						document.getElementById("searchResult").innerHTML = '[找到員工] 員工編號：' +data.number + ', 姓名：' +
																				data.name + ', 性別：' + data.sex;
					} else {
						document.getElementById("searchResult").innerHTML = data.msg;
					}
				}*/
				document.getElementById("WlanResult").innerHTML = request.responseText;
			} else {
				alert("發生錯誤: " + request.status);
			}
		}
	}
}

function handleWlanRequest(wlan_request, html_id) {
	// 發送 Ajax 查詢請求並處理
	var request = new XMLHttpRequest();
	
	request.open("GET", wlan_request);
	
	request.send();

	request.onreadystatechange = function() {
		// 伺服器請求完成
		if (request.readyState === 4) {
			// 伺服器回應成功
			if (request.status === 200) {
				document.getElementById(html_id).innerHTML = request.responseText;
			} else {
				alert("發生錯誤: " + request.status);
			}
		}
	};
}

function OnloadEnumInterfaces() {
	//handleWlanRequest("/enumInterfaces", "WlanOnLoad");
}

function enumInterfaces() {
	handleWlanRequest("/enumInterfaces", "WlanResult");
}

function getNotifications() {
	handleWlanRequest("/getNotifications", "WlanResult");
}

function page1_request() {
	handleWlanRequest("/enumInterfaces", "page1_content");
}

function page2_request() {
	handleWlanRequest("/getAvailableNetworkList", "page2_content");
}

function page3_request() {
	handleWlanRequest("/getNotifications", "page3_content");
}
