var templateCache = {};
var $container = $('#content');
function getFieldFormat(name, value) {
	switch(name) {
		case "romUrl":
		case "spiffUrl":
			return {type: "url"};
		case "authMode":
			return {type: "select", values: [
				"Open",
				"WEP",
				"WEP PSK",
				"WPA PSK",
				"WPA2 PSK",
				"WPA WPA2 PSK",
				"WPA2 ENTERPRISE",
				"MAX"
			]}
		default:
			switch(typeof value) {
			case "boolean": return {type: "checkbox"};
			case "number": return {type: "number"};
			default:
				return {type: "text"};
		}
		
	}
}
function compileTemplate(name) {
	if(!templateCache[name]) {
		templateCache[name] =  $('#'+name+'_tpl').map(function() {
			var compiled = _.template($(this).text());
			return compiled;
		}).get(0);
	}
	return templateCache[name];
}

function infoController(name) {
	$.getJSON("/info").success(function(data) {
		var template = compileTemplate(name);
		$container.html(template({ rows: data }))
	})
}

function switchesController(name) {
	$container.html(compileTemplate(name)());
	$container.find('[data-toggle]').bootstrapToggle();
}

function editConfig(type) {
	var formUrl = "/config/"+type+"/get";
	var saveUrl = "/config/"+type+"/set";

	$.getJSON(formUrl, function(inputs) {
		$container.html(compileTemplate("dynamic_form")({ inputs: inputs }));
		$('#form-submit').click(function() {
	        $.ajax({
	            url: saveUrl,
	            type: 'post',
	            dataType: 'json',
	            data: $('#form').serialize(),
	        }).success(function(data) {
	        	alert("Ok")
	        }).fail(function() {
	        	alert("Error");
	        	console.log("Error: ", arguments)
	        })
		})
	});
}

function editStationController(name) {
	return editConfig("station");
}

function editApController(name) {
	return editConfig("ap");
}

function editOtaController(name) {
	return editConfig("ota");
}


function dispatch(name) {
	if(name=="") {
		name = "info";
	}

	var controller = window[name+"Controller"];
	if(!_.isUndefined(controller)) {
		controller(name);
	} else {
		var template = compileTemplate(name);
		if(!_.isUndefined(template)) {
			$container.html(template());
		} else {
			$container.text("Not found "+controller);
		}
	}
}
function urlChanged() {
	var action = window.location.hash.substring(1);
	dispatch(action);
}


$(window).bind('hashchange', urlChanged);
$(document).ready(urlChanged);
