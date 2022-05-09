import coap, { ObserveWriteStream } from 'coap'; // or coap

const BlockCache = require('../node_modules/coap/lib/cache'); // or coap
import fs from 'fs';

import { mongodbConnection } from './models/connect';

/*
const b = new BlockCache(10000, () => { return null })
b.add('test', { payload: 'sss' })
console.log(b.get('test'))
*/


// Web server
import http from "http";
const httpServer = http.createServer();

import { Server } from "socket.io";
const io = new Server(httpServer);

io.on("connection", (socket) => {
  console.log(`connect ${socket.id}`);

  socket.on("disconnect", () => {
      console.log(`disconnect ${socket.id}`);
  });
});


httpServer.listen(3030);


// Coap Server
const coapServer = coap.createServer();

coapServer.on('request', (req, res) => {
  //let observeClient: any;
  
  res.on('finish', (data: any) => {
    console.log(data)
  });

  if(req.url === "/deviceman"){ // deviceman
    const data = JSON.parse(req.payload.toString().replace(/[']/g, "\""));
    console.log(data);
    if (req.headers.Observe === 0) {
      //observeClient = res;
      return res.write("Conexão Iniciada!" + "\n");
    };
    io.emit("devDataMan", data);
    //return observeClient.write(req.payload);
  }

  if(req.url === "/devicepet"){
    const data = JSON.parse(req.payload.toString().replace(/[']/g, "\""));
    console.log(data);
    if (req.headers.Observe === 0) {
      //observeClient = res;
      return res.write("Conexão Iniciada!" + "\n");
    };
    io.emit("devDataPet", data);
    //return observeClient.write(req.payload);
  }
});




/*   if (req.headers.Observe !== 0) {
    const data = JSON.parse(req.payload.toString().replace(/[']/g, "\""));

    //console.log('Hello ' + req.url + '\n');
  
    //res.statusCode = '2.05';
    //res._request.code = '0.01';
    count++;
    res.end(data)
  }; 
});

*/

coapServer.listen(5683, () => {
  console.log("Servidor em execução!")
}); 


//res.setOption('max-age', 5);
