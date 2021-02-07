const chai = require('chai');
const {
    getAgent,
    doLogin,
    doSucessfulLogin,
    doSucessfulLogout,
    doDeleteUser,
    doSucessfulDeleteUser,
    doAddUser,
    doSucessfulAddUser
} = require('./helpers');
const { isHost } = require('./config');
const expect = chai.expect

describe('config actions', function() {
    it('edit ota config', () => {
        return doSucessfulLogin({login: "admin"})
            .then(resp => {
                var agent = resp.agent;
                var rnd = Math.random();
                var expectedRomUrl = 'abc' + rnd;
                var expectedSpiffUrl = 'defg' + rnd;
                return agent
                    .post("config/ota/set")
                    .type('form')
                    .send({
                        'romUrl': expectedRomUrl,
                        'spiffUrl': expectedSpiffUrl
                        })
                    .then(editRes => {
                        expect(editRes).to.have.status(200);
                        return agent.get("config/ota/get");
                    })
                    .then(getResp => {
                        expect(getResp).to.have.status(200);
                        expect(getResp).to.be.json;
                        var body = getResp.body;
                        expect(body.romUrl).to.be.eql(expectedRomUrl);
                        expect(body.spiffUrl).to.be.eql(expectedSpiffUrl);
                        return;;
                    })
            });
    });
    it('edit gpio config', () => {
        return doSucessfulLogin({login: "admin"})
            .then(resp => {
                var agent = resp.agent;
                var rnd = Math.random();
                var safePins = [2,3,4,5,13,14,15,16];
                var expectedGPIO = {};
                for(var i=0; i<=16; i++) {
                    expectedGPIO["gpio["+i+"]['name']"] = "GPIO_"+i+"_"+rnd;
                    expectedGPIO["gpio["+i+"]['isInput']"] = "true";
                    expectedGPIO["gpio["+i+"]['pull']"] = "true";
                }
                return agent
                    .post("config/gpio/set")
                    .type('form')
                    .send(expectedGPIO)
                    .then(editRes => {
                        expect(editRes).to.have.status(200);
                        return agent.get("config/gpio/get");
                    })
                    .then(getResp => {
                        expect(getResp).to.have.status(200);
                        expect(getResp).to.be.json;
                        var body = getResp.body['gpio'];
                        for(var i=0; i<=16; i++) {
                            expect(body[i].name).to.be.eql("GPIO_"+i+"_"+rnd);
                            var isSafe = safePins.includes(i);
                            expect(body[i].isInput).to.be.eql(isSafe ? true : false);
                            expect(body[i].pull).to.be.eql(isSafe ? true : false);
                        }
                        return;;
                    })
            });
    });
    it('edit station config', () => {
        if(isHost()) {
            return doSucessfulLogin({login: "admin"})
                .then(resp => {
                    var agent = resp.agent;
                    var rnd = Math.floor(Math.random()*10)
                    var expectedCfg = {
                        enabled: true,
                        ssid: "station_"+rnd,
                        password: "station_pass_"+rnd,
                        ip: "1.2.3."+rnd,
                        netmask: "2.2.3."+rnd,
                        gateway: "3.2.3."+rnd,
                    }
                    var expectedRomUrl = 'abc' + rnd;
                    var expectedSpiffUrl = 'defg' + rnd;
                    return agent
                        .post("config/station/set")
                        .type('form')
                        .send(expectedCfg)
                        .then(editRes => {
                            expect(editRes).to.have.status(200);
                            return agent.get("config/station/get");
                        })
                        .then(getResp => {
                            expect(getResp).to.have.status(200);
                            expect(getResp).to.be.json;
                            var body = getResp.body;
                            expect(body).to.be.eql(expectedCfg)
                        })
                });
        }
    });
    it('edit ap config', () => {
        if(isHost()) {
            return doSucessfulLogin({login: "admin"})
                .then(resp => {
                    var agent = resp.agent;
                    var rnd = Math.floor(Math.random()*10)
                    var expectedCfg = {
                        enabled: true,
                        ssid: "station_"+rnd,
                        password: "station_pass_"+rnd,
                        authMode: 2,
                        ip: "1.2.3."+rnd,
                        hidden: true,
                        channel: rnd,
                        beaconInterval: rnd*10
                    }
                    var expectedRomUrl = 'abc' + rnd;
                    var expectedSpiffUrl = 'defg' + rnd;
                    return agent
                        .post("config/ap/set")
                        .type('form')
                        .send(expectedCfg)
                        .then(editRes => {
                            expect(editRes).to.have.status(200);
                            return agent.get("config/ap/get");
                        })
                        .then(getResp => {
                            expect(getResp).to.have.status(200);
                            expect(getResp).to.be.json;
                            var body = getResp.body;
                            expect(body).to.be.eql(expectedCfg)
                        })
                });
        }
    });
});