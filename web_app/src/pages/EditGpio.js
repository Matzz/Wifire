import { GenericForm } from "./GenericForm";
import autoBind from 'react-autobind';

//TODO remove inheritance. Decouple GenericForm to reuse some state handling here
export default class EditGpio extends GenericForm {
    constructor(props) {
        super(props);
        autoBind(this);
		this.state.type = "gpio";
    }
	
	static isPinSafeToUse(pin) {
		var safePins = [2,3,4,5,13,14,15,16];
		return safePins.includes(parseInt(pin));
	}

	fieldName(i, name, fieldType) {
		// Single quotes doesn't work well with ArduinoJson
		return "gpio[" + i + "][" + name + "]:"+fieldType;
	}

	render() {
        if(!this.state.dataLoaded) {
            return null;
        }
		var inputs = this.state.response_data.gpio; // We dont use preformated state.inputs here
		let fields = Object.keys(inputs).map(idx => {
			let pin = inputs[idx];
			let editablePin = (<>
				<td>
					<select name={ this.fieldName(idx, 'isInput', 'boolean') } defaultValue={pin["isInput"]} className="custom-select">
						<option value="true">Input</option>
						<option value="false">Output</option>
					</select>
				</td>
				<td>
					<input
						type="checkbox"
						name={ this.fieldName(idx, 'pull', 'boolean') }
						defaultChecked={pin["pull"]}
						value="true"
						className="form-check-input position-static" />
				</td>
			</>);
			let nonEditablePin = (<td colSpan="2">It is not safe to use that pin</td>);
			return <tr key={idx}>
				<th>{ idx }</th>
				<th>
					<input type="text" name={ this.fieldName(idx, 'name', 'string') } defaultValue={ pin['name'] } className="form-control" />
				</th>
				{ EditGpio.isPinSafeToUse(idx) ? editablePin : nonEditablePin }
			</tr>
		})

		return (
			<form className="form-horizontal" 
				id={ 'formId' in this.state ? this.state.formId : 'form' }
				onSubmit={this.onSubmitCallback.bind(this)}>
			<h2>Edit GPIO configuration</h2>
			<table className="table">
				<thead>
					<tr>
						<th scope="col">Pin number</th>
						<th scope="col">Pin name</th>
						<th scope="col">Mode</th>
						<th scope="col">Pull up (down GPIO 16)</th>
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