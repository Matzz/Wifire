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

    removeUpdateAuthCallback(fn) {
        this.authUpdateCallbacks = this.authUpdateCallbacks.filter(el => el !== fn);
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
			var responseMsg = ApiHandler.getSafe(() => jqXHR.responseJSON.message, null);
			if(responseMsg) {
				msg = responseMsg;
			}
			alert("Error: "+msg);
			console.warn("Error: ", arguments);
		}
	}
	
	getJSON(url, data) {
		return $.ajax({
			dataType: "json",
			url: url,
			data: data
		  })
		  .fail(this.handleRequestFailure);
	}
	
	postJSON(url, data, queryParams) {
		if(queryParams) {
			url = url + '?' + $.param(queryParams);
		}
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

	handleAuthUpdate() {
		var auth = this.getAuthJson();
        this.authUpdateCallbacks.forEach(callback => {
            callback(auth);
		});
	}

	static getSafe(fn, defaultVal) {
		try {
			return fn();
		} catch (e) {
			return defaultVal;
		}
	}
}
