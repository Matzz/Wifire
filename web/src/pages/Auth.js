import React from 'react';
import autoBind from 'react-autobind';
import { withRouter } from 'react-router';

class Signin extends React.Component {
    constructor(props) {
        super(props);
        this.apiHandler = props.apiHandler;
        autoBind(this);
    }

    handleSubmit(event) {
        this.apiHandler.postJSON("/signin", this.apiHandler.getFormData(event.target))
            .done(function(data) {
                alert("You have been signed in.");
                this.apiHandler.handleAuthUpdate();
                this.props.history.push('/');
            }.bind(this));
        event.preventDefault();
        return false;
    }

    render() {
        return (
            <form id="signin_form" className="form-signin" onSubmit={this.handleSubmit}>
                <div className="form-group">
                    <label htmlFor="form_login">Login</label>
                    <input type="text" className="form-control" id="form_login" name="login" placeholder="Enter login" />
                </div>
                <div className="form-group">
                    <label htmlFor="form_password">Password</label>
                    <input type="password" className="form-control" id="form_password" name="password" placeholder="Password" />
                </div>
                <button type="submit" className="btn btn-lg btn-primary btn-block">Submit</button>
            </form>
        )
    }
}

class Signout extends React.Component {
    constructor(props) {
        super(props);
        this.apiHandler = props.apiHandler;
        autoBind(this);
        this.signoutRequest = null;
    }

    signoutRequestCallback(data) {
        this.apiHandler.handleAuthUpdate();
        this.props.history.push('/signin');
    }

    componentDidMount() {
        if(this.signoutRequest === null) {
            this.signoutRequest = this.apiHandler
                .postJSON("/signout", {})
                .done(this.signoutRequestCallback)
                .always(function() {
                    this.signoutRequest = null;
                }.bind(this));
        }
    }

    render() {
        return null;
    }
}

const SigninWithRouter = withRouter(Signin);
const SignoutWithRouter = withRouter(Signout);


export {
    SigninWithRouter as Signin,
    SignoutWithRouter as Signout
} 