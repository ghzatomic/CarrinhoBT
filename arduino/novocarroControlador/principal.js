const bluetooth = require('node-bluetooth');
var readlineSync = require('readline-sync');

const Gamecontroller = require('./gamecontroller');
const ctrl = new Gamecontroller('generic');

// create bluetooth device instance
const device = new bluetooth.DeviceINQ();


function sleep(ms) {
  return new Promise((resolve) => {
    setTimeout(resolve, ms);
  });
}

const main = async () => {
  console.log(ctrl.showDevices())
  device.listPairedDevices(console.log);

  const deviceScan = await device.scan();
  console.log("Encontrados : ")
  deviceScan.forEach((element, i) => {
    console.log(i + " - " + element.address + " - " + element.name)
  });
  const escolhido = 0 //readlineSync.keyInYN('Qual device deseja conectar ?');
  console.log(escolhido)
  const deviceEscolhido = deviceScan[escolhido]
  device.findSerialPortChannel(deviceEscolhido.address, async function (channel) {
    console.log('Found RFCOMM channel for serial port on %s: ', deviceEscolhido.name, channel);

    bluetooth.connect(deviceEscolhido.address, channel, async function (err, connection) {
      if (err) return console.error(err);

      connection.delimiter = 13;
      // connection.on('data', (buffer) =0,0,0,0,0,0> {
      //   console.log('received message:', buffer.toString());
      // });
      ctrl.connect(function () {
        console.log('Game On!');
      });

      data = [0, 0, 0, 0, 0, 0]
      modo_sonar = false
      record = []
      modoGravacao = false
      ctrl.on('Up:press', function () {
        data[0] = 1
        connection.write(new Buffer(data.join(['']) + "|"), () => { });
      });

      ctrl.on('Up:release', function () {
        data[0] = 0
        connection.write(new Buffer(data.join(['']) + "|"), () => { });
      });

      ctrl.on('Down:press', function () {
        data[1] = 1
        connection.write(new Buffer(data.join(['']) + "|"), () => { });
      });

      ctrl.on('Down:release', function () {
        data[1] = 0
        connection.write(new Buffer(data.join(['']) + "|"), () => { });
      });

      ctrl.on('Right:press', function () {
        data[2] = 1
        connection.write(new Buffer(data.join(['']) + "|"), () => { });
      });

      ctrl.on('Right:release', function () {
        data[2] = 0
        connection.write(new Buffer(data.join(['']) + "|"), () => { });
      });

      ctrl.on('Left:press', function () {
        data[3] = 1
        connection.write(new Buffer(data.join(['']) + "|"), () => { });
      });

      ctrl.on('Left:release', function () {
        data[3] = 0
        connection.write(new Buffer(data.join(['']) + "|"), () => { });
      });

      ctrl.on('B:press', function () {
        if (modo_sonar){
          data[4] = 0
          modo_sonar = false
        }else{
          data[4] = 1
          modo_sonar = true
        }
        connection.write(new Buffer(data.join(['']) + "|"), () => { });
      });

      // ctrl.on('B:release', function () {
      //   data[4] = 0
      //   connection.write(new Buffer(data.join(['']) + "|"), () => { });
      // });
      ctrl.on('A:press', function () {
        data[5] = 1
        connection.write(new Buffer(data.join(['']) + "|"), () => { });
      });

      ctrl.on('A:release', function () {
        data[5] = 0
        connection.write(new Buffer(data.join(['']) + "|"), () => { });
      });

    });

  });



};

main();