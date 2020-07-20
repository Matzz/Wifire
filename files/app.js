var templateCache = {};
var $container = $('#content');

function camelCaseToSentence(camelCase) {
	var result = camelCase.replace( /([A-Z])/g, " $1" ).toLowerCase();
	var finalResult = result.charAt(0).toUpperCase() + result.slice(1);
	return finalResult;
}

function getFieldsFormat(inputs, custom_formats) {
	all = {}
	for(name in inputs) {
		var value = inputs[name];
		var input = {}
		input["value"] = value;
		input["label"] = camelCaseToSentence(name);
		if(name in custom_formats) {
			input = _.defaults(custom_formats[name], input)
		} else {
			switch(typeof value) {
				case "boolean":
					input_type = "checkbox";
					break;
				case "number":
					input_type = "number";
					break;
				default:
					input_type = "text";
					break;
			}
			input["type"] = input_type
		}
		all[name] = input;
	}
	return all;
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

function editConfig(type, form_template, custom_field_mapping) {
	if(!form_template) {
		form_template = 'dynamic_form';
	}
	if(!custom_field_mapping) {
		custom_field_mapping = {}
	}
	var formUrl = "/config/"+type+"/get";
	var saveUrl = "/config/"+type+"/set";

	$.getJSON(formUrl, function(response_data) {
		inputs = getFieldsFormat(response_data, custom_field_mapping)
		$container.html(compileTemplate(form_template)({ inputs: inputs, response_data: response_data }));
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
	custom_field_mapping = {
		"authMode": {
			type: "select", 
			values: {
				0: "Open",
				1: "WEP",
				2: "WEP PSK",
				3: "WPA PSK",
				4: "WPA2 PSK",
				5: "WPA WPA2 PSK",
				6: "WPA2 ENTERPRISE",
				7: "MAX"
			}
		}
	}
	return editConfig("ap", null, custom_field_mapping);
}

function editOtaController(name) {
	custom_field_mapping = {
			"romUrl": {
				type: "url"
			},
			"spiffUrl": {
				type: "url"
			}
		}
	return editConfig("ota");
}

function editGpioController(name) {
	return editConfig("gpio", "gpio");
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
