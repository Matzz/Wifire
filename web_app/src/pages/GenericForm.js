import React from 'react';
import autoBind from 'react-autobind';

export class GenericForm extends React.Component {
    constructor(props) {
        super(props);
        this.apiHandler = props.apiHandler;
        this.state = {
            custom_field_mapping: props.custom_field_mapping || {},
            type: props.type,
            response_data: {},
            inputs: {},
            dataLoaded: false,
            submitInProgress: false
        };
        autoBind(this);
    }

    componentDidMount() {
		var formUrl = "/config/"+this.state.type+"/get";
	
		this.apiHandler.getJSON(formUrl)
            .done(function(response_data) {
                this.setState({
                    response_data: response_data,
                    inputs: this.getInputType(response_data, this.state.custom_field_mapping),
                    dataLoaded: true
                });
            }.bind(this));

    }

    onSubmitCallback(event) {
        if(this.state.dataLoaded && !this.state.submitInProgress) {
            this.setState({submitInProgress: true});
            let saveUrl = "/config/"+this.state.type+"/set";
            this.apiHandler.postForm(saveUrl, event.target)
                .done(function(data) {
                    alert("Configuration saved.");
                })
                .always(function() {
                    this.setState({submitInProgress: false});
                }.bind(this));
        }

        event.preventDefault();
    }
	
    camelCaseToSentence(camelCase) {
		var result = camelCase.replace( /([A-Z])/g, " $1" ).toLowerCase();
		var finalResult = result.charAt(0).toUpperCase() + result.slice(1);
		return finalResult;
	}

	getInputType(inputs, custom_formats) {
		let all = {};
		for(let name in inputs) {
			let value = inputs[name];
			var input = {}
			input["value"] = value;
			input["label"] = this.camelCaseToSentence(name);
			if(name in custom_formats) {
				input = Object.assign({}, input, custom_formats[name])
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
                        if(name === "password") {
                            input_type = "password";
                        } else {
                            input_type = "text";
                        }
						break;
				}
				input["html_type"] = input_type
			}
			all[name] = input;
		}
		return all;
	}

    checkbox(name, input) {
        return (
            <div className="form-check">
                <input
                    className="form-check-input"
                    type="checkbox"
                    id={"form_" + name}
                    key={name}
                    name={name+":"+input.json_type}
                    defaultValue="true"
                    defaultChecked={input.value ? true : false}
                    />
            </div>
        )
    }

    select(name, input) {
        return (
            <select
            className="form-control"
            id={"form_" + name}
            key={name}
            name={name+":"+input.json_type}
            defaultValue={input.value}
            >
            { Object.keys(input.values).map((key) =>
                <option value={key} key={key}>{input.values[key]}</option>
            ) }
            </select>
        )
    }

    input(name, input) {
        return (
            <input
            className="form-control"
            type={input.html_type}
            id={"form_" + name}
            key={name}
            name={name+":"+input.json_type}
            defaultValue={input.value}
            />
        )
    }
    
    renderControl(name, input) {
        switch(input.html_type) {
            case "checkbox": return this.checkbox(name, input);
            case "select": return this.select(name, input);
            default: return this.input(name, input);
        }
    }
    render() {
        if(!this.state.dataLoaded) {
            return null;
        }
        
        let controls = [];
        let i = 0;
        for(let name in this.state.inputs)  {
            let input = this.state.inputs[name];
            controls[i++] = (
                <div className="form-group row" id={ "form_row_"+name } key={ name }>
                    <label className="col-sm-2 col-form-label" htmlFor={"form_" + name}>{ input.label }</label>
                    <div className="col-sm-10">
                        { this.renderControl(name, input) }
                    </div>
                </div>
                ) 
        }

        return (
            <form className="form-horizontal"
                id={ 'formId' in this.state ? this.state.formId : 'form' }
                onSubmit={this.onSubmitCallback}
                >
                {/* { 'title' in this.state && <h2>{this.state.title}</h2> } */}
                { controls }

                <div className="col-sm-offset-3">
                    <button type="submit" className="btn btn-primary" id="form_submit">Submit</button>
                </div>
            </form>
            )
    }
}