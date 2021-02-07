const chai = require('chai');
const chaiHttp = require('chai-http');
const { isUndefined } = require('underscore');

const { serverConf } = require('./config');

chai.use(chaiHttp);
const expect = chai.expect

function getAgent() {
    return chai
        .request
        .agent(serverConf.host)
}

function doLogin({agent = getAgent(), login, password}) {
    password = isUndefined(password) ? login : password;
    var promise = agent
        .post('signin')
        .type('form')
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
    .type('form')
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

function doAddUser({agent, login, password, roles = "", enabled = true}) {
    password = isUndefined(password) ? login : password;
    
    var result = agent
        .post("config/users/add")
        .type('form')
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

function doSucessfulAddUser({agent, login, password, roles = "", enabled = true}) {
    return doAddUser({agent: agent, login: login, password: password, roles: roles, enabled: enabled})
        .then(res => {
            expect(res).to.have.status(200);
            return res;
        });
}

module.exports = {
    getAgent: getAgent,
    doLogin: doLogin,
    doSucessfulLogin: doSucessfulLogin,
    doSucessfulLogout: doSucessfulLogout,
    doDeleteUser: doDeleteUser,
    doSucessfulDeleteUser: doSucessfulDeleteUser,
    doAddUser: doAddUser,
    doSucessfulAddUser: doSucessfulAddUser
}