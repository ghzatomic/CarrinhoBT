const Gamecontroller = require('./gamecontroller');
const ctrl = new Gamecontroller('generic');


console.log(ctrl.showDevices())

// ctrl.connect(function() {
//     console.log('Game On!');
// });

// ctrl.on('B:press', function() {
//     console.log('B was pressed');
// });

// ctrl.on('B:release', function() {
//     console.log('B was released');
// });


// ctrl.on('Right:press', function() {
//     console.log("Apertou direita");
// });

// ctrl.on('Right:release', function() {
//     console.log("Soltou direita");
// });