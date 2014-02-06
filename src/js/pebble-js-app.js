var today = new Date();
var dd = today.getDate();
var mm = today.getMonth() + 1;
var month_name = new Array();
month_name[1] = "January";
month_name[2] = "February";
month_name[3] = "March";
month_name[4] = "April";
month_name[5] = "May";
month_name[6] = "June";
month_name[7] = "July";
month_name[8] = "August";
month_name[9] = "September";
month_name[10] = "October";
month_name[11] = "November";
month_name[12] = "December";

function getFact(month, date) {
	var response;
	var req = new XMLHttpRequest();
	req.open('GET', 'http://numbersapi.com/' + month + '/' + date + '/date?json&default=No+fact+found+for+today.', true);
	req.onload = function(e) {
		if (req.readyState == 4) {
			if (req.status == 200) {
				console.log(req.responseText);
				response = JSON.parse(req.responseText);
				var text = response.text;
				var date
				Pebble.sendAppMessage({
					'fact': text.toString(),
					'date': month_name[month] + ' ' + dd,
				});
			}
		}
	}
	req.send(null);
}

Pebble.addEventListener("ready", function(e) {
	console.log("Ready!");
	getFact(mm, dd);
});