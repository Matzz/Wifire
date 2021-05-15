import React, { useState } from 'react';
import { NavLink } from "react-router-dom";
import autoBind from 'react-autobind';
import { withRouter } from 'react-router';
import ApiHandler from "./ApiHandler";
import $ from "jquery";


function CollapsableMenu(props) {
  const [open, setOpen] = useState(false);

  let children = React.Children.toArray(props.children).filter((child) => child !== false)

  var handleClick = function(event) {
    event.preventDefault();
    setOpen(!open)
  };


  return (React.Children.count(children)>0  && <>
    <li className="nav-item">
      <a href="/" onClick={handleClick} className={["dropdown-toggle", (open ? "active" : "")].join(" ")}>
         {props.title}
      </a>
      <ul className={["list-unstyled", (open ? "open" : "closed")].join(" ")}>
        {children}
      </ul>
    </li>
  </>)
}


class Sidebar extends React.Component {
  constructor(props) {
    super(props);
    autoBind(this);
    this.apiHandler = props.apiHandler;
    this.state = {
      auth: ApiHandler.emptyAuth
    };
  }
  updateAuth(auth) {
    this.setState({"auth": auth});
  }
  componentDidMount() {
    this.apiHandler.addUpdateAuthCallback(this.updateAuth);
    this.updateAuth(this.apiHandler.getAuthJson());
  }
  componentWillUnmount() {
    this.apiHandler.removeUpdateAuthCallback(this.updateAuth);
  }
  hasRole(role) {
    let roles = this.state.auth.roles;
    return roles.includes(role) || roles.includes("admin");
  }
  isSignedin(role) {
    var login = this.state.auth.login;
    return login !== "" && login != null;
  }

  handleSidebarCollapse(event) {
    $('#sidebar').toggleClass('active');
    $('#sidebar-collapse-btn').toggleClass('active');
  }

  render() {
    return (
      <>
        <nav id="sidebar">
          <button type="button" id="sidebar-collapse-btn" className="navbar-btn" onClick={this.handleSidebarCollapse}>
              <span></span>
              <span></span>
              <span></span>
          </button>
          <div className="sidebar-header">
            <NavLink className="nav-link" to="/">Wifire <span className="user_name">{this.state.auth.login}</span></NavLink>
          </div>

          <ul className="nav flex-column">
            {!this.isSignedin() && <li className="nav-item"><NavLink className="nav-link" to="/signin">Sign in</NavLink></li>}
            {this.isSignedin() && <li className="nav-item"><NavLink className="nav-link" to="/signout">Sign out</NavLink></li>}
            {this.hasRole("show_status") && <li className="nav-item"><NavLink className="nav-link" to="/status">Status</NavLink></li>}
            <CollapsableMenu title="WiFi configuration">
                {this.hasRole("edit_ap_config") && <li className="nav-item"><NavLink className="nav-link" to="/edit_ap">Ap config</NavLink></li>}
                {this.hasRole("edit_station_config") && <li className="nav-item"><NavLink className="nav-link" to="/edit_station">Station config</NavLink></li>}
            </CollapsableMenu>
            
            <CollapsableMenu title="GPIO">
                {this.hasRole("edit_gpio_config") && <li className="nav-item"><NavLink className="nav-link" to="/edit_gpio">GPIO config</NavLink></li>}
                {this.hasRole("switch_gpio") && <li className="nav-item"><NavLink className="nav-link" to="/switch_gpio">GPIO swich</NavLink></li>}
            </CollapsableMenu>
            {this.hasRole("edit_ota_config") && <li className="nav-item"><NavLink className="nav-link" to="/edit_ota">Ota config</NavLink></li>}
            {this.hasRole("edit_users_config") && <li className="nav-item"><NavLink className="nav-link" to="/users/list">Users</NavLink></li>}
          </ul>
        </nav>
      
      </>
    )
  }
}

const SidebarWithRouter = withRouter(Sidebar);

export {
    SidebarWithRouter as Sidebar
};