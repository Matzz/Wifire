import React from 'react';
import autoBind from 'react-autobind';
import $ from "jquery";

export default class SwitchGpio extends React.Component {
    constructor(props) {
        super(props);
        autoBind(this);
		this.state = {};
		this.apiHandler = props.apiHandler;
    }

	loadConfiguration() {
        var url = "/gpio/available";
		this.apiHandler
            .getJSON(url)
            .done(function (responseData) {
                this.setState({
                    gpio: responseData['gpio'],
                    state: responseData['state'],
                    dataLoaded: true,
                	pinSwichInProgress: false
                });
                return responseData;
            }.bind(this));
	}

	swichPin(event) {
		if(this.state.pinSwichInProgress) {
			event.preventDefault();
			return false;
		}

		let idx = $(event.target).data('pin');
		let newState = this.state;
		let newPinValue = !newState.state[idx];
		newState.state[idx] = newPinValue;
		newState.pinSwichInProgress = true;
		this.setState(newState);
		let pinName = this.state.gpio[idx].name;
        var url = "/gpio/switch";
		let data = {name: pinName, state: newPinValue ? "on" : "off"};
		this.apiHandler
            .getJSON(url, data)
            .done(function (responseData) {
                this.setState({
                    state: responseData['state'],
                    dataLoaded: true,
                	pinSwichInProgress: false
                });
                return responseData;
            }.bind(this));
	}
	
    componentDidMount() {
		this.loadConfiguration();
    }

	swichablePinsRender(gpios, state) {
		let fields = Object.keys(gpios).map(idx => {
			let pin = gpios[idx];
			let isSwichable = !pin['isInput'] && pin['isSafe'];
			let html;
			if(isSwichable) {
				let afterSwichState = state[idx] ? "Off" : "On";
				html = <div key={idx}>
					<div className="col-10 custom-control custom-switch custom-switch-lg">
						<input
							type="checkbox"
							className="custom-control-input"
							id={"swich-btn"+idx}
							key={idx}
							onChange={this.swichPin}
							data-pin={idx}
							checked={state[idx] ? true : false}
							value="1"
							/>
							<label className="custom-control-label" htmlFor={"swich-btn"+idx}>
								<b>{ pin["name"] }</b>&nbsp;
								Click to turn {afterSwichState}
							</label>
					</div>
					<br />
				</div>;
			}
			return html;
		});
		return fields;
	}

	nonSwichablePinsRender(gpios, state) {
		let fields = Object.keys(gpios).map(idx => {
			let pin = gpios[idx];
			let pinState = state[idx] ? "On" : "Off";
			let isInput = pin['isInput'];
			let pinHtml = (<td>{pinState} {isInput ? "(Input pin)" : ""}</td>);
			return <tr key={idx}>
				<th>{ idx }</th>
				<td>
					{ pin["name"] }
				</td>
				{ pinHtml }
			</tr>
		});
		return fields;
	}

	render() {
        if(!this.state.dataLoaded) {
            return null;
        }
		var gpios = this.state.gpio;
		var state = this.state.state;

		return <>
			<h2>Swich pins</h2>
			{ this.swichablePinsRender(gpios, state) }
			<br />

			<h2>Pins state</h2>
			<table className="table table-sm table-striped table-hover">
				<thead>
					<tr>
						<th scope="col">Pin number</th>
						<th scope="col">Pin name</th>
						<th scope="col">State</th>
					</tr>
				</thead>
				<tbody>
					{ this.nonSwichablePinsRender(gpios, state) }
				</tbody>
			</table>
		</>
	}
}