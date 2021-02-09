var $container = $('#content');

function camelCaseToSentence(camelCase) {
	var result = camelCase.replace( /([A-Z])/g, " $1" ).toLowerCase();
	var finalResult = result.charAt(0).toUpperCase() + result.slice(1);
	return finalResult;
}

function handleRequestFailure(jqXHR, textStatus, errorThrown) {
	if(jqXHR.status==403) {
		alert("You don't have access rights to see that page.");
		handleAuthUpdate();
	} else {
		var msg = errorThrown;
		if('responseJSON' in jqXHR && 'message' in jqXHR['responseJSON']) {
			msg = jqXHR.responseJSON['message'];
		}
		alert("Error: "+msg);
		console.warn("Error: ", arguments);
	}
}

function getJSON(url) {
	return $.getJSON(url).fail(handleRequestFailure);
}

function postJSON(url, data) {
	var cfg = {
		url: url,
		data: JSON.stringify(data),
		dataType: 'json',
        contentType	: 'application/json; charset=utf-8'
	};
	return $.post(cfg).fail(handleRequestFailure)
}

function getInputType(inputs, custom_formats) {
	all = {}
	for(name in inputs) {
		var value = inputs[name];
		var input = {}
		input["value"] = value;
		input["label"] = camelCaseToSentence(name);
		if(name in custom_formats) {
			input = _.defaults(custom_formats[name], input)
		} else {
			input["json_type"] = typeof value;
			switch(input["json_type"]) {
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
			input["html_type"] = input_type
		}
		all[name] = input;
	}
	return all;
}

function infoController(name) {
	getJSON("/info").success(function(data) {
		var template = templates[name];
		$container.html(template({ rows: data }))
	});
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

	getJSON(formUrl).success(function(response_data) {
		inputs = getInputType(response_data, custom_field_mapping)
		$container.html(templates[form_template]({
			 title: response_data['title'] ? response_data['title'] : "Edit " + type + " configuration",
			 inputs: inputs,
			 response_data: response_data
			}));
		$('#form').submit(function(event) {
			var data = $(event.target).serializeJSON();
			console.info("Sending ", data);
			postJSON(saveUrl, data)
				.done(function(data) {
					alert("Configuration saved.");
				});
				event.preventDefault();
		});
	});
}

function editStationController(name) {
	return editConfig("station");
}

function editApController(name) {
	custom_field_mapping = {
		"authMode": {
			html_type: "select",
			json_type: "string", 
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
				html_type: "url",
				json_type: "string"
			},
			"spiffUrl": {
				html_type: "url",
				json_type: "string"
			}
		}
	return editConfig("ota");
}

// -0- GPIO actions

function isPinSafeToUse(pin) {
	var safePins = [2,3,4,5,13,14,15,16];
	return safePins.includes(parseInt(pin));
}

function editGpioController(name) {
	return editConfig("gpio", "gpio");
}

// --- Auth actions

$(document).ready(handleAuthUpdate);

function getAuthJson() {
	var authJson = null;
	var jsonStr = Cookies.get('auth');
	if(jsonStr) {
		try {
			authJson = $.parseJSON(jsonStr);
		} catch(error) {
			console.warn("Invalid auth json", jsonStr, error);
		}
	}
	return authJson;
}

function handleAuthUpdate() {
	var auth = getAuthJson();
	var $userName = $('.user_name').text("").hide();
	var $roles = $("[class*=role_]");
	$roles.hide();
	$('.signout_action').hide();
	$('.signin_action').show();
	if(auth) {
		$('.signout_action').show();
		$('.signin_action').hide();
		$userName.text(auth.login).show();
		if(auth.roles.includes("admin")) {
			$roles.show();
		}
		else if(auth.roles.length>0) {
			var selector = ".role_" + auth.roles.join(', .role_');
			$(selector).show();
		}
	}
}

function signinController() {
	$container.html(templates["signin"]);
	$('#form').submit(function(event) {
		postJSON("/signin", $(event.target).serializeJSON())
			.done(function(data) {
				alert("You have been signed in.");
				handleAuthUpdate();
				loadPage("start");
			});
		event.preventDefault();
	});
}

function signoutController() {
	$.post({
		url: "/signout"
	}).done(function(data) {
		Cookies.set('auth', null)
		handleAuthUpdate();
		loadPage("signin");
	}).fail(handleRequestFailure)
}

// --- END Auth actions

// --- Users configuration

function userFormVisible(isVisible) {
	$('#user_form_title').text("");
	$form_container = $('#user_form_container');
	$list_container = $('#user_list_container');
	isVisible ? $form_container.show() : $form_container.hide();
	!isVisible ? $list_container.show() : $list_container.hide();
	if(isVisible) {
		// Reset form so we could reuse it in other actions
		$('#form_enabled').prop('checked', true);
		$('#form_login').prop('value', "")
						.prop('readonly', false);
		$('#form_roles').prop('value', "");
		$('#form_password').prop('value', "");
		$('#user_form_submit').off('click');
		userFormSubmitEnabled(true);
	}
	return isVisible;
}

function userFormSubmitEnabled(isEnabled) {
	$('#user_form_container [type=submit]').prop('disabled', !isEnabled);
}

function usersListController(name) {
	getJSON("/config/users/list").success(function(data) {
		var template = templates["users_list"];
		$container.html(template({ 'data': data }));
		userFormVisible(false);
	});
}

function normalizeRoles(data) {
	if(data.roles) {
		var rolesList = data.roles.split(",");
		var normalizedRoles = _.map(rolesList, function(value) {
			return value.trim();
		}).filter(function(v) {
			return v != "";
		});
		data.roles = _.uniq(normalizedRoles);
	}
	return data;
}

function addUser() {
	userFormVisible(true);
	$('#user_form_title').text("New user");
	$('#user_form_container').submit(function(event) {
		userFormSubmitEnabled(false);
		var data = normalizeRoles($(event.target).serializeJSON());
		postJSON("/config/users/add", data)
			.done(function(data) {
				alert("User added.");
				userFormVisible(false);
				loadPage('usersList');
			})
			.fail(function() {
				userFormSubmitEnabled(true);
			});
			event.preventDefault();
	}).on('reset', function(event) {
		userFormVisible(false)
	});
}

function editUser(isEnabled, login, roles) {
	userFormVisible(true);
	$('#user_form_title').text("Edit user");
	$('#form_enabled').prop('checked', isEnabled);
	$('#form_login').prop('value', login)
	                .prop('readonly', true);
	$('#form_roles').prop('value', roles);
	$('#user_form_container').submit(function(event) {
		userFormSubmitEnabled(false);
		var data = normalizeRoles($(event.target).serializeJSON());
		postJSON("/config/users/edit", data)
			.done(function(data) {
				alert("User " + login + " modified.");
				userFormVisible(false);
				loadPage('usersList');
			}).fail(function(jqXHR, textStatus, errorThrown) {
				userFormSubmitEnabled(true);
			});
		event.preventDefault();
	}).on('reset', function(event) {
		userFormVisible(false);
	});
}

function removeUser(login) {
	if(confirm("Are you sure you want to remove user "+login)) {
		postJSON("/config/users/remove", {'login': login}).done(function(data) {
			alert("User " + login + " removed.");
			userFormVisible(false);
			loadPage('usersList');
		});
	}
}

// --- END Users configuration

function getCurrentPage() {
	return window.location.hash.substring(1);
}

function loadPage(newPage) {
	var currentPage = getCurrentPage();
	if(currentPage == newPage) {
		window.location.reload();
	} else {
		window.location.hash='#' + newPage;
	}
}

function dispatch(name) {
	if(name=="") {
		name = "start";
	}

	var controller = window[name+"Controller"];
	if(!_.isUndefined(controller)) {
		controller(name);
	} else {
		var template = templates[name];
		if(!_.isUndefined(template)) {
			$container.html(template());
		} else {
			$container.text("Not found "+controller);
		}
	}
}

function urlChanged() {
	var currentPage = getCurrentPage();
	dispatch(currentPage);
}


$(window).bind('hashchange', urlChanged);
$(document).ready(urlChanged);
