const chai = require('chai');
const chaiHttp = require('chai-http');

const { serverConf } = require('./config');

chai.use(chaiHttp);
const expect = chai.expect

function isUndefined(v) {
    return typeof(v)=="undefined";
}

function getAgent() {
    return chai
        .request
        .agent(serverConf.host)
}

function doLogin({agent = getAgent(), login, password}) {
    password = isUndefined(password) ? login : password;
    var promise = agent
        .post('signin')
        .send({
            'login': login,
            'password': password
            })
        .then(res => {
            res.agent = agent;
            return res;
        })
    return promise;
}

function doSucessfulLogin({agent = getAgent(), login, password}) {
    return doLogin({agent: agent, login: login, password: password})
        .then(res => {
            expect(res).to.json;
            expect(res).to.have.status(200);
            expect(res).to.have.cookie('auth');
            res.agent = agent;
            return res;
        });
}

function doSucessfulLogout(agent) {
    var response = agent
        .get('signout')
        .then(res => {
            expect(res).to.have.status(200);
            res.agent = agent;
            return res;
        })
    return response;
}

function doDeleteUser({agent, login}) {
    var result = agent
    .post("config/users/remove")
    .send({ 'login': login })
    .then(res => {
        res.agent = agent;
        return res;
    });
    return result;
}

function doSucessfulDeleteUser({agent, login}) {
    return doDeleteUser({agent: agent, login: login})
        .then(res => {
            expect(res).to.have.status(200);
            return res;
        });
}

function doAddUser({agent, login, password, roles = [], enabled = true}) {
    password = isUndefined(password) ? login : password;
    
    var result = agent
        .post("config/users/add")
        .send({
            'enabled': enabled,
            'login': login,
            'password': password,
            'roles': roles
            })
        .then(res => {
            res.agent = agent;
            return res;
        });
    return result;
}

function doSucessfulAddUser({agent, login, password, roles = [], enabled = true}) {
    return doAddUser({agent: agent, login: login, password: password, roles: roles, enabled: enabled})
        .then(res => {
            expect(res).to.have.status(200);
            return res;
        });
}

function doEditUser({agent, login, password, roles = [], enabled = true}) {
    password = isUndefined(password) ? login : password;
    
    var result = agent
        .post("config/users/edit")
        .send({
            'enabled': enabled,
            'login': login,
            'password': password,
            'roles': roles
            })
        .then(res => {
            res.agent = agent;
            return res;
        });
    return result;
}

function doSucessfulEditUser({agent, login, password, roles = [], enabled = true}) {
    return doEditUser({agent: agent, login: login, password: password, roles: roles, enabled: enabled})
        .then(res => {
            expect(res).to.have.status(200);
            return res;
        });
}

module.exports = {
    isUndefined: isUndefined,
    getAgent: getAgent,
    doLogin: doLogin,
    doSucessfulLogin: doSucessfulLogin,
    doSucessfulLogout: doSucessfulLogout,
    doDeleteUser: doDeleteUser,
    doSucessfulDeleteUser: doSucessfulDeleteUser,
    doAddUser: doAddUser,
    doSucessfulAddUser: doSucessfulAddUser,
    doEditUser: doEditUser,
    doSucessfulEditUser: doSucessfulEditUser
}