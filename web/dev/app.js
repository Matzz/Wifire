class ContentManager {
	constructor($container = $('#content'), templates = window.templates, controllers = []) {
	  this.$container = $container;
	  this.templates = templates;
	  this.controllers = controllers;
	  autoBind(this);
	}

	static getCurrentPage() {
		return window.location.hash.substring(1);
	}

	static loadPage(newPage) {
		var currentPage = ContentManager.getCurrentPage();
		if(currentPage == newPage) {
			window.location.reload();
		} else {
			window.location.hash='#' + newPage;
		}
	}

	bindCallbacks() {
		$(window).bind('hashchange', this.handleUrlChanged);
		$(document).ready(this.handleUrlChanged);
		$(document).ready(ApiHandler.handleAuthUpdate);
	}

	addController(name, fn) {
		this.controllers[name] = fn;
	}
	
	addTemplate(name, fn) {
		this.templates[name] = fn;
	}

	dispatch(actionName) {
		if(actionName == "") {
			actionName = "start";
		}
	
		var controller = this.controllers[actionName];
		// Try to find associated controller
		if(!_.isUndefined(controller)) {
			controller(contentManager, actionName);
		} else {
			// Falback to plain template (without controller)
			this.displayTemplate(actionName, {});
		}
	}

	renderTemplate(templateName, options = {}) {
		var template = this.templates[templateName];
		if(!_.isUndefined(template)) {
			template(options);
		} else {
			null
		}
	}

	displayTemplate(templateName, options = {}) {
		var rendered = this.renderTemplate(templateName, options);
		if(rendered !== null) {
			this.$container.html(rendered);
		} else {
			this.$container.text("Not found "+templateName);
		}
	}

	handleUrlChanged() {
		var currentPage = ContentManager.getCurrentPage();
		this.dispatch(currentPage);
	}
}

class FormHelpers {
	static camelCaseToSentence(camelCase) {
		var result = camelCase.replace( /([A-Z])/g, " $1" ).toLowerCase();
		var finalResult = result.charAt(0).toUpperCase() + result.slice(1);
		return finalResult;
	}

	static getInputType(inputs, custom_formats) {
		var all = {};
		for(name in inputs) {
			var value = inputs[name];
			var input = {}
			input["value"] = value;
			input["label"] = FormHelpers.camelCaseToSentence(name);
			if(name in custom_formats) {
				input = _.defaults(custom_formats[name], input)
			} else {
				input["json_type"] = typeof value;
				var input_type;
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

	static editConfig({contentManager, type, template = null, custom_field_mapping = null}) {
		if(!template) {
			template = 'dynamic_form';
		}
		if(!custom_field_mapping) {
			custom_field_mapping = {}
		}
		var formUrl = "/config/"+type+"/get";
		var saveUrl = "/config/"+type+"/set";
	
		ApiHandler.getJSON(formUrl).success(function(response_data) {
			var inputs = FormHelpers.getInputType(response_data, custom_field_mapping)
			contentManager.displayTemplate(template, {
				title: response_data['title'] ? response_data['title'] : "Edit " + type + " configuration",
				inputs: inputs,
				response_data: response_data
			   });
			contentManager.$container.find('#form').submit(function(event) {
				var data = $(event.target).serializeJSON();
				ApiHandler.postJSON(saveUrl, data)
					.done(function(data) {
						alert("Configuration saved.");
					});
				event.preventDefault();
			});
		});
	}
}

class ApiHandler {
	
	static getAuthJson() {
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

	static handleRequestFailure(jqXHR, textStatus, errorThrown) {
		if(jqXHR.status==403) {
			alert("You don't have access rights to see that page.");
			ApiHandler.handleAuthUpdate();
			ContentManager.loadPage("");
		} else {
			var msg = errorThrown;
			if('responseJSON' in jqXHR && 'message' in jqXHR['responseJSON']) {
				msg = jqXHR.responseJSON['message'];
			}
			alert("Error: "+msg);
			console.warn("Error: ", arguments);
		}
	}
	
	static getJSON(url) {
		return $.getJSON(url).fail(ApiHandler.handleRequestFailure);
	}
	
	static postJSON(url, data) {
		var cfg = {
			url: url,
			data: JSON.stringify(data),
			dataType: 'json',
			contentType	: 'application/json; charset=utf-8'
		};
		return $.post(cfg).fail(ApiHandler.handleRequestFailure)
	}

	static handleAuthUpdate() {
		var auth = ApiHandler.getAuthJson();
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
}

class UserEditManager {
	constructor(contentManager) {
		this.contentManager = contentManager;
		this.$c = contentManager.$container;
		this.$form_container = null;
		autoBind(this);
	}

	hideUserForm() { this.userFormVisible(false); }

	showUserForm(title, submitCallback) {
		this.userFormVisible(true);
		this.$form_container.off('submit reset');
		this.$form_container.submit(submitCallback);
		this.$form_container.on('reset', this.hideUserForm);
		this.$c.find('#user_form_title').text(title);
	}

	userFormVisible(isVisible) {
		this.$c.find('#user_form_title').text("");
		var $list_container = this.$c.find('#user_list_container');
		isVisible ? this.$form_container.show() : this.$form_container.hide();
		!isVisible ? $list_container.show() : $list_container.hide();
		if(isVisible) {
			// Reset form so we could reuse it in other actions

			this.$c.find('#form_enabled')
				.prop('checked', true);
			this.$c.find('#form_login')
				.prop('value', "")
				.prop('readonly', false);
			this.$c.find('#form_roles, #form_password')
				.prop('value', "");

			this.userFormSubmitEnabled(true);
		}
		return isVisible;
	}

	normalizeRoles(data) {
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
	
	userFormSubmitEnabled(isEnabled) {
		this.$c.find('#user_form_container [type=submit]').prop('disabled', !isEnabled);
	}
	
	
	addUserCallback(event) {
		this.showUserForm("New user", this.addUserSubmitCallback);
	}
	
	addUserSubmitCallback(event) {
		this.userFormSubmitEnabled(false);
		var formData = $(event.target).serializeJSON()
		formData = this.normalizeRoles(formData);
		ApiHandler.postJSON("/config/users/add", formData)
			.done(_.bind(function(data) {
				alert("User added.");
				this.userFormVisible(false);
				ContentManager.loadPage('usersList');
			}, this))
			.fail(_.bind(function() {
				this.userFormSubmitEnabled(true);
			}, this));
			event.preventDefault();
	}
	
	editUserCallback(event) {
		var $target = $(event.target);
		var isEnabled = $target.data('enabled');
		var login = $target.data('login');
		var roles = $target.data('roles');

		this.showUserForm("Edit user", this.editUserSubmitCallback);
		this.$c.find('#form_enabled').prop('checked', isEnabled);
		this.$c.find('#form_login')
			.prop('value', login)
			.prop('readonly', true);
		this.$c.find('#form_roles')
			.prop('value', roles);
	}

	editUserSubmitCallback(event) {
		this.userFormSubmitEnabled(false);
		var formData = $(event.target).serializeJSON();
		formData = this.normalizeRoles(formData);
		ApiHandler.postJSON("/config/users/edit", formData)
			.done(_.bind(function(data) {
				alert("User " + formData.login + " modified.");
				this.userFormVisible(false);
				ContentManager.loadPage('usersList');
			}, this))
			.fail(_.bind(function(jqXHR, textStatus, errorThrown) {
				this.userFormSubmitEnabled(true);
			}, this));
		event.preventDefault();
	}
	
	removeUserCallback(event) {
		var $target = $(event.target);
		var login = $target.data('login');
		if(confirm("Are you sure you want to remove user "+login)) {
			ApiHandler.postJSON("/config/users/remove", {'login': login})
				.done(_.bind(function(data) {
					alert("User " + login + " removed.");
					this.userFormVisible(false);
					ContentManager.loadPage('usersList');
				}, this));
		}
	}

	bindController() {
		var listHandler = function(contentManager, actionName) {
			var loaded = _.bind(function(data) {
				for(var i in data['users']) {
					data['users'][i]['rolesStr'] = data['users'][i]['roles'].join(",");
				}
				contentManager.displayTemplate("users_list", {
					'data': data
				});
				var $c = contentManager.$container;
				this.$form_container = $c.find('#user_form_container');
				$c.find('.edit-user').on('click', this.editUserCallback);
				$c.find('.remove-user').on('click', this.removeUserCallback);
				$c.find('.add-user').on('click', this.addUserCallback);
				this.hideUserForm();
			}, this);
			ApiHandler.getJSON("/config/users/list").success(loaded);
		}

		this.contentManager.addController("usersList", _.bind(listHandler, this));
		
	}

}

class GPIOHandler {
	static isPinSafeToUse(pin) {
		var safePins = [2,3,4,5,13,14,15,16];
		return safePins.includes(parseInt(pin));
	}
	static bindController(contentManager) {
		contentManager.addController("editGpio", function(contentManager, actionName) {
			FormHelpers.editConfig({
				contentManager: contentManager,
				type: "gpio",
				template: "gpio"
			});
		
		});
	}
}

function addActions(contentManager) {
	
	contentManager.addController("info", function(contentManager, actionName) {
		ApiHandler.getJSON("/info").success(function(data) {
			var opt = { rows: data };
			contentManager.displayTemplate(actionName, opt);
		});
	})
	
	contentManager.addController("editStation", function(contentManager, actionName) {
		FormHelpers.editConfig({
			contentManager: contentManager,
			type: "station"
		});
	});
	
	
	contentManager.addController("editAp",  function(contentManager, actionName) {
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
		FormHelpers.editConfig({
			contentManager: contentManager,
			type: "ap",
			custom_field_mapping: custom_field_mapping
		});
	});
	
	contentManager.addController("editOta", function(contentManager, actionName) {
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
		FormHelpers.editConfig({
			contentManager: contentManager,
			type: "ota",
			custom_field_mapping: custom_field_mapping
		});
	
	});
	
	contentManager.addController("signin", function(contentManager, actionName) {
		contentManager.displayTemplate("signin");
		contentManager.$container.find('#form').submit(function(event) {
			ApiHandler.postJSON("/signin", $(event.target).serializeJSON())
				.done(function(data) {
					alert("You have been signed in.");
					ApiHandler.handleAuthUpdate();
					ContentManager.loadPage("start");
				});
			event.preventDefault();
		});
	});
	
	contentManager.addController("signout", function(contentManager, actionName) {
		$.post({
			url: "/signout"
		}).done(function(data) {
			Cookies.set('auth', null)
			ApiHandler.handleAuthUpdate();
			ContentManager.loadPage("signin");
		}).fail(ApiHandler.handleRequestFailure)
	});
	
}




var contentManager = new ContentManager();
var userEditManager = new UserEditManager(contentManager);
userEditManager.bindController();
GPIOHandler.bindController(contentManager);
addActions(contentManager);
contentManager.bindCallbacks();