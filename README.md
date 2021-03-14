# Wifire
ESP8266 application skeleton based on [Sming framework](https://github.com/SmingHub/Sming) and [ReactJS](https://reactjs.org/).

# Features
- Rich fronend AJAX based architecture
- WiFi Station and Access Point configuration
- GPIO configuration
- GPIO pins switch with timeouts
- Over-the-air updates
- Json based configuration
- Console management (basic)
- React extensible based fronend 

# Extending
Wifire is in an alpha stage now. Nice way of extending in without modifying the source code is not available yet.
It will be added in future releases. For now you could just check it out and modify the source code.


# Modifing backend
The Wifire is made of couple of singleton objects which are created by `Injector` class.
All business logic is defined in `Services` directory. `Model` directory contains configuration classes + json encoder/decoders.
Configuration loading/and persisting is handled by `ConfigProvider` generic class which could be easily extended by the new types.
All configs are stored in spiffis images in Json format.

# Fronend development
Fronend is based on reactjs backed by Node + webpack. To build web app you need to install [npm](https://www.npmjs.com/)
For preformance whole application is bundled into single index.html.gz file.
This file contain all necessary JavaScript and CSS. To bundle it type:
```
make web-upload
```
It will trigger `web-pack` and `spiffs-image-update` commands.

## Modifing the frontend
Packing and flashing web app after each change is cumbersome. Wifire is based on [Create react app](https://create-react-app.dev/) which
add a support of hot code replacement and proxy server. Basically, rather than a web app hosted by ESP chip you start a local web proxy
which redirect all HTTP requests to ESP chip. Thanks to that you could code your fronend app and see changes immediately.
It increases your development performance significantly by removing `change -> make -> flash` loop.
To use that workflow you need to configure remote ESP host so npm will know where to proxy front-end requests.
### Host Arch
You will need to setup a virtual tap interface to contact to the EPS Host emulator. You could use, provided Sming script for that:
```
Sming/Sming/Arch/Host/Tools/setup-network-linux.sh [INTERFACE]
```
Where `[INTERFACE]` is your ethernet interface name returned by `ifconfig`/`ipconfig`.

### ESP Arch
If you develop against real ESP chip you just need an IP of ESP Accespoint / Station.

### ESP web server proxy
The next step is to set Host Emulator / ESP IP address for proxy host. To that edit `web/package.json` `proxy` setting.
After that you could run npm proxy which will serve your application. In `web` folder run:
```
npm start
```
Now all changes which you do in `web/src/` directory will be visible by refreshing 'localhost:3000` web page.
After all, you could run `make web-upload` to bundle and upload your changes to a Spiffis filesystem image.



# Bugs and feature requests
- Session retention after chip restarts. We don't want to loog out all userers after ESP chip restarts.
- Expire session after given time since restart.
- Wifi connection strategies (AP, Station, AP if Station not connected etc, Fallback to AP after disconnect etc.)
- Nice way to extend this application without modifying its source code.
- Ota update is not tested. In plans, there is also an adding support of uploading firmware file rather than providing an external HTTP link.

# Preview
Wifire pages screenshot is available here [here](./docs/preview.md)