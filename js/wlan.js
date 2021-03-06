/*
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
				/*document.getElementById("WlanResult").innerHTML = request.responseText;
			} else {
				alert("發生錯誤: " + request.status);
			}
		}
	}
}
*/

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
	socket_io();
}

function enumInterfaces() {
	document.getElementById("WlanResult").innerHTML = "";
	//handleWlanRequest("/enumInterfaces", "WlanResult");
	socket_io();
}

function getAvailableNetworkList() {
	document.getElementById("WlanJSONResult").innerHTML = "";
	handleWlanRequest("/getAvailableNetworkList", "WlanResult");
}

function getNotifications() {
	handleWlanRequest("/getNotifications", "WlanResult");
}

function page1_request() {
	//handleWlanRequest("/enumInterfaces", "page1_content");
	socket_io();
}

function page2_request() {
	handleWlanRequest("/getAvailableNetworkList", "page2_content");
}

function page3_request() {
	handleWlanRequest("/getNotifications", "page3_content");
}

function socket_io() {
	//var socket = io.connect("http://127.0.0.1:8888");//it will fail when connecting from another device
	var socket = io.connect();

	socket.on('connect', function() {
		//console.log('Client connected');
		
		socket.emit('wifi', {'get':'enumInterfaces'}, (data) => {
		//socket.emit('wifi', {'set':'enumInterfaces'}, (data) => {
			console.log(data);
			//document.getElementById("WlanResult").innerHTML = data;
			
			var obj = JSON.parse(data);
			//console.log('enumInterfaces: '+JSON.stringify(obj));
			console.log('InterfaceDescription: '+obj.InterfaceInfos[0].InterfaceDescription);
			//document.getElementById("WlanResult").innerHTML = JSON.stringify(obj);
			var html_msg = "Number of Interfaces: " + obj.NumInterfaces + "<br>"
				+ "Current Interface Index: " + obj.CurrentIndex + "<br>"
				+ "Interface Information: <br>";
			document.getElementById("WlanJSONResult").innerHTML = html_msg;
			
			//var resultSet = obj.InterfaceInfos;
			//JSON2Table('table', resultSet, 'json', {id:'test'});
			
			var NumInterfaces = parseInt(obj.NumInterfaces);
			
			//<!--  w ww  .ja  v  a 2  s.  c  om-->
			var ul=document.createElement('ul');
			ul.setAttribute('data-role', 'listview');
			for(i=0; i<NumInterfaces; i++)
			{
				var li=document.createElement('li');
				li.innerHTML="InterfaceIndex: "+obj.InterfaceInfos[i].InterfaceIndex;
				ul.appendChild(li);
				
				li=document.createElement('li');
				li.innerHTML="InterfaceGUID: "+obj.InterfaceInfos[i].InterfaceGUID;
				ul.appendChild(li);
				
				li=document.createElement('li');
				li.innerHTML="InterfaceDescription: "+obj.InterfaceInfos[i].InterfaceDescription;
				ul.appendChild(li);
				
				li=document.createElement('li');
				li.innerHTML="InterfaceState: "+obj.InterfaceInfos[i].InterfaceState;
				ul.appendChild(li);
			}
			document.getElementById('WlanJSONResult').appendChild(ul);
		});
	});
	
	socket.on('connect_error', function(error) {
		console.log('connect_error: '+error);
	});
	
	socket.on('reconnect_attempt', (attemptNumber) => {
		console.log('reconnect_attempt: '+attemptNumber);
	});
	
	socket.on('error', function(error) {
		console.log('error: '+error);
	});

	socket.on('message', function(data){
		console.log(data.msg);
		console.log(data.date);
		document.getElementById("WlanResult").innerHTML = JSON.stringify(data);
	});
	
	//socket.emit('ferret', {'name':'tobi'});
}
