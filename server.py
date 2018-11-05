from bottle import route, get, post, run, request, static_file, HTTPResponse, HTTPError

configs = {
    "ap": {
        'enabled': True,
        'ssid': 'ap_ssid',
        'password': 'pwd',
        'authMode': 3,
        'ip': '192.168.8.9',
        'hidden': True,
        'channel': 9,
        'beaconInterval': 1234
    },
    "station": {
        'enabled': True,
        'ssid': 'station_ssid',
        'password': 'pwd',
        'ip': '192.168.8.9',
        'netmastk': '255.255.255.0',
        'gateway': '192.168.8.1'
    },
    "ota": {
        'romUrl': 'http://example.com/romUrl',
        'spiffUrl': 'http://example.com/spiffUrl',
    }
}

@route('/')
def index():
    return static_file("index.htm", "./files")

@route('/info')
def info():
    return HTTPResponse({"device": "info"})

@get('/config/<name>/get')
def get_config(name):
    return HTTPResponse(configs.get(name, {}))

@post('/config/<name>/set')
def set_config(name):
    if request.forms is None:
        return HTTPError(404, "Invalid body")
    elif name not in ['ap', 'ota', 'station']:
        return HTTPError(404, "Invalid config type")
    else:
        global configs
        configs[name] = dict(request.forms)
        return HTTPResponse({"status": "successful"})



@route('/<filename>')
def static(filename):
    return static_file(filename, "./files")

run(host='localhost', port=8080)