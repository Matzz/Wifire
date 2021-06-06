import React from 'react';
import $ from 'jquery';
import autoBind from 'react-autobind';
import { StateProxy, AsyncFormHelper } from "../AsyncFormHelper";

export default class EditGpio extends React.Component {
    constructor(props) {
        super(props);
        autoBind(this);
		this.state = {};
		this.formHelper = new AsyncFormHelper("gpio", props.apiHandler, new StateProxy(this));
    }
	
	isPinSafeToUse(pin) {
		let formData = this.state.formData;
		return formData && formData.safePins && formData.safePins.includes(parseInt(pin));
	}

    componentDidMount() {
        this.formHelper.loadState();
    }

    onSubmitCallback(event) {
        this.formHelper.submitForm(event);
    }

	fieldName(i, name, fieldType) {
		// Single quotes doesn't work well with ArduinoJson
		return "gpio[" + i + "][" + name + "]:"+fieldType;
	}

	inputTypeChange(event) {
		event.preventDefault();
		// formData
		// this.setState({formData[]})
		let fieldName = $(event.target).attr('name');
		let match = fieldName.match(/^gpio\[(\d+)\].*$/);
		let pinId = match[1];
		var gpio = this.state.formData.gpio;
		gpio[pinId] = gpio[pinId] || {};
		gpio[pinId].isInput = $(event.target).val() === 'true';
		gpio[pinId].switchTime = "";
		this.setState({gpio: gpio});
	}

	render() {
        if(!this.state.dataLoaded) {
            return null;
        }
		var inputs = this.state.formData.gpio; // We dont use preformated state.inputs here
		let fields = Object.keys(inputs).map(idx => {
			let pin = inputs[idx];
			var pinHtml;
			if(inputs[idx].isSafe) {
				pinHtml = (<>
					<td>
						<select name={ this.fieldName(idx, 'isInput', 'boolean') } defaultValue={pin["isInput"]} className="custom-select" onChange={this.inputTypeChange}>
							<option value="true">Input</option>
							<option value="false">Output</option>
						</select>
					</td>
					<td>
						{ !pin.isInput ? 
						<input
							type="number"
							name={ this.fieldName(idx, 'switchTime', 'number') }
							defaultValue={pin["switchTime"]}
							value={ this.state.formData['switchTime'] }
							className="form-control w-25 position-static" />
							: ""
						}
					</td>
					<td>
						<div className="form-check">
							<input
								type="checkbox"
								name={ this.fieldName(idx, 'pull', 'boolean') }
								defaultChecked={pin["pull"]}
								value="true"
								className="form-check-input position-static" />
						</div>
					</td>
				</>);
			} else {
				pinHtml = (<td colSpan="3">It is not safe to use that pin</td>);
			}
			return <tr key={idx}>
				<th>{ idx }</th>
				<th>
					<input type="text" name={ this.fieldName(idx, 'name', 'string') } defaultValue={ pin['name'] } className="form-control" />
				</th>
				{ pinHtml }
			</tr>
		})

		return (
			<form className="form-horizontal" 
				id={ 'formId' in this.state ? this.state.formId : 'form' }
				onSubmit={this.onSubmitCallback}>
			<h2>Edit GPIO configuration</h2>
			<p>
				To avoid infinite restart loop, not all pins available ESP pins options are configurable here. What's more, pull-down resistor are also not supported.
				If you need them, please use raw ESP api.
			</p>
			<table className="table">
				<thead>
					<tr>
						<th scope="col">Pin number</th>
						<th scope="col">Pin name</th>
						<th scope="col">Mode</th>
						<th scope="col">Turn on time (seconds)</th>
						<th scope="col">Pull up</th>
					</tr>
				</thead>
				<tbody>
					{ fields }
				</tbody>
			</table>
			<div className="col-sm-offset-3">
				<button type="submit" className="btn btn-primary" id="form_submit">Submit</button>
				<button type="reset" className="btn btn-light" id="form_submit">Reset</button>
				<br /><br />
			</div>
			</form>
		)
	}
}