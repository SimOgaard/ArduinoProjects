var mosca = require("mosca"); //vi använder oss utav mosca

var settings={
    port:1883, //sätter porten
    http:{
        port:1884,
        bundle:true,
        static:"./"
    }
}
var server = new mosca.Server(settings); //skapar servern

server.on("ready", function (){ //när servern är redo
    console.log("enklare än att skapa en minecraft server");
})
server.on("clientConnected", function (client){ //när någon på hemsidan connectar
    console.log("client anslöt", client.id);
})