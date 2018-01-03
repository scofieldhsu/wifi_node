
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

function OnloadEnumInterfaces() {
//alert("WlanEnumInterfaces by JavaScript ");
	// 發送 Ajax 查詢請求並處理
	var request = new XMLHttpRequest();
	
	request.open("GET", "/enumInterfaces");//scofield
	
	request.send();

	request.onreadystatechange = function() {
		// 伺服器請求完成
		if (request.readyState === 4) {
			// 伺服器回應成功
			if (request.status === 200) {
				document.getElementById("WlanOnLoad").innerHTML = request.responseText;
			} else {
				alert("發生錯誤: " + request.status);
			}
		}
	}
}

function enumInterfaces() {
//alert("WlanGetAvailableNetworkList ");
	// 發送 Ajax 查詢請求並處理
	var request = new XMLHttpRequest();
	
	request.open("GET", "/enumInterfaces");//scofield
	
	request.send();

	request.onreadystatechange = function() {
		// 伺服器請求完成
		if (request.readyState === 4) {
			// 伺服器回應成功
			if (request.status === 200) {
				document.getElementById("WlanResult").innerHTML = request.responseText;
			} else {
				alert("發生錯誤: " + request.status);
			}
		}
	}
}

function getNotifications() {
	// 發送 Ajax 查詢請求並處理
	var request = new XMLHttpRequest();
	
	request.open("GET", "/getNotifications");//scofield
	
	request.send();

	request.onreadystatechange = function() {
		// 伺服器請求完成
		if (request.readyState === 4) {
			// 伺服器回應成功
			if (request.status === 200) {
				document.getElementById("WlanResult").innerHTML = request.responseText;
			} else {
				alert("發生錯誤: " + request.status);
			}
		}
	}
}
