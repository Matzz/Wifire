import React, { useState } from 'react';
import { Route, Switch, withRouter, useParams } from "react-router";
import { Link } from 'react-router-dom';
import autoBind from 'react-autobind';

function List(props) {

    if(!props.users) {
        return null;
    }

    let users = props.users.map(user => {
        let rolesStr = user.roles.join(", ");
        return (<tr key={user.login}>
            <td>{user.enabled ? '\u2611' : '\u2612'}</td>
            <td>{user.login}</td>
            <td>{rolesStr}</td>
            <td>
                <Link to={`/users/edit/${user.login}`}>Edit</Link>
                <span> / </span>
                <Link to={`/users/delete/${user.login}`}>Delete</Link>
            </td>
        </tr>)
    })

    return <>
        <table className="table">
            <thead>
                <tr>
                    <th>Enabled</th>
                    <th>Login</th>
                    <th>Roles</th>
                    <th>Actions</th>
                </tr>
            </thead>
            <tbody>
                {users}
            </tbody>
        </table>
        <div className="row">
            <div className="col-sm-12">
                <Link to="/users/add">New user</Link>
            </div>
        </div>
    </>
}

function Form(props) {
    const [state, setState] = useState({ buttonsDisabled: false });
    function submitCallback(event) {
        return props.onSubmitCallback(event, setState);
    }
    function resetCallback(event) {
        return props.onResetCallback(event, setState);
    }

    return (<>
        <form className="form-horizontal" onSubmit={submitCallback} onReset={resetCallback}>
            <h2 >{props.title}</h2>
            <div className="form-group row">
                <label className="col-sm-2 col-form-label" htmlFor="form_enabled">Enabled</label>
                <div className="col-sm-10">
                    <div className="form-check">
                        <input
                            type="checkbox"
                            className="form-check-input"
                            id="form_enabled"
                            name="enabled:boolean"
                            value="true"
                            defaultChecked={props.enabled}
                        />
                    </div>
                </div>
            </div>
            <div className="form-group row">
                <label className="col-sm-2 col-form-label" htmlFor="form_login">Login</label>
                <div className="col-sm-10">
                    <input
                        type="text"
                        className="form-control"
                        id="form_login"
                        name="login"
                        defaultValue={props.login}
                        readOnly={props.loginReadOnly}
                    />
                </div>
            </div>
            <div className="form-group row">
                <label className="col-sm-2 col-form-label" htmlFor="form_roles">Roles</label>
                <div className="col-sm-10">
                    <input
                        type="text"
                        className="form-control"
                        id="form_roles"
                        name="roles"
                        defaultValue={props.roles}
                    />
                    <small className="form-text text-muted">Please provide coma separated list.</small>
                </div>
            </div>
            <div className="form-group row">
                <label className="col-sm-2 col-form-label" htmlFor="form_password">Password</label>
                <div className="col-sm-10">
                    <input
                        type="password"
                        className="form-control"
                        id="form_password"
                        name="password"
                    />
                    <small className="form-text text-muted">Please provide only if you would like to change user password.</small>
                </div>
            </div>
            <div className="row">
                <div className="offset-2 col-sm-1">
                    <button type="submit" className="btn btn-primary" disabled={state.buttonsDisabled}>Submit</button>
                </div>
                <div className="col-sm-1">
                    <button type="reset" className="btn btn-light" disabled={state.buttonsDisabled}>Cancel</button>
                </div>
            </div>
        </form>
    </>
    );
}

function AddUser(props) {
    return <Form
        title="Add user"
        enabled={true}
        onSubmitCallback={props.onSubmitCallback}
        onResetCallback={props.onResetCallback}
        />;
}

function EditUser(props) {
    let { login } = useParams();
    let user = props.users.find(el => el.login === login);
    if(!user) {
        return `User ${login} not found`;
    }
    return <Form
        title={`Edit user ${login}`}
        loginReadOnly={true}
        enabled={user.enabled}
        login={user.login}
        roles={user.roles}
        onSubmitCallback={props.onSubmitCallback}
        onResetCallback={props.onResetCallback}
        />;
}

class DeleteUser extends React.Component {
    constructor(props) {
        super(props);
        this.state = {deleteCompleted: false};
    }
    componentDidMount() {
        let { login } = this.props.match.params;
        this.props.deleteCallback(login);
    }

    render() {
        return "Please wait..."
    }
}
DeleteUser = withRouter(DeleteUser);

class UserManagement extends React.Component {
    constructor(props) {
        super(props);
        this.apiHandler = props.apiHandler;
        autoBind(this);
        this.state = {
            users: []
        }
    }
    reloadList() {
        return this.apiHandler
            .getJSON("/config/users/list")
            .done(function(data) {
                this.setState({users: data.users})
            }.bind(this));
    }

    componentDidMount() {
        this.reloadList();
    }

    backToList() {
        this.reloadList()
            .then(function() {
                this.props.history.push("/users/list")
            }.bind(this));
    }

	normalizeRoles(data) {
		if(data.roles) {
			var rolesList = data.roles.split(",");
			var nonEmptyRoles = rolesList.map(function(value) {
				return value.trim();
			}).filter(function(v) {
				return v !== "";
			});
            const set = new Set();
            for(let v of nonEmptyRoles){
                set.add(v);
            }
			data.roles = Array.from(set);
		}
		return data;
	}

    addUserSubmitCallback(event, setFormState) {
        setFormState({buttonsDisabled: true });
		var formData = this.normalizeRoles(
            this.apiHandler.getFormData(event.target)
            );
		this.apiHandler.postJSON("/config/users/add", formData)
			.done(function() {
                let promise = this.reloadList()
                    .then(function(){
                        this.props.history.push("/users/list");
                    }.bind(this))
                alert("User " + formData.login + " created.");
                return promise;
			}.bind(this))
			.fail(function(jqXHR, textStatus, errorThrown) {
				setFormState({buttonsDisabled: false });
			});
		event.preventDefault();
    }

    editUserSubmitCallback(event, setFormState) {
        setFormState({buttonsDisabled: true });
		var formData = this.normalizeRoles(
            this.apiHandler.getFormData(event.target)
            );
		this.apiHandler.postJSON("/config/users/edit", formData)
			.done(function() {
                let promise = this.reloadList()
                    .then(function(){
                        this.props.history.push("/users/list");
                    }.bind(this));
                alert("User " + formData.login + " modified.");
                return promise;
			}.bind(this))
			.fail(function(jqXHR, textStatus, errorThrown) {
				setFormState({buttonsDisabled: false });
			});
		event.preventDefault();
    }

    deleteCallback(login) {
        if(window.confirm("Are you sure you want to delete user '"+login+"'?")) {
            this.apiHandler.postJSON("/config/users/remove", {'login': login})
                .done(function() {
                    alert("User " + login + " removed.");
                })
                .always(function() {
                    this.backToList();
                }.bind(this));
        } else {
            this.backToList();
        }
    }

    formResetCallback(event, setFormState) {
        setFormState({buttonsDisabled: true });
        this.backToList();
        event.preventDefault();
    }

    render() {
        const { path } = this.props.match;
        return (
            <Switch>
                <Route exact path={`${path}/list`}>
                    <List users={this.state.users} />
                </Route>
                <Route exact path={`${path}/add`}>
                    <AddUser
                        onSubmitCallback={this.addUserSubmitCallback}
                        onResetCallback={this.formResetCallback} />
                </Route>
                <Route exact path={`${path}/edit/:login`}>
                    <EditUser
                        users={this.state.users}
                        onSubmitCallback={this.editUserSubmitCallback}
                        onResetCallback={this.formResetCallback}
                    />
                </Route>
                <Route exact path={`${path}/delete/:login`}>
                    <DeleteUser apiHandler={this.apiHandler} deleteCallback={this.deleteCallback} />
                </Route>
            </Switch>
        )
    }
}

export default withRouter(UserManagement);
