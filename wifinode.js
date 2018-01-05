console.log(`This platform is ${process.platform}`);

const url = require('url');
const qs = require('querystring');

// import the extension via require
//const example = require("./build/Release/example");

console.log('test addon');
const addon = require('./build/Release/addon');
//console.log(addon.hello());
//console.log('');


/*
//console.log('\ntest wlan api sync enumInterfaces');
//console.log(addon.wlanapi_sync_enumInterfaces());
//console.log('end of test wlan api sync EnumInterfaces\n');

//console.log('\ntest wlan api sync getAvailableNetworkList');
//console.log(addon.wlanapi_sync_getAvailableNetworkList());
//console.log('end of test wlan api sync getAvailableNetworkList\n');

console.log('\ntest wlan api sync getNotifications');
console.log(addon.wlanapi_sync_getNotifications());
console.log('end of test wlan api sync getNotifications\n');

*/

/*
console.log('\ntest wlan api async EnumInterfaces');
addon.wlanapi_async_enumInterfaces(function(msg){
	console.log(msg);
});
console.log('end of test wlan api async EnumInterfaces\n');

console.log('\ntest wlan api sync getAvailableNetworkList');
addon.wlanapi_async_getAvailableNetworkList(function(msg){
        console.log(msg);
	});
console.log('end of test wlan api async getAvailableNetworkList\n');
*/


//*
var fs = require('fs');
const http = require('http')
const port = 8888

//var head = 'Content-Type': 'text/html';
var body = '<html><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>';
//body += '<script src="index.js"></script></head><body>';
body += '</head><body>';

var html_body = '';

var path = require("path");

var io = require('socket.io'); // 加入 Socket.IO

function toHtmlTable(msg) {
	//console.log('toHtmlTable() msg:'+msg);
	
	var msg2 = '<table border=1>';
	var str = msg;
	
	var arr1 = str.split("<br>\n");
	//console.log('arr1.length=='+arr1.length);
	
	for (var i=0; i<arr1.length; i++) {
		if (arr1[i]=='')
			continue;

		//console.log('arr1['+i+']=='+arr1[i]);
		
		msg2 += '<tr>';
			
		var arr2 = arr1[i].split(":");
		for (var j=0; j<arr2.length; j++) {
			//console.log('\tarr2['+j+']=='+arr2[j]);
			msg2 += '<td>'+arr2[j]+'</td>';
		}

		msg2 += '</tr>';
	}

	return msg2;
};

const requestHandler = (request, response) => {
	console.log('\n    request.url: '+request.url);
	
	var headers = request.headers;
	//var userAgent = headers['user-agent'];
	//console.log('userAgent: '+userAgent);
	
	//console.log('request.method: '+request.method);

	 urlData = url.parse(request.url, true);
	user = urlData.query;

	if (user.number) {
		console.log('? user.number: '+user.number);

		response.writeHead(200, { 'Content-Type': 'application/json' });
		user.name = 'scofield';
		user.sex = 'male';
		response.write(JSON.stringify(user));
		response.end();

		return;
	}

	//console.log('user.wlan: '+user.wlan);

	user.superman = "scofield";
	json1 = JSON.stringify(user);
	//console.log('json1: '+json1);

	//var req_body = [];
	var formData = '';
	request.on('error', function(err) {
		console.error(err);
	}).on('data', function(chunk) {
		console.log('chunk: '+chunk);
		formData += chunk;
		//req_body.push(chunk);
	}).on('end', function() {
		var user2 = qs.parse(formData);
		console.log('end. name:wlan with value:'+user2.wlan);

		user2.superman = "scofield";
		json2 = JSON.stringify(user2);
		//console.log('json2: '+json2);
		
		//req_body = Buffer.concat(req_body).toString();
		// At this point, we have the headers, method, url and body, and can now
		// do whatever we need to in order to respond to this request.
	});
	//for (var i=0; i<req_body.length; i++)
		//console.log('req_body:'+req_body);

	var getAvailableNetworkList = '';
	var enumInterfaces = '';
	if (request.url == '/') {
		/*response.writeHeader(200, {"Content-Type": "text/html"});  
        response.write(html_body);  
        response.end();  
		return;*/
		return read_file("/index.html", response);
	}
	else if (request.url == '/getAvailableNetworkList') {
		addon.wlanapi_async_getAvailableNetworkList( function(msg) {
			getAvailableNetworkList = msg;

			response.writeHeader(200, {"Content-Type": "text/plaintext"});  
			response.write(toHtmlTable(msg));
			//response.write('<button id="clear_WlanGetAvailableNetworkList">clear result of available wifi network list</button>');
			response.end();
		});
		
		return;
	}
	else if (request.url == '/enumInterfaces') {
		addon.wlanapi_async_enumInterfaces( function(msg) {
			getAvailableNetworkList = msg;

			response.writeHeader(200, {"Content-Type": "text/plaintext"});  
			response.write(toHtmlTable(msg));
			//response.write('<button id="clear_WlanGetAvailableNetworkList">clear result of available wifi network list</button>');
			response.end();
		});
		
		return;
	}
	else if (request.url == '/getNotifications') {
		var msg = addon.wlanapi_sync_getNotifications();
	
		//console.log('getNotifications: '+msg);
		response.writeHeader(200, {"Content-Type": "text/plaintext"});  
		//response.write(toHtmlTable(msg));
		response.write(msg);
		response.end();
		
		return;
	}
	else {
		return read_file(request.url, response);
	}

	response.writeHead(200, { 'Content-Type': 'text/html' });
	response.write(body);

	response.write('username: '+user.username);

	response.write('<form id="signup" method="GET" action="/">');
	response.write('<label>使用者名稱：</label><input type="text" id="id_username" name="username" /><br>');
	response.write('<label>電子郵件：</label><input type="text" id="id_email" name="email" /><br>');
	response.write('<input type="submit" name="wlan" value="Register"/><br></form>');

	response.write('<form method="post" action="/">');
	response.write('<input type ="submit" value="root webpage"/>');
	response.write('	go back to root webpage</form>');

	//response.write(msg);
	
	//response.write('<form method="post" action="/enumInterfaces">');
	//response.write('<input type ="submit" name="enumInterfaces" value="WlanEnumInterfaces"/>');
	//response.write('	enumerate wifi interfaces</form>');
	
	//response.write(msg2);

	//response.write('<form method="post" action="/getAvailableNetworkList">');
	response.write('<form method="post" action="/">');
	//response.write('<input type ="submit" name="getAvailableNetworkList"');
	response.write('<input type ="submit" name="wlan"');
		response.write(' value="WlanGetAvailableNetworkList"/>');
		response.write(' get available wifi network list<br><br>');
	//response.write('<input type ="submit" name="enumInterfaces"');
	response.write('<input type ="submit" name="wlan"');
		response.write(' value="WlanEnumInterfaces"/>');
		response.write(' get available wifi network list<br><br>');
	response.write('</form>');


	//response.write('<div id="div1">'+enumInterfaces+'</div>');
	//response.write('<div id="div2">'+getAvailableNetworkList+'</div>');

	var table_content = '';
	if (enumInterfaces != '')			table_content = toHtmlTable(enumInterfaces);
	if (getAvailableNetworkList != '')	table_content = toHtmlTable(getAvailableNetworkList);
	response.write(table_content);

	response.write('</body></html>');
	response.end();

};

const server = http.createServer(requestHandler);

server.listen(port, (err) => {
	if (err) {
		return console.log('something bad happened', err)
	}

	console.log(`server is listening on ${port} @`+ new Date());
});
/*fs.readFile('./index.html', function (err, html) {

    if (err) throw err;

	html_body = html;

    server.listen(port, (err) => {
		if (err) {
			return console.log('something bad happened', err)
		}

		console.log(`server is listening on ${port} @`+ new Date());
	});
});*/

var serv_io = io.listen(server);// 開啟 Socket.IO 的 listener
serv_io.sockets.on('connection', function(socket) {
	var _date = new Date();
	socket.emit('message', {'msg':'hello world', 'date':_date});
	
	socket.on('wifi', (data, fn) => {
		console.log("wifi: get: "+data.get);
		
		switch (data.get) {
		case 'enumInterfaces':	
			addon.wlanapi_async_enumInterfaces( function(msg) {
				//console.log("ferret: msg:\n"+msg);
				fn(msg);
			});
			break;
    
		default:
			console.log("wifi: get?: "+data.get);
			break;
		}
		
		
	});
});

function contentType(ext) {
    var ct;

    switch (ext) {
    case '.html':	ct = 'text/html';		break;
    case '.css':	ct = 'text/css';		break;
    case '.js':		ct = 'text/javascript';	break;
	case '.gif':	ct = 'image/gif';		break;
	case '.ico':	ct = 'image/x-icon';		break;
    default:		ct = 'text/plain';		break;
    }

    return {'Content-Type': ct};
}

function read_file(filename, response) {
	//console.log('\n    read_file: ' + filename);
	fs.readFile("."+filename, function (err, html) {

		if (err) {
			console.log('Error loading ' + filename);
            response.writeHead(500);
            return response.end('Error loading ' + filename);
        }
		
		var ext = path.extname(filename);
		//console.log('\n    read_file: ext: ' + ext);
        		
		response.writeHeader(200, contentType(ext));  
		response.write(html);
		response.end();
	});
}


//*/


// calling the global method
//var x = 5;
//var g = example.fact(x);
//print(g);

// Accessing the global variable
//var f = example.get_time();
//print(f);

