import React from 'react';
import autoBind from 'react-autobind';


function formatKey(key) {
    // From https://gist.github.com/jonlabelle/5375315
    key = key.replace(/([a-z\xE0-\xFF])([A-Z\xC0\xDF])/g, "$1 $2");
    key = key.toLowerCase(); //add space between camelCase text
    return key;
}

function formatValue(val) {
    switch(typeof(val)) {
        case "boolean":  return val ? '\u2611' : '\u2612';
        default: return val;
    }
}

function renderTable(title, keys, data) {
    return <>
        <br />
        <h4>{title}</h4>

        <table className="table table-sm table-striped table-hover">
            <tbody>
            {keys.map((i) =>
                <tr key={i}>
                    <th className="status_table_col">{ formatKey(i) }</th>
                    <td>{ formatValue(data[i]) }</td>
                </tr>
            )}
            </tbody>
        </table>
    </>
}
export default class Status extends React.Component {
    constructor(props) {
        super(props);
        autoBind(this);
        this.state = {"rows": []};
    }

    componentDidMount() {
		var formUrl = "/status";
		this.props.apiHandler.getJSON(formUrl)
        .done(function(response_data) {
            this.setState({
                rows: response_data
            });
		}.bind(this));
    }


    render() {
        let keys = Object.keys(this.state.rows);
        console.log(keys);
        let heap = keys.filter((key) => key.startsWith("heap"));
        console.log("heap", heap);
        let ap = keys.filter((key) => key.startsWith("wifiAp"));
        let station = keys.filter((key) => key.startsWith("wifiStation"));
        let nonOther = heap + station + ap;
        let other = keys.filter((key) => !nonOther.includes(key));

        return <>
            <h2>Status</h2>
            {renderTable("System", other, this.state.rows)}
            {renderTable("Heap", heap, this.state.rows)}
            {renderTable("Wifi access point", ap, this.state.rows)}
            {renderTable("Wifi station", station, this.state.rows)}
        </>
    }
}