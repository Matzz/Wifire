const chai = require('chai');
const {
    getAgent,
    doLogin,
    doSucessfulLogin,
    doSucessfulLogout,
    doDeleteUser,
    doSucessfulDeleteUser,
    doAddUser,
    doSucessfulAddUser,
    doEditUser,
    doSucessfulEditUser
} = require('./helpers');
const { serverConf } = require('./config');
const expect = chai.expect

describe('login actions', function() {
    it('return an error if user does not exist', () => {
        return doLogin(login = "indalid_user", password = "admin")
            .then(res => {
                expect(res).to.json;
                expect(res).to.have.status(400);
            });
    });
    it('return an error if password is invalid', () => {
        return doLogin(login = "admin", password = "password")
            .then(res => {
                expect(res).to.json;
                expect(res).to.have.status(400);
            });
    });
    it('return a session cookie', () => {
        return doSucessfulLogin({login: "admin"});
    });
    it('logout sucessfully', () => {
        return doSucessfulLogin({login: "admin"})
            .then(res => {
                return doSucessfulLogout(res.agent);
            });
    });
});


describe('permissions', function() {
    it('return an error if accessing restricted page without permissions', () => {
        var urls = ["info",
            "config/ap/get",
            "config/station/get",
            "config/gpio/get",
            "config/staotation/get",
            "config/users/list",
        ];
        return urls.reduce((promise, url) => {
            return promise.then(() => {
                return chai
                    .request(serverConf.host)
                    .get(url)
                    .then(res => {
                        expect(res).to.have.status(403);
                        return res;
                    })
            })
        }, Promise.resolve(true));

    });
    it('return status 200 if accessing restricted page with permissions', () => {
        var urls = ["info",
            "config/ap/get",
            "config/station/get",
            "config/gpio/get",
            "config/ota/get",
            "config/users/list"
        ];
        var result = urls.reduce((promise, url) => {
            return promise.then(resOld => {
                return resOld
                    .agent
                    .get(url)
                    .then(res => {
                        expect(res).to.have.status(200);
                        res.agent = resOld.agent;
                        return res;
                    })
            })
        }, doSucessfulLogin({login: "admin"}));
        return result;
    });
});


describe('user management', function() {
    it('create new user with no permissions', () => {
        var adminAgent = getAgent();
        var userAgent = getAgent();
        var userLogin = "test"
        var userPassword = "test2"

        return doSucessfulLogin({agent: adminAgent, login: "admin"})
            .then(() => { return doSucessfulAddUser({agent: adminAgent, login: userLogin, password: userPassword, roles: ""}) })
            .then(() => { return doSucessfulLogin({agent: userAgent, login: userLogin, password: userPassword}) })
            .then(() => {
                return userAgent
                    .get("info")
                    .then(res => {
                        expect(res).to.have.status(403);
                    })
            })
            .finally(() => {
                return doSucessfulLogout(userAgent)
                    .then(() => doDeleteUser({agent: adminAgent, login: userLogin}))
                    .then(() => doSucessfulLogout(adminAgent));
            })
    });
    it('create new user with limited permissions', () => {
        var adminAgent = getAgent();
        var userAgent = getAgent();
        var userLogin = "test"
        var userPassword = "test2"

        return doSucessfulLogin({agent: adminAgent, login: "admin"})
            .then(() => {
                return doSucessfulAddUser({
                    agent: adminAgent,
                    login: userLogin,
                    password: userPassword,
                    roles: ["edit_ap_config", "show_info"]
                });
            })
            .then(() => { return doSucessfulLogin({agent: userAgent, login: userLogin, password: userPassword}) })
            .then(() => {
                return userAgent
                    .get("config/station/get")
                    .then(stationCfgRes => {
                        expect(stationCfgRes).to.have.status(403);
                        return userAgent
                            .get("config/ap/get")
                            .then(apCfgRes => {
                                expect(apCfgRes).to.have.status(200);
                                return userAgent
                                .get("info")
                                .then(infoRes => {
                                    expect(infoRes).to.have.status(200);
                                });
                            })
                    })
                
            })
            .finally(() => {
                return doSucessfulLogout(userAgent)
                    .then(() => doDeleteUser({agent: adminAgent, login: userLogin}))
                    .then(() => doSucessfulLogout(adminAgent));
            })
    });

    it('modify user permission and password', () => {
        var adminAgent = getAgent();
        var userAgent = getAgent();
        var userLogin = "test"
        var userPassword1 = "test1"
        var userPassword2 = "test2"

        return doSucessfulLogin({agent: adminAgent, login: "admin"})
            .then(() => {
                return doSucessfulAddUser({
                    agent: adminAgent,
                    login: userLogin,
                    password: userPassword1,
                    roles: []
                });
            })
            .then(() => { return doSucessfulLogin({agent: userAgent, login: userLogin, password: userPassword1}) })
            .then(() => {
                return userAgent
                    .get("config/ap/get")
                    .then(stationCfgRes => {
                        expect(stationCfgRes).to.have.status(403);
                        return doSucessfulEditUser({agent: adminAgent, login: userLogin, password: userPassword2, roles: ["edit_ap_config"]})
                    })
                    .then(edutRes => {
                        return  doSucessfulLogin({agent: userAgent, login: userLogin, password: userPassword2});
                    })
                    .then(loginRes => {
                        userAgent
                        .get("config/ap/get")
                        .then(apCfgRes => {
                            expect(apCfgRes).to.have.status(200);
                        })
                    })
                
            })
            .finally(() => {
                return doSucessfulLogout(userAgent)
                    .then(() => doDeleteUser({agent: adminAgent, login: userLogin}))
                    .then(() => doSucessfulLogout(adminAgent));
            })
    });
});