
import $ from "jquery";
import Cookies from 'js-cookie'
import autoBind from 'react-autobind';

export class ApiHandler {
    static emptyAuth = {
        login: "",
        roles: []
    }

    constructor() {
	  autoBind(this);
      this.authUpdateCallbacks = [];
    }

    addUpdateAuthCallback(fn) {
        this.authUpdateCallbacks.push(fn);
    }
	
	getAuthJson() {
		var authJson = null;
		var jsonStr = Cookies.get('auth');
		if(jsonStr) {
			try {
				authJson = $.parseJSON(jsonStr);
			} catch(error) {
				console.warn("Invalid auth json", jsonStr, error);
			}
		} else {
            authJson = {
                login: "",
                roles: []
            }
        }
		return authJson;
	}

	handleRequestFailure(jqXHR, textStatus, errorThrown) {
		if(jqXHR.status === 403) {
			alert("You don't have access rights to see that page.");
			this.handleAuthUpdate();
		} else {
			var msg = errorThrown;
			if('responseJSON' in jqXHR && 'message' in jqXHR['responseJSON']) {
				msg = jqXHR.responseJSON['message'];
			}
			alert("Error: "+msg);
			console.warn("Error: ", arguments);
		}
	}
	
	getJSON(url) {
		return $.getJSON(url).fail(this.handleRequestFailure);
	}
	
	postJSON(url, data) {
		var cfg = {
			url: url,
			data: JSON.stringify(data),
			dataType: 'json',
			contentType	: 'application/json; charset=utf-8'
		};
		return $.post(cfg).fail(this.handleRequestFailure)
	}
	
	postForm(url, formEl) {
		let data = $(formEl).serializeJSON({checkboxUncheckedValue: "false"});
		return this.postJSON(url, data);
	}

	handleAuthUpdate() {
		var auth = this.getAuthJson();
        this.authUpdateCallbacks.forEach(callback => {
            callback(auth);
		});
	}
}