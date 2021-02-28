
import './App.css';
import React from 'react';
import { NavLink } from "react-router-dom";
import { Route, Switch } from "react-router";
import autoBind from 'react-autobind';
import { ApiHandler } from "./ApiHandler";

import HomePage from "./pages/HomePage"
import { Signin, Signout } from './pages/Auth';
import Status from './pages/Status';
import { EditAp, EditStation, EditOta } from "./pages/SimpleForms";
import EditGpio from "./pages/EditGpio";
import UserManagement from "./pages/Users";



class App extends React.Component {
  constructor(props) {
    super(props);
    autoBind(this);
    this.apiHandler = new ApiHandler();
    this.apiHandler.addUpdateAuthCallback(this.updateAuth);
    this.state = {
      auth: ApiHandler.emptyAuth
    };
  }
  updateAuth(auth) {
    this.setState({"auth": auth});
  }
  componentDidMount() {
    this.updateAuth(this.apiHandler.getAuthJson());
  }
  hasRole(role) {
    let roles = this.state.auth.roles;
    return roles.includes(role) || roles.includes("admin");
  }
  isSignedin(role) {
    var login = this.state.auth.login;
    return login !== "" && login != null;
  }

  render() {
    return (
      <>
      <nav className="main-navbar navbar navbar-expand-lg navbar-dark bg-primary">
        <NavLink className="navbar-brand" to="/">Wifire <span className="user_name">{this.state.auth.login}</span></NavLink>
        <ul className="navbar-nav">
          {!this.isSignedin() && <li className="nav-item"><NavLink className="nav-link" to="/signin">Sign in</NavLink></li>}
          {this.isSignedin() && <li className="nav-item"><NavLink className="nav-link" to="/signout">Sign out</NavLink></li>}
          {this.hasRole("show_status") && <li className="nav-item"><NavLink className="nav-link" to="/status">Status</NavLink></li>}
          {this.hasRole("edit_ap_config") && <li className="nav-item"><NavLink className="nav-link" to="/edit_ap">Ap config</NavLink></li>}
          {this.hasRole("edit_station_config") && <li className="nav-item"><NavLink className="nav-link" to="/edit_station">Station config</NavLink></li>}
          {this.hasRole("edit_gpio_config") && <li className="nav-item"><NavLink className="nav-link" to="/edit_gpio">GPIO config</NavLink></li>}
          {this.hasRole("edit_ota_config") && <li className="nav-item"><NavLink className="nav-link" to="/edit_ota">Ota config</NavLink></li>}
          {this.hasRole("edit_users_config") && <li className="nav-item"><NavLink className="nav-link" to="/users/list">Users</NavLink></li>}
        </ul>
      </nav>
      
      <div className="container" id="content">
        <Switch>
          <Route exact path="/">
            <HomePage login={this.state.auth.login} />
          </Route>
          <Route path="/signin">
            <Signin apiHandler={this.apiHandler} />
          </Route>
          <Route path="/signout">
            <Signout apiHandler={this.apiHandler} />
          </Route>
          <Route path="/status">
            <Status apiHandler={this.apiHandler} />
          </Route>
          <Route path="/edit_ap">
            <EditAp apiHandler={this.apiHandler} />
          </Route>
          <Route path="/edit_station">
            <EditStation apiHandler={this.apiHandler} />
          </Route>
          <Route path="/edit_ota">
            <EditOta apiHandler={this.apiHandler} />
          </Route>
          <Route path="/edit_gpio">
            <EditGpio apiHandler={this.apiHandler} />
          </Route>
          <Route path="/users">
            <UserManagement apiHandler={this.apiHandler} />
          </Route>
          <Route>
            Not found.
          </Route>
        </Switch>
      </div>
      </>
    )
  }
}

export default App;