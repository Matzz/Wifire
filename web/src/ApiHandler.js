
import $ from "jquery";
import Cookies from 'js-cookie'
import autoBind from 'react-autobind';
import "jquery-serializejson";

export default class ApiHandler {
    static emptyAuth = {
        login: "",
        roles: []
    }

    constructor(history) {
	  this.history = history;
	  autoBind(this);
      this.authUpdateCallbacks = [];
    }
    
    render() { 
        return null;
    }

    addUpdateAuthCallback(fn) {
        this.authUpdateCallbacks.push(fn);
    }
	
	getAuthJson() {
		var authJson = null;
		var jsonStr = Cookies.get('auth');
		if(jsonStr) {
			try {
				authJson = JSON.parse(jsonStr);
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
			this.history.push('/');
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
	
	getFormData(formEl) {
		return $(formEl).serializeJSON({checkboxUncheckedValue: "false"});
	}
	
	postForm(url, formEl) {
		return this.postJSON(url, this.getFormData(formEl));
	}

	handleAuthUpdate() {
		var auth = this.getAuthJson();
        this.authUpdateCallbacks.forEach(callback => {
            callback(auth);
		});
	}
}
