export class StateProxy {
    constructor(parent) {
        this.get = () => {
            return parent.state;
        }
        this.set = (newState) => parent.setState(newState);
    }

}

/*
Extracted from GenericForm to reuse in other components.
Used to load and save form data.
*/
export class AsyncFormHelper {
    constructor(type, apiHandler, stateProxy) {
        this.type = type;
        this.apiHandler = apiHandler;
        this.state = stateProxy;
    }

    mergeFormData(responseData) {
        // React do not merge nested fields.
        // We do that to keep external formData fields which are not present in response body
        return Object.assign({}, this.state.get().formData, responseData);
    }

    loadState() {
        var formUrl = "/config/" + this.type + "/get";
        return this.apiHandler
            .getJSON(formUrl)
            .done(function (responseData) {
                this.state.set({
                    formData: this.mergeFormData(responseData),
                    dataLoaded: true
                });
                return responseData;
            }.bind(this));
    }

    submitForm(event) {
        event.preventDefault();
        let state = this.state.get(); 
        if (state.dataLoaded && !state.submitInProgress) {
            this.state.set({ submitInProgress: true });
            let saveUrl = "/config/" + this.type + "/set";
            let formData = this.apiHandler.getFormData(event.target);
            let queryParams = 'actionName' in formData ? {'actionName': formData.actionName} : null;
            this.apiHandler
                .postJSON(saveUrl, formData, queryParams)
                .done(function (data) {
                    alert("Configuration saved.");
                    return data;
                })
                .always(function () {
                    this.state.set({ submitInProgress: false });
                }.bind(this));
        }
    }
}