import React from 'react';
import autoBind from 'react-autobind';
import { StateProxy, AsyncFormHelper } from "../AsyncFormHelper";

const selectMessages = Object.freeze({
    scanning: "Scanning for networks...",
    noNetworks: "No networks found.",
    networksAvailable: "You could select a network from the list or configure station manually."
})

const openAuth = "OPEN"

export default class EditStation extends React.Component {
    constructor(props) {
        super(props);
        autoBind(this);
        this.reloadAfterRefreshTime = props.reloadAfterRefreshTime || 5000;
		this.state = {};
        this.apiHandler = props.apiHandler;
        this.state = {
            formData: {dhcp: false},
            networks: [],
            networkSelectInfo: selectMessages.scanning,
            selectedNetwork: "none",
            selectedNetworkDetails: {},
            reloadTimer: null
        };
        this.formHelper = new AsyncFormHelper("station", props.apiHandler, new StateProxy(this));
    }

    componentDidMount() {
        let networksPromise = this.loadNetworks();
        this.formHelper
            .loadState()
            .then(responseData => {
                let formData = this.state.formData;
                // DHCP field is not used by server. It's pure UI input.
                if(formData.ip === "" && formData.gateway === "" && formData.netmask === "") {
                    formData.dhcp = true;
                } else {
                    formData.dhcp = false;
                }
                this.setState({formData: formData});
                return networksPromise
            })
            .then(() => {
                let loadedNetwork = this.state.formData.ssid;
                let nameMatch = this.state.networks.find(network => network.ssid === loadedNetwork)
                if(nameMatch) {
                    // TODO update state if loaded network is present on available networks list
                }
            })
    }
    
    componentWillUnmount() {
        this.clearTimer();
    }

    onSubmitCallback(event) {
        this.formHelper.submitForm(event);
    }

    addQualityFlagAndSort(networks) {
        networks = networks.map(network => {
            // https://www.speedguide.net/faq/how-does-rssi-dbm-relate-to-signal-quality-percent-439
            let quality = 0;
            if(!("rssi" in network)) {
                quality = 0;
            } else if(network.rssi >= -50) {
                quality = 100;
            } else if(network.rssi <= -100) {
                quality = 0;
            } else {
                quality = 2 * (network.rssi + 100)
            }
            network.quality = quality;
            return network;
        }).filter(network => !network.hidden)
        function compare( a, b ) {
            if ( a.rssi < b.rssi ){
              return 1;
            }
            if ( a.rssi > b.rssi ){
              return -1;
            }
            return 0;
          }
        return networks.sort(compare)
    }

    loadAfterTime() {
        if(this.state.reloadTimer === null) {
            let reloadTimer = setTimeout(this.loadNetworks, this.reloadAfterRefreshTime);
            this.setState({
                reloadTimer: reloadTimer,
                networkSelectInfo: selectMessages.scanning
            });
        }
    }

    clearTimer() {
        if(this.state.reloadTimer !== null) {
            clearTimeout(this.state.reloadTimer);
            this.setState({
                reloadTimer: null
            });
        }
    }

    refreshNetworks() {
        let url = "/config/networks/refresh";
        if(this.state.reloadTimer === null) {
            this.loadAfterTime();
            this.apiHandler.getJSON(url);
            }
    }

    loadNetworks() {
        this.clearTimer();
		let url = "/config/networks/get";
		this.apiHandler.getJSON(url)
            .done(function(data, status, jqXHR) {
                var newState = {
                    networks: [],
                    networkSelectInfo: selectMessages.noNetworks,
                    selectedNetwork: ""
                }
                switch(jqXHR.status) {
                    case 200:
                        newState.networks = this.addQualityFlagAndSort(data.networks);
                        newState.networkSelectInfo = selectMessages.networksAvailable;
                    break;
                    case 202:
                        this.loadAfterTime();
                        newState.networkSelectInfo = selectMessages.scanning;
                    break;
                    case 204:
                    break;
                    default:
                        break;
                }
                this.setState(newState);
            }.bind(this));
    }

    networkSelectChange(event) {
        let selectedNetwork = event.target.value;
        if(this.state.dataLoaded) {
            if(selectedNetwork !== "none") {
                let networkDetails = this.state.networks.find(network => network.ssid === selectedNetwork);
                let newState = {
                    selectedNetwork: selectedNetwork,
                    selectedNetworkDetails: networkDetails
                };
                if(networkDetails) {
                    let formData = this.state.formData;
                    formData.ssid = networkDetails.ssid;
                    newState.formData = formData;
                }
                this.setState(newState);
            }

        } else {
            // data was not loaded yet. We don't allow to select network here
            this.setState({selectedNetwork: "none"});

        }
    }

    formFieldChange(event) {
        const target = event.target;
        const value = target.type === 'checkbox' ? target.checked : target.value;
        const nameAndJsonType = target.name;
        const name = nameAndJsonType.substring(0, nameAndJsonType.indexOf(":"));
        let formData = this.state.formData;
        formData[name] = value;
        if(name === "dhcp") {
            formData.ip = "";
            formData.gateway = "";
            formData.netmask = "";
        }
        this.setState(formData);
    }

    genericFormDataLoaded(getFormState, setFormState) {
        // So we can set for values based on selected network
        this.setState({
            getFormState: getFormState,
            setFormState: setFormState
        })
    }

    selectNetworkHtml() {
        return (
            <div className="form-group row" id="form_row_availableNetworks">
                <label className="col-sm-2 col-form-label" htmlFor="form_availableNetworks">Available networks</label>
                <div className="col-sm-10">
                    <div className="input-group">
                        <select
                            className="form-control"
                            id="form_availableNetworks"
                            name="availableNetworks:string"
                            onChange={this.networkSelectChange}
                            value={this.state.selectedNetwork}
                            >
                            <option value="none" key="none"></option>
                            { this.state.networks.map(network => 
                                <option key={network.ssid} value={network.ssid}>
                                    {network.ssid} ({network.quality}%)
                                </option>
                            ) }
                        </select>
                        <div className="input-group-append">
                            <button
                             className="btn btn-outline-secondary"
                             disabled={this.state.reloadTimer !== null}
                             type="button"
                             onClick={this.refreshNetworks}>
                                 {this.state.reloadTimer === null ? "Refresh" : "Refreshing..."}
                            </button>
                        </div>
                    </div>
                    <small className="form-text text-muted">{this.state.networkSelectInfo}</small>
                </div>
            </div>
            )
    }

    render() {
        if(!this.state.dataLoaded) {
            return null;
        }
        let data = this.state.formData

        let addresses = <>
            <div className="form-group row" id="form_row_ip">
                <label className="col-sm-2 col-form-label" htmlFor="form_ip">Ip</label>
                <div className="col-sm-10">
                    <input className="form-control" type="text" id="form_ip" name="ip:string"
                        value={data.ip} onChange={this.formFieldChange} />
                </div>
            </div>
            <div className="form-group row" id="form_row_netmask">
                <label className="col-sm-2 col-form-label" htmlFor="form_netmask">Netmask</label>
                <div className="col-sm-10">
                    <input className="form-control" type="text" id="form_netmask" name="netmask:string"
                        value={data.netmask} onChange={this.formFieldChange} />
                </div>
            </div>
            <div className="form-group row" id="form_row_gateway">
                <label className="col-sm-2 col-form-label" htmlFor="form_gateway">Gateway</label>
                <div className="col-sm-10">
                    <input className="form-control" type="text" id="form_gateway" name="gateway:string"
                        value={data.gateway} onChange={this.formFieldChange} />
                </div>
            </div>
        </>

        return (
            <form className="form-horizontal"
            id={'formId' in this.state ? this.state.formId : 'form'}
            onSubmit={this.onSubmitCallback}>
            <h2>Edit station configuration</h2>

            <div className="form-group row" id="form_row_enabled">
                <label className="col-sm-2 col-form-label" htmlFor="form_enabled">Enabled</label>
                <div className="col-sm-10">
                    <div className="form-check">
                        <input className="form-check-input" type="checkbox" id="form_enabled" name="enabled:boolean"
                            checked={data.enabled} value="true" onChange={this.formFieldChange} />
                    </div>
                </div>
            </div>

            {this.selectNetworkHtml()}

            <div className="form-group row" id="form_row_ssid">
                <label className="col-sm-2 col-form-label" htmlFor="form_ssid">Ssid</label>
                <div className="col-sm-10">
                    <input className="form-control" type="text" id="form_ssid" name="ssid:string"
                        value={data.ssid} onChange={this.formFieldChange} />
                </div>
            </div>
            { this.state.selectedNetworkDetails.authorization !== openAuth &&
                <div className="form-group row" id="form_row_password">
                    <label className="col-sm-2 col-form-label" htmlFor="form_password">Password</label>
                    <div className="col-sm-10">
                        <input className="form-control" type="password" id="form_password" name="password:string"
                            value={data.password} onChange={this.formFieldChange} />
                    </div>
                </div>
            }
            

            <div className="form-group row" id="form_row_hostname">
                <label className="col-sm-2 col-form-label" htmlFor="form_hostname">Ssid</label>
                <div className="col-sm-10">
                    <input className="form-control" type="text" id="form_hostname" name="hostname:string"
                        value={data.hostname} onChange={this.formFieldChange} />
                </div>
            </div>

            <div className="form-group row" id="form_row_ip">
                <label className="col-sm-2 col-form-label" htmlFor="form_dhcp">Use DHCP</label>
                <div className="col-sm-10">
                    <div className="form-check">
                        <input className="form-check-input" type="checkbox" id="form_dhcp" name="dhcp:boolean"
                            checked={data.dhcp} value="true" onChange={this.formFieldChange} />
                    </div>
                </div>
            </div>
            {!this.state.formData.dhcp && addresses}
            <div className="col-sm-offset-3">
                <button type="submit" className="btn btn-primary" id="form_submit">Submit</button>
            </div>
        </form>
        )
    }
}