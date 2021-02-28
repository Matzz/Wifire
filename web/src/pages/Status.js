import React from 'react';
import autoBind from 'react-autobind';

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
        return (
            <table className="table">
                <tbody>
                {Object.keys(this.state.rows).map((i) =>
                    <tr key={i}>
                        <th>{ i }</th>
                        <td>{ this.state.rows[i] }</td>
                    </tr>
                )}
                </tbody>
            </table>
        )
    }
}