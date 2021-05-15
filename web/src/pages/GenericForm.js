import React from 'react';
import autoBind from 'react-autobind';
import {StateProxy, AsyncFormHelper} from '../AsyncFormHelper';

export class GenericForm extends React.Component {
    constructor(props) {
        super(props);
        this.formHelper = new AsyncFormHelper(props.type, props.apiHandler, new StateProxy(this));
        this.state = {
            inputs: {},
            actionName: ""
        };
        autoBind(this);
    }

    componentDidMount() {
        this.formHelper
            .loadState()
            .then(responseData => {
                this.setState({
                    inputs: this.getInputType(responseData, this.props.customFieldMapping || {}),
                });
            });
    }

    onSubmitCallback(event) {
        this.formHelper.submitForm(event);
    }

    camelCaseToSentence(camelCase) {
        var result = camelCase.replace(/([A-Z])/g, " $1").toLowerCase();
        var finalResult = result.charAt(0).toUpperCase() + result.slice(1);
        return finalResult;
    }

    getInputType(inputs, customFormats) {
        let all = {};
        for (let name in inputs) {
            let value = inputs[name];
            var input = {}
            input["value"] = value;
            input["label"] = this.camelCaseToSentence(name);
            if (name in customFormats) {
                input = Object.assign({}, input, customFormats[name])
            } else {
                input["jsonType"] = typeof value;
                var inputType;
                switch (input["jsonType"]) {
                    case "boolean":
                        inputType = "checkbox";
                        break;
                    case "number":
                        inputType = "number";
                        break;
                    default:
                        if (name === "password") {
                            inputType = "password";
                        } else {
                            inputType = "text";
                        }
                        break;
                }
                input["htmlType"] = inputType
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
                    name={name + ":" + input.jsonType}
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
                name={name + ":" + input.jsonType}
                defaultValue={input.value}
            >
                { Object.keys(input.values).map((key) =>
                    <option value={key} key={key}>{input.values[key]}</option>
                )}
            </select>
        )
    }

    input(name, input) {
        return (
            <input
                className="form-control"
                type={input.htmlType}
                id={"form_" + name}
                key={name}
                name={name + ":" + input.jsonType}
                defaultValue={input.value}
            />
        )
    }

    renderControl(name, input) {
        switch (input.htmlType) {
            case "checkbox": return this.checkbox(name, input);
            case "select": return this.select(name, input);
            default: return this.input(name, input);
        }
    }

    setActionName(event) {
        this.setState({actionName: event.target.value});
    }

    render() {
        if (!this.state.dataLoaded) {
            return null;
        }

        let controls = [];
        let i = 0;
        for (let name in this.state.inputs) {
            let input = this.state.inputs[name];
            controls[i++] = (
                <div className="form-group row" id={"form_row_" + name} key={name}>
                    <label className="col-sm-2 col-form-label" htmlFor={"form_" + name}>{input.label}</label>
                    <div className="col-sm-10">
                        {this.renderControl(name, input)}
                    </div>
                </div>
            )
        }
        let submitButtons = this.props.submitButtons || [{text: "Submit", actionName:"submit"}];

        return (
            <form className="form-horizontal"
                id={'formId' in this.state ? this.state.formId : 'form'}
                onSubmit={this.onSubmitCallback}
            >
                { 'title' in this.props && <h2>{this.props.title}</h2> }
                { 'header' in this.props && this.props.header }
                { controls }

                <div className="col-sm-offset-3">
                    <input type="hidden" id="form_action_name" value={this.state.actionName} name="actionName" />
                    {
                        submitButtons.map((btn) => {
                            return <button type="submit" className="btn btn-primary ml-2"
                                key={btn.actionName}
                                value={btn.actionName}
                                onClick={this.setActionName}>
                                    { btn.text }
                                </button>
                        })
                    }
                </div>
            </form>
        )
    }
}