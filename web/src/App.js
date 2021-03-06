

import './App.scss';
import React from 'react';
import { Route, Switch } from "react-router";
import autoBind from 'react-autobind';
import { withRouter } from 'react-router';
import ApiHandler from "./ApiHandler";

import { Sidebar } from "./Sidebar"
import HomePage from "./pages/HomePage"
import { Signin, Signout } from './pages/Auth';
import Status from './pages/Status';
import EditAp from "./pages/EditAp";
import EditStation from "./pages/EditStation";
import EditOta from "./pages/EditOta";
import EditGpio from "./pages/EditGpio";
import UserManagement from "./pages/Users";



class App extends React.Component {
  constructor(props) {
    super(props);
    autoBind(this);
    this.apiHandler = new ApiHandler(props.history);
    this.state = {
      auth: ApiHandler.emptyAuth
    };
  }

  render() {
    return (
      <>
        <Sidebar apiHandler={this.apiHandler} />
      
        <div id="content">
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

const AppWithRouter = withRouter(App);

export {
  AppWithRouter as App
};