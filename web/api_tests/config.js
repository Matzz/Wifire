exports.serverConf = {
	"host": process.env.WIFIRE_HOST || "http://192.168.13.10/",
	"arch": process.env.SMING_ARCH || "Esp8266"
}
exports.isHost = function() {
	return exports.serverConf["arch"] == "Host";
}